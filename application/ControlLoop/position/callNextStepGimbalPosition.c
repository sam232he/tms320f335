/*
 * File: callNextStepGimbalPosition.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 15-Oct-2026 20:51:14
 */

/* Include Files */
#include "callNextStepGimbalPosition.h"
#include "callNextStepGimbalPosition_data.h"
#include "callNextStepGimbalPosition_initialize.h"
#include "callNextStepGimbalPosition_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Type Definitions */
#ifndef typedef_GimbalADRCPosition
#define typedef_GimbalADRCPosition
typedef struct {
  struct3_T GlobalOut;
  struct1_T Init;
  double z1[2];
  double z2[2];
  double z3[2];
  double uPrev[2];
  double b0Prev[2];
} GimbalADRCPosition;
#endif /* typedef_GimbalADRCPosition */

/* Variable Definitions */
static boolean_T Obj_not_empty;

/* Function Declarations */
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double d;
  double d1;
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * Arguments    : const struct0_T *ObservationStruct
 *                const struct1_T *InitStruct
 *                boolean_T ResetFlag
 *                struct3_T *OutStruct
 * Return Type  : void
 */
void callNextStepGimbalPosition(const struct0_T *ObservationStruct,
                                const struct1_T *InitStruct,
                                boolean_T ResetFlag, struct3_T *OutStruct)
{
  static GimbalADRCPosition Obj;
  double Jeq;
  double L2;
  double L3;
  double b0_idx_0;
  double b0_idx_1;
  double c;
  double d;
  double e;
  double wc;
  double y_idx_0_tmp;
  double z;
  double z1p_tmp;
  if (!isInitialized_callNextStepGimbalPosition) {
    callNextStepGimbalPosition_initialize();
  }
  if (!Obj_not_empty) {
    Obj_not_empty = true;
    Obj.Init = *InitStruct;
    Obj.z1[0] = InitStruct->AverageEncInner * 3.1415926535897931 / 180.0;
    Obj.z1[1] = InitStruct->AverageEncOuter * 3.1415926535897931 / 180.0;
    Obj.GlobalOut.ActuationOut[0] = 0.0;
    Obj.z2[0] = 0.0;
    Obj.z3[0] = 0.0;
    Obj.uPrev[0] = 0.0;
    Obj.GlobalOut.ActuationOut[1] = 0.0;
    Obj.z2[1] = 0.0;
    Obj.z3[1] = 0.0;
    Obj.uPrev[1] = 0.0;
    Jeq = ObservationStruct->EncInner * 3.1415926535897931 / 180.0;
    z = sin(Jeq);
    e = cos(Jeq);
    Jeq = ((Obj.Init.Bk + Obj.Init.Ar * (z * z)) + Obj.Init.Ad * (e * e)) -
          Obj.Init.Ard * sin(2.0 * Jeq);
    if (Jeq < Obj.Init.MinJeq) {
      Jeq = Obj.Init.MinJeq;
    }
    Obj.b0Prev[0] = Obj.Init.InnerMotorKt * Obj.Init.Vdc /
                    (100.0 * Obj.Init.InnerMotorR) / Obj.Init.Ae;
    Obj.b0Prev[1] = Obj.Init.OuterMotorKt * Obj.Init.Vdc /
                    (100.0 * Obj.Init.OuterMotorR) * e / Jeq;
  } else if (ResetFlag) {
    Obj.Init = *InitStruct;
    Obj.z1[0] = InitStruct->AverageEncInner * 3.1415926535897931 / 180.0;
    Obj.z1[1] = InitStruct->AverageEncOuter * 3.1415926535897931 / 180.0;
    Obj.z2[0] = 0.0;
    Obj.z3[0] = 0.0;
    Obj.uPrev[0] = 0.0;
    Obj.z2[1] = 0.0;
    Obj.z3[1] = 0.0;
    Obj.uPrev[1] = 0.0;
    Jeq = ObservationStruct->EncInner * 3.1415926535897931 / 180.0;
    z = sin(Jeq);
    e = cos(Jeq);
    Jeq = ((Obj.Init.Bk + Obj.Init.Ar * (z * z)) + Obj.Init.Ad * (e * e)) -
          Obj.Init.Ard * sin(2.0 * Jeq);
    if (Jeq < Obj.Init.MinJeq) {
      Jeq = Obj.Init.MinJeq;
    }
    Obj.b0Prev[0] = Obj.Init.InnerMotorKt * Obj.Init.Vdc /
                    (100.0 * Obj.Init.InnerMotorR) / Obj.Init.Ae;
    Obj.b0Prev[1] = Obj.Init.OuterMotorKt * Obj.Init.Vdc /
                    (100.0 * Obj.Init.OuterMotorR) * e / Jeq;
    Obj.GlobalOut.ActuationOut[0] = 0.0;
    Obj.GlobalOut.ActuationOut[1] = 0.0;
  } else {
    y_idx_0_tmp = ObservationStruct->EncInner * 3.1415926535897931 / 180.0;
    z = sin(y_idx_0_tmp);
    e = cos(y_idx_0_tmp);
    Jeq = ((Obj.Init.Bk + Obj.Init.Ar * (z * z)) + Obj.Init.Ad * (e * e)) -
          Obj.Init.Ard * sin(2.0 * y_idx_0_tmp);
    if (Jeq < Obj.Init.MinJeq) {
      Jeq = Obj.Init.MinJeq;
    }
    b0_idx_0 = Obj.Init.InnerMotorKt * Obj.Init.Vdc /
               (100.0 * Obj.Init.InnerMotorR) / Obj.Init.Ae;
    b0_idx_1 = Obj.Init.OuterMotorKt * Obj.Init.Vdc /
               (100.0 * Obj.Init.OuterMotorR) * e / Jeq;
    wc = 6.0 / Obj.Init.Position.Tsettle;
    z = exp(-(Obj.Init.Position.kob * wc) * Obj.Init.dT);
    c = rt_powd_snf(z, 3.0);
    L2 = 1.5 * (z + 1.0) * ((1.0 - z) * (1.0 - z)) / Obj.Init.dT;
    Jeq = Obj.Init.dT * Obj.Init.dT;
    L3 = rt_powd_snf(1.0 - z, 3.0) / Jeq;
    z1p_tmp = 0.5 * Jeq;
    Jeq = (Obj.z1[0] + Obj.Init.dT * Obj.z2[0]) +
          z1p_tmp * (Obj.z3[0] + Obj.b0Prev[0] * Obj.uPrev[0]);
    e = y_idx_0_tmp - Jeq;
    z = Jeq + (1.0 - c) * e;
    Obj.z1[0] = z;
    y_idx_0_tmp =
        (Obj.z2[0] + Obj.Init.dT * (Obj.z3[0] + Obj.b0Prev[0] * Obj.uPrev[0])) +
        L2 * e;
    Obj.z2[0] = y_idx_0_tmp;
    Jeq = Obj.z3[0] + L3 * e;
    Obj.z3[0] = Jeq;
    d = wc * wc;
    Obj.GlobalOut.ActuationOut[0] =
        ((d * (ObservationStruct->PosRef[0] * 3.1415926535897931 / 180.0 - z) +
          2.0 * wc *
              (ObservationStruct->PosRefDot[0] * 3.1415926535897931 / 180.0 -
               y_idx_0_tmp)) -
         Jeq) /
        b0_idx_0;
    z = fmin(fmax(Obj.GlobalOut.ActuationOut[0], -Obj.Init.MaxActuationOut),
             Obj.Init.MaxActuationOut);
    Obj.GlobalOut.ActuationOut[0] = z;
    Obj.uPrev[0] = z;
    Obj.b0Prev[0] = b0_idx_0;
    Jeq = (Obj.z1[1] + Obj.Init.dT * Obj.z2[1]) +
          z1p_tmp * (Obj.z3[1] + Obj.b0Prev[1] * Obj.uPrev[1]);
    e = ObservationStruct->EncOuter * 3.1415926535897931 / 180.0 - Jeq;
    z = Jeq + (1.0 - c) * e;
    Obj.z1[1] = z;
    y_idx_0_tmp =
        (Obj.z2[1] + Obj.Init.dT * (Obj.z3[1] + Obj.b0Prev[1] * Obj.uPrev[1])) +
        L2 * e;
    Obj.z2[1] = y_idx_0_tmp;
    Jeq = Obj.z3[1] + L3 * e;
    Obj.z3[1] = Jeq;
    Obj.GlobalOut.ActuationOut[1] =
        ((d * (ObservationStruct->PosRef[1] * 3.1415926535897931 / 180.0 - z) +
          2.0 * wc *
              (ObservationStruct->PosRefDot[1] * 3.1415926535897931 / 180.0 -
               y_idx_0_tmp)) -
         Jeq) /
        b0_idx_1;
    z = fmin(fmax(Obj.GlobalOut.ActuationOut[1], -Obj.Init.MaxActuationOut),
             Obj.Init.MaxActuationOut);
    Obj.GlobalOut.ActuationOut[1] = z;
    Obj.uPrev[1] = z;
    Obj.b0Prev[1] = b0_idx_1;
  }
  *OutStruct = Obj.GlobalOut;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void callNextStepGimbalPosition_init(void)
{
  Obj_not_empty = false;
}

/*
 * File trailer for callNextStepGimbalPosition.c
 *
 * [EOF]
 */
