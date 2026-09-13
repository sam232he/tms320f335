#include "gpio_test.h"
#include "../../driver_interface/timer/timer_interface.h"

static const Uint16 gpio_test_pin[] = { GPIO_TEST_LIST };

/* 1 ms ticks. Toggle after 1 s, 2 s, 3 s, 4 s, 5 s. */
static const Uint16 gpio_test_period[] = { 1000U, 2000U, 3000U, 4000U, 5000U };

static Uint16 gpio_test_count[5];
static Uint32 gpio_test_last_tick;

volatile Uint16 gpio_obs[5];

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

    /* One pass per missed 1 ms tick. */
    while (ticks != 0UL)
    {
        ticks--;
        for (i = 0U; i < 5U; i++)
        {
            gpio_test_count[i]++;
            if (gpio_test_count[i] >= gpio_test_period[i])
            {
                gpio_test_count[i] = 0U;
                gpio_toggle(gpio_test_pin[i]);
                gpio_obs[i] = gpio_read(gpio_test_pin[i]);
            }
        }
    }
}
