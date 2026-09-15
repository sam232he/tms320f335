#include "encoder_test.h"
#include "../../driver_interface/timer/timer_interface.h"

static Uint32 encoder_test_last_tick;

volatile Uint32 enc1_counts;
volatile Uint32 enc2_counts;

void encoder_test(void)
{
    Uint32 now;

    now = timer_tick();
    if ((now - encoder_test_last_tick) < 10UL) /* 10 ms */
    {
        return;
    }
    encoder_test_last_tick = now;

    enc1_counts = encoder_read_counts(ENC_1);
    enc2_counts = encoder_read_counts(ENC_2);
}
