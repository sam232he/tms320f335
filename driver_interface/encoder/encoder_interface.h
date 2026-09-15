#ifndef ENCODER_INTERFACE_H
#define ENCODER_INTERFACE_H

#include "DSP2833x_Device.h"

/*
 * SSI master on ENC1/ENC2 (MAX490 CLK out, DATA in).
 * A-SZ35T6Y-21-1-SCBG: 21-bit binary, MSB first.
 * Pin mux is in gpio_interface.
 */

#define ENC_1                       0U
#define ENC_2                       1U
#define ENC_BIT_COUNT               21U

void encoder_init(void);
Uint32 encoder_read_counts(Uint16 enc);     /* 21-bit word, 0 .. 2097151 */

#endif /* ENCODER_INTERFACE_H */
