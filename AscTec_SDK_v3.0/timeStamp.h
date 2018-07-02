/*
 * timeStamp.h
 *
 *  Created on: 23.06.2018
 *      Author: Malte
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

uint32_t counter_highValue;

extern void timer1ISR (void) __irq;
uint64_t getTimeStamp(void);

#endif /* TIMESTAMP_H_ */
