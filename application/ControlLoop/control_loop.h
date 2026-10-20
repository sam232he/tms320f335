#ifndef CONTROL_LOOP_H
#define CONTROL_LOOP_H

#include "DSP2833x_Device.h"

/* Application-facing interface for the generated position and velocity loops. */

typedef struct
{
    double enc_inner_deg;
    double enc_outer_deg;
    double gyro_deg_s[3];
    double accel[3];
    double position_ref_deg[2];
    double position_ref_rate_deg_s[2];
    double velocity_ref_deg_s[2];
    double velocity_ref_accel_deg_s2[2];
} control_loop_observation_t;

typedef struct
{
    double average_enc_inner_deg;
    double average_enc_outer_deg;
    double average_gyro_deg_s[3];
    double average_accel[3];
    double max_angle_inner_deg;
    double max_angle_outer_deg;
    double max_actuation;
    double dt_s;
    double inner_motor_kt;
    double inner_motor_resistance;
    double outer_motor_kt;
    double outer_motor_resistance;
    double bus_voltage;
    double ae;
    double ar;
    double ad;
    double ard;
    double bk;
    double rate_kob;
    double rate_tsettle_s;
    double position_kob;
    double position_tsettle_s;
    double min_jeq;
    double min_abs_cos_epsilon;
} control_loop_parameters_t;

typedef struct
{
    Uint16 pwm_duty[2];       /* converted H-bridge duty: 0 .. 100 % */
} control_loop_output_t;

typedef enum
{
    CONTROL_LOOP_POSITION = 0U,
    CONTROL_LOOP_VELOCITY = 1U
} control_loop_mode_t;

void control_loop_initialize(void);
void control_loop_terminate(void);

/* Read encoder/IMU interfaces into private observation storage. */
void control_loop_input(void);

/* Copy the latest actuator output from control_loop.c private storage. */
void control_loop_output(control_loop_output_t *output);

/* Return controller parameters; defaults are test values until replaced. */
control_loop_parameters_t *control_loop_parameters(void);

/* Run one step using the stored input and selected controller mode. */
void control_loop_run(control_loop_mode_t mode, unsigned char reset);

void control_loop_position_step(const control_loop_observation_t *observation,
                                const control_loop_parameters_t *parameters,
                                unsigned char reset,
                                double actuator[2]);
void control_loop_velocity_step(const control_loop_observation_t *observation,
                                const control_loop_parameters_t *parameters,
                                unsigned char reset,
                                double actuator[2]);

#endif
