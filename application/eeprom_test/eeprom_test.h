#ifndef EEPROM_TEST_H
#define EEPROM_TEST_H

#include "../../driver_interface/eeprom/eeprom_interface.h"

#define EEPROM_TEST_N               4U

/*
 * One-shot write/read of 4 bytes at 0x0100. Blocks until done.
 * eeprom_test_ok: 1 = match, 0 = mismatch.
 */
extern volatile Uint16 eeprom_test_ok;
extern volatile Uint16 eeprom_test_rx[EEPROM_TEST_N];

void eeprom_test(void);

#endif /* EEPROM_TEST_H */
