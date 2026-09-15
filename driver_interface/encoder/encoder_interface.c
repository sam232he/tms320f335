#include "encoder_interface.h"
#include "gpio_interface.h"
#include "DSP28x_Project.h"

static const Uint16 encoder_clk_pin[] =
{
    PIN_ENC1_CLK,
    PIN_ENC2_CLK
};

static const Uint16 encoder_data_pin[] =
{
    PIN_ENC1_DATA,
    PIN_ENC2_DATA
};

void encoder_init(void)
{
    gpio_write(encoder_clk_pin[ENC_1], 1U);
    gpio_write(encoder_clk_pin[ENC_2], 1U);
}

Uint32 encoder_read_counts(Uint16 enc)
{
    Uint32 counts;
    Uint16 i;
    Uint16 clk_pin;
    Uint16 data_pin;

    clk_pin = encoder_clk_pin[enc];
    data_pin = encoder_data_pin[enc];
    counts = 0UL;

    for (i = 0U; i < ENC_BIT_COUNT; i++)
    {
        gpio_write(clk_pin, 0U);
        DELAY_US(1);
        gpio_write(clk_pin, 1U);
        DELAY_US(1);
        counts = (counts << 1) | (Uint32)gpio_read(data_pin);
    }

    return counts;
}
