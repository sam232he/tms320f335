/**
 * main.c
 */
#include "init_interface.h"
#include "can_send.h"
#include "adc_test.h"
#include "gpio_test.h"
#include "pwm_test.h"
#include "sci_test.h"
#include "i2c_test.h"
#include "encoder_test.h"

int main(void)
{
    system_init();

    for (;;)
    {
        can_test();
        adc_test();
        gpio_test();
        pwm_test();
        sci_test();
        i2c_test();
        encoder_test();
    }
}
