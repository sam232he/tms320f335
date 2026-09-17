#ifndef INIT_INTERFACE_H
#define INIT_INTERFACE_H

#include "clock_config.h"
#include "board_pins.h"

/*
 * Bring the CPU to a known run state from flash:
 * watchdog off, PLL, copy ramfuncs, flash waitstates, PIE table,
 * gpio_init(), encoder_init(), adc_init(), pwm_init(), drive_init(),
 * can_init(), sci_init(), i2c_init(), eeprom_init(), timer_init(), EINT.
 *
 * PWM_OUTPUT_ENABLE = 1 muxes GPIO8..11 to EPWM5/6 (schematic motor PWM).
 * Complementary PWM idles at 50%. RST_AB stays low until motor_test
 * calls drive_enable().
 */
void system_init(void);

#endif /* INIT_INTERFACE_H */
