#ifndef INIT_INTERFACE_H
#define INIT_INTERFACE_H

#include "clock_config.h"

/*
 * Bring the CPU to a known run state from flash:
 * watchdog off, PLL, copy ramfuncs, flash waitstates, PIE table,
 * gpio_init(), adc_init(), pwm_init(), can_init(),
 * spi_init(), sci_init(), sci_cmd_init(), timer_init(), EINT.
 */
void system_init(void);

#endif /* INIT_INTERFACE_H */
