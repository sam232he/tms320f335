#include "gyro_interface.h"

static gyro_sample_t gyro_latest_sample = {
    0U, 0U, 0U, 0U, 0U, 0U, 0U
};

const gyro_sample_t *gyro_get_sample(void)
{
    return &gyro_latest_sample;
}

gyro_asic_t gyro_register_asic(gyro_register_t reg)
{
    switch (reg)
    {
    case GYRO_REG_RATE_X:
    case GYRO_REG_ACC_X:
    case GYRO_REG_ACC_Y:
    case GYRO_REG_ACC_Z:
        return GYRO_ASIC_UNO;
    case GYRO_REG_RATE_Y:
    case GYRO_REG_RATE_Z:
        return GYRO_ASIC_DUE;
    case GYRO_REG_TEMP:
        return GYRO_ASIC_UNO;
    default:
        return GYRO_ASIC_UNO;
    }
}

Uint16 gyro_register_address(gyro_register_t reg)
{
    switch (reg)
    {
    case GYRO_REG_RATE_X: return 0x01U;
    case GYRO_REG_RATE_Y: return 0x03U;
    case GYRO_REG_RATE_Z: return 0x01U;
    case GYRO_REG_ACC_X:  return 0x04U;
    case GYRO_REG_ACC_Y:  return 0x05U;
    case GYRO_REG_ACC_Z:  return 0x06U;
    case GYRO_REG_TEMP:   return 0x07U;
    default:              return 0x00U;
    }
}

static Uint16 gyro_byte(Uint16 value)
{
    return (value <= 0xFFU) ? 1U : 0U;
}

static Uint16 gyro_u16_le(const Uint16 *frame)
{
    return (Uint16)(frame[0] | (Uint16)(frame[1] << 8));
}

Uint16 gyro_build_data_request(Uint16 periodic, Uint16 *frame)
{
    if (frame == 0)
    {
        return 0U;
    }
    frame[0] = GYRO_FRAME_START;
    frame[1] = (periodic != 0U) ? (Uint16)'P' : (Uint16)'S';
    frame[2] = (Uint16)'D';
    frame[3] = GYRO_FRAME_MARKER_1;
    frame[4] = GYRO_FRAME_MARKER_2;
    return GYRO_COMMAND_FRAME_LEN;
}

Uint16 gyro_build_register_read_request(Uint16 address, Uint16 *frame)
{
    if ((frame == 0) || (address > 0xFFU))
    {
        return 0U;
    }
    frame[0] = GYRO_FRAME_START;
    frame[1] = address;
    frame[2] = (Uint16)'C';
    frame[3] = GYRO_FRAME_MARKER_1;
    frame[4] = GYRO_FRAME_MARKER_2;
    return GYRO_COMMAND_FRAME_LEN;
}

Uint16 gyro_build_selftest_request(Uint16 *frame)
{
    if (frame == 0)
    {
        return 0U;
    }
    frame[0] = GYRO_FRAME_START;
    frame[1] = (Uint16)'S';
    frame[2] = (Uint16)'C';
    frame[3] = GYRO_FRAME_MARKER_1;
    frame[4] = GYRO_FRAME_MARKER_2;
    return GYRO_COMMAND_FRAME_LEN;
}

Uint16 gyro_start_periodic(gyro_uart_send_fn send)
{
    Uint16 frame[GYRO_COMMAND_FRAME_LEN];

    if (send == 0)
    {
        return 0U;
    }
    (void)gyro_build_data_request(1U, frame);
    return send(frame, GYRO_COMMAND_FRAME_LEN);
}

Uint16 gyro_parse_data_frame(const Uint16 *frame, gyro_sample_t *sample)
{
    if ((frame == 0) || (sample == 0) ||
        !gyro_byte(frame[0]) || !gyro_byte(frame[1]) ||
        !gyro_byte(frame[2]) || !gyro_byte(frame[3]) ||
        !gyro_byte(frame[4]) || !gyro_byte(frame[5]) ||
        !gyro_byte(frame[6]) || (frame[6] != GYRO_DATA_FRAME_ID) ||
        (frame[7] != GYRO_FRAME_MARKER_1) ||
        (frame[8] != GYRO_FRAME_MARKER_2) ||
        !gyro_byte(frame[9]) || !gyro_byte(frame[10]) ||
        !gyro_byte(frame[11]) || !gyro_byte(frame[12]) ||
        !gyro_byte(frame[13]) || !gyro_byte(frame[14]) ||
        !gyro_byte(frame[15]) || !gyro_byte(frame[16]) ||
        !gyro_byte(frame[17]))
    {
        return 0U;
    }

    sample->rate_z = gyro_u16_le(&frame[0]);
    sample->rate_y = gyro_u16_le(&frame[2]);
    sample->rate_x = gyro_u16_le(&frame[4]);
    sample->temperature = gyro_u16_le(&frame[10]);
    sample->acc_z = gyro_u16_le(&frame[12]);
    sample->acc_y = gyro_u16_le(&frame[14]);
    sample->acc_x = gyro_u16_le(&frame[16]);
    gyro_latest_sample = *sample;
    return 1U;
}

Uint16 gyro_parse_register_response(const Uint16 *frame,
                                    Uint16 *address,
                                    Uint16 *value)
{
    if ((frame == 0) || (address == 0) || (value == 0) ||
        !gyro_byte(frame[0]) || !gyro_byte(frame[1]) ||
        !gyro_byte(frame[2]) || !gyro_byte(frame[3]) ||
        !gyro_byte(frame[4]) || !gyro_byte(frame[5]) ||
        (frame[5] != (Uint16)'R') ||
        (frame[6] != GYRO_FRAME_MARKER_1) ||
        (frame[7] != GYRO_FRAME_MARKER_2))
    {
        return 0U;
    }
    *value = gyro_u16_le(&frame[2]);
    *address = frame[4];
    return 1U;
}

Uint16 gyro_parse_selftest_response(const Uint16 *frame, Uint16 *result)
{
    if ((frame == 0) || (result == 0) ||
        !gyro_byte(frame[0]) || !gyro_byte(frame[1]) ||
        !gyro_byte(frame[2]) || !gyro_byte(frame[3]) ||
        !gyro_byte(frame[4]) || (frame[5] != (Uint16)'C') ||
        (frame[6] != GYRO_FRAME_MARKER_1) ||
        (frame[7] != GYRO_FRAME_MARKER_2))
    {
        return 0U;
    }
    *result = frame[3];
    return 1U;
}

Uint16 gyro_read_sample(gyro_uart_transaction_fn transaction,
                        gyro_sample_t *sample)
{
    Uint16 tx[GYRO_COMMAND_FRAME_LEN];
    Uint16 rx[GYRO_DATA_FRAME_LEN];
    Uint16 rx_len;

    if ((transaction == 0) || (sample == 0))
    {
        return 0U;
    }
    (void)gyro_build_data_request(0U, tx);
    rx_len = 0U;
    if ((transaction(tx, GYRO_COMMAND_FRAME_LEN, rx,
                     GYRO_DATA_FRAME_LEN, &rx_len) == 0U) ||
        (rx_len != GYRO_DATA_FRAME_LEN))
    {
        return 0U;
    }
    return gyro_parse_data_frame(rx, sample);
}
