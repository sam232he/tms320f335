#include "gpio_test.h"
#include "../../driver_interface/timer/timer_interface.h"

static const Uint16 gpio_test_pin[] = { GPIO_TEST_LIST };

static Uint16 gpio_test_count;
static Uint32 gpio_test_last_tick;

volatile Uint16 otw_data;
volatile Uint16 fault_data;

void gpio_test(void)
{
    Uint32 now;
    Uint32 ticks;
    static Uint16 i=0U;

    now = timer_tick();
    ticks = now - gpio_test_last_tick;
    if (ticks == 0UL)
    {
        return;
    }
    gpio_test_last_tick = now;

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
