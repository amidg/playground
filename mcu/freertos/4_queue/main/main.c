/*
 * Task A: 
 * - Read Serial Input from user and check for "delay XXX" type of message
 * - Place XXX onto the Queue and write it on Serial port
 * - Print message from Queue2
 * Task B:
 * - Blink LED with XXX delay
 * - Every 100 blinks send message to Queue2
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

// UART data
static uint8_t *data;
static int len;
static int blink_rate;

// Queue stuff
static QueueHandle_t delay_queue, feedback_queue;
static uint8_t feedback[10];

/*
 * function to retrieve delay message
 * Checks if input has format "delay %d"
 */
int32_t get_delay(const char* input) {
    int i=0, j=0, key_ok=0;
    char keyword[6];
    char delay_str[BUF_SIZE];
    for (; *input != '\0'; input++, i++) {
        // get keyword
        keyword[i] = *input;
        if (i == 5 && *input == 32) { // space found
            keyword[i] = '\0'; // we remove space and replace it with NULL
            if (strcmp(keyword, "delay")) {
                return -1;
            }
            key_ok = 1;
        }

        // if keyword is good we can retrieve number
        if (key_ok) {
            delay_str[j] = *input;
            j++;
        }
    }
    if (key_ok) {
        return (int32_t)strtol((const char*)delay_str, NULL, 10);
    } else {
        return -1;
    }
}

void uart_task(void *pvParameter) {
    data = (uint8_t*)malloc(BUF_SIZE);
	while(1) {
        // Read data from the UART
        len = uart_read_bytes(
            ECHO_UART_PORT_NUM, // port
            data, // data pointer
            (BUF_SIZE - 1), // size of the data
            20 / portTICK_PERIOD_MS // timeout?
        );
        data[len] = '\0';

        // Write data to delay queue
        if (len > 0 && delay_queue) { // delay_queue != 0
            // get latest string to text delay value
            if ((blink_rate = get_delay((const char*)data)) > 0) {
                // place object on the queue
                if (xQueueSend(delay_queue, (void*)&blink_rate, (TickType_t)10) != pdTRUE)
                    ESP_LOGI("", "ERROR: %s", "Delay queue is full");
            }
        }

        // Print data on Serial when received Blinked! on queue
        if (xQueueReceive(feedback_queue, feedback, (TickType_t)10) == pdTRUE) {
            ESP_LOGI("", "%s", feedback);
        }
    }
}

void blink_task(void *pvParameter) {
    uint32_t blink_delay = 500;
    uint8_t blink_count = 0;
	while(1) {
        // check queue for 10 ticks
        if (xQueueReceive(delay_queue, &blink_delay, (TickType_t)10) == pdTRUE) {
            ESP_LOGI("", "New Blink Period: %d", (int)blink_delay);
        }

        // portTICK_PERIOD_MS is 1ms because this is the smallest FreeRTOS tick 1ms
        gpio_set_level(BLINK_GPIO, 0); // set low
	    vTaskDelay(blink_delay/portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1); // set high
	    vTaskDelay(blink_delay/portTICK_PERIOD_MS);
        blink_count++;

        if (blink_count >= 100) {
            xQueueSend(feedback_queue, (void*)"Blinked!", (TickType_t)10);
            blink_count = 0;
        }
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

    // create queues
    delay_queue = xQueueCreate( 10, sizeof(uint32_t)); // send delays
    feedback_queue = xQueueCreate( 10, 10*sizeof(uint8_t)); // send Blinked! back

    // create task 1
    xTaskCreatePinnedToCore(
        &uart_task,
        "uart_task",
        ECHO_TASK_STACK_SIZE,
        NULL,
        1,
        NULL,
        0 // pin to core 0
    );

    // create task 2
    xTaskCreatePinnedToCore(
        &blink_task,
        "blink_task",
        ECHO_TASK_STACK_SIZE,
        NULL,
        2,
        NULL,
        0 // pin to core 1
    );
}
