#include "gpio_test.h"
#include "../../driver_interface/timer/timer_interface.h"

static const Uint16 gpio_test_pin[] = { GPIO_TEST_LIST };

static Uint16 gpio_test_count[GPIO_TEST_COUNT];
static Uint32 gpio_test_last_tick;

volatile Uint16 gpio_test_normal_data[GPIO_TEST_COUNT];
volatile Uint16 otw_data;
volatile Uint16 fault_data;

void gpio_test(void)
{
    Uint32 now;
    Uint32 ticks;
    Uint16 i;

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
        for (i = 0U; i < GPIO_TEST_COUNT; i++)
        {
            gpio_test_count[i]++;
            if (gpio_test_count[i] >= (Uint16)((i + 1U) * 250U))
            {
                gpio_test_count[i] = 0U;
                gpio_toggle(gpio_test_pin[i]);
                gpio_test_normal_data[i] = gpio_read(gpio_test_pin[i]);
            }
        }
    }
}
