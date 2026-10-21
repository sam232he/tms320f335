#include <stdint.h>
#include "eeprom_interface.h"
#include "gpio_interface.h"

static uint16_t eeprom_slave(void)
{
    return (uint16_t)(I2C_EEPROM_ADDR | ((gpio_read(I2C_A_PIN_A1) & 1U) << 1));
}

static uint16_t eeprom_range_ok(uint16_t addr, uint16_t n)
{
    uint16_t last;

    if ((n == 0U) || (n > I2C_MAX_BYTES))
    {
        return 0U;
    }
    if (addr > EEPROM_LAST)
    {
        return 0U;
    }
    last = (uint16_t)(addr + n - 1U);
    if (last < addr)
    {
        return 0U;  /* 16-bit wrap */
    }
    if (last > EEPROM_LAST)
    {
        return 0U;
    }
    return 1U;
}

static uint16_t eeprom_same_page(uint16_t addr, uint16_t n)
{
    uint16_t last;

    last = (uint16_t)(addr + n - 1U);
    return ((addr & EEPROM_PAGE_MASK) == (last & EEPROM_PAGE_MASK)) ? 1U : 0U;
}

void eeprom_init(void)
{
    gpio_write(I2C_A_PIN_A1, 0U);
    gpio_write(I2C_A_PIN_WP, 0U);
}

void eeprom_write_protect(uint16_t enable)
{
    gpio_write(I2C_A_PIN_WP, (enable != 0U) ? 1U : 0U);
}

uint16_t eeprom_write(uint16_t addr, const uint16_t *data, uint16_t n)
{
    i2c_msg_t msg;
    uint16_t i;

    if (data == 0)
    {
        return 0U;
    }
    if (eeprom_range_ok(addr, n) == 0U)
    {
        return 0U;
    }
    if (eeprom_same_page(addr, n) == 0U)
    {
        return 0U;
    }
    if (gpio_read(I2C_A_PIN_WP) != 0U)
    {
        return 0U;
    }

    msg.dir = I2C_TX;
    msg.slave = eeprom_slave();
    msg.mem_addr = addr;
    msg.n = n;
    for (i = 0U; i < n; i++)
    {
        msg.data[i] = data[i] & 0xFFU;
    }
    return i2c_tx(I2C_A, &msg);
}

uint16_t eeprom_read(uint16_t addr, uint16_t n)
{
    i2c_msg_t msg;
    uint16_t i;

    if (eeprom_range_ok(addr, n) == 0U)
    {
        return 0U;
    }

    msg.dir = I2C_RX;
    msg.slave = eeprom_slave();
    msg.mem_addr = addr;
    msg.n = n;
    for (i = 0U; i < I2C_MAX_BYTES; i++)
    {
        msg.data[i] = 0U;
    }
    return i2c_tx(I2C_A, &msg);
}

uint16_t eeprom_rx(uint16_t *data, uint16_t n)
{
    i2c_msg_t msg;
    uint16_t i;
    uint16_t copy_n;

    if (data == 0)
    {
        return 0U;
    }
    if (i2c_rx(I2C_A, &msg) == 0U)
    {
        return 0U;
    }

    copy_n = msg.n;
    if (copy_n > n)
    {
        copy_n = n;
    }
    for (i = 0U; i < copy_n; i++)
    {
        data[i] = msg.data[i] & 0xFFU;
    }
    return 1U;
}
