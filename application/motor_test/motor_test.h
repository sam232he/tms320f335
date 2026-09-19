#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

#include "../../driver_interface/drive/drive_interface.h"

/*
 * Watch-window motor command. motor_test owns PWM5/6.
 *
 * Watch motor1 / motor2. Each defaults to MOTOR_IDLE (50% PWM, RST_AB held).
 *   dir  = MOTOR_IDLE, MOTOR_FORWARD, MOTOR_REVERSE
 *   duty = 0..100  (speed, not raw PWM)
 *   pwm  = live complementary PWM (status)
 *
 * Mapping onto the complementary H-bridge:
 *   IDLE or duty 0 -> 50% PWM (near-zero voltage)
 *   FORWARD  100   -> 100% PWM
 *   REVERSE  100   -> 0% PWM
 * Applied PWM slews 1% / 50 ms. Start with a small duty (5..10).
 *
 * Motor 2 RESET_CD is not on the DSP; IDLE is 50% only.
 * FAULT while a motor is not IDLE forces both dirs back to MOTOR_IDLE.
 */

typedef enum
{
    MOTOR_IDLE = 0,
    MOTOR_FORWARD = 1,
    MOTOR_REVERSE = 2
} motor_dir_e;

typedef struct
{
    motor_dir_e dir;
    Uint16 duty;
    Uint16 pwm;
} motor_t;

#define MOTOR_TEST_CENTER               50U
#define MOTOR_TEST_STEP                 1U
#define MOTOR_TEST_STEP_MS              50UL

extern volatile motor_t motor1;
extern volatile motor_t motor2;
extern volatile Uint16 motor_test_rst_ab;
extern volatile Uint16 motor_test_fault;
extern volatile Uint16 motor_test_otw;

void motor_test(void);

#endif /* MOTOR_TEST_H */
