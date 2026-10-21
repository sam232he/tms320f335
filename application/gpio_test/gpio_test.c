#include <stdint.h>
#include "gpio_test.h"
#include "../../driver_interface/timer/timer_interface.h"

static const uint16_t gpio_test_pin[] = { GPIO_TEST_LIST };

/*
 * COFF ABI does not zero .ebss at boot. Without explicit initializers these
 * hold random SRAM after power-up and the catch-up loop below could run for
 * minutes. Explicit init puts them in .cinit so _c_int00 sets them.
 */
static uint16_t gpio_test_count = 0U;
static uint32_t gpio_test_last_tick = 0UL;

/* Never catch up more than this many missed ticks in one call. */
#define GPIO_TEST_MAX_CATCHUP_TICKS     1000UL

volatile uint16_t otw_data;
volatile uint16_t fault_data;

void gpio_test(void)
{
    uint32_t now;
    uint32_t ticks;
    static uint16_t i=0U;

    now = timer_tick();
    ticks = now - gpio_test_last_tick;
    if (ticks == 0UL)
    {
        return;
    }
    gpio_test_last_tick = now;

    /* Bound the catch-up so a large gap (first call, long stall) cannot spin here. */
    if (ticks > GPIO_TEST_MAX_CATCHUP_TICKS)
    {
        ticks = GPIO_TEST_MAX_CATCHUP_TICKS;
    }

    /* Status inputs only. Missing / inactive FAULT and OTW are not errors. */
    otw_data = gpio_read(PIN_OTW);
    fault_data = gpio_read(PIN_FAULT);

    /* One pass per missed 1 ms tick. */
    while (ticks != 0UL)
    {
        ticks--;

            gpio_test_count++;
            if (gpio_test_count >=  (100U))
            {
                gpio_test_count = 0U;
                gpio_toggle(gpio_test_pin[i]);
                i= (i>4) ? (0):(i+1);
            }
        }

}
