#ifndef INIT_INTERFACE_H
#define INIT_INTERFACE_H

#include "clock_config.h"
#include "board_pins.h"

/*
 * Bring the CPU to a known run state from flash:
 * watchdog off, PLL, copy ramfuncs, flash waitstates, PIE table,
 * gpio_init(), encoder_init(), adc_init(), pwm_init(), can_init(),
 * sci_init(), i2c_init(), eeprom_init(), timer_init(), EINT.
 *
 * PWM_OUTPUT_ENABLE = 1 muxes GPIO8..11 to EPWM5/6 (schematic motor PWM).
 * RST_AB stays low so the DRV8432 power stage stays Hi-Z.
 */
void system_init(void);

#endif /* INIT_INTERFACE_H */
