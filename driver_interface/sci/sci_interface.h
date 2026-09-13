#ifndef SCI_INTERFACE_H
#define SCI_INTERFACE_H

#include "clock_config.h"
#include "DSP2833x_Device.h"

/*
 * F28335 SCI-B, 8N1, FIFO. Pin mux is in gpio_interface.
 * Each port has a static TX/RX ring. RX ISR pushes RX. sci_write() pops TX.
 *
 * Pin sets:
 *   GPIO14 SCITXDB, GPIO15 SCIRXDB  mux 2  (default)
 *   GPIO22 SCITXDB, GPIO23 SCIRXDB  mux 3
 *   GPIO28 SCIRXDA, GPIO29 SCITXDA  mux 1  — SCI-A, controlCARD UART
 *
 * LSPCLK = SYSCLKOUT / 4  (InitSysCtrl LOSPCP = 2)
 * baud = LSPCLK / ((BRR + 1) * 8)
 */

#define SCI_PINSET_GPIO14           0U
#define SCI_PINSET_GPIO22           1U
#define SCI_PINSET_GPIO28           2U

#define SCI_PINSET                  SCI_PINSET_GPIO14
#define SCI_BAUD_HZ                 115200UL

#if (SCI_PINSET == SCI_PINSET_GPIO14)
#define SCI_PIN_TX                  14U
#define SCI_PIN_RX                  15U
#define SCI_PIN_MUX                 2U
#define SCI_USE_SCIB                1
#elif (SCI_PINSET == SCI_PINSET_GPIO22)
#define SCI_PIN_TX                  22U
#define SCI_PIN_RX                  23U
#define SCI_PIN_MUX                 3U
#define SCI_USE_SCIB                1
#elif (SCI_PINSET == SCI_PINSET_GPIO28)
#define SCI_PIN_TX                  29U
#define SCI_PIN_RX                  28U
#define SCI_PIN_MUX                 1U
#define SCI_USE_SCIB                0
#else
#error "SCI_PINSET must be SCI_PINSET_GPIO14, _GPIO22, or _GPIO28."
#endif

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

/*
 * Second UART (SCI-A by default). Separate from SCI-B above.
 * Pin sets:
 *   GPIO29 SCITXDA, GPIO28 SCIRXDA  mux 1  — SCI-A, controlCARD UART (default)
 *   GPIO62 SCITXDC, GPIO63 SCIRXDC  mux 1  — SCI-C
 */
#define SCI_CMD_PINSET_GPIO28       0U
#define SCI_CMD_PINSET_GPIO62       1U

#define SCI_CMD_PINSET              SCI_CMD_PINSET_GPIO28
#define SCI_CMD_BAUD_HZ             115200UL

#if (SCI_CMD_PINSET == SCI_CMD_PINSET_GPIO28)
#define SCI_CMD_PIN_TX              29U
#define SCI_CMD_PIN_RX              28U
#define SCI_CMD_PIN_MUX             1U
#define SCI_CMD_USE_SCIC            0
#elif (SCI_CMD_PINSET == SCI_CMD_PINSET_GPIO62)
#define SCI_CMD_PIN_TX              62U
#define SCI_CMD_PIN_RX              63U
#define SCI_CMD_PIN_MUX             1U
#define SCI_CMD_USE_SCIC            1
#else
#error "SCI_CMD_PINSET must be SCI_CMD_PINSET_GPIO28 or _GPIO62."
#endif

#if (SCI_PINSET == SCI_PINSET_GPIO28) && (SCI_CMD_PINSET == SCI_CMD_PINSET_GPIO28)
#error "SCI and SCI_CMD both selected GPIO28/29."
#endif

#define SCI_CMD_BAUD_DIV            ((SCI_LSPCLK_HZ + (SCI_CMD_BAUD_HZ * 4UL)) / (SCI_CMD_BAUD_HZ * 8UL))
#define SCI_CMD_BRR                 (SCI_CMD_BAUD_DIV - 1UL)
#define SCI_CMD_BRR_HIGH            ((SCI_CMD_BRR >> 8) & 0xFFU)
#define SCI_CMD_BRR_LOW             (SCI_CMD_BRR & 0xFFU)

#if (SCI_CMD_BAUD_HZ == 0UL)
#error "SCI_CMD_BAUD_HZ must be > 0."
#endif
#if (SCI_CMD_BAUD_DIV < 1UL)
#error "SCI_CMD_BAUD_HZ too high for LSPCLK."
#endif
#if (SCI_CMD_BAUD_DIV > 65536UL)
#error "SCI_CMD_BAUD_HZ too low for 16-bit BRR."
#endif

typedef enum
{
    SCI = 0,
    SCI_CMD = 1
} sci_module_t;

typedef enum
{
    SCI_TX = 0,
    SCI_RX = 1
} sci_dir_t;

typedef struct
{
    Uint16 data;
} sci_msg_t;

#define SCI_QUEUE_LEN               20U

void sci_init(void);
void sci_cmd_init(void);
Uint16 sci_tx(sci_module_t mod, const sci_msg_t *msg);
Uint16 sci_rx(sci_module_t mod, sci_msg_t *msg);

/*
 * Idle-loop TX: one step per call.
 * CHECK_QUEUE pops into sci_write_held. The next call is FIND_FIFO
 * and retries until the TX FIFO has room, then returns to CHECK_QUEUE.
 */
void sci_write(sci_module_t mod);

#endif /* SCI_INTERFACE_H */
