/*
 * si114x_algorithm.c
 *
 *  Created on: 2014/11/19
 *      Author: Nick
 */

#include "si114x_function.h"
#include "system.h"
#include "si114x_algorithm.h"
#include "Si114x_defs.h"


#define LED1I                 0xb
#define LED2I                 0xb
#define LED3I                 0xb
#define GESTURE_TASKLIST      0x7
#define GESTURE_IRQ           (IE_ALS_EVRYSAMPLE | IE_PS1_EVRYSAMPLE | IE_PS2_EVRYSAMPLE | IE_PS3_EVRYSAMPLE)
#define MEASRATE_FAST         320
#define MEASRATE_SLOW         32000
#define PS_THRESHOLD          270
static si114x_i2c_t si114x_i2c;
static si114x_i2c_t *si114x_handle = &si114x_i2c;


/**************************************************************************//**
 * @brief
  *  Enables or disables the Si1147 interrupt pin
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[in] enable
 *   Set to 1 to enable the interrupt output pin or set to 0 to disable the pin.
 * @return
 *   Returns error codes based on the I2CDRV.
 *****************************************************************************/
int Si114x_SetInterruptOutputEnable(alt_u8 addr, int enable)
{
  int retval;
  si114x_handle->addr = addr;
  si114x_handle->i2c  = I2C_OPENCORES_BASE;
  if (enable)
  {
	  retval = Si114xWriteToRegister(si114x_handle, REG_INT_CFG, 0x01);
  }
  else
  {
	  retval = Si114xWriteToRegister(si114x_handle, REG_INT_CFG, 0);
  }
  return retval;
}
/**************************************************************************//**
 * @brief
  *  Reads the PS measurement data from the Si1147.
 * @param[in] si114x_handle
 *   Contains i2c peripheral information. See definition of si114x_i2c_t.
 * @param[out] sample
 *   The sample data read from the sensor.
 *****************************************************************************/
static void readPSData(HANDLE si114x_handle, Si114x_Sample_TypeDef *sample)
{
	  /*read sample data from si114x */
	  sample->ps1  = Si114xReadFromRegister(si114x_handle, REG_PS1_DATA0);
	  sample->ps1 |= Si114xReadFromRegister(si114x_handle, REG_PS1_DATA1) << 8;
	  sample->ps2  = Si114xReadFromRegister(si114x_handle, REG_PS2_DATA0);
	  sample->ps2 |= Si114xReadFromRegister(si114x_handle, REG_PS2_DATA1) << 8;
	  sample->ps3  = Si114xReadFromRegister(si114x_handle, REG_PS3_DATA0);
	  sample->ps3 |= Si114xReadFromRegister(si114x_handle, REG_PS3_DATA1) << 8;
}




/**************************************************************************//**
 * @brief
  *  Implements the algorithm for detecting gestures on the sensor STK.
  *  Should be called with new sample data every time an interrupt is
  *  received.
 * @param[in] samples
 *   New sample data received from the sensor.
 * @return
 *   Returns the type of gesture detected (as defined by gesture_t).
 *****************************************************************************/
static gesture_t ProcessSi114xSamples(Si114x_Sample_TypeDef *samples)
{
	alt_u16        ps[3];

  static alt_u32 ps_entry_time[3] = { 0, 0, 0 };
  static alt_u32 ps_exit_time[3]  = { 0, 0, 0 };

  static alt_u8  ps_state[3] = { 0, 0, 0 };
  alt_u8 array_counter;
  alt_u32 diff_x ;
  alt_u32 diff_y1 ;
  alt_u32 diff_y2 ;
  alt_u32 ps_time[3] ;
  alt_u32 ps_avg;
  gesture_t  ret = NONE;  /*gesture result return value */
  /*save new samples into ps array */
  ps[2] = samples->ps1;
  ps[1] = samples->ps2;
  ps[0] = samples->ps3;
  /* Check state of all three measurements */
  for (array_counter = 0; array_counter < 3; array_counter++)
  {
    /* If measurement higher than the ps_threshold value, */
    /*   record the time of entry and change the state to look for the exit time */
    if (ps[array_counter] >= PS_THRESHOLD)
    {
      if (ps_state[array_counter] == 0)
      {
        ps_state[array_counter]      = 1;
        ps_entry_time[array_counter] = samples->timestamp;
      }
    }
    else
    {
      if (ps_state[array_counter] == 1)
      {
        ps_state[array_counter]     = 0;
        ps_exit_time[array_counter] = samples->timestamp;
      }
    }
  }

  /* If there is no object in front of the board, look at history to see if a gesture occured */
  if ((ps[0] < PS_THRESHOLD) && (ps[1] < PS_THRESHOLD) && (ps[2] < PS_THRESHOLD))
  {
    /* If the ps_max values are high enough and there exit entry and exit times, */
    /*   then begin processing gestures */
    if ((ps_entry_time[0] != 0) && (ps_entry_time[1] != 0) && (ps_entry_time[2] != 0)
        && (ps_exit_time[0] != 0) && (ps_exit_time[1] != 0) && (ps_exit_time[2] != 0))
    {
      /* Make sure no timestamps overflowed, indicated possibility if any of them are close to overflowing */
      if ((ps_exit_time[0] > 0xFC000000L) || (ps_exit_time[1] > 0xFC000000L) || (ps_exit_time[2] > 0xFC000000L)
          || (ps_entry_time[0] > 0xFC000000L) || (ps_entry_time[1] > 0xFC000000L) || (ps_entry_time[2] > 0xFC000000L))
      {         /* If any of them are close to overflowing, overflow them all so they all have the same reference */
        ps_exit_time[0] += 0x1FFFFFFFL;
        ps_exit_time[1] += 0x1FFFFFFFL;
        ps_exit_time[2] += 0x1FFFFFFFL;

        ps_entry_time[0] += 0x1FFFFFFFL;
        ps_entry_time[1] += 0x1FFFFFFFL;
        ps_entry_time[2] += 0x1FFFFFFFL;
      }

      /* Calculate the midpoint (between entry and exit times) of each waveform */
      /*  the order of these midpoints helps determine the gesture */
      ps_time[0] = (ps_exit_time[0] - ps_entry_time[0]) / 2;
      ps_time[0] = ps_time[0] + ps_entry_time[0];

      ps_time[1] = (ps_exit_time[1] - ps_entry_time[1]) / 2;
      ps_time[1] = ps_time[1] + ps_entry_time[1];

      ps_time[2] = (ps_exit_time[2] - ps_entry_time[2]) / 2;
      ps_time[2] = ps_time[2] + ps_entry_time[2];

      /* The diff_x and diff_y values help determine a gesture by comparing the */
      /*  LED measurements that are on a single axis */
      if (ps_time[1] > ps_time[2])
      {
        diff_x = ps_time[1] - ps_time[2];
      }
      else
      {
        diff_x = ps_time[2] - ps_time[1];
      }
      if( ps_time[0] > ps_time[1] )
      {
        diff_y1 = ps_time[0] - ps_time[1];
      }
	  else
      {
        diff_y1 = ps_time[1] - ps_time[0];
      }

      if( ps_time[0] > ps_time[2] )
      {
        diff_y2 = ps_time[0] - ps_time[2];
      }
	  else
      {
        diff_y2 = ps_time[2] - ps_time[0];
      }


      /* Take the average of all three midpoints to make a comparison point for each midpoint */
      ps_avg = (alt_u32) ps_time[0] + (alt_u32) ps_time[1] + (alt_u32) ps_time[2];
      ps_avg = ps_avg / 3;

      if ((ps_exit_time[0] - ps_entry_time[0]) > 10 || (ps_exit_time[1] - ps_entry_time[1]) > 10 || (ps_exit_time[2] - ps_entry_time[2]) > 10)
      {
        if( ( (ps_time[0] < ps_time[1]) &&  (diff_y1 > diff_x) ) || ( (ps_time[0] <= ps_time[2]) && (diff_y2 > diff_x) ) )
        {
          ret = UP;
        }
        else if  ( ( (ps_time[0] < ps_time[1]) &&  (diff_y1 > diff_x) ) || ( (ps_time[0] > ps_time[2]) && (diff_y2 > diff_x) ) )
        {
          ret = DOWN;
        }
        else if((ps_time[0] < ps_time[1]) && (ps_time[2] < ps_time[1]) && (diff_x > ((diff_y1+diff_y2)/2)))
        {
          ret = RIGHT;
        }
        else if( (ps_time[0] < ps_time[2]) && (ps_time[1] < ps_time[2])  && (diff_x > ((diff_y1+diff_y2)/2)))
        {
          ret = LEFT;
        }
      }
    }
    for (array_counter = 0; array_counter < 3; array_counter++)
    {
      ps_exit_time[array_counter]  = 0;
      ps_entry_time[array_counter] = 0;
    }
  }

  return ret;
}


/**************************************************************************//**
 * @brief
  *  Reads new measurement data and processes a new sample. This function
  *  should be called every time an interrupt for a new sample is received.
 * @param[in] addr
 *   The I2C address of the sensor.
 * @param[in] timestamp
 *   The timestamp for when the sample interrupt was received.
 * @return
 *   Returns the type of gesture detected (as defined by gesture_t).
 *****************************************************************************/
gesture_t Si114x_NewSample(alt_u8 addr, alt_u32 timestamp)
{
  Si114x_Sample_TypeDef sample;
  si114x_handle->addr = addr;
  si114x_handle->i2c  = I2C_OPENCORES_BASE;
  sample.timestamp    = timestamp;
  /*read sample data from si114x */
  readPSData(si114x_handle, &sample);
  /*clear irq*/
  Si114xWriteToRegister(si114x_handle, REG_IRQ_STATUS, Si114xReadFromRegister(si114x_handle, REG_IRQ_STATUS));
  /*look for gestures */
  return ProcessSi114xSamples(&sample);
}



int Si114x_ForceRead(alt_u8 addr)
{
	  Si114x_Sample_TypeDef sample;
	  si114x_handle->addr = addr;
	  si114x_handle->i2c  = I2C_OPENCORES_BASE;
	  Si114xPsAlsForce(si114x_handle);
	  readPSData(si114x_handle, &sample);
	  printf("PS1:%d,PS2:%d,PS3:%d\n",sample.ps1,sample.ps2,sample.ps3);
	  return 0;
}



int Si114x_ConfigureDetection(alt_u8 addr)
{
	signed short   retval = 0;
	si114x_handle->addr = addr;
	si114x_handle->i2c  = I2C_OPENCORES_BASE;

	/* Note that the Si114xReset() actually performs the following functions: */
	/*     1. Pauses all prior measurements */
	/*     2. Clear  i2c registers that need to be cleared */
	/*     3. Clears irq status to make sure INT* is negated */
	/*     4. Delays 10 ms */
	/*     5. Sends HW Key */
	retval += Si114xReset(si114x_handle);

	retval += Si114xWriteToRegister(si114x_handle, REG_PS_LED21, (LED1I << 4) + LED2I);
	retval += Si114xWriteToRegister(si114x_handle, REG_PS_LED3, LED3I);


    retval += Si114xParamSet(si114x_handle, PARAM_CH_LIST, GESTURE_TASKLIST);
    retval += Si114xWriteToRegister(si114x_handle, REG_IRQ_ENABLE, GESTURE_IRQ|0x20);

    /**/
    retval += Si114xWriteToRegister(si114x_handle, 0x05, 0x0);
    retval += Si114xWriteToRegister(si114x_handle, 0x06, 0x0);

    /**/
    retval += Si114xParamSet(si114x_handle, PARAM_PS_ADC_MISC, 0x24);  /* PS_ADC_MISC to high signal range */
    retval += Si114xParamSet(si114x_handle, PARAM_PS1_ADC_MUX, 0x00);  /* PS1_ADCMUX, PS2_ADCMUX, PS3_ADCMUX to small photodiode */
    retval += Si114xParamSet(si114x_handle, PARAM_PS2_ADC_MUX, 0x00);
    retval += Si114xParamSet(si114x_handle, PARAM_PS3_ADC_MUX, 0x00);

    /* Configure the ALS IR channel for the same settings as PS */
    retval += Si114xParamSet(si114x_handle, PARAM_ALSIR_ADC_MISC, RANGE_EN);
    retval += Si114xParamSet(si114x_handle, PARAM_ALSVIS_ADC_MISC, RANGE_EN);

    /* Set up how often the device wakes up to make measurements (10ms) */
    retval += Si114xWriteToRegister(si114x_handle, REG_MEAS_RATE, 0x84);
    retval += Si114xWriteToRegister(si114x_handle, REG_PS_RATE, 0x08);
    /* Enable Autonomous Operation */
    retval += Si114xPsAlsAuto(si114x_handle);

    /* If nothing went wrong after all of this time, the value */
    /* returned will be 0. Otherwise, it will be some negative */
    /* number */
    return retval;

}

int Si114x_ConfigureForceMode(alt_u8 addr)
{
	signed short   retval = 0;
	si114x_handle->addr = addr;
	si114x_handle->i2c  = I2C_OPENCORES_BASE;

	/* Note that the Si114xReset() actually performs the following functions: */
	/*     1. Pauses all prior measurements */
	/*     2. Clear  i2c registers that need to be cleared */
	/*     3. Clears irq status to make sure INT* is negated */
	/*     4. Delays 10 ms */
	/*     5. Sends HW Key */
	retval += Si114xReset(si114x_handle);
	retval += Si114xWriteToRegister(si114x_handle, REG_PS_LED21, (LED1I << 4) + LED2I);
	retval += Si114xWriteToRegister(si114x_handle, REG_PS_LED3, LED3I);
	retval += Si114xParamSet(si114x_handle, PARAM_CH_LIST, GESTURE_TASKLIST);
    /* If nothing went wrong after all of this time, the value */
    /* returned will be 0. Otherwise, it will be some negative */
    /* number */
    return retval;
}


