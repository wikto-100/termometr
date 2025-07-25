#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#define PICO_GP2 2
#define PICO_GP3 3
#define BLINK_SPEED 500

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
    panic("Stack overflow. Task: %s\n", pcTaskName);
}

void vApplicationMallocFailedHook()
{
    panic("malloc failed");
}

static void y_LedTask(void* pvParameters)
{
    gpio_init(PICO_GP2);
    gpio_set_dir(PICO_GP2, GPIO_OUT);

    while (true)
    {
        printf("yellow LED task running on core %u\n", get_core_num());
        gpio_put(PICO_GP2, true);
        vTaskDelay(pdMS_TO_TICKS(BLINK_SPEED));
        gpio_put(PICO_GP2, false);
        vTaskDelay(pdMS_TO_TICKS(BLINK_SPEED));
    }
}
static void g_LedTask(void* pvParameters){
    
    gpio_init(PICO_GP3);
    gpio_set_dir(PICO_GP3, GPIO_OUT);

    // green goes second (alternate blinking)
    vTaskDelay(pdMS_TO_TICKS(BLINK_SPEED));

    while (true)
    {
        printf("green LED task running on core %u\n", get_core_num());
        gpio_put(PICO_GP3, true);
        vTaskDelay(pdMS_TO_TICKS(BLINK_SPEED));
        gpio_put(PICO_GP3, false);
        vTaskDelay(pdMS_TO_TICKS(BLINK_SPEED));
    }
}

int main()
{
    stdio_init_all();
    // Create task handles
    TaskHandle_t g_LedTaskHandle = NULL;
    TaskHandle_t y_LedTaskHandle = NULL;

    // Create tasks and assign handles
    xTaskCreate(g_LedTask, "gLED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &g_LedTaskHandle);
    xTaskCreate(y_LedTask, "yLED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &y_LedTaskHandle);
 
    // Bind tasks to cores
    vTaskCoreAffinitySet(g_LedTaskHandle, 1U << 0);
    vTaskCoreAffinitySet(y_LedTaskHandle, 1U << 1);
 
    vTaskStartScheduler(); 
    panic_unsupported();
}