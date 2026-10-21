#include <stdint.h>

#include "current_sensor_interface.h"
#include "../adc/adc_interface.h"

static adc_ch_t current_sensor_adc_channel(current_sensor_t sensor)
{
    if (sensor == CURRENT_SENSOR_2)
    {
        return ADC_CH_B0;
    }
    return ADC_CH_A0;
}

static uint16_t current_sensor_read_adc(current_sensor_t sensor)
{
    return adc_read(current_sensor_adc_channel(sensor));
}

static float current_sensor_read_voltage(current_sensor_t sensor)
{
    return adc_pin_voltage(current_sensor_read_adc(sensor));
}

float current_sensor_read_amps(current_sensor_t sensor)
{
    float voltage;

    voltage = current_sensor_read_voltage(sensor);
    return (voltage - ACS712_ZERO_CURRENT_V) /
           ACS712_SENSITIVITY_V_PER_A;
}
