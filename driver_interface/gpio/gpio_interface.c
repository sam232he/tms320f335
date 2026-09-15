#include "gpio_interface.h"
#include "sci_interface.h"
#include "i2c_interface.h"
#include "DSP28x_Project.h"

/*
 * F28335 GPIO pull-up disable bits: 0 = pull-up enabled, 1 = pull-up disabled.
 * Mux: 0 = GPIO, 1/2/3 = peripheral functions (PWM, CAN, SCI, ...).
 * Direction is only used when mux is GPIO. Peripheral pins ignore DIR.
 * Qual: 0 = synch to SYSCLKOUT, 1 = 3 samples, 2 = 6 samples, 3 = async.
 * Use async qualification on SCI RX and I2C SDA/SCL.
 */
#define GPIO_MUX_GPIO           0U
#define GPIO_MUX_PERIPH1        1U
#define GPIO_MUX_PERIPH2        2U
#define GPIO_MUX_PERIPH3        3U
#define GPIO_DIR_INPUT          0U
#define GPIO_DIR_OUTPUT         1U
#define GPIO_PULLUP_ENABLE      0U
#define GPIO_PULLUP_DISABLE     1U
#define GPIO_QUAL_SYNC          0U
#define GPIO_QUAL_3_SAMPLES     1U
#define GPIO_QUAL_6_SAMPLES     2U
#define GPIO_QUAL_ASYNC         3U
#define GPIO_LEVEL_LOW          0U
#define GPIO_LEVEL_HIGH         1U

typedef struct
{
    Uint16 pin;         /* GPIO0 .. GPIO87 */
    Uint16 mux;         /* GPIO_MUX_GPIO or GPIO_MUX_PERIPH1/2/3 */
    Uint16 direction;   /* GPIO_DIR_INPUT or GPIO_DIR_OUTPUT */
    Uint16 pullup;      /* GPIO_PULLUP_ENABLE or GPIO_PULLUP_DISABLE */
    Uint16 out_level;   /* initial latch value when configured as output */
    Uint16 qual;        /* input qualification (ports A and B only) */
} gpio_pin_cfg_t;

/*
 * Pin map matches drive.md. PWM5/6 GPIO are muxed for pwm_test.
 * RST_AB stays asserted. RST_CD is unused.
 */
static const gpio_pin_cfg_t gpio_pin_table[] =
{
    /* pin, mux,              dir,             pullup,               out_level,       qual */

    /* DRV8432 RESET_AB: asserted low so PWM GPIO can be tested with outputs Hi-Z. */
    { PIN_RST_AB, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },

    /* DSP_EPWM1_A/B = GPIO10/11 EPWM6A/B. DSP_EPWM2_A/B = GPIO9/8 EPWM5B/A. */
#if (PWM_OUTPUT_ENABLE != 0U)
    { PIN_DSP_EPWM1_A, GPIO_MUX_PERIPH1, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
    { PIN_DSP_EPWM1_B, GPIO_MUX_PERIPH1, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
    { PIN_DSP_EPWM2_A, GPIO_MUX_PERIPH1, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
    { PIN_DSP_EPWM2_B, GPIO_MUX_PERIPH1, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
#else
    { PIN_DSP_EPWM1_A, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
    { PIN_DSP_EPWM1_B, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
    { PIN_DSP_EPWM2_A, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
    { PIN_DSP_EPWM2_B, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
#endif

    /* DRV8432 status. Isolated FAULT/OTW; do not treat as startup faults. */
    { PIN_OTW,   GPIO_MUX_GPIO, GPIO_DIR_INPUT, GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },
    { PIN_FAULT, GPIO_MUX_GPIO, GPIO_DIR_INPUT, GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC },

    /* LED1..LED6. */
    { PIN_LED1, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_HIGH, GPIO_QUAL_SYNC },
    { PIN_LED2, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_HIGH, GPIO_QUAL_SYNC },
    { PIN_LED3, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_HIGH, GPIO_QUAL_SYNC },
    { PIN_LED4, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_HIGH, GPIO_QUAL_SYNC },
    { PIN_LED5, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_HIGH, GPIO_QUAL_SYNC },
    { PIN_LED6, GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_HIGH, GPIO_QUAL_SYNC },

    /* DSP_UART_1/2/3 = SCI_A/B/C. RX async. */
    { SCI_A_PIN_TX, SCI_A_PIN_MUX, GPIO_DIR_OUTPUT, GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC  },
    { SCI_A_PIN_RX, SCI_A_PIN_MUX, GPIO_DIR_INPUT,  GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_ASYNC },
    { SCI_B_PIN_TX, SCI_B_PIN_MUX, GPIO_DIR_OUTPUT, GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC  },
    { SCI_B_PIN_RX, SCI_B_PIN_MUX, GPIO_DIR_INPUT,  GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_ASYNC },
    { SCI_C_PIN_TX, SCI_C_PIN_MUX, GPIO_DIR_OUTPUT, GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC  },
    { SCI_C_PIN_RX, SCI_C_PIN_MUX, GPIO_DIR_INPUT,  GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_ASYNC },

    /* Schematic DSP_CAN is eCAN-B on GPIO16/17. */
    { PIN_DSP_CAN_TX, GPIO_MUX_PERIPH2, GPIO_DIR_OUTPUT, GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC  }, /* CANTXB */
    { PIN_DSP_CAN_RX, GPIO_MUX_PERIPH2, GPIO_DIR_INPUT,  GPIO_PULLUP_ENABLE, GPIO_LEVEL_LOW, GPIO_QUAL_ASYNC }, /* CANRXB */

    /* SSI encoder: CLK master output idle-high, DATA from MAX490. */
    { PIN_ENC1_DATA,   GPIO_MUX_GPIO, GPIO_DIR_INPUT,  GPIO_PULLUP_ENABLE,  GPIO_LEVEL_LOW,  GPIO_QUAL_SYNC },
    { PIN_ENC1_CLK,    GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_HIGH, GPIO_QUAL_SYNC },
    { PIN_ENC2_DATA,   GPIO_MUX_GPIO, GPIO_DIR_INPUT,  GPIO_PULLUP_ENABLE,  GPIO_LEVEL_LOW,  GPIO_QUAL_SYNC },
    { PIN_ENC2_CLK,    GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_HIGH, GPIO_QUAL_SYNC },

    /* CAT24C256 I2C-A. SDA/SCL mux 1, async. A1=0 -> 0x50 if A0=A2=GND. WP=0 allows writes. */
    { I2C_A_PIN_SDA, I2C_A_PIN_MUX, GPIO_DIR_INPUT,  GPIO_PULLUP_ENABLE,  GPIO_LEVEL_LOW, GPIO_QUAL_ASYNC },
    { I2C_A_PIN_SCL, I2C_A_PIN_MUX, GPIO_DIR_INPUT,  GPIO_PULLUP_ENABLE,  GPIO_LEVEL_LOW, GPIO_QUAL_ASYNC },
    { I2C_A_PIN_A1,  GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC  },
    { I2C_A_PIN_WP,  GPIO_MUX_GPIO, GPIO_DIR_OUTPUT, GPIO_PULLUP_DISABLE, GPIO_LEVEL_LOW, GPIO_QUAL_SYNC  }
};

static void gpio_apply_dir_pud_latch(volatile Uint32 *dir,
                                     volatile Uint32 *pud,
                                     volatile Uint32 *set,
                                     volatile Uint32 *clr,
                                     Uint32 mask,
                                     const gpio_pin_cfg_t *cfg)
{
    /*
     * Load the output latch before DIR so an output does not glitch
     * through the reset latch value (typically low).
     */
    if (cfg->mux == GPIO_MUX_GPIO)
    {
        if (cfg->out_level == GPIO_LEVEL_HIGH)
        {
            *set = mask;
        }
        else
        {
            *clr = mask;
        }
    }

    if (cfg->direction == GPIO_DIR_OUTPUT)
    {
        *dir |= mask;
    }
    else
    {
        *dir &= ~mask;
    }

    if (cfg->pullup == GPIO_PULLUP_DISABLE)
    {
        *pud |= mask;
    }
    else
    {
        *pud &= ~mask;
    }
}

static void gpio_apply_pin(const gpio_pin_cfg_t *cfg)
{
    Uint32 data_mask;
    Uint32 mux_mask;
    Uint16 mux_shift;
    Uint16 bit;

    if (cfg->pin <= 15U)
    {
        bit = cfg->pin;
        mux_shift = (Uint16)(bit * 2U);
        mux_mask = 3UL << mux_shift;
        data_mask = 1UL << bit;

        GpioCtrlRegs.GPAMUX1.all = (GpioCtrlRegs.GPAMUX1.all & ~mux_mask) |
                                   (((Uint32)cfg->mux << mux_shift) & mux_mask);
        GpioCtrlRegs.GPAQSEL1.all = (GpioCtrlRegs.GPAQSEL1.all & ~mux_mask) |
                                    (((Uint32)cfg->qual << mux_shift) & mux_mask);
        gpio_apply_dir_pud_latch(&GpioCtrlRegs.GPADIR.all, &GpioCtrlRegs.GPAPUD.all,
                                 &GpioDataRegs.GPASET.all, &GpioDataRegs.GPACLEAR.all,
                                 data_mask, cfg);
    }
    else if (cfg->pin <= 31U)
    {
        bit = (Uint16)(cfg->pin - 16U);
        mux_shift = (Uint16)(bit * 2U);
        mux_mask = 3UL << mux_shift;
        data_mask = 1UL << cfg->pin;

        GpioCtrlRegs.GPAMUX2.all = (GpioCtrlRegs.GPAMUX2.all & ~mux_mask) |
                                   (((Uint32)cfg->mux << mux_shift) & mux_mask);
        GpioCtrlRegs.GPAQSEL2.all = (GpioCtrlRegs.GPAQSEL2.all & ~mux_mask) |
                                    (((Uint32)cfg->qual << mux_shift) & mux_mask);
        gpio_apply_dir_pud_latch(&GpioCtrlRegs.GPADIR.all, &GpioCtrlRegs.GPAPUD.all,
                                 &GpioDataRegs.GPASET.all, &GpioDataRegs.GPACLEAR.all,
                                 data_mask, cfg);
    }
    else if (cfg->pin <= 47U)
    {
        bit = (Uint16)(cfg->pin - 32U);
        mux_shift = (Uint16)(bit * 2U);
        mux_mask = 3UL << mux_shift;
        data_mask = 1UL << bit;

        GpioCtrlRegs.GPBMUX1.all = (GpioCtrlRegs.GPBMUX1.all & ~mux_mask) |
                                   (((Uint32)cfg->mux << mux_shift) & mux_mask);
        GpioCtrlRegs.GPBQSEL1.all = (GpioCtrlRegs.GPBQSEL1.all & ~mux_mask) |
                                    (((Uint32)cfg->qual << mux_shift) & mux_mask);
        gpio_apply_dir_pud_latch(&GpioCtrlRegs.GPBDIR.all, &GpioCtrlRegs.GPBPUD.all,
                                 &GpioDataRegs.GPBSET.all, &GpioDataRegs.GPBCLEAR.all,
                                 data_mask, cfg);
    }
    else if (cfg->pin <= 63U)
    {
        bit = (Uint16)(cfg->pin - 48U);
        mux_shift = (Uint16)(bit * 2U);
        mux_mask = 3UL << mux_shift;
        data_mask = 1UL << (cfg->pin - 32U);

        GpioCtrlRegs.GPBMUX2.all = (GpioCtrlRegs.GPBMUX2.all & ~mux_mask) |
                                   (((Uint32)cfg->mux << mux_shift) & mux_mask);
        GpioCtrlRegs.GPBQSEL2.all = (GpioCtrlRegs.GPBQSEL2.all & ~mux_mask) |
                                    (((Uint32)cfg->qual << mux_shift) & mux_mask);
        gpio_apply_dir_pud_latch(&GpioCtrlRegs.GPBDIR.all, &GpioCtrlRegs.GPBPUD.all,
                                 &GpioDataRegs.GPBSET.all, &GpioDataRegs.GPBCLEAR.all,
                                 data_mask, cfg);
    }
    else if (cfg->pin <= 79U)
    {
        bit = (Uint16)(cfg->pin - 64U);
        mux_shift = (Uint16)(bit * 2U);
        mux_mask = 3UL << mux_shift;
        data_mask = 1UL << bit;

        GpioCtrlRegs.GPCMUX1.all = (GpioCtrlRegs.GPCMUX1.all & ~mux_mask) |
                                   (((Uint32)cfg->mux << mux_shift) & mux_mask);
        gpio_apply_dir_pud_latch(&GpioCtrlRegs.GPCDIR.all, &GpioCtrlRegs.GPCPUD.all,
                                 &GpioDataRegs.GPCSET.all, &GpioDataRegs.GPCCLEAR.all,
                                 data_mask, cfg);
    }
    else if (cfg->pin <= 87U)
    {
        bit = (Uint16)(cfg->pin - 80U);
        mux_shift = (Uint16)(bit * 2U);
        mux_mask = 3UL << mux_shift;
        data_mask = 1UL << (cfg->pin - 64U);

        GpioCtrlRegs.GPCMUX2.all = (GpioCtrlRegs.GPCMUX2.all & ~mux_mask) |
                                   (((Uint32)cfg->mux << mux_shift) & mux_mask);
        gpio_apply_dir_pud_latch(&GpioCtrlRegs.GPCDIR.all, &GpioCtrlRegs.GPCPUD.all,
                                 &GpioDataRegs.GPCSET.all, &GpioDataRegs.GPCCLEAR.all,
                                 data_mask, cfg);
    }
}

/*
 * Same defaults as TI InitGpio(): GPIO input, pull-ups on, qual synch.
 */
static void gpio_init_defaults(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX1.all = 0x0000;
    GpioCtrlRegs.GPAQSEL1.all = 0x0000;
    GpioCtrlRegs.GPAMUX2.all = 0x0000;
    GpioCtrlRegs.GPAQSEL2.all = 0x0000;
    GpioCtrlRegs.GPBMUX1.all = 0x0000;
    GpioCtrlRegs.GPBQSEL1.all = 0x0000;
    GpioCtrlRegs.GPBMUX2.all = 0x0000;
    GpioCtrlRegs.GPBQSEL2.all = 0x0000;
    GpioCtrlRegs.GPCMUX1.all = 0x0000;
    GpioCtrlRegs.GPCMUX2.all = 0x0000;

    GpioCtrlRegs.GPADIR.all = 0x0000;
    GpioCtrlRegs.GPBDIR.all = 0x0000;
    GpioCtrlRegs.GPCDIR.all = 0x0000;

    GpioCtrlRegs.GPAPUD.all = 0x0000;
    GpioCtrlRegs.GPBPUD.all = 0x0000;
    GpioCtrlRegs.GPCPUD.all = 0x0000;
    EDIS;
}

void gpio_write(Uint16 pin, Uint16 level)
{
    Uint32 mask;

    if (pin <= 31U)
    {
        mask = 1UL << pin;
        if (level != 0U)
        {
            GpioDataRegs.GPASET.all = mask;
        }
        else
        {
            GpioDataRegs.GPACLEAR.all = mask;
        }
    }
    else if (pin <= 63U)
    {
        mask = 1UL << (pin - 32U);
        if (level != 0U)
        {
            GpioDataRegs.GPBSET.all = mask;
        }
        else
        {
            GpioDataRegs.GPBCLEAR.all = mask;
        }
    }
    else if (pin <= 87U)
    {
        mask = 1UL << (pin - 64U);
        if (level != 0U)
        {
            GpioDataRegs.GPCSET.all = mask;
        }
        else
        {
            GpioDataRegs.GPCCLEAR.all = mask;
        }
    }
}

void gpio_toggle(Uint16 pin)
{
    if (pin <= 31U)
    {
        GpioDataRegs.GPATOGGLE.all = 1UL << pin;
    }
    else if (pin <= 63U)
    {
        GpioDataRegs.GPBTOGGLE.all = 1UL << (pin - 32U);
    }
    else if (pin <= 87U)
    {
        GpioDataRegs.GPCTOGGLE.all = 1UL << (pin - 64U);
    }
}

Uint16 gpio_read(Uint16 pin)
{
    if (pin <= 31U)
    {
        return (Uint16)((GpioDataRegs.GPADAT.all >> pin) & 1UL);
    }
    if (pin <= 63U)
    {
        return (Uint16)((GpioDataRegs.GPBDAT.all >> (pin - 32U)) & 1UL);
    }
    if (pin <= 87U)
    {
        return (Uint16)((GpioDataRegs.GPCDAT.all >> (pin - 64U)) & 1UL);
    }
    return 0U;
}

void gpio_init(void)
{
    Uint16 i;

    gpio_init_defaults();

    EALLOW;
    for (i = 0U; i < (Uint16)(sizeof(gpio_pin_table) / sizeof(gpio_pin_table[0])); i++)
    {
        gpio_apply_pin(&gpio_pin_table[i]);
    }
    EDIS;
}
