/*
 * twi_master_driver.c
 *
 * Created: 05.07.2018 13:54:58
 *  Author: Legkiy
 */ 

#include "twi_master_driver.h"
#include "lcd_top.h"




TWI_Master_t twi;
uint8_t twi_slAddr;

#define SUCCESS 0xff

inline unsigned char send_start(void)
{
	TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	while (!(TWCR & (1<<TWINT)));
	
	if((TWSR != START)&&(TWSR != R_START))//If status other than START
		return TWSR;						//transmitted(0x08) or Repeated
	return SUCCESS;
}

inline void Send_stop(void)
{
	TWCR = ((1<<TWEN)+(1<<TWINT)+(1<<TWSTO));//Send STOP condition
}

//TWI portE master interrupt
ISR(TWI_vect){
	static unsigned char TWI_bufPtr;
	
	uint8_t currentStatus = TWI_MasterState();
	switch (currentStatus){
	case START:
	case R_START:
		TWI_bufPtr = 0;
	case SLAW_ACK:
	case TX_ACK:
		if (TWI_bufPtr < twi.bytesToWrite) {
			TWDR = twi.data_buf[TWI_bufPtr++];
			TWCR = (1<<TWEN)|                                 // TWI Interface enabled
				(1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interrupt and clear the flag to send byte
				(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           //
				(0<<TWWC);                                 //
		} else { // Send STOP after last byte
			twi.status = TWIM_STATUS_READY;
			twi.result = TWIM_RESULT_OK;                 // Set status bits to completed successfully.
			TWCR = (1<<TWEN)|                                 // TWI Interface enabled
				(0<<TWIE)|(1<<TWINT)|                      // Disable TWI Interrupt and clear the flag
				(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|           // Initiate a STOP condition.
				(0<<TWWC);                                 //
		}
		break;
	case RX_ACK:
		twi.data_buf[TWI_bufPtr++] = TWDR;
    case SLAR_ACK:
		if (TWI_bufPtr < (twi.bytesToRead-1) ) {	// Detect the last byte to NACK it.
			TWCR = (1<<TWEN)|                                 // TWI Interface enabled
					(1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interrupt and clear the flag to read next byte
					(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send ACK after reception
					(0<<TWWC);                                 //  
		} else {	// Send NACK after next reception
			TWCR = (1<<TWEN)|                                 // TWI Interface enabled
					(1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interrupt and clear the flag to read next byte
					(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send NACK after reception
					(0<<TWWC);                                 // 
		}    
		break; 
	case RX_NACK:
	      twi.data_buf[TWI_bufPtr] = TWDR;
		  twi.status = TWIM_STATUS_READY;
	      twi.result = TWIM_RESULT_OK;                 // Set status bits to completed successfully.
	      TWCR = (1<<TWEN)|                                 // TWI Interface enabled
				(0<<TWIE)|(1<<TWINT)|                      // Disable TWI Interrupt and clear the flag
				(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|           // Initiate a STOP condition.
				(0<<TWWC);
		  break;
	case ARB_LOST:
		TWCR = (1<<TWEN)|                              // TWI Interface enabled
			(1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag
			(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|           // Initiate a (RE)START condition.
			(0<<TWWC);                                 //
		break;
	case SLAW_NACK:
	case SLAR_NACK:
	case TX_NACK:
	case BUS_ERR:
	case NO_STATE:
	default:
		twi.status = TWIM_STATUS_READY;                // Store TWSR and automatically sets clears noErrors bit.
														// Reset TWI Interface
		TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins
			(0<<TWIE)|(0<<TWINT)|                      // Disable Interrupt
			(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests
			(0<<TWWC);                                 //
	}
	
}

void TWI_MasterInit(uint8_t scale,
					uint8_t baud)
{
	PRR0 &= ~(1<<PRTWI);
	TWAR = 60;
	TWBR = baud;
	TWSR |= ((_BV(TWPS1)|_BV(TWPS0)) & scale);
	TWCR |= (1<<TWEN)|(1<<TWIE);
}


TWIM_State_t TWI_MasterState()
{
	TWIM_State_t twi_status = (TWIM_State_t) (TWSR & TWI_STATUS_bm);
	return twi_status;
}


bool TWI_MasterReady(void)
{
	bool twi_status = (twi.status == TWIM_STATUS_READY);
	return twi_status;
}

bool TWI_MasterRead(uint8_t addr, uint8_t bytesToRead)
{
	bytesToRead++;
	/*Parameter sanity check. */
	if (bytesToRead > TWIM_BUFFER_SIZE) {
		return false;
	}
	
	/*Initiate transaction if bus is ready. */
    if (twi.status == TWIM_STATUS_READY) {

        twi.status = TWIM_STATUS_BUSY;
        twi.result = TWIM_RESULT_UNKNOWN;
        twi.address = addr<<1;
		
		twi.bytesToRead = bytesToRead;
		twi.bytesRead = 0;
        /* If read command, send the START condition + Address +
         * 'R/_W = 1'
         */
		if (twi.bytesToRead > 0) {
			uint8_t readAddress = twi.address | 0x01;
			twi.data_buf[0] = readAddress;
			TWCR = (1<<TWEN)|                             // TWI Interface enabled.
				(1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interrupt and clear the flag.
				(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
				(0<<TWWC);
		}
        return true;
	} else {
        return false;
    }
}

bool TWI_MasterWrite(uint8_t addr, uint8_t *data, uint8_t bytesToWrite)
{
	bytesToWrite++;
	/*Parameter sanity check. */
	if (bytesToWrite > TWIM_BUFFER_SIZE) {
		return false;
	}
	/*Initiate transaction if bus is ready. */
	if (twi.status == TWIM_STATUS_READY) {
		
 		twi.status = TWIM_STATUS_BUSY;
		twi.result = TWIM_RESULT_UNKNOWN;
		twi.address = addr<<1;
		twi.data_buf[0] = twi.address & ~0x01;
		for (uint8_t bufferIndex=1; bufferIndex < bytesToWrite; bufferIndex++){
			twi.data_buf[bufferIndex] = data[bufferIndex-1];
		}
		
		twi.bytesToWrite = bytesToWrite;
		twi.bytesWritten = 0;
		TWCR = (1<<TWEN)|                             // TWI Interface enabled.
			(1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interrupt and clear the flag.
			(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
			(0<<TWWC);                             //
		return true;
	} else {
		return false;
	}
}
void  Wait_TWI_int(){
	while (!(TWCR & (1<<TWINT)));
}
unsigned char Send_adr(unsigned char adr)
{
	Wait_TWI_int();							//Wait for TWI interrupt flag set

	TWDR = adr;
	TWCR = ((1<<TWINT)+(1<<TWEN));   		//Clear int flag to send byte

	Wait_TWI_int();							//Wait for TWI interrupt flag set

	if((TWSR != SLAW_ACK)&&(TWSR != SLAR_ACK))//If NACK received return
	//TWSR
		return TWSR;
	return SUCCESS;							//Else return SUCCESS
}

unsigned char Send_byte(unsigned char data)
{
	Wait_TWI_int();							//Wait for TWI interrupt flag set

	TWDR = data;
	TWCR = ((1<<TWINT)+(1<<TWEN));   		//Clear int flag to send byte

	Wait_TWI_int();							//Wait for TWI interrupt flag set

	if(TWSR != TX_ACK)				//If NACK received return TWSR
		return TWSR;
	return SUCCESS;							//Else return SUCCESS
}



unsigned char Send_to_TWI(tx_type *data_pack)
{
	unsigned char state,i,j;

	state = SUCCESS;
	
	for(i=0;(data_pack[i].slave_adr != OWN_ADR)&&(state == SUCCESS);i++)
	{
		state = send_start();				//Send START and repeated START
		if (state == SUCCESS)				
			state = Send_adr(data_pack[i].slave_adr);//Send slave address+W/R
		
		/*Dependent on the R/W in the slave address it will receive or 
		transmitt data.*/
		if(!(data_pack[i].slave_adr & R))
		{
			/*If W, it will transmitt data and loops until all data have been
			 shifted out*/
			for(j=0;((j<data_pack[i].size)&&(state == SUCCESS));j++)
				state = Send_byte(data_pack[i].data_ptr[j]);
		}	
		else								//Else read from slave 
		{
			/*If R, it will receive data and loops until all data have been
			 shifted in. When j == 0x00 this is the last byte and a NACK 
			 will be sent to the slave to make it stop transmitting */
			for(j=0;((j<data_pack[i].size)&&(state == SUCCESS));j++)
				;//state = Get_byte(data_pack[i].data_ptr++,j);
		}					
	}
  	Send_stop();							//Send STOP
  	  
	return state;							//Return SUCCESS if OK, 
											//else return TWSR 
}

void TWI_test(void)
{
	
}