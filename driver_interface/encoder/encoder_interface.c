#include <stdint.h>
#include "encoder_interface.h"
#include "gpio_interface.h"
#include "DSP28x_Project.h"

static const uint16_t encoder_clk_pin[] =
{
    PIN_ENC1_CLK,
    PIN_ENC2_CLK
};

static const uint16_t encoder_data_pin[] =
{
    PIN_ENC1_DATA,
    PIN_ENC2_DATA
};

void encoder_init(void)
{
    gpio_write(encoder_clk_pin[ENC_1], 1U);
    gpio_write(encoder_clk_pin[ENC_2], 1U);
}

uint32_t encoder_read_counts(uint16_t enc)
{
    uint32_t counts;
    uint16_t i;
    uint16_t clk_pin;
    uint16_t data_pin;

    clk_pin = encoder_clk_pin[enc];
    data_pin = encoder_data_pin[enc];
    counts = 0UL;

    for (i = 0U; i < ENC_BIT_COUNT; i++)
    {
        gpio_write(clk_pin, 0U);
        DELAY_US(ENC_CLK_HALF_US);
        gpio_write(clk_pin, 1U);
        DELAY_US(ENC_CLK_HALF_US);
        counts = (counts << 1) | (uint32_t)gpio_read(data_pin);
    }

    return counts;
}
