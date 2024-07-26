/*
 * Using FreeRTOS, create two separate tasks that blink the same LED at two different rates. That means controlling 1 LED with two different delay times.
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define BLINK_GPIO 5

static uint32_t blink_rate1 = 500; // ms
static uint32_t blink_rate2 = 333; // ms

void blink_slow_task(void *pvParameter) {
	while(1) {
        gpio_set_level(BLINK_GPIO, 0); // set low
	    vTaskDelay(blink_rate1/portTICK_PERIOD_MS); // portTICK_RATE_MS is 1ms because this is the smallest FreeRTOS tick rate
        gpio_set_level(BLINK_GPIO, 1); // set high
	    vTaskDelay(blink_rate1/portTICK_PERIOD_MS); // portTICK_RATE_MS is 1ms because this is the smallest FreeRTOS tick rate
	}
}

void blink_fast_task(void *pvParameter) {
	while(1) {
        gpio_set_level(BLINK_GPIO, 0); // set low
	    vTaskDelay(blink_rate2/portTICK_PERIOD_MS); // portTICK_RATE_MS is 1ms because this is the smallest FreeRTOS tick rate
        gpio_set_level(BLINK_GPIO, 1); // set high
	    vTaskDelay(blink_rate2/portTICK_PERIOD_MS); // portTICK_RATE_MS is 1ms because this is the smallest FreeRTOS tick rate
	}
}

void app_main()
{
    // setup LED pin to be output
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    // create task 1
    xTaskCreate(
        &blink_slow_task,
        "slow_task",
        2048,
        NULL,
        1,
        NULL
    );

    // create task 2
    xTaskCreate(
        &blink_fast_task,
        "fast_task",
        // 768B is minimal stack size, vanilla FreeRTOS uses words
        2048, 
        NULL,
        2,
        NULL
    );
}
