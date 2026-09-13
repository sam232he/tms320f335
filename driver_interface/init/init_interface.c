#include "init_interface.h"
#include "gpio_interface.h"
#include "can_interface.h"
#include "pwm_interface.h"
#include "adc_interface.h"
#include "spi_interface.h"
#include "sci_interface.h"
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
    adc_init();
    pwm_init();
    can_init();
    spi_init();
    sci_init();
    sci_cmd_init();
    timer_init();

    EINT;
}
