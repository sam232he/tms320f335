#include "sci_test.h"
#include "../../driver_interface/timer/timer_interface.h"

#define SCI_OBS_TX                  0U
#define SCI_OBS_RX                  1U
#define SCI_OBS_TX_N                2U
#define SCI_OBS_RX_N                3U

typedef struct
{
    sci_module_t mod;
    volatile Uint16 *data;
    Uint16 tx;
} sci_test_ch_t;

static Uint32 sci_test_last_tick;

volatile Uint16 sci_a_test_data[4];
volatile Uint16 sci_b_test_data[4];
volatile Uint16 sci_c_test_data[4];

static sci_test_ch_t sci_test_ch[SCI_COUNT] =
{
    { SCI_A, sci_a_test_data, 0U },
    { SCI_B, sci_b_test_data, 0U },
    { SCI_C, sci_c_test_data, 0U }
};

static void sci_test_rx(sci_test_ch_t *ch)
{
    sci_msg_t msg;

    while (sci_rx(ch->mod, &msg) != 0U)
    {
        ch->data[SCI_OBS_RX] = msg.data;
        ch->data[SCI_OBS_RX_N]++;
        (void)sci_tx(ch->mod, &msg);
    }
}

void sci_test(void)
{
    Uint32 now;
    sci_msg_t msg;
    Uint16 i;

    for (i = 0U; i < SCI_COUNT; i++)
    {
        sci_test_rx(&sci_test_ch[i]);
    }

    now = timer_tick();
    if ((now - sci_test_last_tick) >= 200UL) /* 200 ms */
    {
        sci_test_last_tick = now;

        for (i = 0U; i < SCI_COUNT; i++)
        {
            msg.data = sci_test_ch[i].tx;
            (void)sci_tx(sci_test_ch[i].mod, &msg);
            sci_test_ch[i].data[SCI_OBS_TX] = sci_test_ch[i].tx;
            sci_test_ch[i].data[SCI_OBS_TX_N]++;
            sci_test_ch[i].tx = (Uint16)((sci_test_ch[i].tx + 1U) & 0xFFU);
        }
    }

    sci_write(SCI_A);
    sci_write(SCI_B);
    sci_write(SCI_C);
}
