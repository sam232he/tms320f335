#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H

#include "DSP2833x_Device.h"

/*
 * Configure all GPIO pins used by the BSP.
 * Pins not listed in the driver table stay GPIO inputs with pull-ups.
 *
 * GPIO_TEST_LIST are dedicated outputs for gpio_test (1..5 s toggle).
 * Change the five pins here; the table and the test both use this list.
 */
#define GPIO_TEST_0                 32U
#define GPIO_TEST_1                 33U
#define GPIO_TEST_2                 48U
#define GPIO_TEST_3                 49U
#define GPIO_TEST_4                 50U
#define GPIO_TEST_LIST              GPIO_TEST_0, GPIO_TEST_1, GPIO_TEST_2, GPIO_TEST_3, GPIO_TEST_4

void gpio_init(void);
void gpio_write(Uint16 pin, Uint16 level);
void gpio_toggle(Uint16 pin);
Uint16 gpio_read(Uint16 pin);

#endif /* GPIO_INTERFACE_H */
