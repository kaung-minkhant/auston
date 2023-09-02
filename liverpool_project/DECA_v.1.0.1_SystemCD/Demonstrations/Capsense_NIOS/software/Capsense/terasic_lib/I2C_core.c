// --------------------------------------------------------------------
// Copyright (c) 2007 by Terasic Technologies Inc. 
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
//#include "I2C.h"
#include "io.h"
// Note. Remember to reset device befroe acceess I2C interface
#ifdef DEBUG_I2C
    #define I2C_DEBUG(x)    DEBUG(x)  
#else
    #define I2C_DEBUG(x)
#endif

#define SLEEP_TIME 15  //us
#define TRUE 1
#define FALSE 0

bool Write32_Data(alt_32 base_address, alt_32 offset_address, alt_32 Data){
	bool bPass;
    IOWR(base_address, offset_address,Data);
    bPass=TRUE;
    return bPass;
}



bool  Read32_Data(alt_32 base_address, alt_32 offset_address, alt_32 *pData32){
	bool bPass;
	*pData32 =IORD(base_address ,offset_address  );
	bPass=TRUE;
    return bPass;
}

bool oc_i2c_init(alt_32 i2c_base)
{
	bool bSuccess=TRUE;
	alt_u32 read_data;
    // I2c sysclock =50M hz
	// i2c scl max 400k
	//scl =sysclock/(prescale*5)
	//add
	IOWR(i2c_base,2,0);
	IOWR(i2c_base,4,0x1);
	//add
   IOWR(i2c_base, 0,0x18);//write low prescale
   IOWR(i2c_base, 1,0x00);//write hi prescale
   //add



   //enable the I2C core, but disable the IRQ
   IOWR( i2c_base, 2,0x80);

    read_data =IORD(i2c_base, 0);
    if( (read_data & 0x00ff) != 0x18)
    {
    	bSuccess = FALSE;
       	read_data =IORD(i2c_base, 1);
		if((read_data & 0x00ff) != 0x00){
			bSuccess = FALSE;
		}
    }
    else if (bSuccess){
    	read_data =IORD(i2c_base, 2);
        if( (read_data & 0x00ff) != 0x80 ){
            bSuccess = FALSE;
        }
    }

   if (bSuccess)
       printf("\nI2C core is enabled! \r\n");
   else
       printf("\nI2C core is not enabled! \r\n");

   return bSuccess;

}

bool  ACK_single_check(alt_32 i2c_base)
{
	bool bSuccess=TRUE;
	alt_32 read_data;

    usleep(SLEEP_TIME);
    read_data =IORD(i2c_base, 4);
    if(read_data & 0x02)
            bSuccess = FALSE;
     if (bSuccess){
            usleep(SLEEP_TIME);
            read_data =IORD(i2c_base, 4);
                if(read_data & 0x80)
                    bSuccess = FALSE;
     }
    return bSuccess;
}


bool  ACK_check(alt_32 i2c_base){
    bool bSuccess = FALSE;
    int i=0;

    while(!bSuccess && i++ < 10){
        bSuccess = ACK_single_check(i2c_base);
    }

    return bSuccess;
}

bool  OC_I2C_Write(alt_32 i2c_base,alt_u8 device_address,alt_u8 sub_address, alt_u8 pData)
{
    alt_32 Data32;
    alt_u32 Timeout;
    bool bDone = FALSE;
    Timeout = alt_nticks() + alt_ticks_per_second()/2;
  while(!bDone)
  {
	//DWORD reg_data = 0x0;
   //set the tx reg audio chip dev address with write bit
   if (!Write32_Data( i2c_base, 3,device_address)){
       printf("OC_I2C_Write error[0]\r\n");
       return TRUE;
   }
   //set STA and WR bits(bit7 and bit4)
   if (!Write32_Data( i2c_base, 4,0x90)){
       printf("OC_I2C_Write error[1]\r\n");
       return TRUE;
   }
   usleep(SLEEP_TIME);
   //set the txr reg data with reg address + 1 data MSB
   if (!Write32_Data( i2c_base, 3,sub_address)){
       printf("OC_I2C_Write error[3]\r\n");
       return TRUE;
   }

   //set WR bits(bit4)
    if (!Write32_Data( i2c_base, 4,0x10)){
        printf("OC_I2C_Write error[4]\r\n");
        return TRUE;
    }

   //wait TIP bit go to 0 to end Tx
    if (ACK_check( i2c_base))
  	  bDone = TRUE;
    if(alt_nticks() > Timeout)
  	  bDone = TRUE;
  }
#if 1

    //set the txr reg data with the data
      if (!Write32_Data( i2c_base, 3,pData)){
          printf("OC_I2C_Write error[6]\r\n");
          return TRUE;
      }

     //set STO and WR bits(bit7 and bit4)
     if (!Write32_Data( i2c_base, 4,0x10)){
         printf("OC_I2C_Write error[7]\r\n");
          return TRUE;
     }

     //wait TIP bit go to 0 to end Tx
      if(!ACK_check( i2c_base)){
          printf("OC_I2C_Write error[8]\r\n");
          return TRUE;
      }


#else
  //set the txr reg data with the data
    if (!Write32_Data( i2c_base, 3,data&0xff))
        return false;

   //set STO and WR bits(bit7 and bit4)
    if (!Write32_Data( i2c_base, 4,0x10))
        return false;

   //wait TIP bit go to 0 to end Tx
    if(!ACK_check( i2c_base))
        return false;
#endif

     if (!Write32_Data( i2c_base, 4,0x40)){
         printf("OC_I2C_Write error[9]\r\n");
         return TRUE;
     }
     usleep(SLEEP_TIME);

     return FALSE;

}

bool ACK_judge_for_read(alt_32 i2c_base)
{
	bool bSuccess;
    alt_32 this_data;
    usleep(SLEEP_TIME);
    bSuccess =  Read32_Data ( i2c_base, 4, &this_data);
    if (bSuccess){
        if(this_data & 0x02)
            bSuccess = FALSE;
    }
   //wait the rx ACK signal 0-valid

    return bSuccess;

}


bool  OC_I2C_Read(alt_32 i2c_base,alt_u8 device_address,alt_u8 sub_address, alt_u8 *pData8, int nReadLength){

    alt_32 Data32;
    alt_u32 Timeout;
    int i;
    bool bDone = FALSE;
    Timeout = alt_nticks() + alt_ticks_per_second()/2;
    while(!bDone)
    {
      //set the tx reg audio chip dev address with write bit
      IOWR( i2c_base, 3,device_address);
      //set STA and WR bits(bit7 and bit4)
      IOWR( i2c_base, 4,0x90);
      //wait TIP bit go to 0 to end Tx
      usleep(SLEEP_TIME);
      IOWR(i2c_base, 3,sub_address);
      //set WR bits(bit4)
      IOWR( i2c_base, 4,0x10);
      //wait TIP bit go to 0 to end Tx
      usleep(SLEEP_TIME);
      //read
      //set the tx reg audio chip dev address with read bit 1
      IOWR(i2c_base, 3,device_address|0x01);
      //set STA and WR bits(bit7 and bit4)
      IOWR( i2c_base, 4,0x90);
      usleep(SLEEP_TIME);
      //wait TIP bit go to 0 to end Tx
      if (ACK_check( i2c_base))
    	  bDone = TRUE;
      if(alt_nticks() > Timeout)
    	  bDone = TRUE;
     }
    for(i=0;i<nReadLength;i++){
   //set the RD and ACK bit(bit5 and bit3)
    	IOWR( i2c_base, 4,((i+1) == nReadLength)?0x28:0x20);
        if (!ACK_judge_for_read( i2c_base)){
            printf("OC_I2C_Read error[10]\r\n");
            return FALSE;
        }
        Data32=IORD( i2c_base, 3);
        *(pData8+i) =  Data32 & 0xff;
    }
    IOWR( i2c_base, 4,0x40);
    usleep(SLEEP_TIME);


   return TRUE;

}



