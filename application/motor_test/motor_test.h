#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

#include "../../driver_interface/drive/drive_interface.h"

/*
 * Debugger-controlled crawl around 50% duty (near-zero H-bridge voltage).
 * motor_test owns PWM5/6. pwm_test is not called from main.
 *
 * motor_test_sel (CCS watch):
 *   0 idle (default): both 50%, RST_AB held
 *   1 motor 1 (PWM_6 / RST_AB)
 *   2 motor 2 (PWM_5; RESET_CD is hardware-only)
 *   3 both
 * Anything else is idle.
 *
 * Sweep is CENTER +/- SPAN (45..55%). SPAN is capped at SPAN_MAX.
 * Park 200 ms at 50% before enable and after disable.
 * FAULT aborts only in RUN (after enable, or while PWM5 is sweeping).
 * OTW is reported, not an abort. Invalid sel is idle.
 *
 * motor_test_state: 0 idle, 1 hold-enable, 2 run, 3 hold-disable
 * motor_test_duty[0] = DRIVE_1, [1] = DRIVE_2
 */

#define MOTOR_TEST_SEL_IDLE             0U
#define MOTOR_TEST_SEL_1                1U
#define MOTOR_TEST_SEL_2                2U
#define MOTOR_TEST_SEL_3                3U

#define MOTOR_TEST_STATE_IDLE           0U
#define MOTOR_TEST_STATE_HOLD_ENABLE    1U
#define MOTOR_TEST_STATE_RUN            2U
#define MOTOR_TEST_STATE_HOLD_DISABLE   3U

#define MOTOR_TEST_CENTER               50U
#define MOTOR_TEST_SPAN                 5U
#define MOTOR_TEST_SPAN_MAX             10U
#define MOTOR_TEST_STEP                 1U
#define MOTOR_TEST_STEP_MS              50UL
#define MOTOR_TEST_HOLD_MS              200UL

#if (MOTOR_TEST_SPAN > MOTOR_TEST_SPAN_MAX)
#error "MOTOR_TEST_SPAN exceeds MOTOR_TEST_SPAN_MAX."
#endif
#if (MOTOR_TEST_SPAN > MOTOR_TEST_CENTER)
#error "MOTOR_TEST_SPAN larger than CENTER."
#endif
#if ((MOTOR_TEST_CENTER + MOTOR_TEST_SPAN) > 100U)
#error "MOTOR_TEST sweep exceeds 100%."
#endif

extern volatile Uint16 motor_test_sel;
extern volatile Uint16 motor_test_state;
extern volatile Uint16 motor_test_duty[2];
extern volatile Uint16 motor_test_rst_ab;
extern volatile Uint16 motor_test_fault;
extern volatile Uint16 motor_test_otw;

void motor_test(void);

#endif /* MOTOR_TEST_H */
