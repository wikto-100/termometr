// driver_ds18b20_dual.c
/**
 * DS18B20 dual-sensor driver implementation with hard-coded ROMs
 */

#include "driver_ds18b20_dual.h"
#include <stdio.h>

// Hardcoded ROMs for two sensors
static const uint8_t gs_sensor_roms[DS18B20_DUAL_MAX_SENSORS][8] = {
    {0x28, 0xAE, 0x76, 0x56, 0x00, 0x00, 0x00, 0x71},  // Sensor 0
    {0x28, 0x9E, 0x1C, 0x58, 0x00, 0x00, 0x00, 0x25}   // Sensor 1
};

static ds18b20_handle_t gs_handles[DS18B20_DUAL_MAX_SENSORS];

uint8_t ds18b20_dual_init(void)
{
    /* Link interface to each handle */
    for (uint8_t i = 0; i < DS18B20_DUAL_MAX_SENSORS; ++i) {
        DRIVER_DS18B20_LINK_INIT   (&gs_handles[i], ds18b20_handle_t);
        DRIVER_DS18B20_LINK_BUS_INIT   (&gs_handles[i], ds18b20_interface_init);
        DRIVER_DS18B20_LINK_BUS_DEINIT (&gs_handles[i], ds18b20_interface_deinit);
        DRIVER_DS18B20_LINK_BUS_READ   (&gs_handles[i], ds18b20_interface_read);
        DRIVER_DS18B20_LINK_BUS_WRITE  (&gs_handles[i], ds18b20_interface_write);
        DRIVER_DS18B20_LINK_DELAY_MS   (&gs_handles[i], ds18b20_interface_delay_ms);
        DRIVER_DS18B20_LINK_DELAY_US   (&gs_handles[i], ds18b20_interface_delay_us);
        DRIVER_DS18B20_LINK_ENABLE_IRQ (&gs_handles[i], ds18b20_interface_disable_irq);
        DRIVER_DS18B20_LINK_DISABLE_IRQ(&gs_handles[i], ds18b20_interface_enable_irq);
        DRIVER_DS18B20_LINK_DEBUG_PRINT(&gs_handles[i], ds18b20_interface_debug_print);

        /* Initialize core driver */
        if (ds18b20_init(&gs_handles[i]) != 0) {
            ds18b20_interface_debug_print("ds18b20_dual: init sensor %d failed\r\n", i);
            return 1;
        }
        /* Assign fixed ROM and set to MATCH_ROM */
        ds18b20_set_rom     (&gs_handles[i], (uint8_t *)gs_sensor_roms[i]);
        ds18b20_set_mode    (&gs_handles[i], DS18B20_MODE_MATCH_ROM);
        ds18b20_scratchpad_set_resolution(&gs_handles[i], DS18B20_RESOLUTION_12BIT);
    }
    return 0;
}

uint8_t ds18b20_dual_read(float temps[DS18B20_DUAL_MAX_SENSORS])
{
    int16_t raw;
    for (uint8_t i = 0; i < DS18B20_DUAL_MAX_SENSORS; ++i) {
        if (ds18b20_read(&gs_handles[i], &raw, &temps[i]) != 0) {
            return 1;
        }
    }
    return 0;
}

uint8_t ds18b20_dual_deinit(void)
{
    for (uint8_t i = 0; i < DS18B20_DUAL_MAX_SENSORS; ++i) {
        ds18b20_deinit(&gs_handles[i]);
    }
    ds18b20_interface_deinit();
    return 0;
}