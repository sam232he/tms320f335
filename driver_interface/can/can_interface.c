#include <stdint.h>
#include "can_interface.h"
#include "DSP28x_Project.h"
#include "string.h"

typedef enum
{
    CAN_WRITE_ST_CHECK_QUEUE = 0,
    CAN_WRITE_ST_FIND_MBOX
} can_write_state_t;

static can_write_state_t can_write_state[2];
static can_msg_t can_write_held[2];
static can_msg_t can_q_buf[2][2][CAN_QUEUE_LEN];
static volatile uint16_t can_q_head[2][2];
static volatile uint16_t can_q_tail[2][2];
static volatile uint16_t can_q_n[2][2];
static uint16_t can_bo_hold[2];   /* 1 = this bus-off episode; skip TX */

static uint16_t can_q_push(can_module_t mod, can_dir_t dir, const can_msg_t *msg)
{
    uint16_t m;
    uint16_t d;

    if ((msg == 0) || ((mod != CAN_A) && (mod != CAN_B)))
    {
        return 0U;
    }
    if ((dir != CAN_TX) && (dir != CAN_RX))
    {
        return 0U;
    }

    m = (uint16_t)mod;
    d = (uint16_t)dir;
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

static uint16_t can_q_pop(can_module_t mod, can_dir_t dir, can_msg_t *msg)
{
    uint16_t m;
    uint16_t d;
    uint16_t lock;
    uint16_t ok;

    if ((msg == 0) || ((mod != CAN_A) && (mod != CAN_B)))
    {
        return 0U;
    }
    if ((dir != CAN_TX) && (dir != CAN_RX))
    {
        return 0U;
    }

    m = (uint16_t)mod;
    d = (uint16_t)dir;
    /* RX: ISR push vs main pop. Do not EINT in can_q_push (RX ISR). */
    lock = (d == (uint16_t)CAN_RX) ? 1U : 0U;
    if (lock != 0U)
    {
        DINT;
    }

    if (can_q_n[m][d] == 0U)
    {
        ok = 0U;
    }
    else
    {
        *msg = can_q_buf[m][d][can_q_tail[m][d]];
        can_q_tail[m][d]++;
        if (can_q_tail[m][d] >= CAN_QUEUE_LEN)
        {
            can_q_tail[m][d] = 0U;
        }
        can_q_n[m][d]--;
        ok = 1U;
    }

    if (lock != 0U)
    {
        EINT;
    }
    return ok;
}

uint16_t can_tx(can_module_t mod, const can_msg_t *msg)
{
    return can_q_push(mod, CAN_TX, msg);
}

uint16_t can_rx(can_module_t mod, can_msg_t *msg)
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

/*
 * cfg->mask: 1 = ID bit must match. eCAN LAM is inverted (1 = don't care).
 * AME is set only when at least one ID bit is ignored. TX does not filter.
 */
static void can_filter_hw(const can_mbox_cfg_t *cfg, uint32_t *msgid, uint32_t *lam_val)
{
    uint32_t care;
    uint32_t dont_care;

    *lam_val = 0UL;
    if (cfg->dir != CAN_RX)
    {
        return;
    }

    if (cfg->ide == CAN_ID_EXT)
    {
        care = cfg->mask & 0x1FFFFFFFUL;
        dont_care = (~care) & 0x1FFFFFFFUL;
        *lam_val = dont_care;
    }
    else
    {
        care = cfg->mask & 0x7FFUL;
        dont_care = (~care) & 0x7FFUL;
        *lam_val = (dont_care << 18) | 0x3FFFFUL;
    }

    if (dont_care != 0UL)
    {
        *msgid |= 0x40000000UL;
    }
}

void can_mbox_config(const can_mbox_cfg_t *cfg)
{
    volatile struct ECAN_REGS *regs;
    volatile struct MBOX *mbox;
    volatile union CANLAM_REG *lam;
    struct ECAN_REGS shadow;
    uint32_t mbox_bit;
    uint32_t msgid;
    uint32_t lam_val;

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
    mbox_bit = 1UL << cfg->mailbox;

    /* TI: CANME must be 0 before writing MSGID. */
    shadow.CANME.all = regs->CANME.all;
    shadow.CANME.all &= ~mbox_bit;
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

    can_filter_hw(cfg, &msgid, &lam_val);

    mbox->MSGID.all = msgid;
    mbox->MSGCTRL.all = (uint32_t)((cfg->dlc > 8U) ? 8U : cfg->dlc);
    lam->all = lam_val;

    shadow.CANMD.all = regs->CANMD.all;
    if (cfg->dir == CAN_RX)
    {
        shadow.CANMD.all |= mbox_bit;
    }
    else
    {
        shadow.CANMD.all &= ~mbox_bit;
    }
    regs->CANMD.all = shadow.CANMD.all;

    EALLOW;
    shadow.CANMIL.all = regs->CANMIL.all;
    shadow.CANMIL.all &= ~mbox_bit;      /* RX/TX mailbox interrupt on line 0 */
    regs->CANMIL.all = shadow.CANMIL.all;

    shadow.CANMIM.all = regs->CANMIM.all;
    if ((cfg->dir == CAN_RX) && (cfg->enable != 0U))
    {
        shadow.CANMIM.all |= mbox_bit;
    }
    else
    {
        shadow.CANMIM.all &= ~mbox_bit;
    }
    regs->CANMIM.all = shadow.CANMIM.all;
    EDIS;

    if (cfg->enable != 0U)
    {
        shadow.CANME.all = regs->CANME.all;
        shadow.CANME.all |= mbox_bit;
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
    uint32_t rmp;
    uint32_t mask;
    uint32_t msgid;
    uint32_t mdl;
    uint32_t mdh;
    uint16_t i;
    uint16_t dlc;
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
        dlc = (uint16_t)(mbox->MSGCTRL.all & 0x0FUL);
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

        msg.data[0] = (uint16_t)((mdl >> 24) & 0x00FFUL);
        msg.data[1] = (uint16_t)((mdl >> 16) & 0x00FFUL);
        msg.data[2] = (uint16_t)((mdl >> 8) & 0x00FFUL);
        msg.data[3] = (uint16_t)(mdl & 0x00FFUL);
        msg.data[4] = (uint16_t)((mdh >> 24) & 0x00FFUL);
        msg.data[5] = (uint16_t)((mdh >> 16) & 0x00FFUL);
        msg.data[6] = (uint16_t)((mdh >> 8) & 0x00FFUL);
        msg.data[7] = (uint16_t)(mdh & 0x00FFUL);

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

static uint32_t can_pack_msgid(const can_msg_t *msg)
{
    uint32_t msgid;

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

static uint16_t can_tx_mbox_free(can_module_t mod, uint16_t mailbox)
{
    volatile struct ECAN_REGS *regs;
    struct ECAN_REGS shadow;
    uint32_t mask;

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

static void can_tx_mbox_load(can_module_t mod, uint16_t mailbox, const can_msg_t *msg)
{
    volatile struct ECAN_REGS *regs;
    volatile struct MBOX *mbox;
    struct ECAN_REGS shadow;
    uint32_t mask;
    uint16_t dlc;

    regs = can_regs(mod);
    mbox = &can_mboxes(mod)->MBOX0 + mailbox;
    mask = 1UL << mailbox;
    dlc = (msg->dlc > 8U) ? 8U : msg->dlc;

    shadow.CANME.all = regs->CANME.all;
    shadow.CANME.all &= ~mask;
    regs->CANME.all = shadow.CANME.all;

    mbox->MSGID.all = can_pack_msgid(msg);
    mbox->MSGCTRL.all = (uint32_t)dlc;
    mbox->MDL.all = ((uint32_t)msg->data[0] << 24) |
                    ((uint32_t)msg->data[1] << 16) |
                    ((uint32_t)msg->data[2] << 8) |
                    (uint32_t)msg->data[3];
    mbox->MDH.all = ((uint32_t)msg->data[4] << 24) |
                    ((uint32_t)msg->data[5] << 16) |
                    ((uint32_t)msg->data[6] << 8) |
                    (uint32_t)msg->data[7];

    shadow.CANME.all = regs->CANME.all;
    shadow.CANME.all |= mask;
    regs->CANME.all = shadow.CANME.all;

    shadow.CANTRS.all = mask;
    regs->CANTRS.all = shadow.CANTRS.all;
}

static uint16_t can_write_try_mbox(can_module_t mod, uint16_t m)
{
    uint16_t i;

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

/*
 * SPRUEU1: bus-off when CANTEC hits 256. No RX/TX until bus-on.
 * ABO recovers after 128 * 11 recessive bits. If ABO is 0, CCR is set
 * on BO and must be cleared after the same 128 * 11 (CANREC counts them).
 */
static void can_busoff_step(can_module_t mod)
{
    volatile struct ECAN_REGS *regs;
    struct ECAN_REGS shadow;
    uint16_t m;

    m = (uint16_t)mod;
    regs = can_regs(mod);
    shadow.CANES.all = regs->CANES.all;
    if (shadow.CANES.bit.BO == 0U)
    {
        can_bo_hold[m] = 0U;
        return;
    }

    if (can_bo_hold[m] == 0U)
    {
        regs->CANTRR.all = 0xFFFFFFFFUL;
        regs->CANTA.all = 0xFFFFFFFFUL;
        shadow.CANGIF0.all = 0UL;
        shadow.CANGIF0.bit.BOIF0 = 1;
        regs->CANGIF0.all = shadow.CANGIF0.all;
        can_bo_hold[m] = 1U;
    }

    shadow.CANMC.all = regs->CANMC.all;
    if ((shadow.CANMC.bit.ABO == 0U) && (regs->CANREC.bit.REC >= 128U))
    {
        EALLOW;
        shadow.CANMC.all = regs->CANMC.all;
        shadow.CANMC.bit.CCR = 0;
        regs->CANMC.all = shadow.CANMC.all;
        EDIS;
    }
}

void can_write(can_module_t mod)
{
    uint16_t m;

    if ((mod != CAN_A) && (mod != CAN_B))
    {
        return;
    }
    m = (uint16_t)mod;

    can_busoff_step(mod);
    if (can_bo_hold[m] != 0U)
    {
        return;
    }

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

static void can_mbox_init(void)
{
    can_mbox_cfg_t cfg;
    uint16_t i;

    cfg.dir = CAN_TX;
    cfg.ide = CAN_ID_STD;
    cfg.id = 0UL;
    cfg.mask = 0x7FFUL;
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
    cfg.id = 25UL;
    cfg.mask = 0UL;
    cfg.module = CAN_A;
    can_mbox_config(&cfg);
    cfg.module = CAN_B;
    can_mbox_config(&cfg);
}

void can_init(void)
{
    struct ECAN_REGS shadow;

    /* Schematic DSP_CAN is eCAN-B on GPIO16/17. Do not init eCAN-A:
     * its CANTX/CANRX pins are not muxed, so CCE may never set and boot hangs. */
    InitECanb();

    /* SPRUEU1: ABO bus-on after 128 * 11 recessive bits. EALLOW. */
    EALLOW;
    shadow.CANMC.all = ECanaRegs.CANMC.all;
    shadow.CANMC.bit.ABO = 1;
    ECanaRegs.CANMC.all = shadow.CANMC.all;
    shadow.CANMC.all = ECanbRegs.CANMC.all;
    shadow.CANMC.bit.ABO = 1;
    ECanbRegs.CANMC.all = shadow.CANMC.all;
    EDIS;

    can_write_state[CAN_A] = CAN_WRITE_ST_CHECK_QUEUE;
    can_write_state[CAN_B] = CAN_WRITE_ST_CHECK_QUEUE;
    can_bo_hold[CAN_A] = 0U;
    can_bo_hold[CAN_B] = 0U;
    memset(can_write_held, 0x0, sizeof(can_write_held));
    memset(&can_q_buf[0][0][0], 0x0, sizeof(can_q_buf));
    memset((void*) &can_q_head[0][0], 0x0, sizeof(can_q_head));
    memset((void*) &can_q_tail[0][0], 0x0, sizeof(can_q_tail));
    memset((void *)&can_q_n[0][0],0x0,sizeof(can_q_n));

    can_mbox_init();
    can_rx_int_enable();

}
