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
#include "my_includes.h"
#include "AUDIO.h"
#include  "LED.h"
#include <math.h>

#ifdef DEBUG_APP
    #define APP_DEBUG(x)    DEBUG(x)
#else
    #define APP_DEBUG(x)
#endif

typedef enum{
	LOOPBACK_PLAY,
    BEEP_PLAY
}AUDIO_FUNC;
///////////////////////////////////////////////////////////////////////////////
//////////// Internal function prototype & data structure /////////////////////
///////////////////////////////////////////////////////////////////////////////

bool init(void){
    bool bSuccess = TRUE;
    printf("Welcome Audio Demo\n");
    bSuccess = oc_i2c_init_ex(AUDIO_I2C_BASE, 50*1000*1000, 400*1000); // ref clock:50MHz, i2c clk:400K
    if (!bSuccess)
    		printf("AUDIO: failed to init i2c clock\r\n");
     return bSuccess;

}


const char szMenu[][128] = {
    "======== Terasic Audio Demo ==========\n",
    " operation guide:\n",
    " SW0:  Audio Source Selection. DOWN-->BEEP, UP-->LINE-IN\n",
    " Sample Rate---->192K\n",
    " Beep----------->2KHz\n",
    "\n\n"
};

void show_menu(void){
    int i;
    for(i=0;i<sizeof(szMenu)/sizeof(szMenu[0]);i++)
        printf(szMenu[i]);
}


bool init_audio(AUDIO_FUNC audio_func){
    bool bSuccess = TRUE;
   if (audio_func == LOOPBACK_PLAY){
	   AUDIO_Loopback(LOOPBACK);
    }else if (audio_func == BEEP_PLAY){
       AUDIO_Beep_Generator(BEEP);
    }
    return bSuccess;
}


    
int main()
{
    int state=0;
    bool loopback_done=0;
    bool beep_done=0;
	show_menu();
    IOWR(AUDIO_RESET_N_BASE, 0, 0x01);
    IOWR(AUDIO_RESET_N_BASE, 0, 0x00);
    usleep(1000);
    IOWR(AUDIO_RESET_N_BASE, 0, 0x01);
    usleep(1000);
    init();
    while(1)
    {
    	state=IORD_ALTERA_AVALON_PIO_DATA(SW_BASE)&0x01;
  	    usleep(1000);
    	if((state==1)&&(loopback_done==0))
    	{
    		printf("===LOOPBACK_PLAY Mode===\n");
    	    init_audio(LOOPBACK_PLAY);
    	    loopback_done=1;
    	    beep_done=0;
    	}
    	else if((state==0)&&(beep_done==0))
    	{
    		printf("===  BEEP_PLAY   Mode===\n");
    	    init_audio(BEEP_PLAY);
    	    beep_done=1;
    	    loopback_done=0;
    	}
    }


}
