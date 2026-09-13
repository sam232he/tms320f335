#ifndef CAN_SEND_H
#define CAN_SEND_H

/*
 * Queue one standard frame (ID 0x123) on eCAN-A TX every 200 ms.
 * Hardware send is can_write(CAN_A) in the idle loop.
 */
void can_prepare_tx(void);

#endif /* CAN_SEND_H */
