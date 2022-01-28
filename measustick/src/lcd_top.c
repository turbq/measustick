/*
 * lcd_top.c
 *
 * Created: 11.06.2019 10:37:04
 *  Author: Legkiy
 */ 

#include "lcd_top.h"
#include "lcd_1602twi.h"
#include "ina220.h"

uint8_t f_lcdPrint = 0;

void lcd_clr(){
	lcd_instr(LCD_CLEARDISPLAY);
}

inline void lcd_returnhome(){
	lcd_instr(LCD_RETURNHOME);
}

inline void lcd_nextline(){
	lcd_instr(LCD_SETDDRAMADDR|0x40);
}

inline void lcd_setddram(uint8_t addr){
	lcd_instr(LCD_SETDDRAMADDR|addr);
}

void lcd_puts(char *string, uint8_t size){
	lcd_ruprint(string, size);
}

void lcd_printf(char *format, ...){
	char ch[16];
	va_list argptr;
	va_start(argptr, format);
	uint8_t len = vsprintf(ch, format, argptr);
	//ch[len] = ' ';
	va_end(argptr);
	lcd_ruprint(ch, len);
}

void lcd_cout(void)
{
	uint16_t mv_voltage = bus_read();
	int16_t ma_curr = current_read();
	uint32_t pwr = pwr_read()/100;
	
	if (!f_lcdPrint)
		return;
	
	lcd_clr();
	lcd_returnhome();
	if (ma_curr < 0) {
		ma_curr = abs(ma_curr);
		lcd_printf(" %d.%03dA %lu.%luW", ma_curr/1000, ma_curr%1000, pwr/10, pwr%10);
	} else {
		lcd_printf("-%d.%03dA %lu.%luW", ma_curr/1000, ma_curr%1000, pwr/10, pwr%10);
	}
	lcd_nextline();
	lcd_printf(" %d.%03dV %02d:%02d", mv_voltage/1000, mv_voltage%1000, uptime.min, uptime.sec);
	
	f_lcdPrint = 0;
}
