#ifndef SPI_TEST_H
#define SPI_TEST_H

#include "../../driver_interface/spi/spi_interface.h"

/*
 * Every 200 ms: CS low, one incrementing 16-bit word, CS high.
 * Caller-owned CS is SPI_PIN_STE (idle high).
 * spi_obs: [0]=last TX, [1]=last RX, [2]=xfer count. Watch in CCS.
 * Tie SIMO to SOMI for a loopback check (RX == TX).
 */
extern volatile Uint16 spi_obs[3];

void spi_test(void);

#endif /* SPI_TEST_H */
