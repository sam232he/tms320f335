#include "adc_test.h"
#include "../../driver_interface/timer/timer_interface.h"

static const adc_ch_t adc_test_ch[] = { ADC_SLOT_LIST };

static Uint32 adc_test_last_tick;

volatile Uint16 adc_obs[16];

void adc_test(void)
{
    Uint32 now;
    Uint16 i;
    Uint16 n;

    now = timer_tick();
    if ((now - adc_test_last_tick) < 100UL) /* 100 ms */
    {
        return;
    }
    adc_test_last_tick = now;

    n = (Uint16)(sizeof(adc_test_ch) / sizeof(adc_test_ch[0]));
    for (i = 0U; i < n; i++)
    {
        adc_obs[adc_test_ch[i]] = adc_read(adc_test_ch[i]);
    }
}
