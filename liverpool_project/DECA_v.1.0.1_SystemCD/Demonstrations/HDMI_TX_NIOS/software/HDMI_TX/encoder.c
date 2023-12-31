
#include "common.h"
#include "i2c.h"
#include "system.h"
#include "encoder.h"

//#define REG_ACCESS_UDELAY  5000

int reg_read(struct hdmi_encoder *ec, int reg, void *data)
{
	bool r = I2C_Read(I2C_SCL_BASE, I2C_SDA_BASE, ec->slave_addr, (alt_u8)reg, (alt_u8 *)data);
	if (!r)
		return -1;

	//usleep(REG_ACCESS_UDELAY);

	return 0;
}

int reg_write(struct hdmi_encoder *ec, int reg, int data)
{
	//printf("[%02x] <- %02x\n", reg, data);

	bool r = I2C_Write(I2C_SCL_BASE, I2C_SDA_BASE, ec->slave_addr, (alt_u8)reg, (alt_u8)data);
	if (!r)
		return -1;

	//usleep(REG_ACCESS_UDELAY);

	return 0;
}

int reg_update_bits(struct hdmi_encoder *ec, int reg, int mask, int data)
{
	bool r = 0;
	alt_u8 regv = 0;

	r = I2C_Read(I2C_SCL_BASE, I2C_SDA_BASE, ec->slave_addr, reg, &regv);
	if (!r)
		return -1;

	regv &= ~((alt_u8)mask);

	regv |= ((alt_u8)data & (alt_u8)mask);

	//printf("[%02x] <- %02x\n", reg, regv);

	//usleep(REG_ACCESS_UDELAY);

	r = I2C_Write(I2C_SCL_BASE, I2C_SDA_BASE, ec->slave_addr, reg, regv);
	if (!r)
		return -1;

	//usleep(REG_ACCESS_UDELAY);

	return 0;
}

int reg_or_bits(struct hdmi_encoder *ec, int reg, int data)
{
	bool r = 0;
	alt_u8 regv = 0;

	r = I2C_Read(I2C_SCL_BASE, I2C_SDA_BASE, ec->slave_addr, reg, &regv);
	if (!r)
		return -1;

	regv |= (alt_u8)data;

	//printf("[%02x] <- %02x\n", reg, regv);

	//usleep(REG_ACCESS_UDELAY);

	r = I2C_Write(I2C_SCL_BASE, I2C_SDA_BASE, ec->slave_addr, reg, regv);
	if (!r)
		return -1;

	//usleep(REG_ACCESS_UDELAY);

	return 0;
}

int reg_clear_bits(struct hdmi_encoder *ec, int reg, int mask)
{
	bool r = 0;
	alt_u8 regv = 0;

	r = I2C_Read(I2C_SCL_BASE, I2C_SDA_BASE, ec->slave_addr, reg, &regv);
	if (!r)
		return -1;

	regv &= ~((alt_u8)mask);

	//printf("[%02x] <- %02x\n", reg, regv);

	//usleep(REG_ACCESS_UDELAY);

	r = I2C_Write(I2C_SCL_BASE, I2C_SDA_BASE, ec->slave_addr, reg, regv);
	if (!r)
		return -1;

	//usleep(REG_ACCESS_UDELAY);

	return 0;
}
