/*
 * lcd_top.c
 *
 * Created: 11.06.2019 10:37:04
 *  Author: Legkiy
 */ 

#include "lcd_top.h"
#include "lcd_1602twi.h"

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
