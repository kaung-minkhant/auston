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
//==Pointer NUM==
//-WRITE/READ-
#define P_Configuration 	0x00
#define P_Calibration   	0x05
#define P_Mask_Enable   	0x06
#define P_Alert_Limit   	0x07
//-READ-
#define P_Shunt_Voltage 	0x01
#define P_Bus_Voltage  		0x02
#define P_Power         	0x03
#define P_Current       	0x04
#define P_Die_ID       	    0xff
//Register Configuration
#define Configuration_Reg   0x4327  //AVG---4    conversion time---1.1ms   Mode-----Shunt and Bus, continuou
#define Calibration_Reg     0x42ab  //Current_LSB=100uA/bit    RSHUNT=3 milliohms   Calibration=17067=42abh

#define Monitor_I2C_ADRESS  0x80 // 1000_000 + Write bit 0
//Configure/Measure/Calculate
//P_Shunt_Voltage*2.5uv/1000=Shunt_Voltage(mV)
//P_Bus_Voltage*1.25mV/1000=Bus_Voltage(V)
//P_Current*0.1mA/1000=Current(A)
//P_Power*0.1mA*25/1000=Power


int main()
{
   bool bPass ;
   alt_u16 data;
   float Shunt_Voltage,Bus_Voltage,Current,Power;
   printf("==Power Monitor Test ==\r\n");
   //Configuration Register
   bPass = I2C_Write16(PMONITOR_I2C_SCL_BASE,PMONITOR_I2C_SDA_BASE,Monitor_I2C_ADRESS,P_Configuration,Configuration_Reg);
   if(bPass){
	         bPass = I2C_Write16(PMONITOR_I2C_SCL_BASE,PMONITOR_I2C_SDA_BASE,Monitor_I2C_ADRESS,P_Calibration,Calibration_Reg);
	         if(bPass){
	          		 printf("Configuration ok! \n");
	         }else{
	          		 printf("Configuration failed!  \n");
	         }
   }else{
    		 printf("Configuration failed!  \n");
   }
   usleep(2000);
   while(1){
	    printf("==== Power Monitor Test ====\r\n");
        // Read Shunt_Voltage
    	bPass = I2C_Read16(PMONITOR_I2C_SCL_BASE,PMONITOR_I2C_SDA_BASE,Monitor_I2C_ADRESS,P_Shunt_Voltage,&data);
    	if(bPass){
    		      Shunt_Voltage = (float)data*2.5/1000;
    		      printf("Shunt_Voltage = %.3f mV \n",Shunt_Voltage);
    	}else{
   	              printf("Read Shunt_Voltage failed!\n");
    	}
    	// Read Bus_Voltage
    	bPass = I2C_Read16(PMONITOR_I2C_SCL_BASE,PMONITOR_I2C_SDA_BASE,Monitor_I2C_ADRESS,P_Bus_Voltage,&data);
    	if(bPass){
        	      Bus_Voltage = (float)data*1.25/1000;
        	      printf("Bus_Voltage   = %.3f V \n",Bus_Voltage);
    	}else{
    	      	  printf("Read Bus_Voltage failed!\n");
        }
    	//Read Current
    	bPass = I2C_Read16(PMONITOR_I2C_SCL_BASE,PMONITOR_I2C_SDA_BASE,Monitor_I2C_ADRESS,P_Current,&data);
    	if(bPass){
    	   		  Current = (float)data*0.1/1000;
    	   		  printf("Current       = %.3f A \n",Current);
    	}else{
    	      	  printf("Read Current failed!\n");
        }
    	//Read Power
    	bPass = I2C_Read16(PMONITOR_I2C_SCL_BASE,PMONITOR_I2C_SDA_BASE,Monitor_I2C_ADRESS,P_Power,&data);
    	if(bPass){
        	      Power = (float)data*2.5/1000;
        	      printf("Power         = %.3f W \n",Power);
    	}else{
    	      	  printf("Read Power failed!\n");
        }
        usleep(1000*2000);
    }

 }



