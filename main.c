/**
 * main.c
 */
#include "init_interface.h"
#include "can_interface.h"
#include "can_send.h"
#include "adc_test.h"
#include "gpio_test.h"
#include "pwm_test.h"
#include "sci_test.h"
#include "spi_test.h"

int main(void)
{
    system_init();

    for (;;)
    {
        can_prepare_tx();
        can_write(CAN_A);
        adc_test();
        gpio_test();
        pwm_test();
        sci_test();
        sci_write(SCI);
        sci_write(SCI_CMD);
        spi_test();
    }
}
