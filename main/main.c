#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "myGPIO.h"
#include <stdlib.h>
#include "esp_system.h"

#define LED 2
#define BUTTON 5

volatile int gedrukt = 0;

static void IRAM_ATTR ButtonToogle(void *args)
{
    gedrukt = 1;
}

void app_main(void)
{
    MyGPIO_setup_led(LED);
    MyGPIO_setup_button(BUTTON);

    gpio_install_isr_service(0);
    gpio_set_intr_type(BUTTON, GPIO_INTR_NEGEDGE);
    gpio_isr_handler_add(BUTTON, ButtonToogle, (void *)BUTTON);

    while (1)
    {
        gedrukt = 0;

        printf("Wachten...\n");
        vTaskDelay((rand() % 3000 + 1000) / portTICK_PERIOD_MS);

        MyGPIO_schrijven(LED, 1);

        TickType_t start = xTaskGetTickCount();

        while (!gedrukt)
        {
            vTaskDelay(1);
        }

        MyGPIO_schrijven(LED, 0);
        TickType_t einde = xTaskGetTickCount();

        printf("Knop gedrukt! Tijd: %lu ms\n", (einde - start) * portTICK_PERIOD_MS);
    }
}
