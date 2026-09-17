#include "init_interface.h"
#include "gpio_interface.h"
#include "can_interface.h"
#include "pwm_interface.h"
#include "drive_interface.h"
#include "adc_interface.h"
#include "sci_interface.h"
#include "i2c_interface.h"
#include "eeprom_interface.h"
#include "encoder_interface.h"
#include "timer_interface.h"
#include "DSP28x_Project.h"
#include <string.h>

void system_init(void)
{
    InitSysCtrl();

    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);
    InitFlash();

    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    gpio_init();
    encoder_init();
    adc_init();
    pwm_init();
    drive_init();
    can_init();
    sci_init();
    i2c_init();
    eeprom_init();
    timer_init();
DELAY_US(800000);
    EINT;
}
