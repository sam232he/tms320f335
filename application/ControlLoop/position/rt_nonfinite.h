#include <stdbool.h>
/*
 * File: rt_nonfinite.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 15-Oct-2026 20:51:14
 */

#ifndef RT_NONFINITE_H
#define RT_NONFINITE_H

/* Include Files */
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern double rtInf;
extern double rtMinusInf;
extern double rtNaN;
extern float rtInfF;
extern float rtMinusInfF;
extern float rtNaNF;

extern bool rtIsInf(double value);
extern bool rtIsInfF(float value);
extern bool rtIsNaN(double value);
extern bool rtIsNaNF(float value);

#ifdef __cplusplus
}
#endif
#endif
/*
 * File trailer for rt_nonfinite.h
 *
 * [EOF]
 */
