/*
 * timers.h
 *
 * Created: 24.01.2022 13:11:06
 *  Author: legkiyns
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

#include "common.h"

typedef struct {
	uint16_t ms;
	uint32_t glob_sec;       // uptime in seconds
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
} daytime;

extern volatile daytime uptime;

void tc1_fastpwm_on(void);
void tc5_on(void);
void delay_ms(uint16_t);


#endif /* TIMERS_H_ */