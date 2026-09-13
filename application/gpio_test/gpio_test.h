#ifndef GPIO_TEST_H
#define GPIO_TEST_H

#include "../../driver_interface/gpio/gpio_interface.h"

/*
 * Toggle 5 outputs every 1, 2, 3, 4, 5 s. Call from the idle loop.
 * gpio_obs[i] is the last level. Watch in CCS.
 */
extern volatile Uint16 gpio_obs[5];

void gpio_test(void);

#endif /* GPIO_TEST_H */
