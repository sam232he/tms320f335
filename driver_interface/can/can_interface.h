#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include "DSP2833x_Device.h"

typedef enum
{
    CAN_A = 0,
    CAN_B = 1
} can_module_t;

typedef enum
{
    CAN_TX = 0,
    CAN_RX = 1
} can_dir_t;

typedef enum
{
    CAN_ID_STD = 0,
    CAN_ID_EXT = 1
} can_id_type_t;

typedef struct
{
    can_module_t module;
    Uint16 mailbox;     /* 0 .. 31 */
    can_dir_t dir;
    can_id_type_t ide;
    Uint32 id;          /* 11-bit or 29-bit */
    Uint32 lam;         /* RX local acceptance mask; 1 = don't care */
    Uint16 ame;         /* 1 = use LAM */
    Uint16 dlc;         /* 0 .. 8 */
    Uint16 enable;
} can_mbox_cfg_t;

typedef struct
{
    Uint32 id;
    can_id_type_t ide;
    Uint16 dlc;
    Uint16 data[8];
    Uint16 mailbox;
} can_msg_t;

#define CAN_QUEUE_LEN       20U
#define CAN_TX_MBOX_FIRST   0U
#define CAN_TX_MBOX_COUNT   10U   /* mailboxes 0 .. 9 */

void can_init(void);
Uint16 can_tx(can_module_t mod, const can_msg_t *msg);
Uint16 can_rx(can_module_t mod, can_msg_t *msg);
void can_mbox_config(const can_mbox_cfg_t *cfg);

/*
 * Idle-loop TX: one step per call.
 * CHECK_QUEUE pops into can_write_held. The next call is FIND_MBOX
 * and retries until a mailbox 0..9 is free, then returns to CHECK_QUEUE.
 */
void can_write(can_module_t mod);

#endif /* CAN_INTERFACE_H */
