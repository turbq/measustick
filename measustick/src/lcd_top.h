/*
 * lcd_top.h
 *
 * Created: 11.06.2019 10:38:11
 *  Author: Legkiy
 */ 


#ifndef LCD_TOP_H_
#define LCD_TOP_H_

#include "common.h"
#include "lcd_1602twi.h"

void lcd_clr(void);
void lcd_returnhome(void);
void lcd_nextline(void);
void lcd_setddram(uint8_t addr);
void lcd_puts(char *string, uint8_t size);
void lcd_printf(char *format, ...);

#endif /* LCD_TOP_H_ */