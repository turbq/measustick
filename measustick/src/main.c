#include <asf.h>
#include "lcd_top.h"
#include "twi_master_driver.h"
#include "ina220.h"

void test_init(void) 
{
	/* Fast pwm clk/1024 OC1B out*/
	OCR1B = 0x0020;
	TCCR1A |= _BV(COM1B1)|_BV(WGM10);
	TCCR1B |= _BV(WGM12)|_BV(CS11)|_BV(CS10);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	test_init();
	
	uint16_t i=0;
	/* Insert application code here, after the board has been initialized. */
	while(1){
		uint16_t mv_voltage = bus_read();
		int16_t ma_curr = current_read();
		uint32_t pwr = pwr_read()/100;
		
		lcd_clr();
		lcd_returnhome();
		if (ma_curr < 0) {
			ma_curr = abs(ma_curr);
			lcd_printf("-%d.%03dA %lu.%luW", ma_curr/1000, ma_curr%1000, pwr/10, pwr%10);
		} else {
			lcd_printf("%d.%03dA %lu.%luW", ma_curr/1000, ma_curr%1000, pwr/10, pwr%10);
		}
		lcd_nextline();
		lcd_printf("%d.%03dV", mv_voltage/1000, mv_voltage%1000);
		
		delay_s(1);
	}
}
