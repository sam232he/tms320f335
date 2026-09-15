#include "eeprom_interface.h"
#include "gpio_interface.h"

static Uint16 eeprom_slave(void)
{
    return (Uint16)(I2C_EEPROM_ADDR | ((gpio_read(I2C_A_PIN_A1) & 1U) << 1));
}

static Uint16 eeprom_range_ok(Uint16 addr, Uint16 n)
{
    Uint16 last;

    if ((n == 0U) || (n > I2C_MAX_BYTES))
    {
        return 0U;
    }
    if (addr > EEPROM_LAST)
    {
        return 0U;
    }
    last = (Uint16)(addr + n - 1U);
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

static Uint16 eeprom_same_page(Uint16 addr, Uint16 n)
{
    Uint16 last;

    last = (Uint16)(addr + n - 1U);
    return ((addr & EEPROM_PAGE_MASK) == (last & EEPROM_PAGE_MASK)) ? 1U : 0U;
}

void eeprom_init(void)
{
    gpio_write(I2C_A_PIN_A1, 0U);
    gpio_write(I2C_A_PIN_WP, 0U);
}

void eeprom_write_protect(Uint16 enable)
{
    gpio_write(I2C_A_PIN_WP, (enable != 0U) ? 1U : 0U);
}

Uint16 eeprom_write(Uint16 addr, const Uint16 *data, Uint16 n)
{
    i2c_msg_t msg;
    Uint16 i;

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

Uint16 eeprom_read(Uint16 addr, Uint16 n)
{
    i2c_msg_t msg;
    Uint16 i;

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

Uint16 eeprom_rx(Uint16 *data, Uint16 n)
{
    i2c_msg_t msg;
    Uint16 i;
    Uint16 copy_n;

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
