#include "eeprom_test.h"
#include "../../driver_interface/timer/timer_interface.h"

#define EEPROM_TEST_ADDR            0x0100U
#define EEPROM_OBS_TX0              0U
#define EEPROM_OBS_RX0              1U
#define EEPROM_OBS_PASS             2U
#define EEPROM_OBS_FAIL             3U
#define EEPROM_OBS_TX_N             4U
#define EEPROM_OBS_RX_N             5U
#define EEPROM_OBS_STATE            6U
#define EEPROM_OBS_CMP              7U

typedef enum
{
    EEPROM_TEST_ST_WAIT_GAP = 0,
    EEPROM_TEST_ST_WRITE,
    EEPROM_TEST_ST_WAIT_WRITE,
    EEPROM_TEST_ST_WAIT_PROG,
    EEPROM_TEST_ST_READ,
    EEPROM_TEST_ST_WAIT_RX,
    EEPROM_TEST_ST_WAIT_DRAIN
} eeprom_test_st_t;

static eeprom_test_st_t eeprom_test_st;
static Uint32 eeprom_test_last_tick;
static Uint16 eeprom_test_seed;
static Uint16 eeprom_test_tx[EEPROM_TEST_N];

volatile Uint16 eeprom_test_data[8];
volatile Uint16 eeprom_test_tx_data[EEPROM_TEST_N];
volatile Uint16 eeprom_test_rx_data[EEPROM_TEST_N];

static void eeprom_test_fill_write(void)
{
    Uint16 i;

    for (i = 0U; i < EEPROM_TEST_N; i++)
    {
        eeprom_test_tx[i] = (Uint16)((eeprom_test_seed + i) & 0xFFU);
        eeprom_test_tx_data[i] = eeprom_test_tx[i];
    }
    eeprom_test_data[EEPROM_OBS_TX0] = eeprom_test_tx[0];
}

static void eeprom_test_drain_rx(void)
{
    Uint16 dump[EEPROM_TEST_N];

    while (eeprom_rx(dump, EEPROM_TEST_N) != 0U)
    {
    }
}

static void eeprom_test_fail(Uint32 now)
{
    Uint16 i;

    eeprom_test_data[EEPROM_OBS_FAIL]++;
    eeprom_test_data[EEPROM_OBS_CMP] = 0U;
    for (i = 0U; i < EEPROM_TEST_N; i++)
    {
        eeprom_test_rx_data[i] = 0U;
    }
    eeprom_test_last_tick = now;
    eeprom_test_st = EEPROM_TEST_ST_WAIT_DRAIN;
}

static void eeprom_test_compare(const Uint16 *rx)
{
    Uint16 i;
    Uint16 ok;

    ok = 1U;
    for (i = 0U; i < EEPROM_TEST_N; i++)
    {
        eeprom_test_rx_data[i] = rx[i];
        if (rx[i] != eeprom_test_tx[i])
        {
            ok = 0U;
        }
    }
    eeprom_test_data[EEPROM_OBS_RX0] = rx[0];
    eeprom_test_data[EEPROM_OBS_CMP] = ok;
    if (ok != 0U)
    {
        eeprom_test_data[EEPROM_OBS_PASS]++;
    }
    else
    {
        eeprom_test_data[EEPROM_OBS_FAIL]++;
    }
}

void eeprom_test(void)
{
    Uint32 now;
    Uint16 rx[EEPROM_TEST_N];
    Uint16 i;

    now = timer_tick();
    eeprom_test_data[EEPROM_OBS_STATE] = (Uint16)eeprom_test_st;

    switch (eeprom_test_st)
    {
        case EEPROM_TEST_ST_WAIT_GAP:
            eeprom_test_drain_rx();
            if ((now - eeprom_test_last_tick) < 500UL) /* 500 ms between cycles */
            {
                break;
            }
            eeprom_test_last_tick = now;
            eeprom_test_st = EEPROM_TEST_ST_WRITE;
            break;

        case EEPROM_TEST_ST_WRITE:
            eeprom_test_fill_write();
            if (eeprom_write(EEPROM_TEST_ADDR, eeprom_test_tx, EEPROM_TEST_N) == 0U)
            {
                if ((now - eeprom_test_last_tick) >= 100UL)
                {
                    eeprom_test_fail(now);
                }
                break;
            }
            eeprom_test_data[EEPROM_OBS_TX_N]++;
            eeprom_test_last_tick = now;
            eeprom_test_st = EEPROM_TEST_ST_WAIT_WRITE;
            break;

        case EEPROM_TEST_ST_WAIT_WRITE:
            if (i2c_pending() == 0U)
            {
                eeprom_test_last_tick = now;
                eeprom_test_st = EEPROM_TEST_ST_WAIT_PROG;
                break;
            }
            if ((now - eeprom_test_last_tick) >= 100UL)
            {
                eeprom_test_fail(now);
            }
            break;

        case EEPROM_TEST_ST_WAIT_PROG:
            if ((now - eeprom_test_last_tick) < 10UL) /* CAT24C256 tWR = 5 ms */
            {
                break;
            }
            eeprom_test_last_tick = now;
            eeprom_test_st = EEPROM_TEST_ST_READ;
            break;

        case EEPROM_TEST_ST_READ:
            if (eeprom_read(EEPROM_TEST_ADDR, EEPROM_TEST_N) == 0U)
            {
                if ((now - eeprom_test_last_tick) >= 100UL)
                {
                    eeprom_test_fail(now);
                }
                break;
            }
            eeprom_test_last_tick = now;
            eeprom_test_st = EEPROM_TEST_ST_WAIT_RX;
            break;

        case EEPROM_TEST_ST_WAIT_RX:
            for (i = 0U; i < EEPROM_TEST_N; i++)
            {
                rx[i] = 0xFFFFU;
            }
            if (eeprom_rx(rx, EEPROM_TEST_N) != 0U)
            {
                eeprom_test_data[EEPROM_OBS_RX_N]++;
                eeprom_test_compare(rx);
                eeprom_test_seed = (Uint16)((eeprom_test_seed + 1U) & 0xFFU);
                eeprom_test_last_tick = now;
                eeprom_test_st = EEPROM_TEST_ST_WAIT_GAP;
                break;
            }
            if ((now - eeprom_test_last_tick) >= 100UL)
            {
                eeprom_test_fail(now);
            }
            break;

        case EEPROM_TEST_ST_WAIT_DRAIN:
            if (i2c_pending() != 0U)
            {
                break;
            }
            eeprom_test_drain_rx();
            eeprom_test_last_tick = now;
            eeprom_test_st = EEPROM_TEST_ST_WAIT_GAP;
            break;

        default:
            eeprom_test_st = EEPROM_TEST_ST_WAIT_DRAIN;
            break;
    }

    i2c_write(I2C_A);
}
