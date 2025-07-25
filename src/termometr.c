#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "DS18B20.h"

#define DQ_PIN 4

// FreeRTOS hook: stack overflow
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    panic("Stack overflow. Task: %s\n", pcTaskName);
}

// FreeRTOS hook: malloc failure
void vApplicationMallocFailedHook()
{
    panic("malloc failed");
}

// Task: READ ROM using DS18B20 API
static void vReadRomTask(void *pvParameters)
{
    uint8_t rom[8];
    ds18b20_init(DQ_PIN);

    for (;;)
    {
        if (!ds18b20_read_rom(rom))
        {
            printf("⨯ DS18B20 NOT PRESENT\n");
        }
        else
        {
            printf("ROM = %02X-", rom[0]);
            for (int i = 1; i < 7; i++)
                printf("%02X", rom[i]);
            printf("-%02X\n", rom[7]);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main()
{
    stdio_init_all();

    xTaskCreate(
        vReadRomTask,
        "ReadROM",
        configMINIMAL_STACK_SIZE * 2,
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL
    );

    vTaskStartScheduler();
    panic("Scheduler exited unexpectedly\n");
}
