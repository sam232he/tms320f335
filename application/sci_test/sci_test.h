#ifndef SCI_TEST_H
#define SCI_TEST_H

#include "../../driver_interface/sci/sci_interface.h"

/*
 * Every 200 ms queue one incrementing byte on SCI and SCI_CMD TX.
 * Pop RX queues into obs and echo via TX queue.
 * Hardware send is sci_write() in the idle loop (same as can_write).
 * sci_obs / sci_cmd_obs: [0]=last TX, [1]=last RX, [2]=TX count, [3]=RX count.
 * Watch in CCS.
 */
extern volatile Uint16 sci_obs[4];
extern volatile Uint16 sci_cmd_obs[4];

void sci_test(void);

#endif /* SCI_TEST_H */
