/*
 * timers.c
 *
 * Created: 24.01.2022 13:10:34
 *  Author: legkiyns
 */ 

#include "timers.h"
#include "lcd_top.h"
#include "twi_master_driver.h"
#include "ina220.h"

volatile uint16_t ms=0;
volatile daytime uptime;
volatile uint16_t ms_delay;

void delay_ms(uint16_t ms)
{
	ms_delay = ms;
	while(ms_delay);
}

void tc1_fastpwm_on(void)
{
	/* Fast pwm OC1B (pb6) out*/
	OCR1A = 0x007c;
	OCR1B = (0x007c*0.1);
	TCCR1A |= _BV(COM1B1)|_BV(WGM11)|_BV(WGM10);
	TCCR1B |= _BV(WGM13)|_BV(WGM12)|_BV(CS11)|_BV(CS10);
	TIMSK1 |= _BV(OCIE1A);
}

void tc5_on(void)
{
	TCCR5B |= _BV(CS51)|_BV(CS50);
	TIMSK5 |= _BV(TOIE5);
}

ISR(TIMER1_COMPA_vect)
{
	AVR_ENTER_CRITICAL_REGION();
	if (ms_delay > 0){
		ms_delay--;
	}
	if (ms++ == 1000){
		uptime.glob_sec++;
		uptime.sec++;
		ms = 0;
	}
	if (uptime.sec == 60){
		uptime.sec = 0;
		uptime.min++;
	}
	if (uptime.min == 60){
		uptime.min = 0;
		uptime.hour++;
	}
	if (uptime.hour == 24){
		uptime.hour = 0;
		uptime.day++;
	}
	AVR_LEAVE_CRITICAL_REGION();
}

ISR(TIMER5_OVF_vect)
{
	f_lcdPrint = 1;
	sei();
	lcd_cout();
	cli();
}