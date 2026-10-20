/*
 * File: callNextStepGimbalVelocity.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 15-Oct-2026 20:49:39
 */

/* Include Files */
#include "callNextStepGimbalVelocity.h"
#include "callNextStepGimbalVelocity_data.h"
#include "callNextStepGimbalVelocity_initialize.h"
#include "callNextStepGimbalVelocity_types.h"
#include <math.h>

/* Type Definitions */
#ifndef typedef_GimbalADRCRate
#define typedef_GimbalADRCRate
typedef struct {
  struct3_T GlobalOut;
  struct1_T Init;
  double z1[2];
  double z2[2];
  double uPrev[2];
  double b0Prev[2];
} GimbalADRCRate;
#endif /* typedef_GimbalADRCRate */

/* Variable Definitions */
static boolean_T Obj_not_empty;

/* Function Definitions */
/*
 * Arguments    : const struct0_T *ObservationStruct
 *                const struct1_T *InitStruct
 *                boolean_T ResetFlag
 *                struct3_T *OutStruct
 * Return Type  : void
 */
void callNextStepGimbalVelocity(const struct0_T *ObservationStruct,
                                const struct1_T *InitStruct,
                                boolean_T ResetFlag, struct3_T *OutStruct)
{
  static GimbalADRCRate Obj;
  double L2;
  double a_tmp;
  double b0_idx_0;
  double b0_idx_1;
  double c;
  double e;
  double eps;
  double wc;
  if (!isInitialized_callNextStepGimbalVelocity) {
    callNextStepGimbalVelocity_initialize();
  }
  if (!Obj_not_empty) {
    Obj_not_empty = true;
    Obj.Init = *InitStruct;
    Obj.z1[0] = InitStruct->AverageGyroVec[1] * 3.1415926535897931 / 180.0;
    Obj.z1[1] = InitStruct->AverageGyroVec[2] * 3.1415926535897931 / 180.0;
    Obj.GlobalOut.ActuationOut[0] = 0.0;
    Obj.z2[0] = 0.0;
    Obj.uPrev[0] = 0.0;
    Obj.GlobalOut.ActuationOut[1] = 0.0;
    Obj.z2[1] = 0.0;
    Obj.uPrev[1] = 0.0;
    eps = ObservationStruct->EncInner * 3.1415926535897931 / 180.0;
    e = sin(eps);
    a_tmp = cos(eps);
    eps = ((Obj.Init.Bk + Obj.Init.Ar * (e * e)) +
           Obj.Init.Ad * (a_tmp * a_tmp)) -
          Obj.Init.Ard * sin(2.0 * eps);
    if (eps < Obj.Init.MinJeq) {
      eps = Obj.Init.MinJeq;
    }
    Obj.b0Prev[0] = Obj.Init.InnerMotorKt * Obj.Init.Vdc /
                    (100.0 * Obj.Init.InnerMotorR) / Obj.Init.Ae;
    Obj.b0Prev[1] = Obj.Init.OuterMotorKt * Obj.Init.Vdc /
                    (100.0 * Obj.Init.OuterMotorR) * a_tmp / eps;
  } else if (ResetFlag) {
    Obj.Init = *InitStruct;
    Obj.z1[0] = InitStruct->AverageGyroVec[1] * 3.1415926535897931 / 180.0;
    Obj.z1[1] = InitStruct->AverageGyroVec[2] * 3.1415926535897931 / 180.0;
    Obj.z2[0] = 0.0;
    Obj.uPrev[0] = 0.0;
    Obj.z2[1] = 0.0;
    Obj.uPrev[1] = 0.0;
    eps = ObservationStruct->EncInner * 3.1415926535897931 / 180.0;
    e = sin(eps);
    a_tmp = cos(eps);
    eps = ((Obj.Init.Bk + Obj.Init.Ar * (e * e)) +
           Obj.Init.Ad * (a_tmp * a_tmp)) -
          Obj.Init.Ard * sin(2.0 * eps);
    if (eps < Obj.Init.MinJeq) {
      eps = Obj.Init.MinJeq;
    }
    Obj.b0Prev[0] = Obj.Init.InnerMotorKt * Obj.Init.Vdc /
                    (100.0 * Obj.Init.InnerMotorR) / Obj.Init.Ae;
    Obj.b0Prev[1] = Obj.Init.OuterMotorKt * Obj.Init.Vdc /
                    (100.0 * Obj.Init.OuterMotorR) * a_tmp / eps;
    Obj.GlobalOut.ActuationOut[0] = 0.0;
    Obj.GlobalOut.ActuationOut[1] = 0.0;
  } else {
    eps = ObservationStruct->EncInner * 3.1415926535897931 / 180.0;
    e = sin(eps);
    a_tmp = cos(eps);
    eps = ((Obj.Init.Bk + Obj.Init.Ar * (e * e)) +
           Obj.Init.Ad * (a_tmp * a_tmp)) -
          Obj.Init.Ard * sin(2.0 * eps);
    if (eps < Obj.Init.MinJeq) {
      eps = Obj.Init.MinJeq;
    }
    b0_idx_0 = Obj.Init.InnerMotorKt * Obj.Init.Vdc /
               (100.0 * Obj.Init.InnerMotorR) / Obj.Init.Ae;
    b0_idx_1 = Obj.Init.OuterMotorKt * Obj.Init.Vdc /
               (100.0 * Obj.Init.OuterMotorR) * a_tmp / eps;
    wc = 4.0 / Obj.Init.Rate.Tsettle;
    eps = exp(-(Obj.Init.Rate.kob * wc) * Obj.Init.dT);
    c = eps * eps;
    L2 = (1.0 - eps) * (1.0 - eps) / Obj.Init.dT;
    eps = Obj.z1[0] + Obj.Init.dT * (Obj.z2[0] + Obj.b0Prev[0] * Obj.uPrev[0]);
    e = ObservationStruct->GyroVec[1] * 3.1415926535897931 / 180.0 - eps;
    a_tmp = eps + (1.0 - c) * e;
    Obj.z1[0] = a_tmp;
    eps = Obj.z2[0] + L2 * e;
    Obj.z2[0] = eps;
    Obj.GlobalOut.ActuationOut[0] =
        ((wc * (ObservationStruct->OmegaRef[0] * 3.1415926535897931 / 180.0 -
                a_tmp) +
          ObservationStruct->OmegaDotRef[0] * 3.1415926535897931 / 180.0) -
         eps) /
        b0_idx_0;
    a_tmp = fmin(fmax(Obj.GlobalOut.ActuationOut[0], -Obj.Init.MaxActuationOut),
                 Obj.Init.MaxActuationOut);
    Obj.GlobalOut.ActuationOut[0] = a_tmp;
    Obj.uPrev[0] = a_tmp;
    Obj.b0Prev[0] = b0_idx_0;
    eps = Obj.z1[1] + Obj.Init.dT * (Obj.z2[1] + Obj.b0Prev[1] * Obj.uPrev[1]);
    e = ObservationStruct->GyroVec[2] * 3.1415926535897931 / 180.0 - eps;
    a_tmp = eps + (1.0 - c) * e;
    Obj.z1[1] = a_tmp;
    eps = Obj.z2[1] + L2 * e;
    Obj.z2[1] = eps;
    Obj.GlobalOut.ActuationOut[1] =
        ((wc * (ObservationStruct->OmegaRef[1] * 3.1415926535897931 / 180.0 -
                a_tmp) +
          ObservationStruct->OmegaDotRef[1] * 3.1415926535897931 / 180.0) -
         eps) /
        b0_idx_1;
    a_tmp = fmin(fmax(Obj.GlobalOut.ActuationOut[1], -Obj.Init.MaxActuationOut),
                 Obj.Init.MaxActuationOut);
    Obj.GlobalOut.ActuationOut[1] = a_tmp;
    Obj.uPrev[1] = a_tmp;
    Obj.b0Prev[1] = b0_idx_1;
  }
  *OutStruct = Obj.GlobalOut;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void callNextStepGimbalVelocity_init(void)
{
  Obj_not_empty = false;
}

/*
 * File trailer for callNextStepGimbalVelocity.c
 *
 * [EOF]
 */
