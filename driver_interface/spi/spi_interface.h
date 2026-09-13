#ifndef SPI_INTERFACE_H
#define SPI_INTERFACE_H

#include "clock_config.h"
#include "DSP2833x_Device.h"

/*
 * F28335 SPI-A, master, FIFO, 16-bit words. Caller owns chip-select.
 * Pin mux is in gpio_interface.
 *
 * Pin sets (mux 1):
 *   GPIO16 SIMO, GPIO17 SOMI, GPIO18 CLK, GPIO19 STE  — conflicts with CAN-A
 *   GPIO54 SIMO, GPIO55 SOMI, GPIO56 CLK, GPIO57 STE
 *
 * Mode: 0 CPOL=0 CPHA=0, 1 CPOL=0 CPHA=1, 2 CPOL=1 CPHA=0, 3 CPOL=1 CPHA=1
 * (TI CLK_PHASE is inverted vs CPHA.)
 *
 * LSPCLK = SYSCLKOUT / 4  (InitSysCtrl LOSPCP = 2)
 * SPICLK = LSPCLK / (SPIBRR + 1)  when SPIBRR >= 3
 */

#define SPI_PINSET_GPIO16           0U
#define SPI_PINSET_GPIO54           1U

#define SPI_PINSET                  SPI_PINSET_GPIO54
#define SPI_CHAR_BITS               16U
#define SPI_MODE                    0U
#define SPI_BAUD_HZ                 2000000UL

#if (SPI_PINSET == SPI_PINSET_GPIO16)
#define SPI_PIN_SIMO                16U
#define SPI_PIN_SOMI                17U
#define SPI_PIN_CLK                 18U
#define SPI_PIN_STE                 19U
#else
#define SPI_PIN_SIMO                54U
#define SPI_PIN_SOMI                55U
#define SPI_PIN_CLK                 56U
#define SPI_PIN_STE                 57U
#endif

#if (SPI_MODE == 0U)
#define SPI_CLKPOLARITY             0U
#define SPI_CLK_PHASE               1U
#elif (SPI_MODE == 1U)
#define SPI_CLKPOLARITY             0U
#define SPI_CLK_PHASE               0U
#elif (SPI_MODE == 2U)
#define SPI_CLKPOLARITY             1U
#define SPI_CLK_PHASE               1U
#elif (SPI_MODE == 3U)
#define SPI_CLKPOLARITY             1U
#define SPI_CLK_PHASE               0U
#else
#error "SPI_MODE must be 0 .. 3."
#endif

#define SPI_LSPCLK_HZ               ((SYSCLK_MHZ * 1000000UL) / 4UL)
#define SPI_BAUD_DIV                (SPI_LSPCLK_HZ / SPI_BAUD_HZ)
#define SPI_BRR                     (SPI_BAUD_DIV - 1UL)
#define SPI_CHAR_SHIFT              (16U - SPI_CHAR_BITS)
#define SPI_CHAR_MASK               ((1UL << SPI_CHAR_BITS) - 1UL)

#if (SPI_CHAR_BITS < 1U) || (SPI_CHAR_BITS > 16U)
#error "SPI_CHAR_BITS must be 1 .. 16."
#endif
#if (SPI_BAUD_HZ == 0UL)
#error "SPI_BAUD_HZ must be > 0."
#endif
#if (SPI_BAUD_DIV < 4UL) || (SPI_BAUD_DIV > 128UL)
#error "SPI_BAUD_HZ out of range for LSPCLK/4 .. LSPCLK/128."
#endif
#if (SPI_PINSET != SPI_PINSET_GPIO16) && (SPI_PINSET != SPI_PINSET_GPIO54)
#error "SPI_PINSET must be SPI_PINSET_GPIO16 or SPI_PINSET_GPIO54."
#endif

void spi_init(void);
Uint16 spi_write_read(Uint16 tx);   /* one 16-bit word, no CS */

#endif /* SPI_INTERFACE_H */
