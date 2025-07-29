/**
 * @file      main_dual_client.c
 * @brief     FreeRTOS demo: Read two DS18B20 sensors with hard-coded ROMs
 * @version   1.0.0
 * @date      2025-07-29
 * @author    Wiktor Stojek
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "driver_ds18b20_dual.h"

#define TEMP_TASK_STACK    2048
#define TEMP_TASK_PRIO     (tskIDLE_PRIORITY + 1)

/**
 * @brief Reads two temperatures every second and prints them
 */
static void temperature_task(void *params)
{
    float temps[DS18B20_DUAL_MAX_SENSORS];

    /* Initialize both sensors */
    if (ds18b20_dual_init() != 0) {
        printf("ds18b20_dual: init failed\r\n");
        vTaskDelete(NULL);
    }
    printf("ds18b20_dual: sensors initialized\r\n");

    for (;;) {
        if (ds18b20_dual_read(temps) == 0) {
            printf("Sensor0: %.2f°C | Sensor1: %.2f°C\r\n", temps[0], temps[1]);
        } else {
            printf("ds18b20_dual: read failed\r\n");
        }
    }
}

/**
 * @brief Malloc-failed hook
 */
void vApplicationMallocFailedHook(void)
{
    printf("FreeRTOS malloc failed!\r\n");
    for (;;) { tight_loop_contents(); }
}

/**
 * @brief Stack-overflow hook
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("FreeRTOS stack overflow in task %s!\r\n", pcTaskName);
    for (;;) { tight_loop_contents(); }
}

int main(void)
{
    /* Initialize stdio over USB/UART */
    stdio_init_all();


    /* Delay to allow console connection */
    sleep_ms(5000);

    /* Create temperature-reading task */
    if (xTaskCreate(
            temperature_task,
            "temp_task",
            TEMP_TASK_STACK,
            NULL,
            TEMP_TASK_PRIO,
            NULL
        ) != pdPASS)
    {
        printf("Failed to create temp_task\r\n");
        while (1) { tight_loop_contents(); }
    }

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    for (;;) { tight_loop_contents(); }
}
