/**
 * main.c
 */
#include "init_interface.h"
#include "can_send.h"
#include "adc_test.h"
#include "gpio_test.h"
#include "sci_test.h"
#include "eeprom_test.h"
#include "encoder_test.h"
#include "motor_test.h"

volatile Uint16 eeprom_test_run;

int main(void)
{
    system_init();

    for (;;)
    {
        can_test();
        adc_test();
        gpio_test();
        sci_test();
        motor_test();
        encoder_test();
        if (eeprom_test_run != 0U)
        {
            eeprom_test_run = 0U;
            eeprom_test();
        }
    }
}
