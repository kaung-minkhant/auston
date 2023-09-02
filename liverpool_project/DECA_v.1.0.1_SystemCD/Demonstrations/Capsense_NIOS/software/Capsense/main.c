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
#define Device_Addr 0x6e //0x37
#define Button_State 0xaa

int main()
{
  alt_u8 szValue[256];
  alt_u16 Data16;
  alt_u8 szData1[2];
  alt_u8 szData0[1];

  int value,i;
  oc_i2c_init(I2C_OPENCORES_BASE);
  printf("Please touch the CapSense Buttons on DECA.");
  while(1)
  {
    OC_I2C_Read(I2C_OPENCORES_BASE,Device_Addr,Button_State,&szValue,2);
    IOWR(LED_BASE,0,~szValue[0]);
  }
  return 0;
}
