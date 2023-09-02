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
#include "I2C_core.h"
#include "AUDIO.h"
#include "AUDIO_REG.h"
//#include "system.h"


#ifdef DEBUG_AUDIO
    #define AUDIO_DEBUG(x)    DEBUG(x)
#else
    #define AUDIO_DEBUG(x)
#endif

/////////////////////////////////////////////////////////////////////////
///////////// Internal function prototype & const definition ////////////
/////////////////////////////////////////////////////////////////////////
bool aduio_wr_reg(alt_u8 reg_index, alt_u8 data8);



/////////////////////////////////////////////////////////////////////////
///////////// External function body ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
bool TLV320AIC3254_Init(alt_u32 controller_base){
	bool bSuccess;

	bSuccess = 	oc_i2c_init_ex(controller_base, 50*1000*1000, 400*1000); // ref clock:50MHz, i2c clk:400K

	return bSuccess;

}

bool AUDIO_Loopback(alt_u8 InputSource){
    bool bSuccess = TRUE;
    if (InputSource == LOOPBACK){
    	 AUDIO_DEBUG(("[AUDIO] AUDIO_Loopback_Configuration...\r\n"));
    	 //page0 reg configration
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x00, 0x00);  // Initialize to Page 0
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x01, 0x01);  // Initialize the device through software reset
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x04, 0x00);  //MCLK pin is CODEC_CLKIN
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x0b, 0x81);  //NDAC divider powered up & NDAC = 1
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x0c, 0x82);  //MDAC divider powered up & MDAC = 2
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x0d, 0x00);  //DOSR MSB DOSR=128
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x0e, 0x80);  //DOSR LSB DOSR=128
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x12, 0x81);  //NADC divider powered up & NADC=1
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x13, 0x82);  //MADC divider powered up & MADC=2
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x14, 0x80);  //AOSR = 128
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x1b, 0x0d);  //BCLK is output from the device & WCLK is output from the device & DOUT will be high impedance after data has been transferred
         if (bSuccess)
             bSuccess = aduio_wr_reg(0x1d, 0x17);  //Stereo ADC output is routed to Stereo DAC input&&Primary BCLK and Primary WCLK buffers are powered up when they are used in clock generation even when the codec is powered down & BDIV_CLKIN = ADC_MOD_CLK
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x1e, 0x90);  //BCLK N divider powered up & BCLK N divider = 128
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x3c, 0x01);  //DAC Signal Processing Block PRB_P1
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x3d, 0x01);  //ADC Singal Processing Block PRB_R1
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x3f, 0xd4);  //Left DAC Channel Powered Up & Right DAC Channel Powered Up & Left DAC data Left Channel Audio Interface Data & Right DAC data Right Channel Audio Interface Data
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x40, 0x00);  //Left+Righ DAC Channel not muted
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x41, 0xF0);  //Left DAC Channel Digital Volume Control=-7.5dB
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x42, 0xF0);  //Right DAC Channel Digital Volume Control=-7.5dB
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x51, 0xc2);  //Left+Right Channel ADC is powered up & ADC Volume Control Soft-Stepping disabled
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x52, 0x00);  //Right ADC Channel Un-muted

         //page1 reg configration
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x00, 0x01);  //page1
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x01, 0x08);  //Disabled weak connection of AVDD with DVDD
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x02, 0x01);  //Over Current detected for AVDD LDO
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x09, 0x0f);  //LOL+LOR is powered up & Left+Right Mixer Amplifier (MAL) is powered up
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x0a, 0x40);  //Full Chip Common Mode is 0.75V
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x0e, 0x08);  //Left Channel DAC reconstruction filter output is routed to LOL
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x0f, 0x08);  //Right Channel DAC reconstruction filter output is routed to LOR
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x12, 0x1A);  //LOL driver gain is 26dB
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x13, 0x1A);  //LOR driver gain is 26dB
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x18, 0x05);  //Mixer Amplifier Left Volume Control Volume Control = -2.3dB
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x19, 0x05);  //Mixer Amplifier Right Volume Control Volume Control = -2.3dB
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x34, 0x30);  //IN2L is routed to Left MICPGA with 40k resistance
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x36, 0x31);  //CM is routed to Left MICPGA via CM2L with 10k resistance
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x37, 0x30);  //IN2R is routed to Right MICPGA with 40k resistance
         if (bSuccess)
        	 bSuccess = aduio_wr_reg(0x39, 0x31);  //CM is routed to Right MICPGA via CM2R with 10k resistance
    }
     AUDIO_DEBUG(("[AUDIO] AUDIO_Loopback_Configuration %s\r\n", bSuccess?"success":"fail"));

    return bSuccess;
 }

bool AUDIO_Beep_Generator(alt_u8 InputSource){
   bool bSuccess = TRUE;
   if (InputSource ==BEEP ){
	   	   AUDIO_DEBUG(("[AUDIO] AUDIO_Beep_Configuration...\r\n"));
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x00, 0x00);  // Initialize to Page 0
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x01, 0x01);  //software reset
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x04, 0x00);  //MCLK PIN is CODEC_CLKIN
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x1B, 0x0D);  //BCLK is output from the device & WCLK is output from the device & DOUT will be high impedance after data has been transferred
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x0b, 0x81);  //NDAC divider power up / NDAC=1
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x0C, 0x82);  //MDAC divider power up / MDAC=2
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x0D, 0x00);  //DOSR MSB
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x0E, 0x80);  //DOSR LSB  / DOSR=128
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x1E, 0x90);  //BCLK N divider powered up & BCLK N divider = 128
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x3c, 0x19);  //Set the DAC Mode to PRB_P25
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x3f, 0xd4);  //Power up the Left and Right DAC
    //****************************************************************************
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x00, 0x01);  //page1
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x09, 0x0f);  //Power up LOL and LOR drivers
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x0e, 0x08);  //Left DAC----LOL
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x0f, 0x08);  //Right DAC---LOR
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x12, 0x08);  //LOL driver gain is 8dB
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x13, 0x08);  //LOR driver gain is 8dB
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x01, 0x08);  //Disabled weak connection of AVDD with DVDD
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x02, 0x01);  //Eabled Master Analog Power Control
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x7b, 0x01);   //Set the REF charding time to 40ms
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x0a, 0x40);  //Full Chip Common Mode is 0.75V
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x00, 0x00);  //page0
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x40, 0x00);  //Unmute the DAC digital volume control
	   	   //DRC setting
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x41, 0x00);  //Left DAC volume control  0.0db
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x42, 0x00);  //Right DAC volume control  0.0db
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x44, 0x7f);  //Enable DRC
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x45, 0x00);  //DRC Hold Disabled
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x46, 0xe2);  //
	   	   //beep setting   ~2KHz~
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x49, 0xff);  //beep reg3
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x4a, 0xff);  //beep reg4
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x4b, 0xff);  //beep reg5
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x4c, 0x21);  //beep reg6
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x4d, 0x21);  //beep reg7
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x4e, 0x7b);  //beep reg8
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x4f, 0xa3);  //beep reg9
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x48, 0x04);  //beep reg2
	   	   if (bSuccess)
	   		   bSuccess = aduio_wr_reg(0x47, 0x84);  //enable, beep generator
   }

    AUDIO_DEBUG(("[AUDIO] AUDIO_Beep_Configuration %s\r\n", bSuccess?"success":"fail"));

    return bSuccess;
}


bool aduio_wr_reg(alt_u8 reg_index, alt_u8 data8){

	bool bSuccess = TRUE;

	alt_u8 dataR;

    if (bSuccess)
        bSuccess =OC_I2C_Write(AUDIO_I2C_BASE, DEVICE_I2C_ADDR, reg_index, &data8, sizeof(data8));

    usleep(50*1000);

    if (bSuccess)
    	bSuccess = OC_I2C_Read(AUDIO_I2C_BASE, DEVICE_I2C_ADDR,reg_index, &dataR, sizeof(dataR));

	if (!bSuccess){
				printf("AUDIO: failed to read page register\r\n");
	}
	else{
		      if ((reg_index!=0x01)&&(reg_index!=0x02)&&(dataR !=data8))
		      {
    	        bSuccess = FALSE;
    	        AUDIO_DEBUG(("[AUDIO] read audio reg[%02d] = %04Xh\r\n", reg_index, dataR ));
              }
	     }

    return bSuccess;

}
