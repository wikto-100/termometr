#include "onewire.h"
#include <pico/stdlib.h>

// Define the pin variable shared by all translation units
uint8_t ow_pin = 0;

// Initialize 1-Wire on specified GPIO pin
void ow_init(uint8_t pin)
{
    ow_pin = pin;
    gpio_init(ow_pin);
    gpio_pull_up(ow_pin);
    gpio_set_dir(ow_pin, GPIO_IN);
}

// Drive the bus low (master/slave drives low)
void ow_drive_low(void)
{
    gpio_set_dir(ow_pin, GPIO_OUT);
    gpio_put(ow_pin, 0);
}

// Release the bus (float high via pull-up)
void ow_release(void)
{
    gpio_set_dir(ow_pin, GPIO_IN);
}

// Reset pulse + presence detect: returns true if device responds
bool ow_reset_pulse(void)
{
    ow_drive_low();                       // 1) Reset low
    sleep_us(ONEWIRE_RSTL_US);
    ow_release();                         // 2) Release bus
    sleep_us(ONEWIRE_PDH_US);             //    Wait for presence-high window
    bool presence = (gpio_get(ow_pin) == 0);
    sleep_us(ONEWIRE_RSTL_US - ONEWIRE_PDH_US); // 4) Recovery to complete reset
    return presence;
}

// Write a single bit (LSB first)
void ow_write_bit(bool bit)
{
    ow_drive_low();                       // start slot
    if (bit)
    {
        // write '1'
        sleep_us(6);
        ow_release();                    // release early
        sleep_us(ONEWIRE_SLOT_US - 6);
    }
    else
    {
        // write '0'
        sleep_us(ONEWIRE_SLOT_US);
        ow_release();                    // release after full slot
    }
    sleep_us(ONEWIRE_RECOVERY_US);
}

// Read a single bit (LSB first)
bool ow_read_bit(void)
{
    bool bit;
    ow_drive_low();                       // start slot
    sleep_us(6);
    ow_release();                         // release for read
    sleep_us(15);                         // wait for data valid
    bit = gpio_get(ow_pin);
    sleep_us(ONEWIRE_SLOT_US - 15);       // complete slot
    sleep_us(ONEWIRE_RECOVERY_US);
    return bit;
}

// Write a full byte (LSB first)
void ow_write_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        ow_write_bit(byte & 0x01);
        byte >>= 1;
    }
}

// Read a full byte (LSB first)
uint8_t ow_read_byte(void)
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        if (ow_read_bit())
            byte |= (1 << i);
    }
    return byte;
}
