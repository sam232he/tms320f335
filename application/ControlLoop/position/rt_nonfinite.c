#include <stdbool.h>
/*
 * File: rt_nonfinite.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 15-Oct-2026 20:51:14
 */

/*
 * Abstract:
 *      MATLAB for code generation function to initialize non-finites,
 *      (Inf, NaN and -Inf).
 */
/* Include Files */
#include "rt_nonfinite.h"
#include <math.h>

#if defined(__ICL) && __ICL == 1700
#pragma warning(disable : 264)
#endif

double rtNaN = (double)NAN;
double rtInf = (double)INFINITY;
double rtMinusInf = -(double)INFINITY;
float rtNaNF = (float)NAN;
float rtInfF = (float)INFINITY;
float rtMinusInfF = -(float)INFINITY;

#if defined(__ICL) && __ICL == 1700
#pragma warning(default : 264)
#endif

/*
 * Function: rtIsInf ==================================================
 *  Abstract:
 *  Test if value is infinite
 */
bool rtIsInf(double value)
{
  return (isinf(value) != 0U);
}

/*
 * Function: rtIsInfF =================================================
 *  Abstract:
 *  Test if single-precision value is infinite
 */
bool rtIsInfF(float value)
{
  return (isinf((double)value) != 0U);
}

/*
 * Function: rtIsNaN ==================================================
 *  Abstract:
 *  Test if value is not a number
 */
bool rtIsNaN(double value)
{
  return (isnan(value) != 0U);
}

/*
 * Function: rtIsNaNF =================================================
 *  Abstract:
 *  Test if single-precision value is not a number
 */
bool rtIsNaNF(float value)
{
  return (isnan((double)value) != 0U);
}

/*
 * File trailer for rt_nonfinite.c
 *
 * [EOF]
 */
