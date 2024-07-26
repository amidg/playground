/*
 * Using FreeRTOS, create two separate tasks.
 * One listens for an integer over UART (from the Serial Monitor) and sets a variable when it sees an integer.
 * The other task blinks the onboard LED (or other connected LED) at a rate specified by that integer.
 * In effect, you want to create a multi-threaded system that allows for the user interface to run concurrently with the control task (the blinking LED).
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

#define BLINK_GPIO 5
#define BUF_SIZE 1024
#define UART_PORT 0 // default USB
#define ECHO_TEST_TXD           1
#define ECHO_TEST_RXD           3
#define ECHO_TEST_RTS           UART_PIN_NO_CHANGE
#define ECHO_TEST_CTS           UART_PIN_NO_CHANGE
#define ECHO_UART_PORT_NUM      0
#define ECHO_UART_BAUD_RATE     115200 
#define ECHO_TASK_STACK_SIZE    2048

static uint32_t blink_rate = 500; // ms

void blink_task(void *pvParameter) {
	while(1) {
        gpio_set_level(BLINK_GPIO, 0); // set low
        // portTICK_PERIOD_MS is 1ms because this is the smallest FreeRTOS tick 1ms
	    vTaskDelay(blink_rate/portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1); // set high
	    vTaskDelay(blink_rate/portTICK_PERIOD_MS); 
	}
}

void uart_task(void *pvParameter) {
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

    // receive data and update blink example
    static uint8_t data[BUF_SIZE]; // let's avoid malloc and place this in memory
    //static uint8_t *data = (uint8_t *) malloc(BUF_SIZE); // buffer for the incoming data
	while(1) {
        // Read data from the UART
        int len = uart_read_bytes(
            ECHO_UART_PORT_NUM, // port
            data, // data pointer
            (BUF_SIZE - 1), // size of the data
            20 / portTICK_PERIOD_MS // timeout?
        );
        // Write data back to the UART
        //uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, len); // this writes to the FIFO buffer in a blocking way
        if (len) {
            data[len] = '\0';

            blink_rate = (uint32_t)strtol((const char*)data, NULL, 10);
            if (blink_rate > 10 && blink_rate < 10000) {
                sprintf((char*)data, "%d\n", (int)blink_rate);
                ESP_LOGI("", "Requested delay: %s", (char *) data);
            }

        }
    }
}

void app_main()
{
    // setup LED pin to be output
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    // create task 1
    xTaskCreate(
        &blink_task,
        "blink_task",
        1024,
        NULL,
        1,
        NULL
    );

    // create task 2
    xTaskCreate(
        &uart_task,
        "uart_task",
        ECHO_TASK_STACK_SIZE,
        NULL,
        2,
        NULL
    );
}
