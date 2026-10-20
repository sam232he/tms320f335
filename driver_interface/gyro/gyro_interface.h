#ifndef GYRO_INTERFACE_H
#define GYRO_INTERFACE_H

#include "DSP2833x_Device.h"

/*
 * UNO/DUE inertial-sensor register map.
 *
 * UART frame format:
 *   request: 0x45, command/address, command, 0x46, 0x53
 *   data response: 18 bytes, parsed by gyro_parse_data_frame().
 * All register values are returned as raw 16-bit words. Apply the sensor's
 * documented signedness and scale in the application after reading them.
 */
typedef enum
{
    GYRO_ASIC_UNO = 0U,
    GYRO_ASIC_DUE = 1U
} gyro_asic_t;

typedef enum
{
    GYRO_REG_RATE_X = 0U, /* UNO, address 0x01: Gyro X */
    GYRO_REG_RATE_Y,      /* DUE, address 0x03: Gyro Y */
    GYRO_REG_RATE_Z,      /* DUE, address 0x01: Gyro Z */
    GYRO_REG_ACC_X,       /* UNO, address 0x04: Accel X */
    GYRO_REG_ACC_Y,       /* UNO, address 0x05: Accel Y */
    GYRO_REG_ACC_Z,       /* UNO, address 0x06: Accel Z */
    GYRO_REG_TEMP         /* UNO/DUE, address 0x07: Temperature */
} gyro_register_t;

typedef struct
{
    Uint16 rate_x;
    Uint16 rate_y;
    Uint16 rate_z;
    Uint16 acc_x;
    Uint16 acc_y;
    Uint16 acc_z;
    Uint16 temperature;
} gyro_sample_t;

/* Latest decoded IMU sample, owned privately by gyro_interface.c. */
const gyro_sample_t *gyro_get_sample(void);

#define GYRO_FRAME_START             0x45U
#define GYRO_FRAME_MARKER_1          0x46U
#define GYRO_FRAME_MARKER_2          0x53U
#define GYRO_DATA_FRAME_ID           0x01U
#define GYRO_DATA_FRAME_LEN          18U
#define GYRO_REGISTER_RESPONSE_LEN   8U
#define GYRO_SELFTEST_RESPONSE_LEN   8U
#define GYRO_COMMAND_FRAME_LEN       5U

/* Return 1 on success. Buffers contain one UART byte per Uint16 element. */
typedef Uint16 (*gyro_uart_transaction_fn)(const Uint16 *tx,
                                           Uint16 tx_len,
                                           Uint16 *rx,
                                           Uint16 rx_capacity,
                                           Uint16 *rx_len);

/* Return 1 when the complete command was accepted by the UART driver. */
typedef Uint16 (*gyro_uart_send_fn)(const Uint16 *tx, Uint16 tx_len);

/* Look up the ASIC and register address for one measurement. */
gyro_asic_t gyro_register_asic(gyro_register_t reg);
Uint16 gyro_register_address(gyro_register_t reg);

/* Build commands defined by the IMU protocol. Return frame length, or 0. */
Uint16 gyro_build_data_request(Uint16 periodic, Uint16 *frame);
Uint16 gyro_build_register_read_request(Uint16 address, Uint16 *frame);
Uint16 gyro_build_selftest_request(Uint16 *frame);

/* Send PD once; the IMU then streams data frames periodically. */
Uint16 gyro_start_periodic(gyro_uart_send_fn send);

/* Decode responses. Return 1 when framing and fixed fields are valid. */
Uint16 gyro_parse_data_frame(const Uint16 *frame, gyro_sample_t *sample);
Uint16 gyro_parse_register_response(const Uint16 *frame,
                                    Uint16 *address,
                                    Uint16 *value);
Uint16 gyro_parse_selftest_response(const Uint16 *frame, Uint16 *result);

/* Request one data frame through the caller-provided UART transport. */
Uint16 gyro_read_sample(gyro_uart_transaction_fn transaction,
                        gyro_sample_t *sample);

#endif /* GYRO_INTERFACE_H */
