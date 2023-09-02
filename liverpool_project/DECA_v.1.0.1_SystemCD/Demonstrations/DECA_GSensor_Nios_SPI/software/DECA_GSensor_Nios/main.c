#include "debug.h"
#include "terasic_includes.h"

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
#define CTRL_REG4_value      0xB8    // output registers not updated until MSB and LSB have been read,data LSb at lower address,+-16g,High resolution mode,4-wire SPI interface

#define LIS2DH12_DEBUG(x)    DEBUG(x)

alt_u8 reg_addr[2]= {0x20,0x23};
alt_u16 wdata[2]  = {0x2097,0x23b8};
alt_u16 rdata[6]  = {0xa800,0xa900,0xAA00,0xAB00,0xAC00,0xAD00};


bool InitLIS2DH12()
{
	int i;
	bool     bSuccess = TRUE;
	alt_u16  reg_read=0xA000;
	alt_u16   reg_data[2];
	IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_GSENSOR_BASE,0x00);          //Initialize the spi control reg
	for(i=0;i<2;i++){
		IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_GSENSOR_BASE, wdata[i]);
		usleep(100*2000);
	}
	for(i=0;i<2;i++){
		IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_GSENSOR_BASE, (wdata[i]|reg_read));
		usleep(100*200);
		reg_data[i] = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_GSENSOR_BASE);
	    if (reg_data[i] !=(wdata[i]| 0xFF00))
	    {
	    	bSuccess = FALSE;
	    	LIS2DH12_DEBUG(("Read LIS2DH12 reg[%02xh] = %02Xh\r\n", reg_addr[i], reg_data[i] ));

        }
	}
	LIS2DH12_DEBUG(("Gsensor_SPI_Configuration  %s\r\n", bSuccess?"success":"fail"));
	return bSuccess;
}


int main()
{
   bool     bSuccess;
   alt_16   data_x,data_y,data_z;
   alt_u8   data[6];
   alt_u16  nIndex=1;
   alt_16   x, y, z;
   int i;
   //Configuration Register
   printf("== Gsensor SPI Demo ==\r\n");
   bSuccess = InitLIS2DH12();
   usleep(2000);
   while(bSuccess){
	  for(i=0;i<6;i++){
		  IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_GSENSOR_BASE, rdata[i]);
		  usleep(100*200);
		  data[i] = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_GSENSOR_BASE);
	   }
	  data_x = ((data[1]<<8)|data[0]);
	  data_y = ((data[3]<<8)|data[2]);
	  data_z = ((data[5]<<8)|data[4]);
	  x = (float)(data_x/16.0)*12.0;
	  y = (float)(data_y/16.0)*12.0;
	  z = (float)(data_z/16.0)*12.0;
	  printf("[%d]X=%dmg, Y=%dmg, Z=%dmg \r\n",nIndex, x, y, z);
      usleep(1000*2000);
      nIndex++;
    }

 }




