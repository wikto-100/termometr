/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
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
 *
 * @file      driver_ds18b20_interface.c
 * @brief     DS18B20 interface implementation for FreeRTOS on Raspberry Pi Pico
 * @version   2.0.0
 * @date      2025-07-28
 * @author    Shifeng Li (ported by Wiktor Stojek)
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/07/28  <td>2.0      <td>Wiktor Stojek         <td>FreeRTOS+Pico port</td></tr>
 * <tr><td>2021/04/06  <td>2.0      <td>Shifeng Li           <td>Format the code</td></tr>
 * <tr><td>2020/12/20  <td>1.0      <td>Shifeng Li           <td>First upload</td></tr>
 * </table>
 */

#include "driver_ds18b20_interface.h"
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdarg.h>
#include <stdio.h>

/* 1-Wire bus GPIO pin (override with -DDS18B20_INTERFACE_PIN=N if needed) */
#ifndef DS18B20_INTERFACE_PIN
#define DS18B20_INTERFACE_PIN  4
#endif

/**
 * @brief  Initialize the 1-Wire bus GPIO
 * @return 0 on success, 1 on failure
 */
uint8_t ds18b20_interface_init(void)
{
    gpio_init(DS18B20_INTERFACE_PIN);
    gpio_set_function(DS18B20_INTERFACE_PIN, GPIO_FUNC_SIO);
    /* Release bus: input with pull-up */
    gpio_set_dir(DS18B20_INTERFACE_PIN, GPIO_IN);
    gpio_pull_up(DS18B20_INTERFACE_PIN);
    return 0;
}

/**
 * @brief  Deinitialize the 1-Wire bus GPIO
 * @return 0 on success, 1 on failure
 */
uint8_t ds18b20_interface_deinit(void)
{
    gpio_disable_pulls(DS18B20_INTERFACE_PIN);
    gpio_set_dir(DS18B20_INTERFACE_PIN, GPIO_IN);
    gpio_deinit(DS18B20_INTERFACE_PIN);
    return 0;
}

/**
 * @brief      Drive or release the bus line
 * @param[in]  bit 0 to pull low, 1 to release (let high)
 * @return     0 on success, 1 on failure
 */
uint8_t ds18b20_interface_write(uint8_t bit)
{
    if (bit == 0) {
        /* Drive bus low */
        gpio_set_dir(DS18B20_INTERFACE_PIN, GPIO_OUT);
        gpio_put(DS18B20_INTERFACE_PIN, 0);
    } else {
        /* Release bus (internal pull-up holds it high) */
        gpio_set_dir(DS18B20_INTERFACE_PIN, GPIO_IN);
    }
    return 0;
}

/**
 * @brief      Sample the bus line
 * @param[out] *bit receives 0 if low, 1 if high
 * @return     0 on success, 1 on failure
 */
uint8_t ds18b20_interface_read(uint8_t *bit)
{
    if (bit == NULL) {
        return 1;
    }
    *bit = gpio_get(DS18B20_INTERFACE_PIN) ? 1 : 0;
    return 0;
}

/**
 * @brief Delay for given milliseconds (yields to FreeRTOS)
 * @param[in] ms Time to wait in ms
 */
void ds18b20_interface_delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief Delay for given microseconds (busy-wait)
 * @param[in] us Time to wait in µs
 */
void ds18b20_interface_delay_us(uint32_t us)
{
    busy_wait_us(us);
}

/**
 * @brief Disable interrupts (enter critical section)
 */
void ds18b20_interface_disable_irq(void)
{
    taskENTER_CRITICAL();
}

/**
 * @brief Enable interrupts (exit critical section)
 */
void ds18b20_interface_enable_irq(void)
{
    taskEXIT_CRITICAL();
}

/**
 * @brief Formatted debug output (uses stdio)
 */
void ds18b20_interface_debug_print(const char *const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
