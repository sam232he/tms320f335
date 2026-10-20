/*
 * File: callNextStepGimbalPosition_types.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 15-Oct-2026 20:51:14
 */

#ifndef CALLNEXTSTEPGIMBALPOSITION_TYPES_H
#define CALLNEXTSTEPGIMBALPOSITION_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_struct0_T
#define typedef_struct0_T
typedef struct {
  double EncInner;
  double EncOuter;
  double GyroVec[3];
  double AccelVec[3];
  double PosRef[2];
  double PosRefDot[2];
} struct0_T;
#endif /* typedef_struct0_T */

#ifndef typedef_struct2_T
#define typedef_struct2_T
typedef struct {
  double kob;
  double Tsettle;
} struct2_T;
#endif /* typedef_struct2_T */

#ifndef typedef_struct1_T
#define typedef_struct1_T
typedef struct {
  double AverageEncInner;
  double AverageEncOuter;
  double AverageGyroVec[3];
  double AverageAccVec[3];
  double MaxAngleInnerGimbal;
  double MaxAngleOuterGimbal;
  double MaxActuationOut;
  double dT;
  double InnerMotorKt;
  double InnerMotorR;
  double OuterMotorKt;
  double OuterMotorR;
  double Vdc;
  double Ae;
  double Ar;
  double Ad;
  double Ard;
  double Bk;
  struct2_T Rate;
  struct2_T Position;
  double MinJeq;
  double MinAbsCosEpsilon;
} struct1_T;
#endif /* typedef_struct1_T */

#ifndef typedef_struct3_T
#define typedef_struct3_T
typedef struct {
  double ActuationOut[2];
} struct3_T;
#endif /* typedef_struct3_T */

#endif
/*
 * File trailer for callNextStepGimbalPosition_types.h
 *
 * [EOF]
 */
