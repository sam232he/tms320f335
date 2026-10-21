#include <stdint.h>
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
    uint16_t rate_x;
    uint16_t rate_y;
    uint16_t rate_z;
    uint16_t acc_x;
    uint16_t acc_y;
    uint16_t acc_z;
    uint16_t temperature;
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

/* Return 1 on success. Buffers contain one UART byte per uint16_t element. */
typedef uint16_t (*gyro_uart_transaction_fn)(const uint16_t *tx,
                                           uint16_t tx_len,
                                           uint16_t *rx,
                                           uint16_t rx_capacity,
                                           uint16_t *rx_len);

/* Return 1 when the complete command was accepted by the UART driver. */
typedef uint16_t (*gyro_uart_send_fn)(const uint16_t *tx, uint16_t tx_len);

/* Look up the ASIC and register address for one measurement. */
gyro_asic_t gyro_register_asic(gyro_register_t reg);
uint16_t gyro_register_address(gyro_register_t reg);

/* Build commands defined by the IMU protocol. Return frame length, or 0. */
uint16_t gyro_build_data_request(uint16_t periodic, uint16_t *frame);
uint16_t gyro_build_register_read_request(uint16_t address, uint16_t *frame);
uint16_t gyro_build_selftest_request(uint16_t *frame);

/* Send PD once; the IMU then streams data frames periodically. */
uint16_t gyro_start_periodic(gyro_uart_send_fn send);

/* Decode responses. Return 1 when framing and fixed fields are valid. */
uint16_t gyro_parse_data_frame(const uint16_t *frame, gyro_sample_t *sample);
uint16_t gyro_parse_register_response(const uint16_t *frame,
                                    uint16_t *address,
                                    uint16_t *value);
uint16_t gyro_parse_selftest_response(const uint16_t *frame, uint16_t *result);

/* Request one data frame through the caller-provided UART transport. */
uint16_t gyro_read_sample(gyro_uart_transaction_fn transaction,
                        gyro_sample_t *sample);

#endif /* GYRO_INTERFACE_H */
