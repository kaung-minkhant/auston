/*****************************************************************************************
 * File Name    :PortInterface.c
 *
 * Description  : This is a simple C file that shows how to use NIOS II processor
 *                to grab the commands/data provided by USB2.0 IP and take the necessary
 *                action according to it.
 *
 * Copyright (C) 2011 by SLS, All rights reserved
 *
 ******************************************************************************************/

#include "stdio.h"
#include "system.h"
#include "sls_avalon_usb20.h"
#include "sls_usb.h"
#include "sls_ep_configuration.h"
#include <unistd.h>
#include "altera_avalon_pio_regs.h"


#define WRITE       0x01
#define READ        0x02
#define BLK_WRITE   0x03
#define BLK_READ    0x04

#define DATA   		9
#define OFFSET 		0x00

/**************************************************************
 * Set DMA_FLAG as 1 to use DMA
 * Set DMA_FLAG as 0 to use UDSC(USB Data Streamer Controller)
 * NOTE : For support of UDSC, contact support@slscorp.com
 **************************************************************/
#define DMA_FLAG    1  /*1 -> DMA & 0 -> UDSC*/

unsigned int g_address;
unsigned char global_data[DATA];
void BULK_IN(void);
void BULK_OUT(void);
void BLOCK_READ(void);
void BLOCK_WRITE(void);

void usb20sr_error_event(int event);

sls_avalon_usb20sr_dev *pUsb20srDev;
sls_avalon_usb20sr_dev usb20srdev;

void configure_usb20sr(sls_avalon_usb20sr_dev *pUsb20srDev)
{
	usb_initialize_descriptor(pUsb20srDev,(unsigned char*)device_desc_,
										  (unsigned char*)conf_desc_hs_,
										  (unsigned char*)conf_desc_fs_,
										  (unsigned char*)conf_desc_ls_,
										  sizeof(conf_desc_hs_),
										  sizeof(conf_desc_fs_),
										  sizeof(conf_desc_ls_),
										  (unsigned int*)str_desc_indexes_,
										  (unsigned char*)str_desc_,
										  sizeof(str_desc_indexes_),
										  (unsigned char*)device_quali_desc_,
										  (unsigned char*)other_speed_conf_desc_hs_,
										  (unsigned char*)other_speed_conf_desc_fs_);

	usb_ep_config(      pUsb20srDev,    SUPPORTED_CONF,     supported_ep_,      ep_ip_csr_hs_,      ep_ip_csr_fs_,		ep_ip_csr_ls_,
						ep_buf0_sz_hs_, ep_buf1_sz_hs_,     ep_buf0_sz_fs_,     ep_buf1_sz_fs_, 	ep_buf0_sz_ls_, 	ep_buf1_sz_ls_,
						ep_ip_int_hs,	ep_ip_int_fs,		ep_ip_int_ls     );

    /*Use DMA/UDSC's BASE, IRQ & NAME from system.h file*/

	dma_config(pUsb20srDev, DMA_0_BASE, DMA_0_IRQ, DMA_0_NAME, DMA_FLAG);		     /*set DMA_FLAG as 1 while using DMA*/

	//dma_config(pUsb20srDev, UDSC_BASE, UDSC_IRQ, UDSC_NAME, DMA_FLAG);	        /*set DMA_FLAG as 0 while using UDSC*/

	usb20_event_isr(pUsb20srDev, USB_Attached_Event, USB_Detached_Event, USB_Reset_Event, USB_Resume_Event, USB_Suspend_Event, USB_WakeUpCfg_Event);

	if(usb20sr_config(pUsb20srDev, USB20SR_BASE, USB20SR_IRQ, USB20SR_NAME, usb20sr_error_event, HandleClassSpecificControlRequests,HandleClassSpecificDescriptorRequests))
	{
		//printf("\nConfigured !\n");

	}
}

int main()
{
	int i = 0;

	int reg_address;

	int reg_value;

	//ext reset usb20sr, high active
	IOWR_ALTERA_AVALON_PIO_DATA(RST_PIO_BASE,0);
	usleep(20*1000);

	//IOWR_ALTERA_AVALON_PIO_DATA(RST_PIO_BASE,1);
	usleep(20*1000);
	IOWR_ALTERA_AVALON_PIO_DATA(RST_PIO_BASE,0);
	usleep(20*1000);

	pUsb20srDev = &usb20srdev;

	configure_usb20sr(pUsb20srDev);


	usb20sr_disconnect(pUsb20srDev);
//set output 5v test
  while(0){
	usb20sr_phy_register_write(pUsb20srDev,0x4,0x60);
	usleep(2*1000);
	printf("output 5V !\n");
	usb20sr_phy_register_write(pUsb20srDev,0xa,0x66);
	usleep(2*1000);
	while(1);
  }
	usb20sr_set_dev_speed(pUsb20srDev, 1);     /*Connect device in the full speed(0)/high speed(1) mode*/

	usb20sr_connect(pUsb20srDev);


	printf("Connect !\n");
//enable chipselect
	usb20sr_phy_register_enable(pUsb20srDev);
//
//	usb20sr_phy_register_write(pUsb20srDev,0x4,0x01);
//	usb20sr_phy_register_write(pUsb20srDev,0x4,0x01);
//	usb20sr_phy_register_write(pUsb20srDev,0x4,0x01);
//	usb20sr_phy_register_write(pUsb20srDev,0x4,0x01);
//	usb20sr_phy_register_write(pUsb20srDev,0x4,0x01);

   //VID and PID
	//while(1) ;
	{
	usb20sr_phy_register_read(pUsb20srDev,0x0,&reg_value);
	printf("register [0x00] read value is 0x%x!\n",reg_value);
	usb20sr_phy_register_read(pUsb20srDev,0x01,&reg_value);
    printf("register [0x01] read value is 0x%x!\n",reg_value);
	usb20sr_phy_register_read(pUsb20srDev,0x02,&reg_value);
	printf("register [0x02] read value is 0x%x!\n",reg_value);
	usb20sr_phy_register_read(pUsb20srDev,0x03,&reg_value);
	printf("register [0x03] read value is 0x%x!\n",reg_value);

	//Controls UTMI function settings of the PHY.
	usb20sr_phy_register_read(pUsb20srDev,0x04,&reg_value);
	printf("register [0x04] read value is 0x%x!\n",reg_value);


	//Enables alternative interfaces and PHY features.
	usb20sr_phy_register_read(pUsb20srDev,0x07,&reg_value);
	printf("register [0x07] read value is 0x%x!\n",reg_value);

	//OTG and UTMI
	usb20sr_phy_register_read(pUsb20srDev,0x0a,&reg_value);
    printf("register [0x0a] read value is 0x%x!\n",reg_value);

	}

	while(!(ifConfigured(pUsb20srDev)));         /*Check whether device is Configured or not*/

	//usb20sr_phy_register_write(pUsb20srDev,0x4,0x40);
	usleep(20*1000);

//	usb20sr_phy_register_write(pUsb20srDev,0x7,0x10);
//	usleep(20*1000);
//
//	usb20sr_phy_register_write(pUsb20srDev,0xa,0x00);
//	usleep(20*1000);

	printf("\nConfigured !\n");


	while(1)
	{

		if((i = usb20sr_check_bo_ready(pUsb20srDev, 2)) > -1)
		{
			#ifdef DMA_USE
				usb20sr_with_dma_read(pUsb20srDev, 2, global_data, 9, DMA_FLAG);
			#else
				usb20sr_without_dma_read(pUsb20srDev, 2, global_data, 9);
			#endif
		}

		if(i > 0)
		{
			if(global_data[0] == WRITE)
			{
				printf("Word Write\n");
				printf("dma rd address is 0x%x!\n",IORD_ALTERA_AVALON_DMA_RADDRESS(DMA_0_BASE));
				printf("dma wr address is 0x%x!\n",IORD_ALTERA_AVALON_DMA_WADDRESS(DMA_0_BASE));
				printf("dma length is 0x%x!\n",IORD_ALTERA_AVALON_DMA_LENGTH(DMA_0_BASE));
				BULK_OUT();
			}
			else if(global_data[0] == READ)
			{
				printf("Word Read\n");
				printf("dma rd address is 0x%x!\n",IORD_ALTERA_AVALON_DMA_RADDRESS(DMA_0_BASE));
				printf("dma wr address is 0x%x!\n",IORD_ALTERA_AVALON_DMA_WADDRESS(DMA_0_BASE));
				printf("dma length is 0x%x!\n",IORD_ALTERA_AVALON_DMA_LENGTH(DMA_0_BASE));
				BULK_IN();
			}
			else if(global_data[0] == BLK_WRITE)
			{
				printf("Block Write\n");
				printf("dma rd address is 0x%x!\n",IORD_ALTERA_AVALON_DMA_RADDRESS(DMA_0_BASE));
				printf("dma wr address is 0x%x!\n",IORD_ALTERA_AVALON_DMA_WADDRESS(DMA_0_BASE));
				printf("dma length is 0x%x!\n",IORD_ALTERA_AVALON_DMA_LENGTH(DMA_0_BASE));
				BLOCK_WRITE();
			}
			else if(global_data[0] == BLK_READ)
			{
				printf("Block Read\n");
				printf("dma rd address is 0x%x!\n",IORD_ALTERA_AVALON_DMA_RADDRESS(DMA_0_BASE));
				printf("dma wr address is 0x%x!\n",IORD_ALTERA_AVALON_DMA_WADDRESS(DMA_0_BASE));
				printf("dma length is 0x%x!\n",IORD_ALTERA_AVALON_DMA_LENGTH(DMA_0_BASE));
				BLOCK_READ();
			}
			else
			{
				printf("Wrong OpCOde\n");
			}
		}
	}
}

void BULK_IN()
{
	unsigned char Data[4];
	g_address = 0;

	/*Read 32-bit address from USB*/
	g_address = (g_address | global_data[4]);
	g_address = (g_address | (global_data[3] << 8));
	g_address = (g_address | (global_data[2] << 16));
	g_address = (g_address | (global_data[1] << 24));

	/*Write 32-bit DATA to USB*/
	Data[0] = (IORD_8DIRECT(g_address,OFFSET));
	Data[1] = (IORD_8DIRECT(g_address,OFFSET + 1));
	Data[2] = (IORD_8DIRECT(g_address,OFFSET + 2));
	Data[3] = (IORD_8DIRECT(g_address,OFFSET + 3));

	#ifdef DMA_USE
		usb20sr_with_dma_write(pUsb20srDev, 1, Data, 4, DMA_FLAG);
	#else
		usb20sr_without_dma_write(pUsb20srDev, 1, Data, 4);
	#endif
}

void BULK_OUT()
{
	g_address = 0;
	g_address = (g_address | global_data[4]);
	g_address = (g_address | (global_data[3] << 8));
	g_address = (g_address | (global_data[2] << 16));
	g_address = (g_address | (global_data[1] << 24));

	/* Write 32-bit Data */
	IOWR_8DIRECT(g_address, 0x0, global_data[8]);
	IOWR_8DIRECT(g_address, 0x1, global_data[7]);
	IOWR_8DIRECT(g_address, 0x2, global_data[6]);
	IOWR_8DIRECT(g_address, 0x3, global_data[5]);
}

void BLOCK_READ()
{
	unsigned int bulk = 4096;
	unsigned char Data[4096];
	unsigned int NoOfBytesToRead = 0, i = 0, k = 0;
	g_address = 0;

	/*Read 32-bit address from USB*/
	g_address = (g_address | global_data[4]);
	g_address = (g_address | (global_data[3] << 8));
	g_address = (g_address | (global_data[2] << 16));
	g_address = (g_address | (global_data[1] << 24));


	NoOfBytesToRead = (NoOfBytesToRead | (global_data[8]));
	NoOfBytesToRead = (NoOfBytesToRead | (global_data[7] << 8));
	NoOfBytesToRead = (NoOfBytesToRead | (global_data[6] << 16));
	NoOfBytesToRead = (NoOfBytesToRead | (global_data[5] << 24));

	if(NoOfBytesToRead < bulk)
	{
		for(i = 0; i < NoOfBytesToRead; i++)
		{
			Data[i] = (IORD_8DIRECT(g_address, OFFSET + i));
		}

		#ifdef DMA_USE
			usb20sr_with_dma_write(pUsb20srDev, 1, Data, NoOfBytesToRead, DMA_FLAG);
		#else
			usb20sr_without_dma_write(pUsb20srDev, 1, Data, NoOfBytesToRead);
		#endif
	}
	else
	{
		int loopcount = NoOfBytesToRead / bulk;
		int mod = NoOfBytesToRead % bulk;
		if(mod != 0)
			loopcount++;
		for(k = 0; k < loopcount; k++)
		{
			if((k == (loopcount - 1)) && (mod != 0))
				bulk = mod;

			unsigned char D[bulk];

			for(i = 0; i < bulk; i++)
				D[i] = (IORD_8DIRECT(g_address, (OFFSET + i) + (k * 4096)));

			while((usb20sr_check_bi_ready(pUsb20srDev, 1)) == -1);

			#ifdef DMA_USE
				usb20sr_with_dma_write(pUsb20srDev, 1, D, bulk, DMA_FLAG);
			#else
				usb20sr_without_dma_write(pUsb20srDev, 1, D, bulk);
			#endif
		}
	 }
}

void BLOCK_WRITE()
{
	unsigned int bulk = 4096;
	unsigned int NoOfBytesToWrite = 0;
	unsigned int i = 0, k = 0;

	g_address = 0;
	g_address = (g_address | global_data[4]);
	g_address = (g_address | (global_data[3] << 8));
	g_address = (g_address | (global_data[2] << 16));
	g_address = (g_address | (global_data[1] << 24));

	NoOfBytesToWrite = (NoOfBytesToWrite | (global_data[8]));
	NoOfBytesToWrite = (NoOfBytesToWrite | (global_data[7] << 8));
	NoOfBytesToWrite = (NoOfBytesToWrite | (global_data[6] << 16));
	NoOfBytesToWrite = (NoOfBytesToWrite | (global_data[5] << 24));

	if(NoOfBytesToWrite < bulk)
	{
		unsigned char g_data[NoOfBytesToWrite];
		while((usb20sr_check_bo_ready(pUsb20srDev, 2)) <= -1);

		#ifdef DMA_USE
			usb20sr_with_dma_read(pUsb20srDev, 2, g_data, NoOfBytesToWrite, DMA_FLAG);
		#else
			usb20sr_without_dma_read(pUsb20srDev, 2, g_data, NoOfBytesToWrite);
		#endif

		for(i = 0; i < NoOfBytesToWrite; i++)
			IOWR_8DIRECT(g_address, i, g_data[i]);
	}
	else
	{
		int loopcount = NoOfBytesToWrite / bulk;
		int mod = NoOfBytesToWrite % bulk;

		if(mod != 0)
			loopcount++;

		for(k = 0; k < loopcount; k++)
		{
			if(k == loopcount - 1 && mod != 0)
				bulk = mod;

			unsigned char g_data[bulk];

			while((i = usb20sr_check_bo_ready(pUsb20srDev, 2)) == -1);

			#ifdef DMA_USE
				i = usb20sr_with_dma_read(pUsb20srDev, 2, g_data, bulk, DMA_FLAG);
			#else
				i = usb20sr_without_dma_read(pUsb20srDev, 2, g_data, bulk);
			#endif

			if(i == -1)
				return;

			for(i = 0; i < bulk; i++)
				IOWR_8DIRECT(g_address, i + (k * 4096), g_data[i]);
		}
	}
}

void USB_Attached_Event(sls_avalon_usb20sr_dev *usb20dev)
{
    printf("\nAttEvent\n");
}

void USB_Detached_Event(sls_avalon_usb20sr_dev *usb20dev)
{
    printf("\nDetaEvent\n");
}

void USB_Reset_Event(sls_avalon_usb20sr_dev *usb20dev)
{
    printf("\nResetEvent\n");
}

void USB_Resume_Event(sls_avalon_usb20sr_dev *usb20dev)
{
    printf("\nResume\n");
}

void USB_Suspend_Event(sls_avalon_usb20sr_dev *usb20dev)
{
    printf("\nSuspend\n");
}

void USB_WakeUpCfg_Event(sls_avalon_usb20sr_dev *usb20dev, unsigned char cfg)
{
    printf("\nWakeUpCfgEvent\n");
}

void HandleClassSpecificControlRequests(volatile __CONTROL_REQUEST_STATUS *CONTROL_REQUEST_STATUS,
										volatile unsigned int base, char stage)
{

}
void HandleClassSpecificDescriptorRequests(volatile __CONTROL_REQUEST_STATUS *CONTROL_REQUEST_STATUS,
											   volatile unsigned int base, char stage)
{

}


#define PRINTF_USB_EVENT(str,...) printf(str,##__VA_ARGS__)

void usb20sr_error_event(int event)
{

	//printf("usb event before and ERROR_CODE_MASK is 0x%x!\n",event);
	volatile int data = event & ERROR_CODE_MASK;

	//printf("usb event is 0x%x!\n",event);

	if(event & EP_NO_FROM_GENERATED_ERROR_MASK)
	{
		if(((event & EP_TYPE_MASK) >> 30) == IN_EP_TYPE)
			PRINTF_USB_EVENT("IN ");
		else if(((event & EP_TYPE_MASK) >> 30) == OUT_EP_TYPE)
			PRINTF_USB_EVENT("OUT ");
		else if(((event & EP_TYPE_MASK) >> 30) == CONTROL_EP_TYPE)
			PRINTF_USB_EVENT("CONTROL ");

		PRINTF_USB_EVENT("Endpoint %d cause ", (event & EP_NO_MASK) >> 24);
	}
	else
		//PRINTF_USB_EVENT("\nUSB cause ");

	if(data & TIME_OUT_ERROR)
		PRINTF_USB_EVENT("\nUSB cause TIME_OUT_ERROR\n");

	if(data & BAD_PCK_ERROR)
		PRINTF_USB_EVENT("\nUSB cause BAD_PCK_ERROR\n");

	if(data & UNEXPTED_PID_ERROR)
		PRINTF_USB_EVENT("\nUSB cause UNEXPECTED_PID_ERROR\n");

	if(data & PID_SEQ_ERROR)
		PRINTF_USB_EVENT("\nUSB cause PID_SEQ_ERROR\n");

	if(data & BAD_TCKN_ERROR)
		PRINTF_USB_EVENT("\nUSB cause BAD_TCKN_ERROR\n");

	if(data & PID_ERROR)
		PRINTF_USB_EVENT("\nUSB cause PID_ERROR\n");

	if(data & NO_EP_ERROR)
		PRINTF_USB_EVENT("\nUSB cause NO_EP_ERROR\n");

	if(data & UTMI_RX_ERROR)
		//PRINTF_USB_EVENT("\nUSB cause UTMI_RX_ERROR\n");

	if(data & ISO_PKT_LOSS)
		PRINTF_USB_EVENT("\nUSB cause ISO_PKT_LOSS\n");
}
