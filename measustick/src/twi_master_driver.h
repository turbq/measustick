/*
 * twi_master_driver.h
 *
 * Created: 05.07.2018 13:51:46
 *  Author: Legkiy
 */ 


#ifndef TWI_MASTER_DRIVER_H_
#define TWI_MASTER_DRIVER_H_

#include "common.h"

#define TWI_BAUD				0x05	//TWI freq ~100kHz

/* Status codes for master */
typedef enum TWIM_States_enum {
	START = 0x08,
	R_START = 0x10,
	SLAW_ACK = 0x18,
	SLAW_NACK = 0x20,
	TX_ACK = 0x28,
	TX_NACK = 0x30,
	ARB_LOST = 0x38,
	SLAR_ACK = 0x40,
	SLAR_NACK = 0x48,
	RX_ACK = 0x50,
	RX_NACK = 0x58,
	NO_STATE = 0xF8,
	BUS_ERR = 0x00
} TWIM_State_t;

#define TWI_STATUS_bm 0xF8

/*! Transaction status defines. */
#define TWIM_STATUS_READY              0
#define TWIM_STATUS_BUSY               1

/*! Buffer size defines */
#define TWIM_BUFFER_SIZE         10

/*! Transaction result enumeration. */
typedef enum TWIM_RESULT_enum {
	TWIM_RESULT_UNKNOWN          = (0x00<<0),
	TWIM_RESULT_OK               = (0x01<<0),
	TWIM_RESULT_BUFFER_OVERFLOW  = (0x02<<0),
	TWIM_RESULT_ARBITRATION_LOST = (0x03<<0),
	TWIM_RESULT_BUS_ERROR        = (0x04<<0),
	TWIM_RESULT_NACK_RECEIVED    = (0x05<<0),
	TWIM_RESULT_FAIL             = (0x06<<0),
} TWIM_RESULT_t;

typedef volatile uint8_t register8_t;

typedef struct TWI_Master {
	register8_t address;				/*!< Slave address */
	register8_t data_buf[TWIM_BUFFER_SIZE];	/*!< Data + slave*/
	register8_t bytesToWrite;						//Number of bytes to write
	register8_t bytesToRead;						//Number of bytes to read
	register8_t bytesWritten;						//Number of bytes written
	register8_t bytesRead;							//Number of bytes read
	register8_t status;					/*!< Status of transaction */
	register8_t result;					/*!< Result of transaction */
}TWI_Master_t;

extern TWI_Master_t twi;

void TWI_MasterInit(uint8_t scale, uint8_t baud);
TWIM_State_t TWI_MasterState(void);
bool TWI_MasterReady(void);
bool TWI_MasterWrite(uint8_t addr, uint8_t *data, uint8_t bytesToWrite);
bool TWI_MasterRead(uint8_t addr, uint8_t bytesToRead);

#endif /* TWI_MASTER_DRIVER_H_ */