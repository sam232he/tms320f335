#ifndef I2C_TEST_H
#define I2C_TEST_H

#include "../../driver_interface/i2c/i2c_interface.h"

/*
 * Write 4 bytes to CAT24C256 at 0x0030, wait out tWC, read them back.
 * i2c_test_data:
 *   [0]=last TX[0], [1]=last RX[0], [2]=pass, [3]=fail,
 *   [4]=write count, [5]=read count, [6]=state, [7]=last compare.
 * i2c_test_tx_data / i2c_test_rx_data are the 4-byte payloads.
 * i2c_write(I2C_A) runs every call.
 */
extern volatile Uint16 i2c_test_data[8];
extern volatile Uint16 i2c_test_tx_data[4];
extern volatile Uint16 i2c_test_rx_data[4];

void i2c_test(void);

#endif /* I2C_TEST_H */
