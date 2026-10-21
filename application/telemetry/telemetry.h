#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

/*
 * SCI-B telemetry is emitted as five independent packets per measurement
 * cycle. The receiver can use packet_index to identify each packet; a
 * missing packet does not invalidate the other packets.
 *
 * Packet indexes:
 *   0: encoder values and raw counts
 *   1: gyro rates and temperature
 *   2: accelerometer values
 *   3: motor PWM output
 *   4: raw motor-current ADC values and drive status
 *
 * Packet 4 payload: tick(u32), current1_adc(u16), current2_adc(u16),
 * current1_fault(u16), current2_fault(u16), drive_fault(u16),
 * drive_otw(u16), validity(u16).
 */
#define TELEMETRY_SYNC_1                  0xAAU
#define TELEMETRY_SYNC_2                  0xBBU
#define TELEMETRY_PERIOD_MS               100UL /* 10 Hz */
#define TELEMETRY_PACKET_COUNT            5U
#define TELEMETRY_FRAME_HEADER_LEN        4U
#define TELEMETRY_MAX_PAYLOAD_LEN         24U
#define TELEMETRY_MAX_FRAME_LEN           30U

/* Frame bytes: AA BB packet_index payload_length payload[ length ] crc16_l crc16_h
 * CRC-16/CCITT-FALSE covers packet_index through the final payload byte.
 */

/* Measurement validity bits in the measurement payload. */
#define TELEMETRY_VALID_ENCODER           0x0001U
#define TELEMETRY_VALID_GYRO              0x0002U
#define TELEMETRY_VALID_ACCEL             0x0004U
#define TELEMETRY_VALID_IMU_TEMPERATURE   0x0008U
#define TELEMETRY_VALID_PWM               0x0010U
#define TELEMETRY_VALID_MOTOR_CURRENT     0x0020U
#define TELEMETRY_VALID_MOTOR_TEMPERATURE 0x0040U
#define TELEMETRY_VALID_MOTOR_FAULT       0x0080U
#define TELEMETRY_VALID_MOTOR_OTW         0x0100U
#define TELEMETRY_VALID_CURRENT_1_FAULT   0x0200U
#define TELEMETRY_VALID_CURRENT_2_FAULT   0x0400U

void telemetry_init(void);
uint16_t telemetry_update(void);
uint32_t telemetry_frame_count(void);
uint32_t telemetry_drop_count(void);

#endif /* TELEMETRY_H */
