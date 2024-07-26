/*
 * Create functionality that turns of LED after 5 seconds if nothing entered on UART
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

// timer
static TimerHandle_t timeout;

// UART data
static uint8_t *data;
static int len;

// mutex
static SemaphoreHandle_t binary_semphr = NULL;

void uart_task(void* parameter) {
    data = (uint8_t*)malloc(BUF_SIZE*sizeof(uint8_t));
    while(1) {
        len = uart_read_bytes(
            ECHO_UART_PORT_NUM, // port
            data, // data pointer
            (BUF_SIZE - 1), // size of the data
            20 / portTICK_PERIOD_MS // timeout?
        );
        data[len] = '\0';
        if (len > 0) {
            gpio_set_level(BLINK_GPIO, 1);
            uart_write_bytes(ECHO_UART_PORT_NUM, data, BUF_SIZE);
            // start timer acts as reset
            xTimerStart(timeout, portMAX_DELAY);
            //xTimerReset(timeout, portMAX_DELAY);
        }
    }
}

void backlight_callback(TimerHandle_t xTimer) {
    gpio_set_level(BLINK_GPIO, 0);
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

    // create timer
    binary_semphr = xSemaphoreCreateBinary();
    timeout = xTimerCreate(
        "timeout timer", // text name
        5000/portTICK_PERIOD_MS, // timeout in ticks
        pdFALSE, // do not restart timer when expires
        (void*)0, // id
        backlight_callback
    );

    // create tasks
    xTaskCreatePinnedToCore(
        &uart_task,
        "producer",
        TASK_STACK_SIZE,
        NULL,
        1,
        NULL,
        1 // pin to core 1
    );
}
