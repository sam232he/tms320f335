#include "can_send.h"
#include "can_interface.h"
#include "timer_interface.h"

void can_prepare_tx(void)
{
    static Uint32 last_tick;
    Uint32 now;
    can_msg_t msg;
    Uint16 i;

    now = timer_tick();
    if ((now - last_tick) < 200UL)  /* 200 ms */
    {
        return;
    }
    last_tick = now;

    msg.id = 0x123UL;
    msg.ide = CAN_ID_STD;
    msg.dlc = 8U;
    msg.mailbox = 0U;
    for (i = 0U; i < 8U; i++)
    {
        msg.data[i] = (Uint16)(i + 1U);
    }

    (void)can_tx(CAN_A, &msg);
}
