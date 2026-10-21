#include <stdint.h>
#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H

#include "DSP2833x_Device.h"
#include "board_pins.h"

/*
 * Configure all GPIO pins used by the BSP.
 * Pins not listed in the driver table stay GPIO inputs with pull-ups.
 *
 * Board-test outputs are the six schematic LEDs (GPIO_LED_LIST).
 */

#define GPIO_TEST_LIST              GPIO_LED_LIST
#define GPIO_TEST_COUNT             GPIO_LED_COUNT

void gpio_init(void);
void gpio_write(uint16_t pin, uint16_t level);
void gpio_toggle(uint16_t pin);
uint16_t gpio_read(uint16_t pin);

#endif /* GPIO_INTERFACE_H */
