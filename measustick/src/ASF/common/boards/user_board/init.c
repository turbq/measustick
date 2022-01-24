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
	delay_init(8000000);
 	CLKPR = (1<<CLKPCE);
 	CLKPR = 0; //prescaler /1
	
	ioport_set_pin_dir(LED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_low(LED);
	TWI_MasterInit(0x00, 8);
	
 	sei();
	ina220_init();
	lcd_init();
	lcd_clr();
	
}
