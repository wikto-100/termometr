#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdbool.h>
#include <stdint.h>
#include <pico/stdlib.h>

// 1-Wire timing constants (in microseconds)
#define ONEWIRE_RSTL_US      480   // Reset low time
#define ONEWIRE_PDH_US       60    // Presence-detect high time before sample
#define ONEWIRE_SLOT_US      70    // Total time-slot for write or read
#define ONEWIRE_RECOVERY_US   1    // Recovery time between time-slots

// Shared bus pin across translation units
typedef uint8_t ow_pin_t;
extern ow_pin_t ow_pin;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the 1-Wire bus on the specified GPIO pin.
 * Must be called once before any other onewire functions.
 */
void ow_init(ow_pin_t pin);

/**
 * Drive the bus low (master or slave drives the line low).
 */
void ow_drive_low(void);

/**
 * Release the bus (float high via pull-up resistor).
 */
void ow_release(void);

/**
 * Perform the Reset and Presence-detect sequence.
 * Returns true if a device responded with a presence pulse.
 */
bool ow_reset_pulse(void);

/**
 * Write a single bit (LSB first) on the 1-Wire bus.
 * @param bit 0 or 1 to write.
 */
void ow_write_bit(bool bit);

/**
 * Read a single bit (LSB first) from the 1-Wire bus.
 * @return The bit read (0 or 1).
 */
bool ow_read_bit(void);

/**
 * Write a full byte (LSB first) on the 1-Wire bus.
 * @param byte The byte to send.
 */
void ow_write_byte(uint8_t byte);

/**
 * Read a full byte (LSB first) from the 1-Wire bus.
 * @return The byte read.
 */
uint8_t ow_read_byte(void);

#ifdef __cplusplus
}
#endif

#endif // ONEWIRE_H
