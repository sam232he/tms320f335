#ifndef EEPROM_TEST_H
#define EEPROM_TEST_H

#include "../../driver_interface/eeprom/eeprom_interface.h"

#define EEPROM_TEST_N               8U

/*
 * Write 8 bytes through eeprom_write(), then read them back with
 * eeprom_read() / eeprom_rx(). CAT24C256 at 0x0100 (page-aligned).
 * i2c_write(I2C_A) runs every call. After the write xfer and queue
 * go idle, wait 10 ms for tWR before the read. Timeouts drain I2C
 * (pending xfer/queue + leftover RX) before the next cycle.
 *
 * eeprom_test_data:
 *   [0]=last TX[0], [1]=last RX[0], [2]=pass, [3]=fail,
 *   [4]=write count, [5]=read count, [6]=state, [7]=last compare (1=match).
 * eeprom_test_tx_data / eeprom_test_rx_data are the 8-byte payloads.
 */
extern volatile Uint16 eeprom_test_data[8];
extern volatile Uint16 eeprom_test_tx_data[EEPROM_TEST_N];
extern volatile Uint16 eeprom_test_rx_data[EEPROM_TEST_N];

void eeprom_test(void);

#endif /* EEPROM_TEST_H */
