#include "spi_interface.h"
#include "DSP28x_Project.h"

Uint16 spi_write_read(Uint16 tx)
{
    SpiaRegs.SPITXBUF = (Uint16)((tx & SPI_CHAR_MASK) << SPI_CHAR_SHIFT);
    while (SpiaRegs.SPIFFRX.bit.RXFFST == 0U)
    {
    }
    return (Uint16)(SpiaRegs.SPIRXBUF & SPI_CHAR_MASK);
}

void spi_init(void)
{
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.bit.SPICHAR = (Uint16)(SPI_CHAR_BITS - 1U);
    SpiaRegs.SPICCR.bit.SPILBK = 0;
    SpiaRegs.SPICCR.bit.CLKPOLARITY = SPI_CLKPOLARITY;

    SpiaRegs.SPICTL.bit.SPIINTENA = 0;
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = SPI_CLK_PHASE;
    SpiaRegs.SPICTL.bit.OVERRUNINTENA = 0;

    SpiaRegs.SPIBRR = SPI_BRR;
    SpiaRegs.SPIPRI.bit.FREE = 1;
    SpiaRegs.SPIPRI.bit.SOFT = 0;

    SpiaRegs.SPIFFTX.all = 0xE040;
    SpiaRegs.SPIFFRX.all = 0x204F;
    SpiaRegs.SPIFFCT.all = 0x0000;

    SpiaRegs.SPICCR.bit.SPISWRESET = 1;
    SpiaRegs.SPIFFTX.bit.TXFIFO = 1;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;
}
