/*
 * timeStamp.c
 *
 *  Created on: 23.06.2018
 *      Author: Malte
 */

#include "irq.h"
#include <stdint.h>
#include "timeStamp.h"
#include "LPC214x.h"


uint64_t getTimeStamp()
{
	return (((uint64_t)counter_highValue)<<32) + T1TC;
}

void timer1ISR(void) __irq
{
	T1IR = 0x01;      //Clear the timer 1 interrupt
	IENABLE; //Handles Nested Interrupt
	counter_highValue++;
	IDISABLE; //Handles Nested Interrupt
	VICVectAddr = 0;	// Acknowledge Interrupt
}

