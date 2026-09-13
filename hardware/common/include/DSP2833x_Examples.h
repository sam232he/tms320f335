// TI File $Revision: /main/9 $
// Checkin $Date: July 2, 2008   14:31:12 $
//###########################################################################
//
// FILE:   DSP2833x_Examples.h
//
// TITLE:  DSP2833x Device Definitions.
//
//###########################################################################
// $TI Release: $
// $Release Date: $
// $Copyright:
// Copyright (C) 2009-2023 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

#ifndef DSP2833x_EXAMPLES_H
#define DSP2833x_EXAMPLES_H

#include "clock_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DSP28_DIVSEL
#define DSP28_DIVSEL     2
#endif

#ifndef DSP28_PLLCR
#define DSP28_PLLCR   10
#endif

#ifndef CPU_RATE
#define CPU_RATE    6.667L
#endif

#ifndef CPU_FRQ_100MHZ
#if DSP28_28332 || DSP28_28333
  #define CPU_FRQ_100MHZ  1
  #define CPU_FRQ_150MHZ  0
#else
  #define CPU_FRQ_100MHZ  0
  #define CPU_FRQ_150MHZ  1
#endif
#endif

//
// Include Example Header Files
//

//
// Prototypes for global functions within the .c files.
//
#include "DSP2833x_GlobalPrototypes.h"                                              
#include "DSP2833x_EPwm_defines.h"       // Macros used for PWM examples.
#include "DSP2833x_Dma_defines.h"        // Macros used for DMA examples.
#include "DSP2833x_I2c_defines.h"        // Macros used for I2C examples.

#define PARTNO_28335  0xEF
#define PARTNO_28334  0xEE
#define PARTNO_28333  0xEA
#define PARTNO_28332  0xED

//
// Include files not used with DSP/BIOS
//
#ifndef DSP28_BIOS
#include "DSP2833x_DefaultIsr.h"
#endif

//
// DO NOT MODIFY THIS LINE.
//
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / \
        (long double)CPU_RATE) - 9.0L) / 5.0L)

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_EXAMPLES_H definition

//
// End of file
//

