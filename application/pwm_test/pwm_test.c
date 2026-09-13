#include "pwm_test.h"
#include "../../driver_interface/timer/timer_interface.h"

#define PWM_TEST_STEP               1U
#define PWM_TEST_MAX                100U

typedef struct
{
    pwm_module_t mod;
    Uint16 duty;
    Uint16 up;          /* 1 = ramp toward 100%, 0 = ramp toward 0% */
} pwm_test_ch_t;

/* Staggered start so each pin has a different duty at any instant. */
static pwm_test_ch_t pwm_test_ch[] =
{
    { PWM_1,  0U, 1U },
    { PWM_2, 25U, 1U },
    { PWM_3, 50U, 0U },
    { PWM_4, 75U, 0U }
};

static Uint32 pwm_test_last_tick;

volatile Uint16 pwm_obs[7];

static void pwm_test_advance(pwm_test_ch_t *ch)
{
    if (ch->up != 0U)
    {
        if (ch->duty >= (PWM_TEST_MAX - PWM_TEST_STEP))
        {
            ch->duty = PWM_TEST_MAX;
            ch->up = 0U;
        }
        else
        {
            ch->duty += PWM_TEST_STEP;
        }
    }
    else if (ch->duty <= PWM_TEST_STEP)
    {
        ch->duty = 0U;
        ch->up = 1U;
    }
    else
    {
        ch->duty -= PWM_TEST_STEP;
    }
}

void pwm_test(void)
{
    Uint32 now;
    Uint16 i;
    Uint16 n;

    now = timer_tick();
    if ((now - pwm_test_last_tick) < 10UL) /* 10 ms */
    {
        return;
    }
    pwm_test_last_tick = now;

    n = (Uint16)(sizeof(pwm_test_ch) / sizeof(pwm_test_ch[0]));
    for (i = 0U; i < n; i++)
    {
        pwm_set_duty(pwm_test_ch[i].mod, pwm_test_ch[i].duty);
        pwm_obs[pwm_test_ch[i].mod] = pwm_test_ch[i].duty;
        pwm_test_advance(&pwm_test_ch[i]);
    }
}
