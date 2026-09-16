#ifndef EEPROM_INTERFACE_H
#define EEPROM_INTERFACE_H

#include "i2c_interface.h"

/*
 * CAT24C256WI-GT3 on I2C-A. Pin mux is in gpio_interface.
 * Protocol: datasheets/CAT24C256WI-GT3.md (onsemi CAT24C256/D Rev. 16).
 *
 *   EEPROM_SDA  GPIO32/SDAA  pin 74
 *   EEPROM_SCL  GPIO33/SCLA  pin 75
 *   EEPROM_A1   GPIO22       pin 66  (A0/A2 assumed GND)
 *   EEPROM_WP   GPIO23       pin 67
 *
 * 32,768 bytes, 64-byte pages, 16-bit address (A15 don't-care).
 * Slave = I2C_EEPROM_ADDR | (A1 << 1)  -> 0x50 when A1=0.
 *
 * One queued access is 1..I2C_MAX_BYTES (14). I2C TX FIFO is 16 bytes
 * minus 2 address bytes. Writes that would wrap a 64-byte page are
 * refused; sequential reads may cross a page.
 *
 * WP high rejects the chip's first data byte (NACK). eeprom_write()
 * refuses that case so I2C NACK retry is not treated as tWR busy.
 *
 * tWR (5 ms max) is handled by i2c_write() NACK retry. No delay here.
 * Idle pump stays i2c_write(I2C_A). eeprom_init() is called from system_init.
 *
 * onsemi ECC: writes in multiples of 4 bytes get full endurance.
 * Not enforced.
 */

#define EEPROM_SIZE                 0x8000U /* 32768 bytes */
#define EEPROM_LAST                 0x7FFFU
#define EEPROM_PAGE_SIZE            64U
#define EEPROM_PAGE_MASK            0xFFC0U /* ~63, 64-byte page base */

void eeprom_init(void);
void eeprom_write_protect(Uint16 enable);   /* 0 = writes allowed, 1 = WP high */

Uint16 eeprom_write(Uint16 addr, const Uint16 *data, Uint16 n);
Uint16 eeprom_read(Uint16 addr, Uint16 n);
Uint16 eeprom_rx(Uint16 *data, Uint16 n);   /* pop completed read; 1 = ok */

#endif /* EEPROM_INTERFACE_H */
