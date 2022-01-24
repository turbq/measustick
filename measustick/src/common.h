/*
 * includes.h
 *
 * Created: 24.06.2019 16:23:06
 *  Author: Legkiy
 */ 


#ifndef INCLUDES_H_
#define INCLUDES_H_

//std libs
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//avr
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <asf.h>

/*! \brief This macro will protect the following code from interrupts. */
#define AVR_ENTER_CRITICAL_REGION( ) uint8_t volatile saved_sreg = SREG; \
                                     cli();

/*! \brief This macro must always be used in conjunction with AVR_ENTER_CRITICAL_REGION
 *        so the interrupts are enabled again.
 */
#define AVR_LEAVE_CRITICAL_REGION( ) SREG = saved_sreg;

#define TRUE 1
#define FALSE 0

#endif /* INCLUDES_H_ */