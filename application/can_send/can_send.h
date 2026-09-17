#ifndef CAN_SEND_H
#define CAN_SEND_H

/*
 * Echo RX on eCAN-B: same payload, ID + 1. Schematic DSP_CAN is
 * GPIO16/17 CANTXB/CANRXB. can_write(CAN_B) runs every call.
 */
void can_test(void);

#endif /* CAN_SEND_H */
