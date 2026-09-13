#ifndef TIMER_INTERFACE_H
#define TIMER_INTERFACE_H

#include "../init/clock_config.h"
#include "../../hardware/headers/include/DSP2833x_Device.h"

/*
 * F28335 CPU Timer 0. Counts SYSCLKOUT, no prescale. PIE INT1.7.
 * ISR increments timer_tick() once per period.
 *
 * PRD = SYSCLK_MHZ * TIMER_PERIOD_US
 * 150 MHz, 1000 us -> 150,000 clocks = 1 ms.
 */

#define TIMER_PERIOD_US             1000UL      /* 1 ms */

#define TIMER_PRD                   (SYSCLK_MHZ * TIMER_PERIOD_US)

#if (TIMER_PERIOD_US == 0UL)
#error "TIMER_PERIOD_US must be > 0."
#endif
#if (TIMER_PERIOD_US > (0xFFFFFFFFUL / SYSCLK_MHZ))
#error "TIMER_PERIOD_US too long for 32-bit PRD at this SYSCLK."
#endif

void timer_init(void);
Uint32 timer_tick(void);            /* 1 ms count */

#endif /* TIMER_INTERFACE_H */
