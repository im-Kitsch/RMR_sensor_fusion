/*
 * ringBuffer.h
 *
 *  Created on: 16.06.2018
 *      Author: Malte
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

/* --- INCLUDES --- */
#include <stdint.h>

/* --- DEFINES --- */
#define TRANSMIT_BUFFER_SIZE	300
#define RECEIVE_BUFFER_SIZE		300

/* --- EXTERN VARIABLES --- */
uint8_t buf_transmit[TRANSMIT_BUFFER_SIZE]; //Ringbuffer Transmit
uint8_t buf_receive[RECEIVE_BUFFER_SIZE]; //Ringbuffer Receive
uint16_t pRead_buf_transmit;
uint16_t pWrite_buf_transmit;
uint16_t pRead_buf_receive;
uint16_t pWrite_buf_receive;

/* --- EXTERN FLAGS --- */
uint8_t zeroDetected;

void pushToTXBuffer(uint8_t *data, uint16_t length);
char ReadLastMessageFromRXBuffer(uint8_t *data, uint16_t length);

uint32_t OV_Write;



#endif /* RINGBUFFER_H_ */
