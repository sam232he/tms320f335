#include "sci_interface.h"
#include "DSP28x_Project.h"

typedef enum
{
    SCI_WRITE_ST_CHECK_QUEUE = 0,
    SCI_WRITE_ST_FIND_FIFO
} sci_write_state_t;

static sci_write_state_t sci_write_state[SCI_COUNT];
static sci_msg_t sci_write_held[SCI_COUNT];
static sci_msg_t sci_q_buf[SCI_COUNT][2][SCI_QUEUE_LEN];
static volatile Uint16 sci_q_head[SCI_COUNT][2];
static volatile Uint16 sci_q_tail[SCI_COUNT][2];
static volatile Uint16 sci_q_n[SCI_COUNT][2];

static Uint16 sci_mod_ok(sci_module_t mod)
{
    return ((Uint16)mod < SCI_COUNT) ? 1U : 0U;
}

static Uint16 sci_q_push(sci_module_t mod, sci_dir_t dir, const sci_msg_t *msg)
{
    Uint16 m;
    Uint16 d;

    if ((msg == 0) || (sci_mod_ok(mod) == 0U))
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
    Uint16 lock;
    Uint16 ok;

    if ((msg == 0) || (sci_mod_ok(mod) == 0U))
    {
        return 0U;
    }
    if ((dir != SCI_TX) && (dir != SCI_RX))
    {
        return 0U;
    }

    m = (Uint16)mod;
    d = (Uint16)dir;
    /* RX: ISR push vs main pop. Do not EINT in sci_q_push (RX ISR). */
    lock = (d == (Uint16)SCI_RX) ? 1U : 0U;
    if (lock != 0U)
    {
        DINT;
    }

    if (sci_q_n[m][d] == 0U)
    {
        ok = 0U;
    }
    else
    {
        *msg = sci_q_buf[m][d][sci_q_tail[m][d]];
        sci_q_tail[m][d]++;
        if (sci_q_tail[m][d] >= SCI_QUEUE_LEN)
        {
            sci_q_tail[m][d] = 0U;
        }
        sci_q_n[m][d]--;
        ok = 1U;
    }

    if (lock != 0U)
    {
        EINT;
    }
    return ok;
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
    switch (mod)
    {
        case SCI_B: return &ScibRegs;
        case SCI_C: return &ScicRegs;
        default:    return &SciaRegs;
    }
}

static void sci_port_init(volatile struct SCI_REGS *regs)
{
    regs->SCICCR.all = 0x0007U;       /* 8 data, 1 stop, no parity, idle */
    regs->SCICTL1.all = 0x0003U;      /* RXENA + TXENA, hold SWRESET */
    regs->SCICTL2.all = 0x0000U;
    regs->SCIHBAUD = (Uint16)SCI_BRR_HIGH;
    regs->SCILBAUD = (Uint16)SCI_BRR_LOW;
    regs->SCIFFTX.all = 0xE040U;
    regs->SCIFFRX.all = 0x2061U;      /* RXFFIL=1, RXFFIENA, RXFFINTCLR, reset */
    regs->SCIFFCT.all = 0x0000U;
    regs->SCIPRI.bit.FREE = 1;
    regs->SCIPRI.bit.SOFT = 0;

    regs->SCICTL1.all = 0x0063U;      /* SWRESET + RXERRINTENA */
    regs->SCIFFTX.bit.TXFIFOXRESET = 1;
    regs->SCIFFRX.bit.RXFIFORESET = 1;
}

/* SPRUFZ5: BRKDT (and latched RXERROR) clear only via SWRESET. FIFO
 * pointers are not restored; pulse TX/RX FIFO reset after. */
static void sci_rx_recover(volatile struct SCI_REGS *regs)
{
    regs->SCICTL1.all = 0x0003U;      /* SWRESET=0, RX/TX still selected */
    regs->SCIFFTX.bit.TXFIFOXRESET = 0;
    regs->SCIFFRX.bit.RXFIFORESET = 0;
    regs->SCIFFTX.bit.TXFIFOXRESET = 1;
    regs->SCIFFRX.bit.RXFIFORESET = 1;
    regs->SCICTL1.all = 0x0063U;      /* SWRESET + RXERRINTENA */
    regs->SCIFFRX.bit.RXFFOVRCLR = 1;
    regs->SCIFFRX.bit.RXFFINTCLR = 1;
}

/*
 * Drain the hardware RX FIFO into the software RX queue.
 * Always empty the FIFO even if the queue is full.
 * Framing/parity FIFO bytes are dropped. BRKDT/RXERROR need SWRESET
 * or this port stops receiving until a device reset (SPRUFZ5).
 */
static void sci_rx_to_queue(sci_module_t mod)
{
    volatile struct SCI_REGS *regs;
    sci_msg_t msg;
    Uint16 raw;

    regs = sci_regs(mod);
    while (regs->SCIFFRX.bit.RXFFST != 0U)
    {
        raw = regs->SCIRXBUF.all;
        if ((raw & 0xC000U) != 0U)    /* SCIFFFE | SCIFFPE */
        {
            continue;
        }
        msg.data = raw & 0xFFU;
        (void)sci_q_push(mod, SCI_RX, &msg);
    }
    if (regs->SCIRXST.bit.RXERROR != 0U)
    {
        sci_rx_recover(regs);
    }
    else
    {
        regs->SCIFFRX.bit.RXFFOVRCLR = 1;
        regs->SCIFFRX.bit.RXFFINTCLR = 1;
    }
}

interrupt void sci_rx_isr_a(void)
{
    sci_rx_to_queue(SCI_A);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

interrupt void sci_rx_isr_b(void)
{
    sci_rx_to_queue(SCI_B);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

interrupt void sci_rx_isr_c(void)
{
    sci_rx_to_queue(SCI_C);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

static void sci_rx_int_enable(void)
{
    EALLOW;
    PieVectTable.SCIRXINTA = &sci_rx_isr_a;
    PieVectTable.SCIRXINTB = &sci_rx_isr_b;
    PieVectTable.SCIRXINTC = &sci_rx_isr_c;
    EDIS;

    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;   /* SCIRXINTA */
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   /* SCIRXINTB */
    PieCtrlRegs.PIEIER8.bit.INTx5 = 1;   /* SCIRXINTC */
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8 | PIEACK_GROUP9;
    IER |= (M_INT8 | M_INT9);
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

    if (sci_mod_ok(mod) == 0U)
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
    Uint16 i;

    sci_port_init(&SciaRegs);
    sci_port_init(&ScibRegs);
    sci_port_init(&ScicRegs);

    for (i = 0U; i < SCI_COUNT; i++)
    {
        sci_write_state[i] = SCI_WRITE_ST_CHECK_QUEUE;
    }

    sci_rx_int_enable();
}
