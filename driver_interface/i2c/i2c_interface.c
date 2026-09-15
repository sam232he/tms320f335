#include "i2c_interface.h"
#include "DSP28x_Project.h"

/*
 * I2CMDR values from Example_2833xI2C_eeprom.c:
 *   0x0020  IRS=1
 *   0x6E20  FREE STT STP MST TRX IRS   master transmit with stop
 *   0x2620  STT MST TRX IRS            master transmit, no stop (set address)
 *   0x2C20  STT STP MST IRS            master receive with stop (repeated start)
 */
#define I2C_MDR_IRS                 0x0020U
#define I2C_MDR_WRITE_STOP          0x6E20U
#define I2C_MDR_ADDR_NOSTOP         0x2620U
#define I2C_MDR_READ_STOP           0x2C20U
#define I2C_NACK_RETRY_MAX          200U

typedef enum
{
    I2C_XFER_IDLE = 0,
    I2C_XFER_WRITE_BUSY,
    I2C_XFER_ADDR_BUSY,
    I2C_XFER_NEED_RESTART,
    I2C_XFER_READ_BUSY,
    I2C_XFER_NEED_RETRY
} i2c_xfer_st_t;

static volatile Uint16 i2c_xfer_st;
static volatile Uint16 i2c_nack_n;
static i2c_msg_t i2c_held;
static i2c_msg_t i2c_cmd_buf[I2C_QUEUE_LEN];
static Uint16 i2c_cmd_head;
static Uint16 i2c_cmd_tail;
static Uint16 i2c_cmd_n;
static i2c_msg_t i2c_rx_buf[I2C_QUEUE_LEN];
static Uint16 i2c_rx_head;
static Uint16 i2c_rx_tail;
static Uint16 i2c_rx_n;

static Uint16 i2c_mod_ok(i2c_module_t mod)
{
    return ((Uint16)mod < I2C_COUNT) ? 1U : 0U;
}

static Uint16 i2c_msg_ok(const i2c_msg_t *msg)
{
    if (msg == 0)
    {
        return 0U;
    }
    if ((msg->dir != I2C_TX) && (msg->dir != I2C_RX))
    {
        return 0U;
    }
    if ((msg->n == 0U) || (msg->n > I2C_MAX_BYTES))
    {
        return 0U;
    }
    return 1U;
}

static Uint16 i2c_cmd_push(const i2c_msg_t *msg)
{
    if (i2c_cmd_n >= I2C_QUEUE_LEN)
    {
        return 0U;
    }
    i2c_cmd_buf[i2c_cmd_head] = *msg;
    i2c_cmd_head++;
    if (i2c_cmd_head >= I2C_QUEUE_LEN)
    {
        i2c_cmd_head = 0U;
    }
    i2c_cmd_n++;
    return 1U;
}

static Uint16 i2c_cmd_pop(i2c_msg_t *msg)
{
    if (i2c_cmd_n == 0U)
    {
        return 0U;
    }
    *msg = i2c_cmd_buf[i2c_cmd_tail];
    i2c_cmd_tail++;
    if (i2c_cmd_tail >= I2C_QUEUE_LEN)
    {
        i2c_cmd_tail = 0U;
    }
    i2c_cmd_n--;
    return 1U;
}

static Uint16 i2c_rx_push(const i2c_msg_t *msg)
{
    if (i2c_rx_n >= I2C_QUEUE_LEN)
    {
        return 0U;
    }
    i2c_rx_buf[i2c_rx_head] = *msg;
    i2c_rx_head++;
    if (i2c_rx_head >= I2C_QUEUE_LEN)
    {
        i2c_rx_head = 0U;
    }
    i2c_rx_n++;
    return 1U;
}

static Uint16 i2c_rx_pop(i2c_msg_t *msg)
{
    if (i2c_rx_n == 0U)
    {
        return 0U;
    }
    *msg = i2c_rx_buf[i2c_rx_tail];
    i2c_rx_tail++;
    if (i2c_rx_tail >= I2C_QUEUE_LEN)
    {
        i2c_rx_tail = 0U;
    }
    i2c_rx_n--;
    return 1U;
}

Uint16 i2c_tx(i2c_module_t mod, const i2c_msg_t *msg)
{
    if ((i2c_mod_ok(mod) == 0U) || (i2c_msg_ok(msg) == 0U))
    {
        return 0U;
    }
    return i2c_cmd_push(msg);
}

Uint16 i2c_rx(i2c_module_t mod, i2c_msg_t *msg)
{
    if ((msg == 0) || (i2c_mod_ok(mod) == 0U))
    {
        return 0U;
    }
    return i2c_rx_pop(msg);
}

Uint16 i2c_idle(void)
{
    return (i2c_xfer_st == (Uint16)I2C_XFER_IDLE) ? 1U : 0U;
}

static Uint16 i2c_hw_ready(Uint16 check_bb)
{
    if (I2caRegs.I2CMDR.bit.STP == 1U)
    {
        return 0U;
    }
    if ((check_bb != 0U) && (I2caRegs.I2CSTR.bit.BB == 1U))
    {
        return 0U;
    }
    return 1U;
}

static Uint16 i2c_hw_start_write(void)
{
    Uint16 i;

    if (i2c_hw_ready(1U) == 0U)
    {
        return 0U;
    }

    I2caRegs.I2CSAR = i2c_held.slave;
    I2caRegs.I2CCNT = (Uint16)(i2c_held.n + 2U);
    I2caRegs.I2CDXR = (i2c_held.mem_addr >> 8) & 0xFFU;
    I2caRegs.I2CDXR = i2c_held.mem_addr & 0xFFU;
    for (i = 0U; i < i2c_held.n; i++)
    {
        I2caRegs.I2CDXR = i2c_held.data[i] & 0xFFU;
    }
    I2caRegs.I2CMDR.all = I2C_MDR_WRITE_STOP;
    return 1U;
}

static Uint16 i2c_hw_start_addr(void)
{
    if (i2c_hw_ready(1U) == 0U)
    {
        return 0U;
    }

    I2caRegs.I2CSAR = i2c_held.slave;
    I2caRegs.I2CCNT = 2U;
    I2caRegs.I2CDXR = (i2c_held.mem_addr >> 8) & 0xFFU;
    I2caRegs.I2CDXR = i2c_held.mem_addr & 0xFFU;
    I2caRegs.I2CMDR.all = I2C_MDR_ADDR_NOSTOP;
    return 1U;
}

static Uint16 i2c_hw_start_read(void)
{
    if (i2c_hw_ready(0U) == 0U)
    {
        return 0U;
    }

    I2caRegs.I2CSAR = i2c_held.slave;
    I2caRegs.I2CCNT = i2c_held.n;
    I2caRegs.I2CMDR.all = I2C_MDR_READ_STOP;
    return 1U;
}

static void i2c_fail_or_retry(void)
{
    i2c_nack_n++;
    if (i2c_nack_n >= I2C_NACK_RETRY_MAX)
    {
        i2c_xfer_st = (Uint16)I2C_XFER_IDLE;
    }
    else
    {
        i2c_xfer_st = (Uint16)I2C_XFER_NEED_RETRY;
    }
}

static void i2c_start_held(void)
{
    DINT;
    if (i2c_held.dir == I2C_TX)
    {
        if (i2c_hw_start_write() != 0U)
        {
            i2c_xfer_st = (Uint16)I2C_XFER_WRITE_BUSY;
        }
        else
        {
            i2c_fail_or_retry();
        }
    }
    else if (i2c_hw_start_addr() != 0U)
    {
        i2c_xfer_st = (Uint16)I2C_XFER_ADDR_BUSY;
    }
    else
    {
        i2c_fail_or_retry();
    }
    EINT;
}

void i2c_write(i2c_module_t mod)
{
    if (i2c_mod_ok(mod) == 0U)
    {
        return;
    }

    if (i2c_xfer_st == (Uint16)I2C_XFER_NEED_RESTART)
    {
        DINT;
        if (i2c_hw_start_read() != 0U)
        {
            i2c_xfer_st = (Uint16)I2C_XFER_READ_BUSY;
        }
        EINT;
        return;
    }

    if (i2c_xfer_st == (Uint16)I2C_XFER_NEED_RETRY)
    {
        i2c_start_held();
        return;
    }

    if (i2c_xfer_st != (Uint16)I2C_XFER_IDLE)
    {
        return;
    }

    if (i2c_cmd_pop(&i2c_held) == 0U)
    {
        return;
    }

    i2c_nack_n = 0U;
    i2c_start_held();
}

interrupt void i2c_int1a_isr(void)
{
    Uint16 src;
    Uint16 i;

    src = I2caRegs.I2CISRC.bit.INTCODE;

    if (src == I2C_SCD_ISRC)
    {
        if (i2c_xfer_st == (Uint16)I2C_XFER_WRITE_BUSY)
        {
            i2c_xfer_st = (Uint16)I2C_XFER_IDLE;
        }
        else if (i2c_xfer_st == (Uint16)I2C_XFER_ADDR_BUSY)
        {
            /* NACK during address setup commanded a stop. Retry later. */
            i2c_fail_or_retry();
        }
        else if (i2c_xfer_st == (Uint16)I2C_XFER_READ_BUSY)
        {
            for (i = 0U; i < i2c_held.n; i++)
            {
                i2c_held.data[i] = I2caRegs.I2CDRR & 0xFFU;
            }
            (void)i2c_rx_push(&i2c_held);
            i2c_xfer_st = (Uint16)I2C_XFER_IDLE;
        }
    }
    else if (src == I2C_ARDY_ISRC)
    {
        if (I2caRegs.I2CSTR.bit.NACK == 1U)
        {
            I2caRegs.I2CMDR.bit.STP = 1;
            I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;
        }
        else if (i2c_xfer_st == (Uint16)I2C_XFER_ADDR_BUSY)
        {
            i2c_xfer_st = (Uint16)I2C_XFER_NEED_RESTART;
        }
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

static void i2c_int_enable(void)
{
    EALLOW;
    PieVectTable.I2CINT1A = &i2c_int1a_isr;
    EDIS;

    PieCtrlRegs.PIEIER8.bit.INTx1 = 1;   /* I2CINT1A */
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
    IER |= M_INT8;
}

void i2c_init(void)
{
    i2c_xfer_st = (Uint16)I2C_XFER_IDLE;
    i2c_nack_n = 0U;

    /* I2CPSC / I2CCLKx must be written while IRS=0. */
    I2caRegs.I2CMDR.all = 0U;
    I2caRegs.I2CSAR = I2C_EEPROM_ADDR;
    I2caRegs.I2CPSC.all = I2C_IPSC;
    I2caRegs.I2CCLKL = I2C_ICCL;
    I2caRegs.I2CCLKH = I2C_ICCH;
    I2caRegs.I2CIER.all = 0x24U;          /* SCD + ARDY, same as TI example */

    I2caRegs.I2CMDR.all = I2C_MDR_IRS;
    DELAY_US(1);

    I2caRegs.I2CFFTX.all = 0x6000U;       /* I2CFFEN + TXFFRST */
    I2caRegs.I2CFFRX.all = 0x2040U;       /* RXFFRST + RXFFINTCLR */

    i2c_int_enable();
}
