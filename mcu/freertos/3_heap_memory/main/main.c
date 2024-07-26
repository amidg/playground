/*
 * Using FreeRTOS, create two separate tasks.
 * One listens for input over UART (from the Serial Monitor). Upon receiving a newline character (‘\n’), the task allocates a new section of heap memory (using pvPortMalloc()) and stores the string up to the newline character in that section of heap. It then notifies the second task that a message is ready.
 *
 * The second task waits for notification from the first task. When it receives that notification, it prints the message in heap memory to the Serial Monitor. Finally, it deletes the allocated heap memory (using vPortFree()).
 */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_heap_caps_init.h"
#include "esp_heap_caps.h"

#define BUF_SIZE 1024
#define UART_PORT 0 // default USB
#define ECHO_TEST_TXD           1
#define ECHO_TEST_RXD           3
#define ECHO_TEST_RTS           UART_PIN_NO_CHANGE
#define ECHO_TEST_CTS           UART_PIN_NO_CHANGE
#define ECHO_UART_PORT_NUM      0
#define ECHO_UART_BAUD_RATE     115200 
#define ECHO_TASK_STACK_SIZE    2048

// receive data and update blink example
static uint8_t data_ready = 0;
static uint8_t uart_data[BUF_SIZE];
static uint8_t *data;
static int len; // data length

void uart_rx_task(void *pvParameter) {
	while(1) {
        // Read data from the UART
        len = uart_read_bytes(
            ECHO_UART_PORT_NUM, // port
            uart_data, // data pointer
            (BUF_SIZE - 1), // size of the data
            20 / portTICK_PERIOD_MS // timeout?
        );
        uart_data[len] = '\0';
        // Write data back to the UART
        if (len && !data_ready) {
            // allocate space on heap
            //data = (uint8_t*)heap_caps_malloc(len*sizeof(uint8_t), MALLOC_CAP_8BIT);
            data = (uint8_t*)pvPortMalloc(len*sizeof(uint8_t));
            if (data != NULL) {
                memcpy(data, uart_data, len);
                //for (;*uart_data != '\0'; uart_data++, data++)
                //    *data = *uart_data;
                //data[BUF_SIZE-1] = '\0';
                data_ready = 1;
            } else {
                ESP_LOGI("", "ERROR: %s", "cannot allocate memory");
            }
        }
    }
}

void uart_tx_task(void *pvParameter) {
	while(1) {
        if (data_ready) {
            // this writes to the FIFO buffer in a blocking way
            uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, len);
            ESP_LOGI("", "Received Message: %s", (char *) data);
            vPortFree(data);
            data = NULL;
            data_ready = 0;
        }
        vTaskDelay(100);
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

    // create task 1
    xTaskCreatePinnedToCore(
        &uart_rx_task,
        "rx_task",
        1024,
        NULL,
        1,
        NULL,
        0 // pin to core 0
    );

    // create task 2
    xTaskCreatePinnedToCore(
        &uart_tx_task,
        "tx_task",
        ECHO_TASK_STACK_SIZE,
        NULL,
        1,
        NULL,
        0 // pin to core 1
    );
}
