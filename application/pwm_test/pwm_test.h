#ifndef PWM_TEST_H
#define PWM_TEST_H

#include "../../driver_interface/pwm/pwm_interface.h"

/*
 * Triangle-ramp duty on PWM1..PWM4. PWM5 is the ADC SOCA timer.
 * PWM6 stays off (GPIO10 is CANRXB). Call from the idle loop.
 * pwm_obs[PWM_1] .. pwm_obs[PWM_4] are duty percent (0..100). Watch in CCS.
 */
extern volatile Uint16 pwm_obs[7];

void pwm_test(void);

#endif /* PWM_TEST_H */
