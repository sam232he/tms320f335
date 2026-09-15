#ifndef CAN_SEND_H
#define CAN_SEND_H

/*
 * Queue one standard frame (ID 0x123) on eCAN-B TX every 200 ms.
 * Schematic DSP_CAN is GPIO16/17 CANTXB/CANRXB.
 * can_write(CAN_B) runs every call.
 */
void can_test(void);

#endif /* CAN_SEND_H */
