#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

#include "../init/clock_config.h"
#include "../../hardware/headers/include/DSP2833x_Device.h"

/*
 * F28335 sequencer ADC. Cascaded SEQ (max 16 slots: A0-A7 and B0-B7).
 * Trigger: EPWM5 SOCA (pwm_interface.h). ISR copies all slots and resets SEQ1.
 *
 * adc_ch_t values are the CONV select: 0-7 = A0-A7, 8-15 = B0-B7.
 * Add a channel by appending to ADC_SLOT_LIST (conversion order).
 * adc_read() takes the pin (ADC_CH_A0), not a slot index.
 *
 * Schematic ADC (drive.md). TMS320F28335 range is 0 V to 3 V:
 *   adc_v = adc_code * 3.0 / 4096.0
 * No populated resistor divider is shown on these nets, so firmware
 * reports raw counts and ADC pin voltage only.
 *
 *   DSP_ADCINA0 / pin 42 / ADCINA0 = IOUT_1  (current sensor 1 net)
 *       direct ADC pin voltage only; no populated divider
 *   DSP_ADCINA1 / pin 41 / ADCINA1 = PS_OUT  (pressure sensor net)
 *       direct ADC pin voltage only; no populated divider
 *   DSP_ADCINB0 / pin 46 / ADCINB0 = IOUT_2  (current sensor 2 net)
 *       direct ADC pin voltage only; no populated divider
 *
 * Sensor engineering-unit conversion (ACS711 current, MPXHZ6400 pressure)
 * is intentionally disabled until external sensors are connected and
 * scaling is verified. Do not apply datasheet transfer functions here.
 *
 * Open / missing sensor nets are not fatal for board-only testing.
 *
 * Clocks (150 MHz SYSCLKOUT):
 *   ADC_HISPCP  -> HSPCLK = SYSCLKOUT / (2 * ADC_HISPCP) = 25 MHz  (chip max)
 *   ADC_ADCCLKPS -> ADCCLK = HSPCLK / (2 * ADC_ADCCLKPS) when != 0
 *                  1 -> 12.5 MHz (80 ns/tick). TI SOC example rate. Do not
 *                  raise ADCCLK above 25 MHz.
 *   ADC_ACQ_PS  -> sample window = (ADC_ACQ_PS + 1) ADCCLK ticks
 *                  15 -> 16 ticks = 1.28 us, then ~13 ticks convert (~1.04 us)
 *                  ~2.3 us per pin. Leave at 15 for bring-up.
 *                  Raise if the pin is high-Z (noisy / low readings).
 *                  Lower only if the source is low-Z and the sequence must
 *                  fit more channels into one PWM period.
 */

typedef enum
{
    ADC_CH_A0 = 0,
    ADC_CH_A1 = 1,
    ADC_CH_A2 = 2,
    ADC_CH_A3 = 3,
    ADC_CH_A4 = 4,
    ADC_CH_A5 = 5,
    ADC_CH_A6 = 6,
    ADC_CH_A7 = 7,
    ADC_CH_B0 = 8,
    ADC_CH_B1 = 9,
    ADC_CH_B2 = 10,
    ADC_CH_B3 = 11,
    ADC_CH_B4 = 12,
    ADC_CH_B5 = 13,
    ADC_CH_B6 = 14,
    ADC_CH_B7 = 15
} adc_ch_t;

#define ADC_HISPCP                  ((SYSCLK_MHZ + 49U) / 50U)  /* 150 MHz -> 3 -> 25 MHz HSPCLK */
#define ADC_ADCCLKPS                1U      /* HSPCLK/2 = 12.5 MHz ADCCLK; leave at 1 */
#define ADC_ACQ_PS                  15U     /* 16 ADCCLK sample window (~1.28 us); leave at 15 */

#define ADC_SLOT_LIST               ADC_CH_A0, ADC_CH_A1, ADC_CH_B0

void adc_init(void);
Uint16 adc_read(adc_ch_t ch);           /* last 12-bit result, 0 .. 4095 */
float adc_pin_voltage(Uint16 adc_code); /* adc_v = adc_code * 3.0 / 4096.0 */

#endif /* ADC_INTERFACE_H */
