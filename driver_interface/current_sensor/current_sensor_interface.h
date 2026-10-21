#ifndef CURRENT_SENSOR_INTERFACE_H
#define CURRENT_SENSOR_INTERFACE_H

/*
 * ACS712 current sensors connected to the board ADC inputs.
 *
 * The driver exposes only the scaled current value. Adjust the two ACS712
 * constants below to match the installed sensor variant and any analog
 * conditioning on the board.
 *
 * Default: ACS712-05B sensitivity (185 mV/A), with an ADC-compatible 0..3 V
 * signal whose zero-current output is centered at 1.5 V. The board ADC driver
 * reports a 0..3 V, 12-bit input range. If the installed part is ACS712-20A
 * or ACS712-30A, change ACS712_SENSITIVITY_V_PER_A accordingly.
 */
#define ACS712_ADC_VREF_V              3.0F
#define ACS712_ADC_COUNTS              4096.0F
#define ACS712_ZERO_CURRENT_V          1.5F
#define ACS712_SENSITIVITY_V_PER_A     0.185F

typedef enum
{
    CURRENT_SENSOR_1 = 0U, /* ADCINA0 / A0, fault GPIO29 */
    CURRENT_SENSOR_2 = 1U  /* ADCINB0 / B0, fault GPIO30 */
} current_sensor_t;

/* Read the selected ACS712 and return the scaled current in amperes. */
float current_sensor_read_amps(current_sensor_t sensor);

#endif /* CURRENT_SENSOR_INTERFACE_H */
