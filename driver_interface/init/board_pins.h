#ifndef BOARD_PINS_H
#define BOARD_PINS_H

/*
 * TMS320F28335 pin map from drive.md / drive.pdf (DSP.SchDoc).
 *
 * Tests use the schematic peripherals:
 *   PWM5/PWM6, eCAN-B, SCI-A/B/C, I2C-A EEPROM, ADCINA0/A1/B0, LED1..6,
 *   ENC1/ENC2 SSI (CLK out idle-high, DATA in).
 * RST_AB stays asserted so DRV8432 outputs stay Hi-Z while PWM GPIO
 * is exercised.
 */

#define BOARD_TEST_MODE                 1U
#define PWM_OUTPUT_ENABLE               1U
#define BOARD_TEST_TOGGLE_DRV_RESET     0U

#if (BOARD_TEST_TOGGLE_DRV_RESET != 0U)
#error "DRV8432 RST_AB must stay asserted for board-only testing."
#endif

/* Motor 1: schematic net DSP_EPWM1_* on ePWM6, not ePWM1. */
#define PIN_DSP_EPWM1_A                 10U     /* pin 19, EPWM6A */
#define PIN_DSP_EPWM1_B                 11U     /* pin 20, EPWM6B */

/* Motor 2: schematic net DSP_EPWM2_* on ePWM5, not ePWM2. */
#define PIN_DSP_EPWM2_A                  9U     /* pin 18, EPWM5B */
#define PIN_DSP_EPWM2_B                  8U     /* pin 17, EPWM5A */

#define PIN_OTW                         12U     /* pin 21, input from DRV8432 OTW */
#define PIN_FAULT                       13U     /* pin 24, input from DRV8432 FAULT */
#define PIN_RST_AB                      26U     /* pin 72, output to DRV8432 RESET_AB */
/* RST_CD is not connected on the DSP sheet. Do not allocate a GPIO. */

#define PIN_DSP_TX1                     35U     /* pin 148, SCITXDA */
#define PIN_DSP_RX1                     36U     /* pin 145, SCIRXDA */
#define PIN_DSP_TX2                     14U     /* pin 25, SCITXDB */
#define PIN_DSP_RX2                     15U     /* pin 26, SCIRXDB */
#define PIN_DSP_TX3                     63U     /* pin 114, SCITXDC */
#define PIN_DSP_RX3                     62U     /* pin 113, SCIRXDC */

#define PIN_DSP_CAN_TX                  16U     /* pin 27, CANTXB */
#define PIN_DSP_CAN_RX                  17U     /* pin 28, CANRXB */

#define PIN_ENC1_DATA                   73U     /* pin 130 */
#define PIN_ENC1_CLK                    72U     /* pin 129 */
#define PIN_ENC2_DATA                   67U     /* pin 122 */
#define PIN_ENC2_CLK                    66U     /* pin 119 */

#define PIN_LED1                        74U     /* pin 131 */
#define PIN_LED2                        75U     /* pin 132 */
#define PIN_LED3                        76U     /* pin 133 */
#define PIN_LED4                        77U     /* pin 134 */
#define PIN_LED5                        78U     /* pin 135 */
#define PIN_LED6                        79U     /* pin 136 */

#define PIN_EEPROM_SDA                  32U     /* pin 74 */
#define PIN_EEPROM_SCL                  33U     /* pin 75 */
#define PIN_EEPROM_A1                   22U     /* pin 66 */
#define PIN_EEPROM_WP                   23U     /* pin 67 */

#define PIN_DSP_XA12                    84U
#define PIN_DSP_XA13                    85U
#define PIN_DSP_XA14                    86U
#define PIN_DSP_XA15                    87U

#define GPIO_LED_LIST \
    PIN_LED1, PIN_LED2, PIN_LED3, PIN_LED4, PIN_LED5, PIN_LED6
#define GPIO_LED_COUNT                  6U

#endif /* BOARD_PINS_H */
