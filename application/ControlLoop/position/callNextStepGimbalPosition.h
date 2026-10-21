#include <stdbool.h>
/*
 * File: callNextStepGimbalPosition.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 15-Oct-2026 20:51:14
 */

#ifndef CALLNEXTSTEPGIMBALPOSITION_H
#define CALLNEXTSTEPGIMBALPOSITION_H

/* Include Files */
#include "callNextStepGimbalPosition_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void callNextStepGimbalPosition(const struct0_T *ObservationStruct,
                                       const struct1_T *InitStruct,
                                       bool ResetFlag,
                                       struct3_T *OutStruct);

void callNextStepGimbalPosition_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for callNextStepGimbalPosition.h
 *
 * [EOF]
 */
