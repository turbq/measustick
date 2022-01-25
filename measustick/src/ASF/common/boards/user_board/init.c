/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "timers.h"
#include "twi_master_driver.h"
#include "lcd_top.h"
#include "ina220.h"

#define LED IOPORT_CREATE_PIN(PORTB, 6)

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
 	CLKPR = (1<<CLKPCE);
 	CLKPR = 0; //prescaler /1
	wdt_enable(WDTO_1S);
	WDTCSR |= _BV(WDIE);
	
	ioport_set_pin_dir(LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_low(LED);
	
	sei();
	tc1_fastpwm_on();
	TWI_MasterInit(0x00, 8);
	
	ina220_init();
	lcd_init();
	lcd_clr();
	lcd_printf("Radioavionica %c%c",0x01,0x02);
	lcd_nextline();
	lcd_printf("corporation   %c%c",0x04,0x03);
	delay_ms(200);
	tc5_on();
}

ISR(WDT_vect)
{
	sei();
	lcd_clr();
	delay_ms(500);
	lcd_printf("Reset");
	delay_ms(1000);
	cli();
}
