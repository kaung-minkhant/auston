#include "stdio.h"
#include "system.h"
#include "sls_avalon_usb20.h"
#include "sls_usb.h"
#include "sls_ep_configuration.h"
#include "altera_avalon_pio_regs.h"
//#define READ_STREAM 1

#define WRITE_STREAM 1


#define DATA   		512

/**************************************************************
 * Set DMA_FLAG as 1 to use DMA
 * Set DMA_FLAG as 0 to use UDSC(USB Data Streamer Controller)
 * NOTE : For support of UDSC, contact support@slscorp.com
 **************************************************************/
#define DMA_FLAG    1  /*1 -> DMA & 0 -> UDSC*/

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

	dma_config(pUsb20srDev, DMA_0_BASE, DMA_0_IRQ, DMA_0_NAME, DMA_FLAG);		/*set DMA_FLAG as 1 while using DMA*/

	//dma_config(pUsb20srDev, UDSC_BASE, UDSC_IRQ, UDSC_NAME, DMA_FLAG);	     /*set DMA_FLAG as 0 while using UDSC*/

	usb20_event_isr(pUsb20srDev, USB_Attached_Event, USB_Detached_Event, USB_Reset_Event, USB_Resume_Event, USB_Suspend_Event, USB_WakeUpCfg_Event);

    if(usb20sr_config(pUsb20srDev, USB20SR_BASE, USB20SR_IRQ, USB20SR_NAME, usb20sr_error_event, HandleClassSpecificControlRequests,HandleClassSpecificDescriptorRequests))
    {}
}

#ifdef READ_STREAM

int main()
{
	int read_count = 0, i = 0;
    unsigned char buf[DATA];

    pUsb20srDev = &usb20srdev;

	configure_usb20sr(pUsb20srDev);

	usb20sr_disconnect(pUsb20srDev);

	//usb20sr_set_dev_speed(pUsb20srDev, 0);     /*Connect device in the full speed(0)/high speed(1) mode*/

	usb20sr_connect(pUsb20srDev);
	printf("Connect !\n");
	while(!(ifConfigured(pUsb20srDev)));         /*Check whether device is Configured or not*/
	printf("\nConfigured !\n");

	while(1)
	{
		if((i = usb20sr_check_bo_ready(pUsb20srDev, 2)) > -1)
		{
			#ifdef DMA_USE
				read_count = usb20sr_with_dma_read(pUsb20srDev, 2, buf, DATA, DMA_FLAG);
			#else
				read_count = usb20sr_without_dma_read(pUsb20srDev, 2, buf, DATA);
			#endif
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
	volatile int data = event & ERROR_CODE_MASK;

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
		PRINTF_USB_EVENT("\nUSB cause ");

	if(data & TIME_OUT_ERROR)
		PRINTF_USB_EVENT("TIME_OUT_ERROR\n");

	if(data & BAD_PCK_ERROR)
		PRINTF_USB_EVENT("BAD_PCK_ERROR\n");

	if(data & UNEXPTED_PID_ERROR)
		PRINTF_USB_EVENT("UNEXPECTED_PID_ERROR\n");

	if(data & PID_SEQ_ERROR)
		PRINTF_USB_EVENT("PID_SEQ_ERROR\n");

	if(data & BAD_TCKN_ERROR)
		PRINTF_USB_EVENT("BAD_TCKN_ERROR\n");

	if(data & PID_ERROR)
		PRINTF_USB_EVENT("PID_ERROR\n");

	if(data & NO_EP_ERROR)
		PRINTF_USB_EVENT("NO_EP_ERROR\n");

	if(data & UTMI_RX_ERROR)
		PRINTF_USB_EVENT("UTMI_RX_ERROR\n");

	if(data & ISO_PKT_LOSS)
		PRINTF_USB_EVENT("ISO_PKT_LOSS\n");
}

#endif

#ifdef WRITE_STREAM

int main()
{
    int write_count = 0, i = 0;
    unsigned char buf[DATA];

    //ext reset usb20sr, high active
    IOWR_ALTERA_AVALON_PIO_DATA(RST_PIO_BASE,0);
    usleep(20*1000);

    pUsb20srDev = &usb20srdev;

    configure_usb20sr(pUsb20srDev);

    usb20sr_disconnect(pUsb20srDev);

    usb20sr_set_dev_speed(pUsb20srDev, 1);     /*Connect device in the full speed(0)/high speed(1) mode*/

    usb20sr_connect(pUsb20srDev);
    printf("Connect !\n");
    while(!(ifConfigured(pUsb20srDev)));        /*Check whether device is Configured or not*/
    printf("\nConfigured !\n");

    for(i=0;i<DATA;i++)
    	buf[i]=i;

    while(1)
    {
        if((i = usb20sr_check_bi_ready(pUsb20srDev, 1)) > -1)
        {
            #ifdef DMA_USE
                write_count = usb20sr_with_dma_write(pUsb20srDev, 1, buf, DATA, DMA_FLAG);
            #else
                write_count = usb20sr_without_dma_write(pUsb20srDev, 1, buf, DATA);
            #endif
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
    volatile int data = event & ERROR_CODE_MASK;

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
        PRINTF_USB_EVENT("\nUSB cause ");

    if(data & TIME_OUT_ERROR)
        PRINTF_USB_EVENT("TIME_OUT_ERROR\n");

    if(data & BAD_PCK_ERROR)
        PRINTF_USB_EVENT("BAD_PCK_ERROR\n");

    if(data & UNEXPTED_PID_ERROR)
        PRINTF_USB_EVENT("UNEXPECTED_PID_ERROR\n");

    if(data & PID_SEQ_ERROR)
        PRINTF_USB_EVENT("PID_SEQ_ERROR\n");

    if(data & BAD_TCKN_ERROR)
        PRINTF_USB_EVENT("BAD_TCKN_ERROR\n");

    if(data & PID_ERROR)
        PRINTF_USB_EVENT("PID_ERROR\n");

    if(data & NO_EP_ERROR)
        PRINTF_USB_EVENT("NO_EP_ERROR\n");

    if(data & UTMI_RX_ERROR)
        PRINTF_USB_EVENT("UTMI_RX_ERROR\n");

    if(data & ISO_PKT_LOSS)
        PRINTF_USB_EVENT("ISO_PKT_LOSS\n");
}

#endif
