/*
 * I2C_ocre.h
 *
 *  Created on: 2014-6-3
 *      Author: niubility
 */

#ifndef I2C_OCRE_H_
#define I2C_OCRE_H_
#include "alt_types.h"  // alt_u32
#include "my_includes.h"

#define DEVICE_I2C_ADDR  0x30      //  AUDIO I2C

bool  oc_i2c_init(alt_32 i2c_base);
bool  oc_i2c_init_ex(alt_32 i2c_base, alt_32 ref_clk, alt_32 i2c_clk);
bool  oc_i2c_uninit(alt_32 i2c_base);
bool  ACK_single_check(alt_32 i2c_base);
bool  ACK_check(alt_32 i2c_base);
bool  OC_I2C_Write(alt_32 i2c_base,alt_u8 device_address,alt_u8 sub_address, alt_u8 *pData, int nWriteLength);
bool  ACK_judge_for_read(alt_32 i2c_base);
bool  OC_I2C_Read(alt_32 i2c_base,alt_u8 device_address,alt_u8 sub_address, alt_u8 *pData8, int nReadLength);
bool  OC_I2CL_Write(alt_32 i2c_base,alt_u8 device_address,int sub_address,alt_u8 *pData, int nWriteLength);
bool  OC_I2CL_Read(alt_32 i2c_base,alt_u8 device_address,int sub_address, alt_u8 *pData8);

//
bool  OC_I2C_Read_Continue(alt_32 i2c_base,alt_u8 device_address, alt_u8 *pData8, int nReadLength);

#endif /* I2C_OCRE_H_ */
