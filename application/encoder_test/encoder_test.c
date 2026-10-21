#include <stdint.h>
#include "encoder_test.h"
#include "../../driver_interface/timer/timer_interface.h"

static uint32_t encoder_test_last_tick = 0UL;   /* COFF: .ebss is not zeroed */

volatile uint32_t enc1_counts;
volatile uint32_t enc2_counts;

void encoder_test(void)
{
    uint32_t now;

    now = timer_tick();
    if ((now - encoder_test_last_tick) < 10UL) /* 10 ms */
    {
        return;
    }
    encoder_test_last_tick = now;

    enc1_counts = encoder_read_counts(ENC_1);
    enc2_counts = encoder_read_counts(ENC_2);
}
