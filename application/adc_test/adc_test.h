#ifndef ADC_TEST_H
#define ADC_TEST_H

#include "../../driver_interface/adc/adc_interface.h"

/*
 * Copy enabled ADC pins (ADC_SLOT_LIST) into adc_obs[].
 * Index by pin: adc_obs[ADC_CH_A0]. Watch in CCS.
 * Call from the idle loop; copies every 100 ms (1 ms timer_tick).
 */
extern volatile Uint16 adc_obs[16];

void adc_test(void);

#endif /* ADC_TEST_H */
