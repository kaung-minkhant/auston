// --------------------------------------------------------------------
// Copyright (c) 2014 by Terasic Technologies Inc.
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

#include "terasic_includes.h"
#include "I2C.h"

#define R_TEMPERATURE      0x00
#define R_HUMIDITY         0x01
#define R_CONFIGURATION    0x02
#define R_MANUFACTURER_ID  0xFE
#define R_DEVICE_ID        0xFF

#define HDC1000_I2C_ADRESS  0x80 // 1000_000 + Write bit 0

bool Read_Configuration(void);
bool Write_Configuration(alt_u8 acquisition_mode , alt_u8 temp_resolution,alt_u8 humidity_resolution);


int main()
{
   bool bPass ;
   alt_u16 data;
   double result;
   alt_u8 rh_temp[4] ;

   printf("==HDC1000 Humidity/Temperature Test ==\r\n");

   bPass = I2C_Read16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_MANUFACTURER_ID,&data);
   if(bPass){
	   printf("Manufacturer ID is %x\r\n",data);// 0x5449 expected
   }else{
	   printf("I2C MultipleRead Failed!\r\n");
   }

   bPass = I2C_Read16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_DEVICE_ID,&data);
   if(bPass){
	   printf("Device ID is %x\r\n",data); // 0x1000 expected
   }else{
	   printf("I2C Read16 Failed!\r\n");
   }

   bPass =  Read_Configuration();
   if(!bPass)   printf("I2C Read_Configuration Failed!\r\n");

   usleep(2000*1000);

    while(1){

    	   if(Write_Configuration(0,0,0)){
				//Read_Configuration(); // printf configuration
			}else{
				printf("[Note] Set Configuration Failed!\r\n");
			}

    	   // In Read action, a Conversion time delay had been added,if not ,please monitor DRDY_n signal.
    	   //  DRDY_n  <<<<-
    	   // delay time :  14 bit RHct = 6.5 ms ,14bit TEMPct = 6.35 ms
    	   // so add > 6.5+6.35 -> set 15 ms delay in i2c ready


        // Temperature
    	   bPass = I2C_Read16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_TEMPERATURE,&data);
    	   if(bPass){
    		   result = (double)data/397.188 -40.0;
    		   //  data/2^16 * 165 - 40
    	        printf("[Separate] Temperature: %.2f*C \n",result);
    	   }else{
   	            printf("[Separate] Read Temperature failed!  \n");
    	   }
    	// Humidity
    	   bPass = I2C_Read16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_HUMIDITY,&data);
    	   if(bPass){
    		   result = (double)((double)data/65536)*100;
    		   //  data/2^16 * 165 - 40
    	        printf("[Separate] Relative Humidity: %.2f%% \n",result);
    	   }else{
   	            printf("[Separate] Read Relative Humidity failed!  \n");
    	   }

    	// together
    	   if(Write_Configuration(1,0,0)){
				//Read_Configuration(); // printf configuration
			}else{
				printf("[Note] Set Configuration Failed!\r\n");
			}

    	   bPass =  I2C_MultipleRead(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_TEMPERATURE,rh_temp,4);
    	   if(bPass){

    		   data = (rh_temp[0] << 8) | rh_temp[1] ;
    		   result = (double)data/397.188 -40.0;
    		   //  data/2^16 * 165 - 40
    	       printf("[Together] Temperature: %.2f*C \n",result);

    		   data = (rh_temp[2] << 8) | rh_temp[3] ;
    		   result = (double)((double)data/65536)*100;
    		   //  data/2^16 * 165 - 40
    	        printf("[Together] Relative Humidity: %.2f%% \n",result);

    	   }else{
   	            printf("[Together] Read Temperature and Relative Humidity failed! \n");
    	   }

        usleep(1000*1000);
    }

 }


bool Write_Configuration(alt_u8 acquisition_mode , alt_u8 temp_resolution,alt_u8 humidity_resolution)
{
	bool bPass;
	alt_u16 data;

	data = ((acquisition_mode&0x0001) <<12) + ((temp_resolution&0x0001)<<10) + ((humidity_resolution&0x0003)<<8);

//	printf("Write Configuration Data = 0x%x \r\n",data);

	bPass = I2C_Write16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_CONFIGURATION,data);

	return bPass;
}

bool Read_Configuration(void)
{
	bool bPass;
	alt_u16 data;

	bPass = I2C_Read16(RH_TEMP_I2C_SCL_BASE,RH_TEMP_I2C_SDA_BASE,HDC1000_I2C_ADRESS,R_CONFIGURATION,&data);

	if(bPass){

		printf("Read Configuration Data = 0x%x \r\n",data);

		if(((data>>12)&0x0001) == 0) printf("  Acquisition mode:Temperature or Humidity is acquired.\r\n ");
		else printf("  Acquisition mode:Temperature and Humidity are acquired in sequence,Temperature first.\r\n ");

		if(((data>>10)&0x0001) == 0) printf("  Temperature Measurement Resolution: 14 bit.\r\n ");
		else  printf("  Temperature Measurement Resolution: 11 bit.\r\n ");

		if(((data>> 8)&0x0003) == 0) printf("  Humidity Measurement Resolution: 14 bit.\r\n ");
		else if(((data>> 8)&0x0003) == 1) printf("  Humidity Measurement Resolution: 11 bit.\r\n ");
		else  printf("  Humidity Measurement Resolution: 8 bit.\r\n ");

	}

	return bPass;
}
