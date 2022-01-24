/*
 * ina220.h
 *
 * Created: 20.01.2022 11:01:08
 *  Author: legkiyns
 */ 


#ifndef INA220_H_
#define INA220_H_

#include "common.h"

#define TWI_INA220_SLAVE_ADDR	0x40

void ina220_init(void);
uint16_t bus_read(void);
int16_t shunt_read(void);
int16_t current_read(void);
uint32_t pwr_read(void);



#endif /* INA220_H_ */