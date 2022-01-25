#include <asf.h>
#include "lcd_top.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	
	/* Insert application code here, after the board has been initialized. */
	while(1){
		wdt_reset();
	}
}
