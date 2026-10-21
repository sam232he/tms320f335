#include <stdint.h>
#include "adc_test.h"
#include "../../driver_interface/timer/timer_interface.h"

static const adc_ch_t adc_test_ch[] = { ADC_SLOT_LIST };

static uint32_t adc_test_last_tick = 0UL;   /* COFF: .ebss is not zeroed */

volatile uint16_t adc_test_counts[16];
volatile float adc_test_volts[16];

void adc_test(void)
{
    uint32_t now;
    uint16_t i;
    uint16_t n;
    uint16_t code;

    now = timer_tick();
    if ((now - adc_test_last_tick) < 100UL) /* 100 ms */
    {
        return;
    }
    adc_test_last_tick = now;

    n = (uint16_t)(sizeof(adc_test_ch) / sizeof(adc_test_ch[0]));
    for (i = 0U; i < n; i++)
    {
        code = adc_read(adc_test_ch[i]);
        adc_test_counts[adc_test_ch[i]] = code;
        /* Direct ADC pin voltage only. No ACS711 / MPXHZ6400 conversion. */
        adc_test_volts[adc_test_ch[i]] = adc_pin_voltage(code);
    }
}
