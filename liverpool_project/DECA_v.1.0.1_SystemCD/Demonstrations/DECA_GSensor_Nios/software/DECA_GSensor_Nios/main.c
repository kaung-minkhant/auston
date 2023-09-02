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
#include "I2C_core.h"
#include "debug.h"
//-WRITE/READ-
#define TEMP_CFG_REG 	0x1F
#define CTRL_REG1   	0x20
#define CTRL_REG4   	0x23
#define STATUS_REG      0x27
//-READ-
#define OUT_TEMP_L 	    0x0C
#define OUT_TEMP_H 	    0x0D
#define OUT_X_L 	    0x28
#define OUT_X_H  	    0x29
#define OUT_Y_L         0x2A
#define OUT_Y_H         0x2B
#define OUT_Z_L         0x2C
#define OUT_Z_H         0x2D

//Register Configuration
#define TEMP_CFG_REG_value   0xC0    // Temperature sensor (T) enable
#define CTRL_REG1_value      0x97    // X/Y/Z-axis enable,HR mode (1.344 kHz);High-resolution mode(12-bit data output),12mg/digit
#define CTRL_REG4_value      0xB8    // output registers not updated until MSB and LSB have been read,data LSb at lower address,+-16g,High resolution mode,

#define Gsensor_I2C_ADRESS   0x32    // 0011_00 + SA0 + bit 0----write(32h),read(33h)
#define LIS2DH12_DEBUG(x)    DEBUG(x)



bool LIS2DH12_wr_reg(alt_u8 reg_index, alt_u8 data8){
	bool bSuccess = TRUE;
	alt_u8 dataR;
    if (bSuccess){
    	bSuccess = OC_I2C_Write(I2C_OPENCORES_0_BASE,Gsensor_I2C_ADRESS,reg_index,&data8, 1);}
    usleep(50*1000);
    if (bSuccess)
    	bSuccess = OC_I2C_Read(I2C_OPENCORES_0_BASE, Gsensor_I2C_ADRESS,reg_index, &dataR, 1);
	if (!bSuccess){
				printf("LIS2DH12: failed to read page register\r\n");
	}
	else{
		      if (dataR !=data8)
		      {
		    	bSuccess = FALSE;
		    	LIS2DH12_DEBUG(("Read LIS2DH12 reg[%02xh] = %02Xh\r\n", reg_index, dataR ));

              }
	     }

    return bSuccess;

}

bool LIS2DH12_Init(void){
	 bool  bSuccess= TRUE;
	 LIS2DH12_DEBUG(("Gsensor_Configuration...\r\n"));
	 bSuccess = oc_i2c_init_ex(I2C_OPENCORES_0_BASE, 50*1000*1000, 400*1000); // ref clock:50MHz, i2c clk:400K
	 if (!bSuccess)
	     printf("failed to init i2c clock\r\n");
	 if (bSuccess)
		 bSuccess= LIS2DH12_wr_reg(CTRL_REG1,CTRL_REG1_value);
	 if (bSuccess)
		 bSuccess = LIS2DH12_wr_reg(TEMP_CFG_REG,TEMP_CFG_REG_value);
	 if(bSuccess)
		 bSuccess =  LIS2DH12_wr_reg(CTRL_REG4,CTRL_REG4_value);

	 LIS2DH12_DEBUG(("Gsensor_Configuration  %s\r\n", bSuccess?"success":"fail"));

	 return bSuccess;
}


int main()
{
   bool     bPass,bSuccess;
   alt_16   data_x,data_y,data_z,temp_data;
   alt_u8   temp_reg[2],data[6],status;
   alt_u16  nIndex=1;
   alt_16   x, y, z;
   int      i,j;
   float    temp;

   //Configuration Register
   printf("== Gsensor Demo ==\r\n");
   bSuccess=LIS2DH12_Init();
   usleep(2000);
   while(bSuccess){
	    bPass = OC_I2C_Read(I2C_OPENCORES_0_BASE, Gsensor_I2C_ADRESS, STATUS_REG, &status, sizeof(status));
	    if (bPass){
	    	if (!(status & 0x04))
		    	continue;
	    }else{
	    	continue;
	    }
	   // Read Temperature
	    for(j=0; j<2;j++)
    	bPass = OC_I2C_Read(I2C_OPENCORES_0_BASE, Gsensor_I2C_ADRESS,OUT_TEMP_L+j, &temp_reg[j], 1);
    	if(bPass){
		      temp_data = ((temp_reg[1]<<8)|temp_reg[0]);
	          temp = (float)(temp_data/256.0);
	          printf("[%d]Temperature: %.2f *C \n",nIndex, temp);
    	}else{
    	   	      printf("Read Temperature failed!\n");}

        // Read X/Y/Z-axis
    	for(i=0; i<6;i++)
    		bPass = OC_I2C_Read(I2C_OPENCORES_0_BASE, Gsensor_I2C_ADRESS,OUT_X_L+i,  &data[i], 1);
    	if(bPass){
		      data_x = ((data[1]<<8)|data[0]);
		      data_y = ((data[3]<<8)|data[2]);
		      data_z = ((data[5]<<8)|data[4]);
	          x = (float)(data_x/16.0)*12.0;
	          y = (float)(data_y/16.0)*12.0;
	          z = (float)(data_z/16.0)*12.0;
		      printf("   X=%dmg, Y=%dmg, Z=%dmg \r\n", x, y, z);
    	}else{
   	              printf("Read Gsensor failed!\n");}

        usleep(1000*2000);
        nIndex++;
    }

 }



