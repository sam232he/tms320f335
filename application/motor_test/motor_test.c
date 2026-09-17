#include "motor_test.h"
#include "../../driver_interface/timer/timer_interface.h"

#define MOTOR_TEST_LO   (MOTOR_TEST_CENTER - MOTOR_TEST_SPAN)
#define MOTOR_TEST_HI   (MOTOR_TEST_CENTER + MOTOR_TEST_SPAN)

volatile Uint16 motor_test_sel;
volatile Uint16 motor_test_state;
volatile Uint16 motor_test_duty[2] = { MOTOR_TEST_CENTER, MOTOR_TEST_CENTER };
volatile Uint16 motor_test_rst_ab;
volatile Uint16 motor_test_fault;
volatile Uint16 motor_test_otw;

static Uint16 motor_test_run_sel;
static Uint16 motor_test_pending_sel;
static Uint16 motor_test_sweep;
static Uint16 motor_test_sweep_up;
static Uint32 motor_test_hold_tick;
static Uint32 motor_test_step_tick;

static Uint16 motor_test_norm_sel(Uint16 sel)
{
    if ((sel == MOTOR_TEST_SEL_1) ||
        (sel == MOTOR_TEST_SEL_2) ||
        (sel == MOTOR_TEST_SEL_3))
    {
        return sel;
    }
    return MOTOR_TEST_SEL_IDLE;
}

static Uint16 motor_test_wants_m1(Uint16 sel)
{
    return ((sel == MOTOR_TEST_SEL_1) || (sel == MOTOR_TEST_SEL_3)) ? 1U : 0U;
}

static Uint16 motor_test_wants_m2(Uint16 sel)
{
    return ((sel == MOTOR_TEST_SEL_2) || (sel == MOTOR_TEST_SEL_3)) ? 1U : 0U;
}

static void motor_test_park(void)
{
    drive_set_duty(DRIVE_1, MOTOR_TEST_CENTER);
    drive_set_duty(DRIVE_2, MOTOR_TEST_CENTER);
    motor_test_duty[0] = MOTOR_TEST_CENTER;
    motor_test_duty[1] = MOTOR_TEST_CENTER;
    motor_test_sweep = MOTOR_TEST_CENTER;
    motor_test_sweep_up = 1U;
}

static void motor_test_hold_rst(void)
{
    (void)drive_disable(DRIVE_1);
    motor_test_rst_ab = 0U;
}

static void motor_test_release_rst(Uint16 sel)
{
    if (motor_test_wants_m1(sel) != 0U)
    {
        motor_test_rst_ab = drive_enable(DRIVE_1);
    }
    else
    {
        motor_test_hold_rst();
    }
}

static void motor_test_apply_sweep(Uint16 sel, Uint16 sweep)
{
    Uint16 d1;
    Uint16 d2;

    d1 = MOTOR_TEST_CENTER;
    d2 = MOTOR_TEST_CENTER;
    if (motor_test_wants_m1(sel) != 0U)
    {
        d1 = sweep;
    }
    if (motor_test_wants_m2(sel) != 0U)
    {
        d2 = sweep;
    }

    drive_set_duty(DRIVE_1, d1);
    drive_set_duty(DRIVE_2, d2);
    motor_test_duty[0] = d1;
    motor_test_duty[1] = d2;
}

static void motor_test_advance_sweep(void)
{
    if (motor_test_sweep_up != 0U)
    {
        if (motor_test_sweep >= MOTOR_TEST_HI)
        {
            motor_test_sweep = MOTOR_TEST_HI;
            motor_test_sweep_up = 0U;
        }
        else
        {
            motor_test_sweep += MOTOR_TEST_STEP;
        }
    }
    else if (motor_test_sweep <= MOTOR_TEST_LO)
    {
        motor_test_sweep = MOTOR_TEST_LO;
        motor_test_sweep_up = 1U;
    }
    else
    {
        motor_test_sweep -= MOTOR_TEST_STEP;
    }
}

static void motor_test_enter_hold_enable(Uint16 sel, Uint32 now)
{
    motor_test_pending_sel = sel;
    motor_test_state = MOTOR_TEST_STATE_HOLD_ENABLE;
    motor_test_hold_tick = now;
    motor_test_park();
    motor_test_hold_rst();
}

static void motor_test_enter_hold_disable(Uint32 now)
{
    motor_test_sel = MOTOR_TEST_SEL_IDLE;
    motor_test_run_sel = MOTOR_TEST_SEL_IDLE;
    motor_test_pending_sel = MOTOR_TEST_SEL_IDLE;
    motor_test_state = MOTOR_TEST_STATE_HOLD_DISABLE;
    motor_test_hold_tick = now;
    motor_test_park();
    motor_test_hold_rst();
}

static void motor_test_update_status(void)
{
    motor_test_fault = drive_fault();
    motor_test_otw = drive_otw();
}

void motor_test(void)
{
    Uint32 now;
    Uint16 want;

    now = timer_tick();
    want = motor_test_norm_sel(motor_test_sel);
    if (want != motor_test_sel)
    {
        motor_test_sel = want;
    }

    motor_test_update_status();

    switch (motor_test_state)
    {
        case MOTOR_TEST_STATE_HOLD_ENABLE:
            if (want == MOTOR_TEST_SEL_IDLE)
            {
                motor_test_enter_hold_disable(now);
                break;
            }
            if (want != motor_test_pending_sel)
            {
                motor_test_enter_hold_enable(want, now);
                break;
            }
            motor_test_park();
            if ((now - motor_test_hold_tick) >= MOTOR_TEST_HOLD_MS)
            {
                motor_test_release_rst(want);
                motor_test_run_sel = want;
                motor_test_state = MOTOR_TEST_STATE_RUN;
                motor_test_step_tick = now;
                motor_test_apply_sweep(want, motor_test_sweep);
            }
            break;

        case MOTOR_TEST_STATE_RUN:
            if (motor_test_fault != 0U)
            {
                motor_test_enter_hold_disable(now);
                break;
            }
            if (want == MOTOR_TEST_SEL_IDLE)
            {
                motor_test_enter_hold_disable(now);
                break;
            }
            if (want != motor_test_run_sel)
            {
                motor_test_enter_hold_enable(want, now);
                break;
            }
            if ((now - motor_test_step_tick) >= MOTOR_TEST_STEP_MS)
            {
                motor_test_step_tick = now;
                motor_test_advance_sweep();
                motor_test_apply_sweep(want, motor_test_sweep);
            }
            break;

        case MOTOR_TEST_STATE_HOLD_DISABLE:
            motor_test_park();
            motor_test_hold_rst();
            if (want != MOTOR_TEST_SEL_IDLE)
            {
                motor_test_enter_hold_enable(want, now);
                break;
            }
            if ((now - motor_test_hold_tick) >= MOTOR_TEST_HOLD_MS)
            {
                motor_test_state = MOTOR_TEST_STATE_IDLE;
            }
            break;

        case MOTOR_TEST_STATE_IDLE:
        default:
            motor_test_state = MOTOR_TEST_STATE_IDLE;
            motor_test_park();
            motor_test_hold_rst();
            if (want != MOTOR_TEST_SEL_IDLE)
            {
                motor_test_enter_hold_enable(want, now);
            }
            break;
    }
}
