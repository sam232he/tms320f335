#include <stdint.h>
#include "control_loop.h"
#include "../control_command/control_command.h"
#include "../../driver_interface/encoder/encoder_interface.h"
#include "../../driver_interface/gyro/gyro_interface.h"
#include "position/callNextStepGimbalPosition_initialize.h"
#include "position/callNextStepGimbalPosition_terminate.h"
#include "velocity/callNextStepGimbalVelocity_initialize.h"
#include "velocity/callNextStepGimbalVelocity_terminate.h"

static control_loop_observation_t control_loop_input_data = {0};
static control_loop_output_t control_loop_output_data = {0};
static double control_loop_actuator_data[2] = {0.0, 0.0};
static control_loop_parameters_t control_loop_parameters_data = {0};
static control_loop_mode_t control_loop_last_mode = CONTROL_LOOP_POSITION;

/* A SZ35T6Y-21 encoder is a 21-bit absolute, one-revolution count. */
#define CONTROL_LOOP_ENCODER_COUNTS_PER_REV  2097152.0
#define CONTROL_LOOP_GYRO_LSB_TO_DEG_S       1.0
#define CONTROL_LOOP_ACCEL_LSB_TO_UNIT       1.0

static double control_loop_encoder_to_deg(uint32_t counts)
{
    return ((double)counts * 360.0) /
           CONTROL_LOOP_ENCODER_COUNTS_PER_REV;
}

static double control_loop_signed16(uint16_t raw)
{
    int32_t value;

    value = (int32_t)raw;
    if ((raw & 0x8000U) != 0U)
    {
        value -= (int32_t)65536;
    }
    return (double)value;
}

static uint16_t control_loop_actuator_to_duty(double actuator)
{
    double duty;

    if (actuator > 100.0)
    {
        actuator = 100.0;
    }
    else if (actuator < -100.0)
    {
        actuator = -100.0;
    }

    /* Signed percentage to complementary H-bridge duty. */
    duty = 50.0 + (0.5 * actuator);
    return (uint16_t)(duty + 0.5);
}

static void control_loop_update_pwm_output(void)
{
    control_loop_output_data.pwm_duty[0] =
        control_loop_actuator_to_duty(control_loop_actuator_data[0]);
    control_loop_output_data.pwm_duty[1] =
        control_loop_actuator_to_duty(control_loop_actuator_data[1]);
}

void control_loop_initialize(void)
{
    callNextStepGimbalPosition_initialize();
    callNextStepGimbalVelocity_initialize();

    control_loop_last_mode = CONTROL_LOOP_POSITION;

    /* Test defaults; replace these here when final values are available. */
    control_loop_parameters_data.average_enc_inner_deg = 0.0;
    control_loop_parameters_data.average_enc_outer_deg = 0.0;
    control_loop_parameters_data.average_gyro_deg_s[0] = 0.0;
    control_loop_parameters_data.average_gyro_deg_s[1] = 0.0;
    control_loop_parameters_data.average_gyro_deg_s[2] = 0.0;
    control_loop_parameters_data.average_accel[0] = 0.0;
    control_loop_parameters_data.average_accel[1] = 0.0;
    control_loop_parameters_data.average_accel[2] = 0.0;
    control_loop_parameters_data.max_angle_inner_deg = 180.0;
    control_loop_parameters_data.max_angle_outer_deg = 180.0;
    control_loop_parameters_data.max_actuation = 100.0;
    control_loop_parameters_data.dt_s = 0.001;
    control_loop_parameters_data.inner_motor_kt = 1.0;
    control_loop_parameters_data.inner_motor_resistance = 1.0;
    control_loop_parameters_data.outer_motor_kt = 1.0;
    control_loop_parameters_data.outer_motor_resistance = 1.0;
    control_loop_parameters_data.bus_voltage = 24.0;
    control_loop_parameters_data.ae = 1.0;
    control_loop_parameters_data.ar = 1.0;
    control_loop_parameters_data.ad = 1.0;
    control_loop_parameters_data.ard = 1.0;
    control_loop_parameters_data.bk = 1.0;
    control_loop_parameters_data.rate_kob = 1.0;
    control_loop_parameters_data.rate_tsettle_s = 0.1;
    control_loop_parameters_data.position_kob = 1.0;
    control_loop_parameters_data.position_tsettle_s = 0.1;
    control_loop_parameters_data.min_jeq = 0.1;
    control_loop_parameters_data.min_abs_cos_epsilon = 0.1;

    /* Test references; replace these when command inputs are available. */
    control_loop_input_data.position_ref_deg[0] = 10.0;
    control_loop_input_data.position_ref_deg[1] = 10.0;
    control_loop_input_data.position_ref_rate_deg_s[0] = 0.0;
    control_loop_input_data.position_ref_rate_deg_s[1] = 0.0;
    control_loop_input_data.velocity_ref_deg_s[0] = 5.0;
    control_loop_input_data.velocity_ref_deg_s[1] = 5.0;
    control_loop_input_data.velocity_ref_accel_deg_s2[0] = 0.0;
    control_loop_input_data.velocity_ref_accel_deg_s2[1] = 0.0;
}

void control_loop_terminate(void)
{
    callNextStepGimbalPosition_terminate();
    callNextStepGimbalVelocity_terminate();
}

void control_loop_input(void)
{
    const gyro_sample_t *gyro;

    gyro = gyro_get_sample();
    control_loop_input_data.enc_inner_deg =
        control_loop_encoder_to_deg(encoder_read_counts(ENC_1));
    control_loop_input_data.enc_outer_deg =
        control_loop_encoder_to_deg(encoder_read_counts(ENC_2));

    control_loop_input_data.gyro_deg_s[0] =
        control_loop_signed16(gyro->rate_x) * CONTROL_LOOP_GYRO_LSB_TO_DEG_S;
    control_loop_input_data.gyro_deg_s[1] =
        control_loop_signed16(gyro->rate_y) * CONTROL_LOOP_GYRO_LSB_TO_DEG_S;
    control_loop_input_data.gyro_deg_s[2] =
        control_loop_signed16(gyro->rate_z) * CONTROL_LOOP_GYRO_LSB_TO_DEG_S;

    control_loop_input_data.accel[0] =
        control_loop_signed16(gyro->acc_x) * CONTROL_LOOP_ACCEL_LSB_TO_UNIT;
    control_loop_input_data.accel[1] =
        control_loop_signed16(gyro->acc_y) * CONTROL_LOOP_ACCEL_LSB_TO_UNIT;
    control_loop_input_data.accel[2] =
        control_loop_signed16(gyro->acc_z) * CONTROL_LOOP_ACCEL_LSB_TO_UNIT;

    if (control_command_mode() == CONTROL_LOOP_POSITION)
    {
        control_loop_input_data.position_ref_deg[0] =
            control_command_inner_angle();
        control_loop_input_data.position_ref_deg[1] =
            control_command_outer_angle();
        control_loop_input_data.position_ref_rate_deg_s[0] = 0.0;
        control_loop_input_data.position_ref_rate_deg_s[1] = 0.0;
    }
    else
    {
        control_loop_input_data.velocity_ref_deg_s[0] =
            control_command_inner_angle();
        control_loop_input_data.velocity_ref_deg_s[1] =
            control_command_outer_angle();
        control_loop_input_data.velocity_ref_accel_deg_s2[0] = 0.0;
        control_loop_input_data.velocity_ref_accel_deg_s2[1] = 0.0;
    }

}

void control_loop_output(control_loop_output_t *output)
{
    if (output != 0)
    {
        *output = control_loop_output_data;
    }
}

control_loop_parameters_t *control_loop_parameters(void)
{
    return &control_loop_parameters_data;
}

void control_loop_run(control_loop_mode_t mode, uint16_t reset)
{
    mode = control_command_mode();
    if (mode != control_loop_last_mode)
    {
        reset = 1U;
        control_loop_last_mode = mode;
    }
    control_loop_input();

    if (mode == CONTROL_LOOP_VELOCITY)
    {
        control_loop_velocity_step(&control_loop_input_data,
                                   &control_loop_parameters_data, reset,
                                   control_loop_actuator_data);
    }
    else
    {
        control_loop_position_step(&control_loop_input_data,
                                   &control_loop_parameters_data, reset,
                                   control_loop_actuator_data);
    }

    control_loop_update_pwm_output();
}
