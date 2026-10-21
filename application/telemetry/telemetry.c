#include <stdint.h>

#include "telemetry.h"
#include "../../driver_interface/adc/adc_interface.h"
#include "../../driver_interface/drive/drive_interface.h"
#include "../../driver_interface/encoder/encoder_interface.h"
#include "../../driver_interface/gpio/gpio_interface.h"
#include "../../driver_interface/gyro/gyro_interface.h"
#include "../../driver_interface/sci/sci_interface.h"
#include "../../driver_interface/timer/timer_interface.h"
#include "../ControlLoop/control_loop.h"

#define TELEMETRY_CRC_INIT 0xFFFFU
#define TELEMETRY_CRC_POLY 0x1021U

typedef struct
{
    uint32_t tick;
    uint32_t encoder_inner;
    uint32_t encoder_outer;
    gyro_sample_t gyro;
    uint16_t gyro_valid;
    uint16_t motor_current_adc[2];
    uint16_t current_sensor_fault[2];
    uint16_t motor_fault;
    uint16_t motor_otw;
    uint16_t motor_status_valid;
    control_loop_output_t output;
} telemetry_cycle_t;

/* Stored as uint16_t because this TI compiler configuration has no uint8_t. */
static uint16_t telemetry_packet_index;
static uint32_t telemetry_last_tick;
static uint32_t telemetry_frames;
static uint32_t telemetry_drops;
static uint16_t telemetry_cycle_active;
static telemetry_cycle_t telemetry_cycle;

static uint16_t telemetry_crc16(const uint16_t *frame, uint16_t first,
                                uint16_t count)
{
    uint16_t crc;
    uint16_t i;
    uint16_t bit;

    crc = TELEMETRY_CRC_INIT;
    for (i = 0U; i < count; i++)
    {
        crc ^= (uint16_t)((frame[first + i] & 0x00FFU) << 8);
        for (bit = 0U; bit < 8U; bit++)
        {
            if ((crc & 0x8000U) != 0U)
            {
                crc = (uint16_t)((crc << 1) ^ TELEMETRY_CRC_POLY);
            }
            else
            {
                crc = (uint16_t)(crc << 1);
            }
        }
    }
    return crc;
}

static void telemetry_put_u16(uint16_t *frame, uint16_t *index,
                              uint16_t value)
{
    frame[(*index)++] = value & 0x00FFU;
    frame[(*index)++] = (value >> 8) & 0x00FFU;
}

static void telemetry_put_u32(uint16_t *frame, uint16_t *index,
                              uint32_t value)
{
    frame[(*index)++] = (uint16_t)(value & 0x000000FFUL);
    frame[(*index)++] = (uint16_t)((value >> 8) & 0x000000FFUL);
    frame[(*index)++] = (uint16_t)((value >> 16) & 0x000000FFUL);
    frame[(*index)++] = (uint16_t)((value >> 24) & 0x000000FFUL);
}

static void telemetry_put_float32(uint16_t *frame, uint16_t *index,
                                  float value)
{
    union
    {
        float value;
        uint32_t bits;
    } encoded;

    encoded.value = value;
    telemetry_put_u32(frame, index, encoded.bits);
}

static void telemetry_begin_frame(uint16_t *frame, uint16_t payload_length)
{
    frame[0] = TELEMETRY_SYNC_1;
    frame[1] = TELEMETRY_SYNC_2;
    frame[2] = telemetry_packet_index;
    frame[3] = payload_length;
}

static uint16_t telemetry_finish_frame(uint16_t *frame, uint16_t index)
{
    uint16_t crc;

    crc = telemetry_crc16(frame, 2U, (uint16_t)(index - 2U));
    telemetry_put_u16(frame, &index, crc);
    return index;
}

static void telemetry_capture_cycle(void)
{
    const gyro_sample_t *gyro;

    telemetry_cycle.tick = timer_tick();
    telemetry_cycle.encoder_inner = encoder_read_counts(ENC_1);
    telemetry_cycle.encoder_outer = encoder_read_counts(ENC_2);
    gyro = gyro_get_sample();
    telemetry_cycle.gyro_valid = 0U;
    if (gyro != 0)
    {
        telemetry_cycle.gyro = *gyro;
        telemetry_cycle.gyro_valid = 1U;
    }
    else
    {
        telemetry_cycle.gyro.rate_x = 0U;
        telemetry_cycle.gyro.rate_y = 0U;
        telemetry_cycle.gyro.rate_z = 0U;
        telemetry_cycle.gyro.acc_x = 0U;
        telemetry_cycle.gyro.acc_y = 0U;
        telemetry_cycle.gyro.acc_z = 0U;
        telemetry_cycle.gyro.temperature = 0U;
    }
    telemetry_cycle.motor_current_adc[0] = adc_read(ADC_CH_A0);
    telemetry_cycle.motor_current_adc[1] = adc_read(ADC_CH_B0);
    telemetry_cycle.current_sensor_fault[0] =
        (gpio_read(PIN_CURRENT_SENSOR_1_FAULT) == 0U) ? 1U : 0U;
    telemetry_cycle.current_sensor_fault[1] =
        (gpio_read(PIN_CURRENT_SENSOR_2_FAULT) == 0U) ? 1U : 0U;
    telemetry_cycle.motor_fault = drive_fault();
    telemetry_cycle.motor_otw = drive_otw();
    telemetry_cycle.motor_status_valid = (uint16_t)
        (TELEMETRY_VALID_MOTOR_CURRENT |
         TELEMETRY_VALID_MOTOR_FAULT |
         TELEMETRY_VALID_MOTOR_OTW |
         TELEMETRY_VALID_CURRENT_1_FAULT |
         TELEMETRY_VALID_CURRENT_2_FAULT);
    control_loop_output(&telemetry_cycle.output);
}

static uint16_t telemetry_build_encoder(uint16_t *frame)
{
    uint16_t index;
    float inner_deg;
    float outer_deg;

    telemetry_begin_frame(frame, 22U);
    index = TELEMETRY_FRAME_HEADER_LEN;
    inner_deg = (float)(((double)telemetry_cycle.encoder_inner * 360.0) /
                        2097152.0);
    outer_deg = (float)(((double)telemetry_cycle.encoder_outer * 360.0) /
                        2097152.0);
    telemetry_put_u32(frame, &index, telemetry_cycle.tick);
    telemetry_put_float32(frame, &index, inner_deg);
    telemetry_put_float32(frame, &index, outer_deg);
    telemetry_put_u32(frame, &index, telemetry_cycle.encoder_inner);
    telemetry_put_u32(frame, &index, telemetry_cycle.encoder_outer);
    telemetry_put_u16(frame, &index, TELEMETRY_VALID_ENCODER);
    return telemetry_finish_frame(frame, index);
}

static uint16_t telemetry_build_gyro(uint16_t *frame)
{
    uint16_t index;
    uint16_t valid;

    telemetry_begin_frame(frame, 14U);
    index = TELEMETRY_FRAME_HEADER_LEN;
    valid = telemetry_cycle.gyro_valid ?
        (uint16_t)(TELEMETRY_VALID_GYRO | TELEMETRY_VALID_IMU_TEMPERATURE) : 0U;
    telemetry_put_u32(frame, &index, telemetry_cycle.tick);
    telemetry_put_u16(frame, &index, telemetry_cycle.gyro.rate_x);
    telemetry_put_u16(frame, &index, telemetry_cycle.gyro.rate_y);
    telemetry_put_u16(frame, &index, telemetry_cycle.gyro.rate_z);
    telemetry_put_u16(frame, &index, telemetry_cycle.gyro.temperature);
    telemetry_put_u16(frame, &index, valid);
    return telemetry_finish_frame(frame, index);
}

static uint16_t telemetry_build_accel(uint16_t *frame)
{
    uint16_t index;
    uint16_t valid;

    telemetry_begin_frame(frame, 14U);
    index = TELEMETRY_FRAME_HEADER_LEN;
    valid = telemetry_cycle.gyro_valid ? TELEMETRY_VALID_ACCEL : 0U;
    telemetry_put_u32(frame, &index, telemetry_cycle.tick);
    telemetry_put_u16(frame, &index, telemetry_cycle.gyro.acc_x);
    telemetry_put_u16(frame, &index, telemetry_cycle.gyro.acc_y);
    telemetry_put_u16(frame, &index, telemetry_cycle.gyro.acc_z);
    telemetry_put_u16(frame, &index, 0U);
    telemetry_put_u16(frame, &index, valid);
    return telemetry_finish_frame(frame, index);
}

static uint16_t telemetry_build_motor_output(uint16_t *frame)
{
    uint16_t index;

    telemetry_begin_frame(frame, 10U);
    index = TELEMETRY_FRAME_HEADER_LEN;
    telemetry_put_u32(frame, &index, telemetry_cycle.tick);
    telemetry_put_u16(frame, &index, telemetry_cycle.output.pwm_duty[0]);
    telemetry_put_u16(frame, &index, telemetry_cycle.output.pwm_duty[1]);
    telemetry_put_u16(frame, &index, TELEMETRY_VALID_PWM);
    return telemetry_finish_frame(frame, index);
}

static uint16_t telemetry_build_motor_status(uint16_t *frame)
{
    uint16_t index;

    /* Current is reported as raw 12-bit ADC counts, not amperes. */
    telemetry_begin_frame(frame, 18U);
    index = TELEMETRY_FRAME_HEADER_LEN;
    telemetry_put_u32(frame, &index, telemetry_cycle.tick);
    telemetry_put_u16(frame, &index, telemetry_cycle.motor_current_adc[0]);
    telemetry_put_u16(frame, &index, telemetry_cycle.motor_current_adc[1]);
    telemetry_put_u16(frame, &index, telemetry_cycle.current_sensor_fault[0]);
    telemetry_put_u16(frame, &index, telemetry_cycle.current_sensor_fault[1]);
    telemetry_put_u16(frame, &index, telemetry_cycle.motor_fault);
    telemetry_put_u16(frame, &index, telemetry_cycle.motor_otw);
    telemetry_put_u16(frame, &index, telemetry_cycle.motor_status_valid);
    return telemetry_finish_frame(frame, index);
}

static uint16_t telemetry_build_packet(uint16_t *frame)
{
    switch (telemetry_packet_index)
    {
        case 0U: return telemetry_build_encoder(frame);
        case 1U: return telemetry_build_gyro(frame);
        case 2U: return telemetry_build_accel(frame);
        case 3U: return telemetry_build_motor_output(frame);
        case 4U: return telemetry_build_motor_status(frame);
        default: return 0U;
    }
}

void telemetry_init(void)
{
    telemetry_packet_index = 0U;
    telemetry_last_tick = timer_tick();
    telemetry_frames = 0UL;
    telemetry_drops = 0UL;
    telemetry_cycle_active = 0U;
}

uint16_t telemetry_update(void)
{
    uint32_t now;
    uint16_t frame[TELEMETRY_MAX_FRAME_LEN];
    uint16_t length;

    now = timer_tick();
    if (telemetry_cycle_active == 0U)
    {
        if ((now - telemetry_last_tick) < TELEMETRY_PERIOD_MS)
        {
            return 0U;
        }
        telemetry_last_tick = now;
        telemetry_capture_cycle();
        telemetry_packet_index = 0U;
        telemetry_cycle_active = 1U;
    }

    length = telemetry_build_packet(frame);
    if ((length == 0U) || (length > TELEMETRY_MAX_FRAME_LEN) ||
        (sci_tx_bytes(SCI_B, frame, length) == 0U))
    {
        telemetry_drops++;
        return 0U;
    }

    telemetry_frames++;
    telemetry_packet_index++;
    if (telemetry_packet_index >= TELEMETRY_PACKET_COUNT)
    {
        telemetry_packet_index = 0U;
        telemetry_cycle_active = 0U;
    }
    return 1U;
}

uint32_t telemetry_frame_count(void)
{
    return telemetry_frames;
}

uint32_t telemetry_drop_count(void)
{
    return telemetry_drops;
}
