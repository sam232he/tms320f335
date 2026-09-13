#include "sci_interface.h"
#include "DSP28x_Project.h"

typedef enum
{
    SCI_WRITE_ST_CHECK_QUEUE = 0,
    SCI_WRITE_ST_FIND_FIFO
} sci_write_state_t;

static sci_write_state_t sci_write_state[2];
static sci_msg_t sci_write_held[2];
static sci_msg_t sci_q_buf[2][2][SCI_QUEUE_LEN];
static Uint16 sci_q_head[2][2];
static Uint16 sci_q_tail[2][2];
static Uint16 sci_q_n[2][2];

static Uint16 sci_q_push(sci_module_t mod, sci_dir_t dir, const sci_msg_t *msg)
{
    Uint16 m;
    Uint16 d;

    if ((msg == 0) || ((mod != SCI) && (mod != SCI_CMD)))
    {
        return 0U;
    }
    if ((dir != SCI_TX) && (dir != SCI_RX))
    {
        return 0U;
    }

    m = (Uint16)mod;
    d = (Uint16)dir;
    if (sci_q_n[m][d] >= SCI_QUEUE_LEN)
    {
        return 0U;
    }

    sci_q_buf[m][d][sci_q_head[m][d]] = *msg;
    sci_q_head[m][d]++;
    if (sci_q_head[m][d] >= SCI_QUEUE_LEN)
    {
        sci_q_head[m][d] = 0U;
    }
    sci_q_n[m][d]++;
    return 1U;
}

static Uint16 sci_q_pop(sci_module_t mod, sci_dir_t dir, sci_msg_t *msg)
{
    Uint16 m;
    Uint16 d;

    if ((msg == 0) || ((mod != SCI) && (mod != SCI_CMD)))
    {
        return 0U;
    }
    if ((dir != SCI_TX) && (dir != SCI_RX))
    {
        return 0U;
    }

    m = (Uint16)mod;
    d = (Uint16)dir;
    if (sci_q_n[m][d] == 0U)
    {
        return 0U;
    }

    *msg = sci_q_buf[m][d][sci_q_tail[m][d]];
    sci_q_tail[m][d]++;
    if (sci_q_tail[m][d] >= SCI_QUEUE_LEN)
    {
        sci_q_tail[m][d] = 0U;
    }
    sci_q_n[m][d]--;
    return 1U;
}

Uint16 sci_tx(sci_module_t mod, const sci_msg_t *msg)
{
    return sci_q_push(mod, SCI_TX, msg);
}

Uint16 sci_rx(sci_module_t mod, sci_msg_t *msg)
{
    return sci_q_pop(mod, SCI_RX, msg);
}

static volatile struct SCI_REGS *sci_regs(sci_module_t mod)
{
    if (mod == SCI_CMD)
    {
#if SCI_CMD_USE_SCIC
        return &ScicRegs;
#else
        return &SciaRegs;
#endif
    }
#if SCI_USE_SCIB
    return &ScibRegs;
#else
    return &SciaRegs;
#endif
}

static void sci_port_init(volatile struct SCI_REGS *regs, Uint16 brr_h, Uint16 brr_l)
{
    regs->SCICCR.all = 0x0007U;       /* 8 data, 1 stop, no parity, idle */
    regs->SCICTL1.all = 0x0003U;      /* RXENA + TXENA, hold SWRESET */
    regs->SCICTL2.all = 0x0000U;
    regs->SCIHBAUD = brr_h;
    regs->SCILBAUD = brr_l;
    regs->SCIFFTX.all = 0xE040U;
    regs->SCIFFRX.all = 0x2061U;      /* RXFFIL=1, RXFFIENA, RXFFINTCLR, reset */
    regs->SCIFFCT.all = 0x0000U;
    regs->SCIPRI.bit.FREE = 1;
    regs->SCIPRI.bit.SOFT = 0;

    regs->SCICTL1.all = 0x0023U;      /* release SWRESET */
    regs->SCIFFTX.bit.TXFIFOXRESET = 1;
    regs->SCIFFRX.bit.RXFIFORESET = 1;
}

/*
 * Drain the hardware RX FIFO into the software RX queue.
 * Always empty the FIFO even if the queue is full.
 */
static void sci_rx_to_queue(sci_module_t mod)
{
    volatile struct SCI_REGS *regs;
    sci_msg_t msg;

    regs = sci_regs(mod);
    while (regs->SCIFFRX.bit.RXFFST != 0U)
    {
        msg.data = (Uint16)(regs->SCIRXBUF.all & 0xFFU);
        (void)sci_q_push(mod, SCI_RX, &msg);
    }
    regs->SCIFFRX.bit.RXFFOVRCLR = 1;
    regs->SCIFFRX.bit.RXFFINTCLR = 1;
}

#if !SCI_USE_SCIB || !SCI_CMD_USE_SCIC
interrupt void sci_rx_isr_a(void)
{
#if !SCI_USE_SCIB
    sci_rx_to_queue(SCI);
#endif
#if !SCI_CMD_USE_SCIC
    sci_rx_to_queue(SCI_CMD);
#endif
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
#endif

#if SCI_USE_SCIB
interrupt void sci_rx_isr_b(void)
{
    sci_rx_to_queue(SCI);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
#endif

#if SCI_CMD_USE_SCIC
interrupt void sci_rx_isr_c(void)
{
    sci_rx_to_queue(SCI_CMD);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}
#endif

static void sci_rx_int_enable(void)
{
    EALLOW;
#if SCI_USE_SCIB
    PieVectTable.SCIRXINTB = &sci_rx_isr_b;
#else
    PieVectTable.SCIRXINTA = &sci_rx_isr_a;
#endif
#if SCI_CMD_USE_SCIC
    PieVectTable.SCIRXINTC = &sci_rx_isr_c;
#else
    PieVectTable.SCIRXINTA = &sci_rx_isr_a;
#endif
    EDIS;

#if SCI_USE_SCIB
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   /* SCIRXINTB */
#else
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;   /* SCIRXINTA */
#endif
#if SCI_CMD_USE_SCIC
    PieCtrlRegs.PIEIER8.bit.INTx5 = 1;   /* SCIRXINTC */
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
    IER |= M_INT8;
#else
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;   /* SCIRXINTA */
#endif
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
    IER |= M_INT9;
}

static Uint16 sci_tx_fifo_free(sci_module_t mod)
{
    return (sci_regs(mod)->SCIFFTX.bit.TXFFST < 16U) ? 1U : 0U;
}

static void sci_tx_load(sci_module_t mod, const sci_msg_t *msg)
{
    sci_regs(mod)->SCITXBUF = (Uint16)(msg->data & 0xFFU);
}

static Uint16 sci_write_try_fifo(sci_module_t mod, Uint16 m)
{
    if (sci_tx_fifo_free(mod) != 0U)
    {
        sci_tx_load(mod, &sci_write_held[m]);
        return 1U;
    }
    return 0U;
}

void sci_write(sci_module_t mod)
{
    Uint16 m;

    if ((mod != SCI) && (mod != SCI_CMD))
    {
        return;
    }
    m = (Uint16)mod;

    switch (sci_write_state[m])
    {
        case SCI_WRITE_ST_CHECK_QUEUE:
            if (sci_q_pop(mod, SCI_TX, &sci_write_held[m]) == 0U)
            {
                break;
            }
            sci_write_state[m] = SCI_WRITE_ST_FIND_FIFO;
            break;

        case SCI_WRITE_ST_FIND_FIFO:
            if (sci_write_try_fifo(mod, m) != 0U)
            {
                sci_write_state[m] = SCI_WRITE_ST_CHECK_QUEUE;
            }
            break;

        default:
            sci_write_state[m] = SCI_WRITE_ST_CHECK_QUEUE;
            break;
    }
}

void sci_init(void)
{
    sci_port_init(sci_regs(SCI), (Uint16)SCI_BRR_HIGH, (Uint16)SCI_BRR_LOW);

    sci_write_state[SCI] = SCI_WRITE_ST_CHECK_QUEUE;
}

void sci_cmd_init(void)
{
    sci_port_init(sci_regs(SCI_CMD), (Uint16)SCI_CMD_BRR_HIGH, (Uint16)SCI_CMD_BRR_LOW);

    sci_write_state[SCI_CMD] = SCI_WRITE_ST_CHECK_QUEUE;

    sci_rx_int_enable();
}
