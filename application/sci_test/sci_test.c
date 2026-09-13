#include "sci_test.h"
#include "../../driver_interface/timer/timer_interface.h"

#define SCI_OBS_TX                  0U
#define SCI_OBS_RX                  1U
#define SCI_OBS_TX_N                2U
#define SCI_OBS_RX_N                3U

static Uint32 sci_test_last_tick;
static Uint16 sci_test_tx;
static Uint16 sci_cmd_test_tx;

volatile Uint16 sci_obs[4];
volatile Uint16 sci_cmd_obs[4];

static void sci_test_rx(sci_module_t mod, volatile Uint16 *obs)
{
    sci_msg_t msg;

    while (sci_rx(mod, &msg) != 0U)
    {
        obs[SCI_OBS_RX] = msg.data;
        obs[SCI_OBS_RX_N]++;
        (void)sci_tx(mod, &msg);
    }
}

void sci_test(void)
{
    Uint32 now;
    sci_msg_t msg;

    sci_test_rx(SCI, sci_obs);
    sci_test_rx(SCI_CMD, sci_cmd_obs);

    now = timer_tick();
    if ((now - sci_test_last_tick) < 200UL) /* 200 ms */
    {
        return;
    }
    sci_test_last_tick = now;

    msg.data = sci_test_tx;
    (void)sci_tx(SCI, &msg);
    sci_obs[SCI_OBS_TX] = sci_test_tx;
    sci_obs[SCI_OBS_TX_N]++;
    sci_test_tx = (Uint16)((sci_test_tx + 1U) & 0xFFU);

    msg.data = sci_cmd_test_tx;
    (void)sci_tx(SCI_CMD, &msg);
    sci_cmd_obs[SCI_OBS_TX] = sci_cmd_test_tx;
    sci_cmd_obs[SCI_OBS_TX_N]++;
    sci_cmd_test_tx = (Uint16)((sci_cmd_test_tx + 1U) & 0xFFU);
}
