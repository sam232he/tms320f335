#include "drive_interface.h"
#include "gpio_interface.h"

void drive_init(void)
{
    gpio_write(PIN_RST_AB, 0U);
}

Uint16 drive_enable(Uint16 motor)
{
    if (motor != DRIVE_1)
    {
        return 0U;
    }

    gpio_write(PIN_RST_AB, 1U);
    return 1U;
}

Uint16 drive_disable(Uint16 motor)
{
    if (motor != DRIVE_1)
    {
        return 0U;
    }

    gpio_write(PIN_RST_AB, 0U);
    return 1U;
}

/*
 *  ~50% is near-zero voltage. 0 and 100 are
 * opposite full DC, not coast/stop. If motor_app uses 0 as off, the
 * motor slams reverse as soon as reset is released.
 */
void drive_set_duty(Uint16 motor, Uint16 duty)
{
    if (motor == DRIVE_1)
    {
        pwm_set_duty(PWM_6, duty);
    }
    else if (motor == DRIVE_2)
    {
        pwm_set_duty(PWM_5, duty);
    }
}

Uint16 drive_fault(void)
{
    return (gpio_read(PIN_FAULT) == 0U) ? 1U : 0U;
}

Uint16 drive_otw(void)
{
    return (gpio_read(PIN_OTW) == 0U) ? 1U : 0U;
}
