#include <stdint.h>
#ifndef ENCODER_TEST_H
#define ENCODER_TEST_H

#include "../../driver_interface/encoder/encoder_interface.h"

/*
 * Copy SSI counts from encoder_read_counts() into CCS watch vars.
 * Call from the idle loop; updates every 10 ms (1 ms timer_tick).
 */
extern volatile uint32_t enc1_counts;
extern volatile uint32_t enc2_counts;

void encoder_test(void);

#endif /* ENCODER_TEST_H */
