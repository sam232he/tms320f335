#ifndef ADC_TEST_H
#define ADC_TEST_H

#include "../../driver_interface/adc/adc_interface.h"

/*
 * Copy enabled ADC pins (ADC_SLOT_LIST) into adc_test_counts[] /
 * adc_test_volts[]. Index by pin: adc_test_counts[ADC_CH_A0].
 *
 * adc_test_counts = raw 12-bit counts
 * adc_test_volts  = ADC pin voltage = adc_code * 3.0 / 4096.0
 *
 * No schematic divider is populated. Sensor amps / kPa conversion is
 * intentionally disabled. Open sensor nets are not fatal.
 *
 * Call from the idle loop; copies every 100 ms (1 ms timer_tick).
 */
extern volatile Uint16 adc_test_counts[16];
extern volatile float adc_test_volts[16];

void adc_test(void);

#endif /* ADC_TEST_H */
