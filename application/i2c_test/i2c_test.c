#include "i2c_test.h"
#include "../../driver_interface/timer/timer_interface.h"

#define I2C_TEST_ADDR               0x0030U
#define I2C_TEST_N                  4U
#define I2C_OBS_TX0                 0U
#define I2C_OBS_RX0                 1U
#define I2C_OBS_PASS                2U
#define I2C_OBS_FAIL                3U
#define I2C_OBS_TX_N                4U
#define I2C_OBS_RX_N                5U
#define I2C_OBS_STATE               6U
#define I2C_OBS_CMP                 7U

typedef enum
{
    I2C_TEST_ST_WAIT_GAP = 0,
    I2C_TEST_ST_WRITE,
    I2C_TEST_ST_WAIT_WRITE,
    I2C_TEST_ST_WAIT_PROG,
    I2C_TEST_ST_READ,
    I2C_TEST_ST_WAIT_RX
} i2c_test_st_t;

static i2c_test_st_t i2c_test_st;
static Uint32 i2c_test_last_tick;
static Uint16 i2c_test_seed;
static i2c_msg_t i2c_test_tx_msg;

volatile Uint16 i2c_test_data[8];
volatile Uint16 i2c_test_tx_data[4];
volatile Uint16 i2c_test_rx_data[4];

static void i2c_test_fill_write(void)
{
    Uint16 i;

    i2c_test_tx_msg.dir = I2C_TX;
    i2c_test_tx_msg.slave = I2C_EEPROM_ADDR;
    i2c_test_tx_msg.mem_addr = I2C_TEST_ADDR;
    i2c_test_tx_msg.n = I2C_TEST_N;
    for (i = 0U; i < I2C_TEST_N; i++)
    {
        i2c_test_tx_msg.data[i] = (Uint16)((i2c_test_seed + i) & 0xFFU);
        i2c_test_tx_data[i] = i2c_test_tx_msg.data[i];
    }
    i2c_test_data[I2C_OBS_TX0] = i2c_test_tx_msg.data[0];
}

static void i2c_test_compare(const i2c_msg_t *rx)
{
    Uint16 i;
    Uint16 ok;

    ok = 1U;
    for (i = 0U; i < I2C_TEST_N; i++)
    {
        i2c_test_rx_data[i] = rx->data[i];
        if (rx->data[i] != i2c_test_tx_msg.data[i])
        {
            ok = 0U;
        }
    }
    i2c_test_data[I2C_OBS_RX0] = rx->data[0];
    i2c_test_data[I2C_OBS_CMP] = ok;
    if (ok != 0U)
    {
        i2c_test_data[I2C_OBS_PASS]++;
    }
    else
    {
        i2c_test_data[I2C_OBS_FAIL]++;
    }
}

void i2c_test(void)
{
    Uint32 now;
    i2c_msg_t msg;
    Uint16 i;

    now = timer_tick();
    i2c_test_data[I2C_OBS_STATE] = (Uint16)i2c_test_st;

    switch (i2c_test_st)
    {
        case I2C_TEST_ST_WAIT_GAP:
            if ((now - i2c_test_last_tick) < 500UL) /* 500 ms between cycles */
            {
                break;
            }
            i2c_test_st = I2C_TEST_ST_WRITE;
            break;

        case I2C_TEST_ST_WRITE:
            i2c_test_fill_write();
            if (i2c_tx(I2C_A, &i2c_test_tx_msg) == 0U)
            {
                break;
            }
            i2c_test_data[I2C_OBS_TX_N]++;
            i2c_test_last_tick = now;
            i2c_test_st = I2C_TEST_ST_WAIT_WRITE;
            break;

        case I2C_TEST_ST_WAIT_WRITE:
            if (i2c_idle() != 0U)
            {
                i2c_test_last_tick = now;
                i2c_test_st = I2C_TEST_ST_WAIT_PROG;
                break;
            }
            if ((now - i2c_test_last_tick) >= 100UL)
            {
                i2c_test_data[I2C_OBS_FAIL]++;
                i2c_test_data[I2C_OBS_CMP] = 0U;
                i2c_test_last_tick = now;
                i2c_test_st = I2C_TEST_ST_WAIT_GAP;
            }
            break;

        case I2C_TEST_ST_WAIT_PROG:
            if ((now - i2c_test_last_tick) < 10UL) /* CAT24C256 tWC = 5 ms */
            {
                break;
            }
            i2c_test_st = I2C_TEST_ST_READ;
            break;

        case I2C_TEST_ST_READ:
            msg.dir = I2C_RX;
            msg.slave = I2C_EEPROM_ADDR;
            msg.mem_addr = I2C_TEST_ADDR;
            msg.n = I2C_TEST_N;
            for (i = 0U; i < I2C_TEST_N; i++)
            {
                msg.data[i] = 0U;
            }
            if (i2c_tx(I2C_A, &msg) == 0U)
            {
                break;
            }
            i2c_test_last_tick = now;
            i2c_test_st = I2C_TEST_ST_WAIT_RX;
            break;

        case I2C_TEST_ST_WAIT_RX:
            if (i2c_rx(I2C_A, &msg) != 0U)
            {
                i2c_test_data[I2C_OBS_RX_N]++;
                i2c_test_compare(&msg);
                i2c_test_seed = (Uint16)((i2c_test_seed + 1U) & 0xFFU);
                i2c_test_last_tick = now;
                i2c_test_st = I2C_TEST_ST_WAIT_GAP;
                break;
            }
            if ((now - i2c_test_last_tick) >= 100UL) /* 100 ms timeout */
            {
                i2c_test_data[I2C_OBS_FAIL]++;
                i2c_test_data[I2C_OBS_CMP] = 0U;
                i2c_test_last_tick = now;
                i2c_test_st = I2C_TEST_ST_WAIT_GAP;
            }
            break;

        default:
            i2c_test_st = I2C_TEST_ST_WAIT_GAP;
            break;
    }

    i2c_write(I2C_A);
}
