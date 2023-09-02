/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */
#include <stdio.h>
#include "system.h"
#include "I2C_core.h"
#include "si114x_algorithm.h"
#include "si114x_function.h"
#define Device_Addr 0xb4

/** Flag that is used whenever we have get an gesture process interrupt. */
static volatile bool processGestures = 0;
bool processDemoSwitch = 0;
bool Abort = 0;

/**************************************************************************//**
 * @brief Enable gesture mode.
 *****************************************************************************/
static void enableGestureMode(void)
{

  Si114x_ConfigureDetection(Device_Addr);
  Si114x_SetInterruptOutputEnable(Device_Addr, 1);
}


/**************************************************************************//**
 * @brief Enable Force mode.
 *****************************************************************************/
static void enableForceMode(void)
{
	Si114x_ConfigureForceMode(Device_Addr);
}




/**************************************************************************//**
 * @brief This function is called whenever a new gesture needs to be processed.
 *        It is reponsible for setting up the animations.
 *****************************************************************************/
static void handleGestures(void)
{
  gesture_t gestureInput = NONE;

  /* get prox sensor sample */
  gestureInput = Si114x_NewSample(Device_Addr, alt_nticks());
  if (gestureInput != NONE)
  {
    if (gestureInput == UP)
    {
      printf("UP\n");
    }
    else if(gestureInput == DOWN)
    {
      printf("DOWN\n");
    }
    else if (gestureInput == RIGHT)
    {
      printf("RIGHT\n");
    }
    else if (gestureInput == LEFT)
    {
      printf("LEFT\n");
    }
  }


}

static void Si114x_ISR(void *context)
{
	processGestures = 1;
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(INTERRUPT_BASE,0);
}

static void Button_ISR(void *context)
{
	Abort = 1;
	processDemoSwitch = ~processDemoSwitch;
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_BASE,0);
}



void Init_Interrupt()
{
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(INTERRUPT_BASE, 0x1);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(INTERRUPT_BASE, 0x0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_BASE, 0x1);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_BASE, 0x0);
	alt_ic_isr_register(BUTTON_IRQ_INTERRUPT_CONTROLLER_ID, BUTTON_IRQ, Button_ISR,(void*)BUTTON_BASE, NULL);
	alt_ic_isr_register(INTERRUPT_IRQ_INTERRUPT_CONTROLLER_ID, INTERRUPT_IRQ, Si114x_ISR,(void*)INTERRUPT_BASE, NULL);
}


int main()
{
  Init_Interrupt();
  oc_i2c_init(I2C_OPENCORES_BASE);


  printf("Press KEY[0] can switch mode...\n");
  usleep(1000000);
  while(1)
  {
	Abort = 0;
	if(processDemoSwitch)
	{
	  printf("Gesture mode , please wait..\n");
	  usleep(3000000);
	  enableGestureMode();
	  printf("Gesture mode is ready\n");
	}
	else
	{
	  printf("Force Mode , please wait..\n");
	  usleep(3000000);
	  enableForceMode();
	  printf("Force mode is ready\n");
	}

	while(1)
    {
	    if(processGestures & processDemoSwitch)
	    {
	       if(!Abort)
	       {
             handleGestures();
             processGestures = 0;
	       }
	    }
	    else if(!processDemoSwitch)
	    {
	       if(!Abort)
	         Si114x_ForceRead(Device_Addr);
	       usleep(100000);
	    }
	    if(Abort)
	      break;

    }
  }
  return 0;
}
