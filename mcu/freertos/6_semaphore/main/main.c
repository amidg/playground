/*
 * Challenge: use a mutex and counting semaphores to protect the shared buffer 
 * so that each number (0 throguh 4) is printed exactly 3 times to the Serial 
 * monitor (in any order). Do not use queues to do this!
 * 
 * Hint: you will need 2 counting semaphores in addition to the mutex, one for 
 * remembering number of filled slots in the buffer and another for 
 * remembering the number of empty slots in the buffer.
 */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define BLINK_GPIO 5
#define BUF_SIZE 1024
#define ECHO_UART_PORT_NUM      0
#define ECHO_TEST_TXD           1
#define ECHO_TEST_RXD           3
#define ECHO_TEST_RTS           UART_PIN_NO_CHANGE
#define ECHO_TEST_CTS           UART_PIN_NO_CHANGE
#define ECHO_UART_BAUD_RATE     115200 
#define TASK_STACK_SIZE         2048

// Tasks
#define NUM_PRODUCER_TASKS  5
#define NUM_CONSUMER_TASKS  2

// circular buffer
static uint8_t circular_buffer[BUF_SIZE];
static uint8_t* buffer_ptr = circular_buffer;

// UART data
static uint8_t *data;
static int len;

// data sharing
static SemaphoreHandle_t uart_binary_semphr, mutex, semphr_empty, semphr_filled;

void producer(void *pvParameter) {
    uint8_t id = *(int*)pvParameter;
	while(1) {
        for (int i=0; i < 3; i++) {
            xSemaphoreTake(semphr_empty, portMAX_DELAY);

            // take over access and write
            xSemaphoreTake(mutex, portMAX_DELAY);
            *(++buffer_ptr) = id;
            xSemaphoreGive(mutex);

            // release semaphores
            xSemaphoreGive(semphr_filled);
        }
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}

void consumer(void *pvParameter) {
    uint8_t id = *(int*)pvParameter;
    char* output = (char*)malloc(8*sizeof(char));
	while(1) {
        // signal to everyone we took over filled one
        xSemaphoreTake(semphr_filled, portMAX_DELAY);

        xSemaphoreTake(mutex, portMAX_DELAY);
        sprintf(output, "%d\n", (int)*buffer_ptr);
        uart_write_bytes(ECHO_UART_PORT_NUM, output, 8);
        //ESP_LOGI("", "%d\n",(int)*buffer_ptr);
        --buffer_ptr; // decrease
        xSemaphoreGive(mutex);
        // signal we are done
        xSemaphoreGive(semphr_empty);
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}

void app_main()
{
    // config UART
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

    ESP_ERROR_CHECK(
        uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE*2, 0, 0, NULL, intr_alloc_flags)
    );
    ESP_ERROR_CHECK(
        uart_param_config(ECHO_UART_PORT_NUM, &uart_config)
    );
    ESP_ERROR_CHECK(
        uart_set_pin(
            ECHO_UART_PORT_NUM,
            ECHO_TEST_TXD,
            ECHO_TEST_RXD,
            ECHO_TEST_RTS,
            ECHO_TEST_CTS
        )
    );

    // create binary semaphore to control serial port access
    uart_binary_semphr = xSemaphoreCreateBinary();  // controls access to the serial
    mutex = xSemaphoreCreateMutex();                // controls access to buffer
    semphr_empty = xSemaphoreCreateCounting(BUF_SIZE, BUF_SIZE);    // how many empty
    semphr_filled = xSemaphoreCreateCounting(BUF_SIZE, 0);          // how many filled

    // create tasks
    for (int i = 0; i < NUM_PRODUCER_TASKS; i++) {
        xTaskCreatePinnedToCore(
            producer, // does not use pointer because copies function
            "producer",
            TASK_STACK_SIZE,
            (void*)&i,
            1,
            NULL,
            1 // pin to core 1
        );
    }

    for (int i = 0; i < NUM_CONSUMER_TASKS; i++) {
        xTaskCreatePinnedToCore(
            consumer,
            "consumer",
            TASK_STACK_SIZE,
            (void*)&i,
            1,
            NULL,
            1 // pin to core 1
        );
    }
}
