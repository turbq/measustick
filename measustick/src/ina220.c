/*
 * ina220.c
 *
 * Created: 20.01.2022 11:00:15
 *  Author: legkiyns
 */ 
#include "ina220.h"
#include "twi_master_driver.h"

void ina220_init()
{
	uint8_t config[] = {0x01, 0x3f, 0x9f};
	uint8_t calib[] = {0x05, 0x20, 0x00};
	TWI_MasterWrite(TWI_INA220_SLAVE_ADDR, config, 3);
	delay_ms(20);
	TWI_MasterWrite(TWI_INA220_SLAVE_ADDR, calib, 3);
	delay_ms(20);
}

uint16_t bus_read(void)
{
	uint8_t read_reg[] = {0x02};
	uint16_t result;
	uint8_t *read_data = malloc(sizeof twi.data_buf);
	
	TWI_MasterWrite(TWI_INA220_SLAVE_ADDR, read_reg, 1);
	delay_ms(20);
	TWI_MasterRead(TWI_INA220_SLAVE_ADDR, 2);
	delay_ms(20);
	
	read_data[0] = twi.data_buf[1];
	read_data[1] = twi.data_buf[2];
	result = (read_data[1]|(read_data[0]<<8));
	result = (result>>3)*4;
	
	return result;
}

int16_t shunt_read(void)
{
	uint8_t read_reg[] = {0x01};
	uint16_t result;
	uint8_t *read_data = malloc(sizeof twi.data_buf);
	
	TWI_MasterWrite(TWI_INA220_SLAVE_ADDR, read_reg, 1);
	delay_ms(20);
	TWI_MasterRead(TWI_INA220_SLAVE_ADDR, 2);
	delay_ms(20);
	
	read_data[0] = twi.data_buf[1];
	read_data[1] = twi.data_buf[2];
	result = (read_data[1]|(read_data[0]<<8));
	
	return result;
}

int16_t current_read(void)
{
	int16_t result;
	uint8_t read_reg[] = {0x04};
	uint8_t *read_data = malloc(sizeof twi.data_buf);
	
	TWI_MasterWrite(TWI_INA220_SLAVE_ADDR, read_reg, 1);
	delay_ms(20);
	TWI_MasterRead(TWI_INA220_SLAVE_ADDR, 2);
	delay_ms(20);
	
	read_data[0] = twi.data_buf[1];
	read_data[1] = twi.data_buf[2];
	
	result = (read_data[1]|(read_data[0]<<8));
	return result;
}

uint32_t pwr_read(void)
{
	uint32_t result;
	uint8_t read_reg[] = {0x03};
	uint8_t *read_data = malloc(sizeof twi.data_buf);
	
	TWI_MasterWrite(TWI_INA220_SLAVE_ADDR, read_reg, 1);
	delay_ms(20);
	TWI_MasterRead(TWI_INA220_SLAVE_ADDR, 2);
	delay_ms(20);
	
	read_data[0] = twi.data_buf[1];
	read_data[1] = twi.data_buf[2];
	
	result = ((uint32_t)(read_data[1]|(read_data[0]<<8))*20);
	return result;
}

