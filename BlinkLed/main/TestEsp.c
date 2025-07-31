#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"

void app_main(void)
{
    // Reset GPIOs
    gpio_reset_pin(GPIO_NUM_39);
    gpio_reset_pin(GPIO_NUM_15);

    // Set GPIOs 15 (Input and Pull-up)
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_15, GPIO_PULLUP_ONLY);

    // Set GPIO 39 (Output)
    gpio_set_direction(GPIO_NUM_39, GPIO_MODE_OUTPUT);

    while (true)
    {
        if (gpio_get_level(GPIO_NUM_15) == 1)
        {
            gpio_set_level(GPIO_NUM_39, 0);
        }
        else
        {
            gpio_set_level(GPIO_NUM_39, 1);
        }
        // Delay for a while to avoid busy-waiting
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
