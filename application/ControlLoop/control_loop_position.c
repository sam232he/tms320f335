#include <stdbool.h>
#include <stdint.h>
#include "control_loop.h"
#include "position/callNextStepGimbalPosition.h"
#include "position/callNextStepGimbalPosition_terminate.h"

static void position_observation(const control_loop_observation_t *in,
                                 struct0_T *out)
{
    uint16_t i;
    out->EncInner = in->enc_inner_deg;
    out->EncOuter = in->enc_outer_deg;
    for (i = 0; i < 3; ++i) {
        out->GyroVec[i] = in->gyro_deg_s[i];
        out->AccelVec[i] = in->accel[i];
    }
    for (i = 0; i < 2; ++i) {
        out->PosRef[i] = in->position_ref_deg[i];
        out->PosRefDot[i] = in->position_ref_rate_deg_s[i];
    }
}

static void position_parameters(const control_loop_parameters_t *in,
                                struct1_T *out)
{
    uint16_t i;
    out->AverageEncInner = in->average_enc_inner_deg;
    out->AverageEncOuter = in->average_enc_outer_deg;
    for (i = 0; i < 3; ++i) {
        out->AverageGyroVec[i] = in->average_gyro_deg_s[i];
        out->AverageAccVec[i] = in->average_accel[i];
    }
    out->MaxAngleInnerGimbal = in->max_angle_inner_deg;
    out->MaxAngleOuterGimbal = in->max_angle_outer_deg;
    out->MaxActuationOut = in->max_actuation;
    out->dT = in->dt_s;
    out->InnerMotorKt = in->inner_motor_kt;
    out->InnerMotorR = in->inner_motor_resistance;
    out->OuterMotorKt = in->outer_motor_kt;
    out->OuterMotorR = in->outer_motor_resistance;
    out->Vdc = in->bus_voltage;
    out->Ae = in->ae;
    out->Ar = in->ar;
    out->Ad = in->ad;
    out->Ard = in->ard;
    out->Bk = in->bk;
    out->Rate.kob = in->rate_kob;
    out->Rate.Tsettle = in->rate_tsettle_s;
    out->Position.kob = in->position_kob;
    out->Position.Tsettle = in->position_tsettle_s;
    out->MinJeq = in->min_jeq;
    out->MinAbsCosEpsilon = in->min_abs_cos_epsilon;
}

void control_loop_position_step(const control_loop_observation_t *observation,
                                const control_loop_parameters_t *parameters,
                                uint16_t reset,
                                double actuator[2])
{
    struct0_T generated_observation;
    struct1_T generated_parameters;
    struct3_T generated_output;

    position_observation(observation, &generated_observation);
    position_parameters(parameters, &generated_parameters);
    callNextStepGimbalPosition(&generated_observation, &generated_parameters,
                               (bool)(reset != 0U), &generated_output);
    actuator[0] = generated_output.ActuationOut[0];
    actuator[1] = generated_output.ActuationOut[1];
}
