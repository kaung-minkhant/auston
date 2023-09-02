/*
 * si114x_algorithm.h
 *
 *  Created on: 2014/11/19
 *      Author: Nick
 */

#ifndef SI114X_ALGORITHM_H_
#define SI114X_ALGORITHM_H_


#define LED1I                 0xb
#define LED2I                 0xb
#define LED3I                 0xb
#define UV_TASKLIST           0x80
#define GESTURE_TASKLIST      0x7
#define HOVER_TASKLIST        0x1
#define UV_IRQ_EN             IE_ALS_EVRYSAMPLE
#define GESTURE_IRQ           (IE_ALS_EVRYSAMPLE | IE_PS1_EVRYSAMPLE | IE_PS2_EVRYSAMPLE | IE_PS3_EVRYSAMPLE)
#define MEASRATE_FAST         320
#define MEASRATE_SLOW         32000
#define PS_HOVER_THRESHOLD    295
#define PS_THRESHOLD          275
#define SI1147_DEVICE_ID      0x47

/*
   The Si114x PGM toolkit functions uses a void* to pass hardware parameters
   through to the lower level i2c functions. The struct below is passed down
   to the low level i2c functions using that pointer.
*/


/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
/** Si114x gestures */
typedef enum
{
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  TAP
} gesture_t;
/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/** Interrupt Sample */
typedef struct
{
  alt_u32 timestamp;         /* Timestamp to record */
  alt_u16 vis;               /* VIS */
  alt_u16 ir;                /* IR */
  alt_u16 ps1;               /* PS1 */
  alt_u16 ps2;               /* PS2 */
  alt_u16 ps3;               /* PS3 */
  alt_u16 aux;               /* AUX */
} Si114x_Sample_TypeDef;

int Si114x_SetInterruptOutputEnable(alt_u8 addr, int enable);
static gesture_t ProcessSi1147Samples(Si114x_Sample_TypeDef *samples);
gesture_t Si114x_NewSample(alt_u8 addr, alt_u32 timestamp);
int Si114x_ConfigureDetection(alt_u8 addr);
int Si114x_ForceRead(alt_u8 addr);



#endif /* SI114X_ALGORITHM_H_ */
