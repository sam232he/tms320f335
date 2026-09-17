#include "can_send.h"
#include "can_interface.h"

void can_test(void)
{
    can_msg_t msg;

    while (can_rx(CAN_B, &msg) != 0U)
    {
        if (msg.ide == CAN_ID_EXT)
        {
            msg.id = (msg.id + 1UL) & 0x1FFFFFFFUL;
        }
        else
        {
            msg.id = (msg.id + 1UL) & 0x7FFUL;
        }
        (void)can_tx(CAN_B, &msg);
    }

    can_write(CAN_B);
}
