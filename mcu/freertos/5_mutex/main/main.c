/*
 * Create code that reads blink parameter from the serial before creating a task
 * Use Mutex to protect the variable so that task has enough time to read it before it goes out of scope 
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
#define UART_PORT 0 // default USB
#define ECHO_TEST_TXD           1
#define ECHO_TEST_RXD           3
#define ECHO_TEST_RTS           UART_PIN_NO_CHANGE
#define ECHO_TEST_CTS           UART_PIN_NO_CHANGE
#define ECHO_UART_PORT_NUM      0
#define ECHO_UART_BAUD_RATE     115200 
#define TASK_STACK_SIZE         2048

// UART data
static uint8_t *data;
static int len;

// mutex
static SemaphoreHandle_t mutex = NULL;
static uint8_t shared_int = 0;

void blink_task(void *pvParameter) {
    uint32_t blink_delay = *(int*)pvParameter;
    //xSemaphoreGive(mutex);
	while(blink_delay > 0) {
        gpio_set_level(BLINK_GPIO, 0); // set low
	    vTaskDelay(blink_delay/portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1); // set high
	    vTaskDelay(blink_delay/portTICK_PERIOD_MS);
        
        // useless stuff
        if (!(shared_int%2)) {
            xSemaphoreTake(mutex, 0);
            shared_int++;
            ESP_LOGI("", "Shared increased by task 1: %d", (int)shared_int);
            xSemaphoreGive(mutex);
        }
    }
}

void test_task(void *pvParameter) {
	while(1) {
        if (shared_int%2 == 1) {
            xSemaphoreTake(mutex, 0);
            shared_int++;
            ESP_LOGI("", "Shared increased by task 2: %d", (int)shared_int);
            xSemaphoreGive(mutex);
        }
        vTaskDelay(1000);
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

    // config GPIO
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    vTaskDelay(pdMS_TO_TICKS(10000));
    ESP_LOGI("", "%s", "Hello World!");

    // create mutex
    mutex = xSemaphoreCreateMutex();

    // wait until serial is entered
    int blink_rate = 0;
    data = (uint8_t*)malloc(BUF_SIZE);
    while (!blink_rate) {
        len = uart_read_bytes(
            ECHO_UART_PORT_NUM, // port
            data, // data pointer
            (BUF_SIZE - 1), // size of the data
            20 / portTICK_PERIOD_MS // timeout?
        );
        data[len] = '\0';
        if (len > 0) {
            blink_rate = (int32_t)strtol((const char*)data, NULL, 10);
            ESP_LOGI("", "Blink Rate: %d", (int)blink_rate);
        }
    }

    // take mutex to control the execution
    // create blink task
    xTaskCreatePinnedToCore(
        &blink_task,
        "blink_task",
        TASK_STACK_SIZE,
        (void*)&blink_rate,
        1,
        NULL,
        1 // pin to core 1
    );

    xTaskCreatePinnedToCore(
        &test_task,
        "test_task",
        TASK_STACK_SIZE,
        NULL,
        1,
        NULL,
        1 // pin to core 1
    );
}
