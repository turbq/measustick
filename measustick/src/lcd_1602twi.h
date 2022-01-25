/*
 * lcd_1602twi.h
 *
 * Created: 06.07.2018 11:50:58
 *  Author: Legkiy
 */ 


#ifndef LCD_1602TWI_H_
#define LCD_1602TWI_H_

#include "common.h"
#include "twi_master_driver.h"

#define LCD_SLAVE_ADDR			0x3e

// instruction modifiers
#define LCD_COBYTE	0x80u
#define LCD_RS		0x40u

// commands
#define LCD_CLEARDISPLAY	0x01u
#define LCD_RETURNHOME		0x02u
#define LCD_ENTRYMODESET	0x04u
#define LCD_DISPLAYCONTROL	0x08u
#define LCD_CURSORSHIFT		0x10u	//IS = 0
#define LCD_SETOSCFREQ		0x10u	//IS = 1
#define LCD_FUNCTIONSET		0x20u
#define LCD_SETCGRAMADDR	0x40u	//IS = 0
#define LCD_SETICONADDR		0x40u	//IS = 1 The ICON RAM address is from "00H" to "0FH".
#define LCD_POWERICONCONTR	0x50u	//IS = 1
#define LCD_FOLLOWERCTRL	0x60u	//IS = 1
#define LCD_SETCONTRAST		0x70u	//IS = 1
#define LCD_SETDDRAMADDR	0x80u

// flags for display entry mode (I/D & S bits)
#define LCD_ENTRYDECREMENT	0x00u
#define LCD_ENTRYINCREMENT	0x02u
#define LCD_ENTRYSHIFTON	0x01u	//shift display according to I/D value (ENTRYINCREMENT)
#define LCD_ENTRYSHIFTOFF	0x00u

// flags for display on/off control (D & C & B bits)
#define LCD_DISPLAYON	0x04u
#define LCD_DISPLAYOFF	0x00u
#define LCD_DISPLAY_bm	(1<<2)
#define LCD_CURSORON	0x02u
#define LCD_CURSOROFF	0x00u
#define LCD_CURSOR_bm	(1<<1)
#define LCD_BLINKON		0x01u
#define LCD_BLINKOFF	0x00u
#define LCD_BLINK_bm	(1<<0)

// flags for display/cursor shift (S/C & R/L bits) 
#define LCD_DISPLAYMOVE 0x08u	//screen controlled by R/L bit
#define LCD_CURSORMOVE	0x00u	//cursor controlled by R/L bit
#define LCD_MOVERIGHT	0x04u	//right
#define LCD_MOVELEFT	0x00u	//left

// flags for function set (DL & N & DH & *0 & IS bits)
#define LCD_8BITMODE	0x10u
#define LCD_4BITMODE	0x00u
#define LCD_DLBIT_bm	(1<<4)	//data length
#define LCD_2LINE		0x08u
#define LCD_1LINE		0x00u
#define LCD_NBIT_bm		(1<<3)	//line number
#define LCD_5x16DOTS	0x04u
#define LCD_5x8DOTS		0x00u
#define LCD_DHBIT_bm	(1<<2)	//double height
#define LCD_IS1			0x01u
#define LCD_IS0			0x00u
#define LCD_ISBIT_bm	(1<<0)	//instruction table

// flags for OSCFREQ set for vdd = 3v
#define LCD_BS1_4BIAS	0x08u	//1/4bias
#define LCD_BS1_5BIAS	0x00u	//1/5bias
#define LCD_OSC_347		0x07u
#define LCD_OSC_274		0x06u
#define LCD_OSC_221		0x05u
#define LCD_OSC_183		0x04u
#define LCD_OSC_161		0x03u
#define LCD_OSC_144		0x02u
#define LCD_OSC_131		0x01u
#define LCD_OSC_122		0x00u

// flags for Power/Icon control/Contrast set
#define LCD_ION			0x08u	//ICON display on
#define LCD_BON			0x04u	//Booster circuit on
#define LCD_CTRST1_bm	0x03u	//contrast MSB C5 C4

// flags for Follower control
#define LCD_FON		0x08u	//follower circuit on
#define LCD_RAB_MSK	0x07u	//Rab2~0 select follower amplified ratio

// flags for Contrast set
#define LCD_CTRST0_bm	0x0fu	//Contrast C3~C0 LSB

#define CHAR1	0
#define CHAR2	8
#define CHAR3	16
#define CHAR4	24
#define CHAR5	32

typedef union
{
	unsigned char allFlags; //!< Variable used to clear all flags.
	struct
	{
		//unsigned char dirty:1; //!< 1 bit wide bitfield used to flag display garbage.
		//unsigned char refresh:1; //!< 1 bit wide bitfield.
		//unsigned char drawing:1; //!< used to know about 2 string output
		unsigned char print:1; //!< forces printing
		unsigned char dimm:1; //!< flag for dimming
	};
} DisplayFlags;

extern DisplayFlags DispStatus;

void lcd_addchar(uint8_t CGaddress, uint8_t *newChar);
void lcd_instr(uint8_t instr);
void lcd_instr_seq(uint8_t *string, uint8_t size); //uses sequence of instructions
void lcd_RamWrite(uint8_t *string, uint8_t size);

void lcd_ruprint(char *string, uint8_t size);
void lcd_init(void);
void lcd_sendseq(uint8_t *instr, uint8_t size, uint8_t ctrl);
void replace(char *string, uint8_t size);




#endif /* LCD_1602TWI_H_ */