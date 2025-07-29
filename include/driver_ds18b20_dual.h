// driver_ds18b20_dual.h
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
 * @file      driver_ds18b20_dual.h
 * @brief     DS18B20 dual-sensor driver (hard-coded ROMs)
 * @version   2.1.0
 * @date      2025-07-29
 * @author    Wiktor Stojek
 */

#ifndef DRIVER_DS18B20_DUAL_H
#define DRIVER_DS18B20_DUAL_H

#include "driver_ds18b20_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Number of sensors on shared bus */
#define DS18B20_DUAL_MAX_SENSORS 2

/**
 * @brief  Initialize both sensors with hard-coded ROMs
 * @return 0 on success, 1 on failure
 */
uint8_t ds18b20_dual_init(void);

/**
 * @brief      Read temperatures from both sensors
 * @param[out] temps Array of length DS18B20_DUAL_MAX_SENSORS for °C values
 * @return     0 on success, 1 on failure
 */
uint8_t ds18b20_dual_read(float temps[DS18B20_DUAL_MAX_SENSORS]);

/**
 * @brief  Deinitialize sensors and release bus
 * @return 0 on success, 1 on failure
 */
uint8_t ds18b20_dual_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
