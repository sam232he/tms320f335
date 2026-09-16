#ifndef ENCODER_INTERFACE_H
#define ENCODER_INTERFACE_H

#include "DSP2833x_Device.h"

/*
 * SSI master on ENC1/ENC2 (MAX490 CLK out, DATA in).
 * A-SZ35T6Y-21-1-SBG: 21-bit binary, MSB first.
 * Pin mux is in gpio_interface.
 *
 * Instructions: 100 kHz < fclk < 300 kHz. Half-period 2 us -> 250 kHz.
 */

#define ENC_1                       0U
#define ENC_2                       1U
#define ENC_BIT_COUNT               21U
#define ENC_CLK_HALF_US             2U      /* low or high; period 4 us */

void encoder_init(void);
Uint32 encoder_read_counts(Uint16 enc);     /* 21-bit word, 0 .. 2097151 */

#endif /* ENCODER_INTERFACE_H */
