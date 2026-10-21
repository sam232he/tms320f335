/**
 * main.c
 */
#include "init_interface.h"
#include "application/application_init/application_initialize.h"
#include "can_send.h"
#include "adc_test.h"
#include "gpio_test.h"
#include "sci_test.h"
#include "eeprom_test.h"
#include "encoder_test.h"
#include "motor_test.h"
#include "pwm_test.h"
#include "application/telemetry/telemetry.h"
#include "driver_interface/sci/sci_interface.h"

volatile Uint16 eeprom_test_run = 0;

int main(void)
{
    system_init();
    application_initialize();

    for (;;)
    {
        can_test();
        adc_test();
        gpio_test();
        sci_test();
        pwm_test();
        (void)telemetry_update();
        sci_write(SCI_B);
//        motor_test();
        encoder_test();
        if (eeprom_test_run != 0U)
        {
            eeprom_test_run = 0U;
            eeprom_test();
        }
    }
}
