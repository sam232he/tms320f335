#ifndef PWM_TEST_H
#define PWM_TEST_H

#include "../../driver_interface/pwm/pwm_interface.h"

/*
 * Triangle-ramp duty on the schematic PWM modules:
 *   PWM6 = DSP_EPWM1 (GPIO10 EPWM6A = 1A, GPIO11 EPWM6B = 1B)
 *   PWM5 = DSP_EPWM2 (GPIO9  EPWM5B = 2A, GPIO8  EPWM5A = 2B) and ADC SOCA
 * pwm_test_duty[PWM_5] / pwm_test_duty[PWM_6] are duty percent (0..100).
 */
extern volatile Uint16 pwm_test_duty[7];

void pwm_test(void);

#endif /* PWM_TEST_H */
