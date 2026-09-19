#include "motor_test.h"
#include "../../driver_interface/timer/timer_interface.h"

volatile motor_t motor1 = { MOTOR_IDLE, 0U, MOTOR_TEST_CENTER };
volatile motor_t motor2 = { MOTOR_IDLE, 0U, MOTOR_TEST_CENTER };
volatile Uint16 motor_test_rst_ab;
volatile Uint16 motor_test_fault;
volatile Uint16 motor_test_otw;

static Uint32 motor_test_step_tick;

static Uint16 motor_test_map(motor_dir_e dir, Uint16 mag)
{
    Uint16 delta;

    if (dir == MOTOR_IDLE)
    {
        return MOTOR_TEST_CENTER;
    }

    /* 0 -> 0, 1 -> 1, 100 -> 50 so FORWARD 100 = 100% PWM. */
    delta = (Uint16)(((Uint32)mag * 50UL + 50UL) / 100UL);
    if (delta > 50U)
    {
        delta = 50U;
    }
    if (dir == MOTOR_FORWARD)
    {
        return (Uint16)(MOTOR_TEST_CENTER + delta);
    }
    return (Uint16)(MOTOR_TEST_CENTER - delta);
}

static Uint16 motor_test_slew(Uint16 now, Uint16 tgt)
{
    if (now == tgt)
    {
        return now;
    }
    if (now < tgt)
    {
        now = (Uint16)(now + MOTOR_TEST_STEP);
        if (now > tgt)
        {
            now = tgt;
        }
        return now;
    }
    now = (Uint16)(now - MOTOR_TEST_STEP);
    if (now < tgt)
    {
        now = tgt;
    }
    return now;
}

/* 50% PWM on both = near-zero H-bridge voltage (still switching). */
static void motor_test_set_zero_voltage(void)
{
    motor1.pwm = MOTOR_TEST_CENTER;
    motor2.pwm = MOTOR_TEST_CENTER;
    drive_set_duty(DRIVE_1, motor1.pwm);
    drive_set_duty(DRIVE_2, motor2.pwm);
}

/* RST_AB low: motor 1 outputs Hi-Z. Motor 2 is unchanged (no RESET_CD). */
static void motor_test_hiz_motor1(void)
{
    (void)drive_disable(DRIVE_1);
    motor_test_rst_ab = 0U;
}

static void motor1_update(void)
{
    Uint16 tgt;

    tgt = motor_test_map(motor1.dir, motor1.duty);

    if (motor1.dir != MOTOR_IDLE)
    {
        if (motor_test_rst_ab == 0U)
        {
            motor_test_rst_ab = drive_enable(DRIVE_1);
        }
        motor1.pwm = motor_test_slew(motor1.pwm, tgt);
    }
    else
    {
        motor1.pwm = motor_test_slew(motor1.pwm, MOTOR_TEST_CENTER);
        if (motor1.pwm == MOTOR_TEST_CENTER)
        {
            motor_test_hiz_motor1();
        }
    }

    drive_set_duty(DRIVE_1, motor1.pwm);
}

static void motor2_update(void)
{
    motor2.pwm = motor_test_slew(motor2.pwm, motor_test_map(motor2.dir, motor2.duty));
    drive_set_duty(DRIVE_2, motor2.pwm);
}

void motor_test(void)
{
    Uint32 now;

    motor_test_fault = drive_fault();
    motor_test_otw = drive_otw();
    if ((motor_test_fault != 0U) &&
        ((motor1.dir != MOTOR_IDLE) || (motor2.dir != MOTOR_IDLE)))
    {
        motor1.dir = MOTOR_IDLE;
        motor2.dir = MOTOR_IDLE;
        motor_test_set_zero_voltage();
        motor_test_hiz_motor1();
        return;
    }

    now = timer_tick();
    if ((now - motor_test_step_tick) < MOTOR_TEST_STEP_MS)
    {
        return;
    }
    motor_test_step_tick = now;

    motor1_update();
    motor2_update();
}
