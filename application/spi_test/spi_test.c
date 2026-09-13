#include "spi_test.h"
#include "../../driver_interface/gpio/gpio_interface.h"
#include "../../driver_interface/timer/timer_interface.h"

#define SPI_OBS_TX                  0U
#define SPI_OBS_RX                  1U
#define SPI_OBS_N                   2U

static Uint32 spi_test_last_tick;
static Uint16 spi_test_tx;

volatile Uint16 spi_obs[3];

void spi_test(void)
{
    Uint32 now;
    Uint16 rx;

    now = timer_tick();
    if ((now - spi_test_last_tick) < 200UL) /* 200 ms */
    {
        return;
    }
    spi_test_last_tick = now;

    gpio_write(SPI_PIN_STE, 0U);
    rx = spi_write_read(spi_test_tx);
    gpio_write(SPI_PIN_STE, 1U);

    spi_obs[SPI_OBS_TX] = spi_test_tx;
    spi_obs[SPI_OBS_RX] = rx;
    spi_obs[SPI_OBS_N]++;
    spi_test_tx++;
}
