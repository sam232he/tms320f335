#include "can_interface.h"
#include "DSP28x_Project.h"

typedef enum
{
    CAN_WRITE_ST_CHECK_QUEUE = 0,
    CAN_WRITE_ST_FIND_MBOX
} can_write_state_t;

static can_write_state_t can_write_state[2];
static can_msg_t can_write_held[2];
static can_msg_t can_q_buf[2][2][CAN_QUEUE_LEN];
static Uint16 can_q_head[2][2];
static Uint16 can_q_tail[2][2];
static Uint16 can_q_n[2][2];

static Uint16 can_q_push(can_module_t mod, can_dir_t dir, const can_msg_t *msg)
{
    Uint16 m;
    Uint16 d;

    if ((msg == 0) || ((mod != CAN_A) && (mod != CAN_B)))
    {
        return 0U;
    }
    if ((dir != CAN_TX) && (dir != CAN_RX))
    {
        return 0U;
    }

    m = (Uint16)mod;
    d = (Uint16)dir;
    if (can_q_n[m][d] >= CAN_QUEUE_LEN)
    {
        return 0U;
    }

    can_q_buf[m][d][can_q_head[m][d]] = *msg;
    can_q_head[m][d]++;
    if (can_q_head[m][d] >= CAN_QUEUE_LEN)
    {
        can_q_head[m][d] = 0U;
    }
    can_q_n[m][d]++;
    return 1U;
}

static Uint16 can_q_pop(can_module_t mod, can_dir_t dir, can_msg_t *msg)
{
    Uint16 m;
    Uint16 d;

    if ((msg == 0) || ((mod != CAN_A) && (mod != CAN_B)))
    {
        return 0U;
    }
    if ((dir != CAN_TX) && (dir != CAN_RX))
    {
        return 0U;
    }

    m = (Uint16)mod;
    d = (Uint16)dir;
    if (can_q_n[m][d] == 0U)
    {
        return 0U;
    }

    *msg = can_q_buf[m][d][can_q_tail[m][d]];
    can_q_tail[m][d]++;
    if (can_q_tail[m][d] >= CAN_QUEUE_LEN)
    {
        can_q_tail[m][d] = 0U;
    }
    can_q_n[m][d]--;
    return 1U;
}

Uint16 can_tx(can_module_t mod, const can_msg_t *msg)
{
    return can_q_push(mod, CAN_TX, msg);
}

Uint16 can_rx(can_module_t mod, can_msg_t *msg)
{
    return can_q_pop(mod, CAN_RX, msg);
}

static volatile struct ECAN_REGS *can_regs(can_module_t mod)
{
    if (mod == CAN_B)
    {
        return &ECanbRegs;
    }
    return &ECanaRegs;
}

static volatile struct ECAN_MBOXES *can_mboxes(can_module_t mod)
{
    if (mod == CAN_B)
    {
        return &ECanbMboxes;
    }
    return &ECanaMboxes;
}

static volatile struct LAM_REGS *can_lams(can_module_t mod)
{
    if (mod == CAN_B)
    {
        return &ECanbLAMRegs;
    }
    return &ECanaLAMRegs;
}

void can_mbox_config(const can_mbox_cfg_t *cfg)
{
    volatile struct ECAN_REGS *regs;
    volatile struct MBOX *mbox;
    volatile union CANLAM_REG *lam;
    struct ECAN_REGS shadow;
    Uint32 mask;
    Uint32 msgid;

    if ((cfg == 0) || (cfg->mailbox > 31U))
    {
        return;
    }
    if ((cfg->module != CAN_A) && (cfg->module != CAN_B))
    {
        return;
    }

    regs = can_regs(cfg->module);
    mbox = &can_mboxes(cfg->module)->MBOX0 + cfg->mailbox;
    lam = &can_lams(cfg->module)->LAM0 + cfg->mailbox;
    mask = 1UL << cfg->mailbox;

    /* TI: CANME must be 0 before writing MSGID. */
    shadow.CANME.all = regs->CANME.all;
    shadow.CANME.all &= ~mask;
    regs->CANME.all = shadow.CANME.all;

    if (cfg->ide == CAN_ID_EXT)
    {
        msgid = cfg->id & 0x1FFFFFFFUL;
        msgid |= 0x80000000UL;
    }
    else
    {
        msgid = (cfg->id & 0x7FFUL) << 18;
    }

    if (cfg->ame != 0U)
    {
        msgid |= 0x40000000UL;
    }

    mbox->MSGID.all = msgid;
    mbox->MSGCTRL.all = (Uint32)((cfg->dlc > 8U) ? 8U : cfg->dlc);
    lam->all = cfg->lam;

    shadow.CANMD.all = regs->CANMD.all;
    if (cfg->dir == CAN_RX)
    {
        shadow.CANMD.all |= mask;
    }
    else
    {
        shadow.CANMD.all &= ~mask;
    }
    regs->CANMD.all = shadow.CANMD.all;

    EALLOW;
    shadow.CANMIL.all = regs->CANMIL.all;
    shadow.CANMIL.all &= ~mask;          /* RX/TX mailbox interrupt on line 0 */
    regs->CANMIL.all = shadow.CANMIL.all;

    shadow.CANMIM.all = regs->CANMIM.all;
    if ((cfg->dir == CAN_RX) && (cfg->enable != 0U))
    {
        shadow.CANMIM.all |= mask;
    }
    else
    {
        shadow.CANMIM.all &= ~mask;
    }
    regs->CANMIM.all = shadow.CANMIM.all;
    EDIS;

    if (cfg->enable != 0U)
    {
        shadow.CANME.all = regs->CANME.all;
        shadow.CANME.all |= mask;
        regs->CANME.all = shadow.CANME.all;
    }
}

/*
 * SPRUEU1: DBO=0 stores payload starting at the MSB of MDL, ending at the LSB of MDH.
 * Drain every RMP bit (TRM: MIV only reports the highest pending mailbox).
 * Clear RMP with a 32-bit write of the one-hot mask (SPRA876 / TI E2E). Never use
 * CANRMP.bit.RMPn. Always clear RMP even if the queue is full.
 */
static void can_rx_to_queue(can_module_t mod)
{
    volatile struct ECAN_REGS *regs;
    volatile struct MBOX *mbox;
    struct ECAN_REGS shadow;
    Uint32 rmp;
    Uint32 mask;
    Uint32 msgid;
    Uint32 mdl;
    Uint32 mdh;
    Uint16 i;
    Uint16 dlc;
    can_msg_t msg;

    regs = can_regs(mod);
    rmp = regs->CANRMP.all;
    if (rmp == 0UL)
    {
        return;
    }

    for (i = 0U; i < 32U; i++)
    {
        mask = 1UL << i;
        if ((rmp & mask) == 0UL)
        {
            continue;
        }

        mbox = &can_mboxes(mod)->MBOX0 + i;
        msgid = mbox->MSGID.all;
        dlc = (Uint16)(mbox->MSGCTRL.all & 0x0FUL);
        mdl = mbox->MDL.all;
        mdh = mbox->MDH.all;

        if (dlc > 8U)
        {
            dlc = 8U;
        }

        msg.mailbox = i;
        msg.dlc = dlc;
        if ((msgid & 0x80000000UL) != 0UL)
        {
            msg.ide = CAN_ID_EXT;
            msg.id = msgid & 0x1FFFFFFFUL;
        }
        else
        {
            msg.ide = CAN_ID_STD;
            msg.id = (msgid >> 18) & 0x7FFUL;
        }

        msg.data[0] = (Uint16)((mdl >> 24) & 0x00FFUL);
        msg.data[1] = (Uint16)((mdl >> 16) & 0x00FFUL);
        msg.data[2] = (Uint16)((mdl >> 8) & 0x00FFUL);
        msg.data[3] = (Uint16)(mdl & 0x00FFUL);
        msg.data[4] = (Uint16)((mdh >> 24) & 0x00FFUL);
        msg.data[5] = (Uint16)((mdh >> 16) & 0x00FFUL);
        msg.data[6] = (Uint16)((mdh >> 8) & 0x00FFUL);
        msg.data[7] = (Uint16)(mdh & 0x00FFUL);

        (void)can_q_push(mod, CAN_RX, &msg);

        shadow.CANRMP.all = mask;
        regs->CANRMP.all = shadow.CANRMP.all;
    }
}

interrupt void can_rx_isr_a(void)
{
    can_rx_to_queue(CAN_A);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

interrupt void can_rx_isr_b(void)
{
    can_rx_to_queue(CAN_B);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

static void can_rx_int_enable(void)
{
    struct ECAN_REGS shadow;

    EALLOW;
    PieVectTable.ECAN0INTA = &can_rx_isr_a;
    PieVectTable.ECAN0INTB = &can_rx_isr_b;

    shadow.CANGIM.all = ECanaRegs.CANGIM.all;
    shadow.CANGIM.bit.I0EN = 1;
    ECanaRegs.CANGIM.all = shadow.CANGIM.all;

    shadow.CANGIM.all = ECanbRegs.CANGIM.all;
    shadow.CANGIM.bit.I0EN = 1;
    ECanbRegs.CANGIM.all = shadow.CANGIM.all;
    EDIS;

    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;   /* ECAN0INTA */
    PieCtrlRegs.PIEIER9.bit.INTx7 = 1;   /* ECAN0INTB */
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
    IER |= M_INT9;
}

static Uint32 can_pack_msgid(const can_msg_t *msg)
{
    Uint32 msgid;

    if (msg->ide == CAN_ID_EXT)
    {
        msgid = msg->id & 0x1FFFFFFFUL;
        msgid |= 0x80000000UL;
    }
    else
    {
        msgid = (msg->id & 0x7FFUL) << 18;
    }
    return msgid;
}

static Uint16 can_tx_mbox_free(can_module_t mod, Uint16 mailbox)
{
    volatile struct ECAN_REGS *regs;
    struct ECAN_REGS shadow;
    Uint32 mask;

    regs = can_regs(mod);
    mask = 1UL << mailbox;

    shadow.CANTRS.all = regs->CANTRS.all;
    if ((shadow.CANTRS.all & mask) != 0UL)
    {
        return 0U;
    }

    shadow.CANTA.all = regs->CANTA.all;
    if ((shadow.CANTA.all & mask) != 0UL)
    {
        shadow.CANTA.all = mask;
        regs->CANTA.all = shadow.CANTA.all;
    }
    return 1U;
}

static void can_tx_mbox_load(can_module_t mod, Uint16 mailbox, const can_msg_t *msg)
{
    volatile struct ECAN_REGS *regs;
    volatile struct MBOX *mbox;
    struct ECAN_REGS shadow;
    Uint32 mask;
    Uint16 dlc;

    regs = can_regs(mod);
    mbox = &can_mboxes(mod)->MBOX0 + mailbox;
    mask = 1UL << mailbox;
    dlc = (msg->dlc > 8U) ? 8U : msg->dlc;

    shadow.CANME.all = regs->CANME.all;
    shadow.CANME.all &= ~mask;
    regs->CANME.all = shadow.CANME.all;

    mbox->MSGID.all = can_pack_msgid(msg);
    mbox->MSGCTRL.all = (Uint32)dlc;
    mbox->MDL.all = ((Uint32)msg->data[0] << 24) |
                    ((Uint32)msg->data[1] << 16) |
                    ((Uint32)msg->data[2] << 8) |
                    (Uint32)msg->data[3];
    mbox->MDH.all = ((Uint32)msg->data[4] << 24) |
                    ((Uint32)msg->data[5] << 16) |
                    ((Uint32)msg->data[6] << 8) |
                    (Uint32)msg->data[7];

    shadow.CANME.all = regs->CANME.all;
    shadow.CANME.all |= mask;
    regs->CANME.all = shadow.CANME.all;

    shadow.CANTRS.all = mask;
    regs->CANTRS.all = shadow.CANTRS.all;
}

static Uint16 can_write_try_mbox(can_module_t mod, Uint16 m)
{
    Uint16 i;

    for (i = CAN_TX_MBOX_FIRST; i < (CAN_TX_MBOX_FIRST + CAN_TX_MBOX_COUNT); i++)
    {
        if (can_tx_mbox_free(mod, i) != 0U)
        {
            can_tx_mbox_load(mod, i, &can_write_held[m]);
            return 1U;
        }
    }
    return 0U;
}

void can_write(can_module_t mod)
{
    Uint16 m;

    if ((mod != CAN_A) && (mod != CAN_B))
    {
        return;
    }
    m = (Uint16)mod;

    switch (can_write_state[m])
    {
        case CAN_WRITE_ST_CHECK_QUEUE:
            if (can_q_pop(mod, CAN_TX, &can_write_held[m]) == 0U)
            {
                break;
            }
            can_write_state[m] = CAN_WRITE_ST_FIND_MBOX;
            break;

        case CAN_WRITE_ST_FIND_MBOX:
            if (can_write_try_mbox(mod, m) != 0U)
            {
                can_write_state[m] = CAN_WRITE_ST_CHECK_QUEUE;
            }
            break;

        default:
            can_write_state[m] = CAN_WRITE_ST_CHECK_QUEUE;
            break;
    }
}

static void can_tx_mbox_init(void)
{
    can_mbox_cfg_t cfg;
    Uint16 i;

    cfg.dir = CAN_TX;
    cfg.ide = CAN_ID_STD;
    cfg.id = 0UL;
    cfg.lam = 0UL;
    cfg.ame = 0U;
    cfg.dlc = 8U;
    cfg.enable = 1U;

    for (i = CAN_TX_MBOX_FIRST; i < (CAN_TX_MBOX_FIRST + CAN_TX_MBOX_COUNT); i++)
    {
        cfg.mailbox = i;
        cfg.module = CAN_A;
        can_mbox_config(&cfg);
        cfg.module = CAN_B;
        can_mbox_config(&cfg);
    }

    cfg.mailbox = 16U;
    cfg.dir = CAN_RX;
    cfg.id = 0UL;
    cfg.lam = 0x1FFFFFFFUL;
    cfg.ame = 1U;
    cfg.module = CAN_A;
    can_mbox_config(&cfg);
    cfg.module = CAN_B;
    can_mbox_config(&cfg);
}

void can_init(void)
{
    InitECan();

    can_write_state[CAN_A] = CAN_WRITE_ST_CHECK_QUEUE;
    can_write_state[CAN_B] = CAN_WRITE_ST_CHECK_QUEUE;

    can_tx_mbox_init();
    can_rx_int_enable();
}
