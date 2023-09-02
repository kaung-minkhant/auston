// --------------------------------------------------------------------
// Copyright (c) 2013 by Terasic Technologies Inc.
// --------------------------------------------------------------------
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     Terasic Technologies Inc
//                     356 Fu-Shin E. Rd Sec. 1. JhuBei City,
//                     HsinChu County, Taiwan
//                     302
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// --------------------------------------------------------------------

#ifndef AUDIO_H_
#define AUDIO_H_



typedef enum{
	LOOPBACK=0,
    BEEP
}INPUT_SOURCE;


bool AUDIO_Init(void);

// config 



// play & record
bool AUDIO_DacFifoNotFull(void);
void AUDIO_DacFifoSetData(short ch_left, short ch_right);
bool AUDIO_AdcFifoNotEmpty(void);
void AUDIO_AdcFifoGetData(short *pch_left, short *pch_right);
void AUDIO_FifoClear(void);







#endif /*AUDIO_H_*/
