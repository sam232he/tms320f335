#ifndef SCI_INTERFACE_H
#define SCI_INTERFACE_H

#include "clock_config.h"
#include "board_pins.h"
#include "DSP2833x_Device.h"

/*
 * F28335 SCI-A/B/C, 8N1, FIFO. Pin mux is in gpio_interface.
 * Each port has a static TX/RX ring. RX ISR pushes RX. sci_write() pops TX.
 *
 * Schematic (drive.md):
 *   SCI_A  GPIO35/36  SCITXDA/SCIRXDA  mux 1  DSP_UART_1
 *   SCI_B  GPIO14/15  SCITXDB/SCIRXDB  mux 2  DSP_UART_2
 *   SCI_C  GPIO63/62  SCITXDC/SCIRXDC  mux 1  DSP_UART_3
 *
 * LSPCLK = SYSCLKOUT / 4  (InitSysCtrl LOSPCP = 2)
 * baud = LSPCLK / ((BRR + 1) * 8)
 */

typedef enum
{
    SCI_A = 0,
    SCI_B = 1,
    SCI_C = 2
} sci_module_t;

#define SCI_COUNT                   3U

#define SCI_A_PIN_TX                PIN_DSP_TX1
#define SCI_A_PIN_RX                PIN_DSP_RX1
#define SCI_A_PIN_MUX               1U

#define SCI_B_PIN_TX                PIN_DSP_TX2
#define SCI_B_PIN_RX                PIN_DSP_RX2
#define SCI_B_PIN_MUX               2U

#define SCI_C_PIN_TX                PIN_DSP_TX3
#define SCI_C_PIN_RX                PIN_DSP_RX3
#define SCI_C_PIN_MUX               1U

#define SCI_BAUD_HZ                 115200UL
#define SCI_LSPCLK_HZ               ((SYSCLK_MHZ * 1000000UL) / 4UL)
#define SCI_BAUD_DIV                ((SCI_LSPCLK_HZ + (SCI_BAUD_HZ * 4UL)) / (SCI_BAUD_HZ * 8UL))
#define SCI_BRR                     (SCI_BAUD_DIV - 1UL)
#define SCI_BRR_HIGH                ((SCI_BRR >> 8) & 0xFFU)
#define SCI_BRR_LOW                 (SCI_BRR & 0xFFU)

#if (SCI_BAUD_HZ == 0UL)
#error "SCI_BAUD_HZ must be > 0."
#endif
#if (SCI_BAUD_DIV < 1UL)
#error "SCI_BAUD_HZ too high for LSPCLK."
#endif
#if (SCI_BAUD_DIV > 65536UL)
#error "SCI_BAUD_HZ too low for 16-bit BRR."
#endif

typedef enum
{
    SCI_TX = 0,
    SCI_RX = 1
} sci_dir_t;

typedef struct
{
    Uint16 data;
} sci_msg_t;

#define SCI_QUEUE_LEN               40U

void sci_init(void);
Uint16 sci_tx(sci_module_t mod, const sci_msg_t *msg);
Uint16 sci_rx(sci_module_t mod, sci_msg_t *msg);

/*
 * Idle-loop TX: one step per call.
 * CHECK_QUEUE pops into sci_write_held. The next call is FIND_FIFO
 * and retries until the TX FIFO has room, then returns to CHECK_QUEUE.
 */
void sci_write(sci_module_t mod);

#endif /* SCI_INTERFACE_H */
