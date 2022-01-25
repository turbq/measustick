/*
 * lcd_1602twi.c
 *
 * Created: 06.07.2018 11:52:12
 *  Author: Legkiy
 */ 

#include "common.h"
#include "lcd_1602twi.h"


DisplayFlags DispStatus;

inline void lcd_addchar\
(uint8_t CGaddress, uint8_t *newChar){
	lcd_instr(LCD_SETCGRAMADDR|CGaddress);
	lcd_RamWrite(newChar, 8);
}

inline void lcd_instr(uint8_t string){
	lcd_instr_seq((uint8_t*) &string, 1);
}

inline void lcd_instr_seq(uint8_t *string, uint8_t size){
	lcd_sendseq(string, size, 0x00);
}

inline void lcd_RamWrite(uint8_t *string, uint8_t size){
	lcd_sendseq(string, size, LCD_RS);
}

inline void lcd_ruprint(char *string, uint8_t size){
	replace(string, size);
	lcd_RamWrite((uint8_t*) string, size);
}

void lcd_init(){
	uint8_t *initseq = \
	(uint8_t [])\
	{
		0x38, 0x39, 0x14, 0x7f, 0x5e,\
		0x6b, 0x38, 0x0c, 0x01, 0x06
	};
	uint8_t sz = 10;
	
	//The busy state lasts atleast for 40 ms after VDD rises to stable.
	delay_ms(40);
	lcd_instr_seq(initseq, sz);
	
	uint8_t degree_char[] = {\
		0b01100,\
		0b10010,\
		0b10010,\
		0b01100,\
		0b00000,\
		0b00000,\
		0b00000,\
		0b00000
	};
	uint8_t batthigh_char[] = {\
		0b01110,\
		0b11111,\
		0b11111,\
		0b11111,\
		0b11111,\
		0b11111,\
		0b11111,\
		0b00000
	};
	uint8_t battmed_char[] = {\
		0b01110,\
		0b11011,\
		0b10001,\
		0b11111,\
		0b11111,\
		0b11111,\
		0b11111,\
		0b00000
	};
	uint8_t battlow_char[] = {\
		0b01110,\
		0b11011,\
		0b10001,\
		0b10001,\
		0b10001,\
		0b11111,\
		0b11111,\
		0b00000
	};
	
	uint8_t ctit_char[] = {\
		0b01010,\
		0b00100,\
		0b00000,\
		0b00000,\
		0b00000,\
		0b00100,\
		0b11011,\
		0b00000
	};
	
	uint8_t tl_char[] = {\
		0b00001,\
		0b00001,\
		0b00001,\
		0b00001,\
		0b00010,\
		0b00100,\
		0b00010,\
		0b01101
	};
	
	uint8_t tr_char[] = {\
		0b00000,\
		0b00000,\
		0b10000,\
		0b10000,\
		0b01000,\
		0b00100,\
		0b01010,\
		0b10110
	};
	
	uint8_t br_char[] = {\
		0b01001,\
		0b10011,\
		0b10101,\
		0b10101,\
		0b10011,\
		0b01010,\
		0b00110,\
		0b11000
	};
	
	uint8_t bl_char[] = {\
		0b10010,\
		0b11001,\
		0b10101,\
		0b10101,\
		0b11001,\
		0b01010,\
		0b01100,\
		0b00011
	};
	
	lcd_addchar(CHAR1, degree_char);
	lcd_addchar(CHAR2, tl_char);
	lcd_addchar(CHAR3, tr_char);
	lcd_addchar(CHAR4, br_char);
	lcd_addchar(CHAR5, bl_char);
	lcd_instr(LCD_SETDDRAMADDR);
}

void lcd_sendseq(uint8_t *instr, uint8_t size, uint8_t ctrl){
	uint8_t word[size*2];
	
	for (uint8_t i=0; i<size-1; i++){
		word[2*i] = LCD_COBYTE|ctrl;
		word[2*i+1] = *(instr+i);
	}
	word[size*2-2] = ctrl;
	word[size*2-1] = *(instr+(size-1));
	
	uint8_t remainder = size*2%8;
	uint8_t cycles = size*2/8 + (remainder ? 1 : 0);
	uint8_t *ptr=NULL;
	
	for(uint8_t i=0; i<cycles-1; i++){
		ptr = & word[i*8];
		TWI_MasterWrite(LCD_SLAVE_ADDR, ptr, 8);
		while(!(TWI_MasterReady()));
		delay_ms(1);
	}
	
	if (remainder){
		ptr = & word[(cycles-1)*8];
		TWI_MasterWrite(LCD_SLAVE_ADDR, ptr, remainder);
		while(!(TWI_MasterReady()));
		delay_ms(1);
	} else {
		ptr = & word[(cycles-1)*8];
		TWI_MasterWrite(LCD_SLAVE_ADDR, ptr, 8);
		while(!(TWI_MasterReady()));
		delay_ms(1);
	}
}

void replace(char *string, uint8_t size){
	for (uint8_t i=0; i<size; i++){
		switch (string[i])
		{
		case 'À':
			string[i] = 'A';
			break;
		case 'Á':
			string[i] = 0xa0;
			break;
		case 'Â':
			string[i] = 'B';
			break;
		case 'Ã':
			string[i] = 0xa1;
			break;
		case 'Ä':
			string[i] = 0xe0;
			break;
		case 'Å':
			string[i] = 'E';
			break;
		case '¨':
			string[i] = 0xa2;
			break;
		case 'Æ':
			string[i] = 0xa3;
			break;
		case 'Ç':
			string[i] = 0xa4;
			break;
		case 'È':
			string[i] = 0xa5;
			break;
		case 'É':
			string[i] = 0xa6;
			break;
		case 'Ê':
			string[i] = 'K';
			break;
		case 'Ë':
			string[i] = 0xa7;
			break;
		case 'Ì':
			string[i] = 'M';
			break;
		case 'Í':
			string[i] = 'H';
			break;
		case 'Î':
			string[i] = 'O';
			break;
		case 'Ï':
			string[i] = 0xa8;
			break;
		case 'Ð':
			string[i] = 'P';
			break;
		case 'Ñ':
			string[i] = 'C';
			break;
		case 'Ò':
			string[i] = 'T';
			break;
		case 'Ó':
			string[i] = 0xa9;
			break;
		case 'Ô':
			string[i] = 0xaa;
			break;
		case 'Õ':
			string[i] = 'X';
			break;
		case 'Ö':
			string[i] = 0xe1;
			break;
		case '×':
			string[i] = 0xab;
			break;
		case 'Ø':
			string[i] = 0xac;
			break;
		case 'Ù':
			string[i] = 0xe2;
			break;
		case 'Ú':
			string[i] = 0xad;
			break;
		case 'Û':
			string[i] = 0xae;
			break;
		case 'Ü':
			string[i] = 'b';
			break;
		case 'Ý':
			string[i] = 0xaf;
			break;
		case 'Þ':
			string[i] = 0xb0;
			break;
		case 'ß':
			string[i] = 0xb1;
			break;
		case 'à':
			string[i] = 'a';
			break;
		case 'á':
			string[i] = 0xb2;
			break;
		case 'â':
			string[i] = 0xb3;
			break;
		case 'ã':
			string[i] = 0xb4;			
			break;
		case 'ä':
			string[i] = 0xe3;			
			break;
		case 'å':
			string[i] = 'e';
			break;
		case '¸':
			string[i] = 0xb5;		
			break;
		case 'æ':
			string[i] = 0xb6;
			break;
		case 'ç':
			string[i] = 0xb7;
			break;
		case 'è':
			string[i] = 0xb8;			
			break;
		case 'é':
			string[i] = 0xb9;	
			break;
		case 'ê':
			string[i] = 0xba;	
			break;
		case 'ë':
			string[i] = 0xbb;	
			break;
		case 'ì':
			string[i] = 0xbc;	
			break;
		case 'í':
			string[i] = 0xbd;	
			break;
		case 'î':
			string[i] = 'o';	
			break;
		case 'ï':
			string[i] = 0xbe;	
			break;
		case 'ð':
			string[i] = 'p';	
			break;
		case 'ñ':
			string[i] = 'c';	
			break;
		case 'ò':
			string[i] = 0xbf;
			break;
		case 'ó':
			string[i] = 'y';	
			break;
		case 'ô':
			string[i] = 0xe4;	
			break;
		case 'õ':
			string[i] = 'x';	
			break;
		case 'ö':
			string[i] = 0xe5;	
			break;
		case '÷':
			string[i] = 0xc0;	
			break;
		case 'ø':
			string[i] = 0xc1;	
			break;
		case 'ù':
			string[i] = 0xe6;	
			break;
		case 'ú':
			string[i] = 0xc2;		
			break;
		case 'û':
			string[i] = 0xc3;			
			break;
		case 'ü':
			string[i] = 0xc4;
			break;
		case 'ý':
			string[i] = 0xc5;
			break;
		case 'þ':
			string[i] = 0xc6;
			break;
		case 'ÿ':
			string[i] = 0xc7;	
			break;
		default:
			string[i] = string[i];
			break;
		}
	}
}

