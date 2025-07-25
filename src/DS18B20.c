#include "DS18B20.h"
#include "onewire.h"
#include <pico/stdlib.h>

bool ds18b20_read_rom(uint8_t rom[8]) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0x33);  // READ ROM
    for (int i = 0; i < 8; i++) {
        rom[i] = ow_read_byte();
    }
    return true;
}

bool ds18b20_match_rom(const uint8_t rom[8]) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0x55);  // MATCH ROM
    for (int i = 0; i < 8; i++) {
        ow_write_byte(rom[i]);
    }
    return true;
}

bool ds18b20_skip_rom(void) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0xCC);  // SKIP ROM
    return true;
}

bool ds18b20_alarm_search(void) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0xEC);  // ALARM SEARCH
    return true;
}

bool ds18b20_start_conversion(void) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0xCC);  // SKIP ROM
    ow_write_byte(0x44);  // CONVERT T
    return true;
}

bool ds18b20_write_scratchpad(int8_t th, int8_t tl, uint8_t config) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0xCC);  // SKIP ROM
    ow_write_byte(0x4E);  // WRITE SCRATCHPAD
    ow_write_byte((uint8_t)th);
    ow_write_byte((uint8_t)tl);
    ow_write_byte(config);
    return true;
}

bool ds18b20_read_scratchpad(uint8_t scratchpad[9]) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0xCC);  // SKIP ROM
    ow_write_byte(0xBE);  // READ SCRATCHPAD
    for (int i = 0; i < 9; i++) {
        scratchpad[i] = ow_read_byte();
    }
    return true;
}

bool ds18b20_copy_scratchpad(void) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0xCC);  // SKIP ROM
    ow_write_byte(0x48);  // COPY SCRATCHPAD
    return true;
}

bool ds18b20_recall_eeprom(void) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0xCC);  // SKIP ROM
    ow_write_byte(0xB8);  // RECALL EEPROM
    return true;
}

bool ds18b20_read_power_supply(bool *external) {
    if (!ow_reset_pulse()) {
        return false;
    }
    ow_write_byte(0xCC);  // SKIP ROM
    ow_write_byte(0xB4);  // READ POWER SUPPLY
    *external = ow_read_bit();
    return true;
}
