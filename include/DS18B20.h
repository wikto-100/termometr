#ifndef DS18B20_H
#define DS18B20_H

#include <stdint.h>
#include <stdbool.h>
#include "onewire.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize DS18B20 on the specified GPIO pin.
 * Must be called before any other DS18B20 functions.
 * Uses onewire primitives for bus setup.
 */
static inline void ds18b20_init(uint8_t pin) {
    ow_init(pin);
}

/**
 * Read the 64-bit ROM code of a single DS18B20 on the bus.
 * @param rom  Array of 8 bytes to receive: [family, serial0..5, crc]
 * @return true if presence detected and ROM read successfully.
 */
bool ds18b20_read_rom(uint8_t rom[8]);

/**
 * Select a specific DS18B20 by its ROM code.
 * Use before device-specific commands when multiple devices share the bus.
 * @param rom  Array of 8 bytes containing the target ROM.
 * @return true if presence detected after Match ROM.
 */
bool ds18b20_match_rom(const uint8_t rom[8]);

/**
 * Skip ROM: address all devices on the bus.
 * Only use when exactly one DS18B20 is present.
 * @return true if presence detected.
 */
bool ds18b20_skip_rom(void);

/**
 * Perform an alarm search to find devices with temperature outside TH/TL.
 * Requires implementing the Search ROM procedure on return.
 * @return true if bus reset/presence succeeded.
 */
bool ds18b20_alarm_search(void);

/**
 * Start a temperature conversion on selected or skipped device(s).
 * Must wait up to 750 ms for 12-bit resolution before reading.
 * @return true if presence detected and command issued.
 */
bool ds18b20_start_conversion(void);

/**
 * Write TH, TL, and configuration (resolution) to the scratchpad.
 * @param th      Upper alarm threshold (°C integer)
 * @param tl      Lower alarm threshold (°C integer)
 * @param config  Resolution bits (in upper bits of config byte)
 * @return true if command accepted.
 */
bool ds18b20_write_scratchpad(int8_t th, int8_t tl, uint8_t config);

/**
 * Read the 9-byte scratchpad: temp LSB/MSB, TH, TL, config, reserved[3], CRC.
 * @param scratchpad  Buffer of at least 9 bytes.
 * @return true if data read successfully.
 */
bool ds18b20_read_scratchpad(uint8_t scratchpad[9]);

/**
 * Copy scratchpad registers (TH, TL, config) to EEPROM.
 * Takes up to 10 ms to complete.
 * @return true if command accepted.
 */
bool ds18b20_copy_scratchpad(void);

/**
 * Recall EEPROM to scratchpad (reload TH, TL, config from EEPROM).
 * @return true if command accepted.
 */
bool ds18b20_recall_eeprom(void);

/**
 * Read power supply mode: parasite (false) or external VDD (true).
 * @param external  Output flag: true=VDD, false=parasite.
 * @return true if presence detected and bit read.
 */
bool ds18b20_read_power_supply(bool *external);

#ifdef __cplusplus
}
#endif

#endif // DS18B20_H
