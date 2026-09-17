#include "eeprom_test.h"
#include "../../driver_interface/timer/timer_interface.h"

#define EEPROM_TEST_ADDR            0x0100U
#define EEPROM_TEST_TWR_MS          10UL    /* CAT24C256 tWR = 5 ms */

static const Uint16 eeprom_test_tx[EEPROM_TEST_N] = {
    0x11U, 0x22U, 0x33U, 0x44U
};

volatile Uint16 eeprom_test_ok;
volatile Uint16 eeprom_test_rx[EEPROM_TEST_N];

void eeprom_test(void)
{
    Uint16 rx[EEPROM_TEST_N];
    Uint16 i;
    Uint32 t0;

    eeprom_test_ok = 0U;
    for (i = 0U; i < EEPROM_TEST_N; i++)
    {
        eeprom_test_rx[i] = 0U;
        rx[i] = 0U;
    }

    while (eeprom_write(EEPROM_TEST_ADDR, eeprom_test_tx, EEPROM_TEST_N) == 0U)
    {
        i2c_write(I2C_A);
    }

    while (i2c_pending() != 0U)
    {
        i2c_write(I2C_A);
    }

    t0 = timer_tick();
    while ((timer_tick() - t0) < EEPROM_TEST_TWR_MS)
    {
        i2c_write(I2C_A);
    }

    while (eeprom_read(EEPROM_TEST_ADDR, EEPROM_TEST_N) == 0U)
    {
        i2c_write(I2C_A);
    }

    while (eeprom_rx(rx, EEPROM_TEST_N) == 0U)
    {
        i2c_write(I2C_A);
    }

    eeprom_test_ok = 1U;
    for (i = 0U; i < EEPROM_TEST_N; i++)
    {
        eeprom_test_rx[i] = rx[i];
        if (rx[i] != eeprom_test_tx[i])
        {
            eeprom_test_ok = 0U;
        }
    }
}
