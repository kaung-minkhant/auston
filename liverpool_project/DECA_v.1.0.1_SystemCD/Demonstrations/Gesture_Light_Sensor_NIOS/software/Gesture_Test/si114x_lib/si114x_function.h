/*
 * si114x_function.h
 *
 *  Created on: 2014/11/19
 *      Author: Nick
 */

//-----------------------------------------------------------------------------
// Si114x_functions.h
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:
//
// Function Prototypes, register and bit definitions for reusable low-level
// Si114x functions.
//
// Target:         Si114x
// Command Line:   None
//
//-----------------------------------------------------------------------------

#ifndef SI114X_FUNCTIONS_H
#define SI114X_FUNCTIONS_H


#ifndef PGM_TOOLKIT_TYPES
#define PGM_TOOLKIT_TYPES

#include "terasic_includes.h"

typedef void *            HANDLE;

typedef struct {
    alt_u32 i2c;
    alt_u8 addr;
  } si114x_i2c_t;

typedef struct{
	alt_u32 OS_I2C_BASE;
	alt_u32 ADDR;
}I2C_TypeDef;




//
// Function Prototypes
//
alt_u8 Si114xParamSet(HANDLE si114x_handle, alt_u8 address, alt_u8 data);
alt_u8 Si114xParamSet(HANDLE si114x_handle, alt_u8 address, alt_u8 data);
alt_u8 Si114xReset(HANDLE si114x_handle);
alt_u8 Si114xPauseAll(HANDLE si114x_handle);
static alt_u8 _sendCmd(HANDLE si114x_handle, alt_u8 command);
static alt_u8 _PsAlsPause (HANDLE si114x_handle);
static alt_u8 _waitUntilSleep(HANDLE si114x_handle);
alt_u8 Si114xWriteToRegister(HANDLE si114x_handle, alt_u8 address, alt_u8 data);
int Si114x_ConfigureForceMode(alt_u8 addr);

// Commands to the Si114x
alt_u8 Si114xReset      (HANDLE si114x_handle);
alt_u8 Si114xPauseAll   (HANDLE si114x_handle);
alt_u8 Si114xNop        (HANDLE si114x_handle);
alt_u8 Si114xPsForce    (HANDLE si114x_handle);
alt_u8 Si114xAlsForce   (HANDLE si114x_handle);
alt_u8 Si114xPsAlsForce (HANDLE si114x_handle);
alt_u8 Si114xPsAlsAuto  (HANDLE si114x_handle);

// Read/Write Access to Parameter RAM is implemented as a 'Command' to the
// Si114x

#endif /* SI114X_FUNCTION_H_ */

#endif // #define SI114X_FUNCTIONS_H



