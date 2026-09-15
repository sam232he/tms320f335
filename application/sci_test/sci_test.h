#ifndef SCI_TEST_H
#define SCI_TEST_H

#include "../../driver_interface/sci/sci_interface.h"

/*
 * Every 200 ms queue one incrementing byte on SCI_A, SCI_B, and SCI_C TX.
 * Pop RX queues into sci_*_test_data and echo via TX queue.
 * sci_write() for A/B/C runs every call.
 * sci_a_test_data / sci_b_test_data / sci_c_test_data:
 *   [0]=last TX, [1]=last RX, [2]=TX count, [3]=RX count.
 */
extern volatile Uint16 sci_a_test_data[4];
extern volatile Uint16 sci_b_test_data[4];
extern volatile Uint16 sci_c_test_data[4];

void sci_test(void);

#endif /* SCI_TEST_H */
