/*
 * si114x_function.c
 *
 *  Created on: 2014/11/19
 *      Author: Nick
 */

#include "si114x_function.h"
#include "Si114x_defs.h"

alt_u8 Si114xWriteToRegister(HANDLE si114x_handle, alt_u8 address, alt_u8 data)
{
   OC_I2C_Write(((si114x_i2c_t*)si114x_handle)->i2c,((si114x_i2c_t*)si114x_handle)->addr,address,data);
   return 0;
}
alt_u8 Si114xReadFromRegister(HANDLE si114x_handle, alt_u8 address)
{
   alt_u8 data;
   OC_I2C_Read(((si114x_i2c_t*)si114x_handle)->i2c,((si114x_i2c_t*)si114x_handle)->addr,address,&data,1);
   return data;
}

//-----------------------------------------------------------------------------
// Si114xParamSet writes to the PARAM_WR and CMD register
//
alt_u8 Si114xParamSet(HANDLE si114x_handle, alt_u8 address, alt_u8 data)
{
	alt_u8     retval;
	alt_u8 Temp = address | 0xa0;
    if((retval = _waitUntilSleep(si114x_handle))!=0) return retval;
    Si114xWriteToRegister(si114x_handle,REG_PARAM_WR,data);
    Si114xWriteToRegister(si114x_handle,REG_COMMAND,Temp);
    while(1)
    {
        retval=Si114xReadFromRegister(si114x_handle, REG_PARAM_RD);
        if (retval==data) break;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// The following functions writes to the CMD register and can therefore
// wake up the internal MCU

alt_u8 Si114xReset(HANDLE si114x_handle)
{
    alt_u8 retval = 0;

    retval+=Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE,  0x00);
    retval+=Si114xWriteToRegister(si114x_handle, REG_ALS_RATE, 0x0);
    retval+=Si114xPauseAll(si114x_handle);
    // The clearing of the registers could be redundant, but it is okay.
    // This is to make sure that these registers are cleared.
    retval+=Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE,  0x00);
    retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_ENABLE, 0x00);
    retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_MODE1,  0x00);
    retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_MODE2,  0x00);
    retval+=Si114xWriteToRegister(si114x_handle, REG_INT_CFG  ,  0x00);
    retval+=Si114xWriteToRegister(si114x_handle, REG_IRQ_STATUS, 0xFF);

    // Perform the Reset Command
    retval+=Si114xWriteToRegister(si114x_handle, REG_COMMAND, 1);

    // Delay for 10 ms. This delay is needed to allow the Si114x
    // to perform internal reset sequence.
    usleep(10000);

    // Write Hardware Key
    retval+=Si114xWriteToRegister(si114x_handle, REG_HW_KEY, HW_KEY_VAL0);

    return retval;
}

alt_u8 Si114xPauseAll(HANDLE si114x_handle)
{
	alt_u8 data;
    while (1)
    {
        // Keep sending nops until the response is zero
        while (1)
        {
            if ((Si114xReadFromRegister(si114x_handle, REG_RESPONSE))==0)
                break;
            else
                Si114xNop(si114x_handle);
        }

        // Pause the device
        _PsAlsPause(si114x_handle);

        // Wait for response
        while(1)
        {
            if ((Si114xReadFromRegister(si114x_handle, REG_RESPONSE))!=0)
                break;
        }

        // When the PsAlsPause() response is good, we expect it to be a '1'.
        if ((Si114xReadFromRegister(si114x_handle, REG_RESPONSE))==1)
            break;  // otherwise, start over.
    }
    return 0;
}


alt_u8 Si114xNop       (HANDLE si114x_handle) {return _sendCmd(si114x_handle,0x00);}
alt_u8 Si114xPsForce   (HANDLE si114x_handle) {return _sendCmd(si114x_handle,0x05);}
alt_u8 Si114xAlsForce  (HANDLE si114x_handle) {return _sendCmd(si114x_handle,0x06);}
alt_u8 Si114xPsAlsForce(HANDLE si114x_handle) {return _sendCmd(si114x_handle,0x07);}
alt_u8 Si114xPsAlsAuto (HANDLE si114x_handle) {return _sendCmd(si114x_handle,0x0F);}

static alt_u8 _sendCmd(HANDLE si114x_handle, alt_u8 command)
{
	alt_u8  response;
    alt_u8  retval;
    // Get the response register contents
    if ((response=Si114xReadFromRegister(si114x_handle, REG_RESPONSE))<0)
        return response;


    // Double-check the response register is consistent
    while(1)
    {
        if((retval=_waitUntilSleep(si114x_handle)) != 0) return retval;

        if(command==0) break; // Skip if the command is NOP

        retval=Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
        if(retval==response) break;
        else if(retval<0) return retval;
        else response = retval;
    }

    // Send the Command
    if ((retval=Si114xWriteToRegister(si114x_handle, REG_COMMAND, command)) !=0)
        return retval;
    // Expect a change in the response register
    while(1)
    {
        if(command==0) break; // Skip if the command is NOP

        retval= Si114xReadFromRegister(si114x_handle, REG_RESPONSE);
        if (retval != response) break;
        else if(retval<0) return retval;
    }
    return 0;
}
//-----------------------------------------------------------------------------
// use this to pause measurements
static alt_u8 _PsAlsPause (HANDLE si114x_handle)
{
    return _sendCmd(si114x_handle,0x0B);
}

//-----------------------------------------------------------------------------
// Following functions reads/writes i2c registers that will not wake up
// the Si114x's internal MCU
//
static alt_u8 _waitUntilSleep(HANDLE si114x_handle)
{

	alt_u8 retval;
    // This loops until the Si114x is known to be in its sleep state
    // or if an i2c error occurs
    while (1)
    {
        retval = Si114xReadFromRegister(si114x_handle, REG_CHIP_STAT);
        if (retval == 1) break;
        if (retval <  0) return retval;
    }
    return 0;
}
