#ifndef GPIO_TEST_H
#define GPIO_TEST_H

#include "../../driver_interface/gpio/gpio_interface.h"

/*
 * Blink schematic LED1..LED6. Call from the idle loop.
 * gpio_test_normal_data[i] is the last LED level.
 * otw_data / fault_data are inputs and are never treated as startup errors.
 */
extern volatile Uint16 gpio_test_normal_data[GPIO_TEST_COUNT];
extern volatile Uint16 otw_data;
extern volatile Uint16 fault_data;

void gpio_test(void);

#endif /* GPIO_TEST_H */
