/*
 * ringBuffer.c
 *
 *  Created on: 16.06.2018
 *      Author: Malte
 */

#include "ringbuffer.h"
#include "main.h"
#include <stdint.h>
#include "stm32f7xx_hal.h"

extern uint16_t pWrite_buf_transmit;
extern uint8_t buf_transmit[];
extern SPI_HandleTypeDef hspi3;


void pushToTXBuffer(uint8_t *data, uint16_t length)
{
	for(uint16_t index_data = 0; index_data < length; index_data++)
	{
		buf_transmit[pWrite_buf_transmit++] = data[index_data];
		if(pWrite_buf_transmit >= TRANSMIT_BUFFER_SIZE)
		{
			pWrite_buf_transmit = 0;
		}
	}

	//Enable TX Interrupt
	//hspi3.Instance->CR2 |= (SPI_IT_TXE); //Enable Interrupt, because new data is available
}

/**
 * Writes the last message into *data.
 * @param *data Requested Message is inserted here
 * @return '0' for no message (0x00) found, '1' for message (0x00) found
 */
char ReadLastMessageFromRXBuffer(uint8_t *data)
{
	//Look for last zero in RX Buffer
	uint16_t lastZeroReceived = pWrite_buf_transmit - 1;
	while(lastZeroReceived != 0x00)
	{
		if(buf_receive[lastZeroReceived] < 0)
			lastZeroReceived = RECEIVE_BUFFER_SIZE - 1;
		else if(lastZeroReceived == pWrite_buf_transmit)
			return 0;
		lastZeroReceived--;
	}

	//Zero was found and is stored in 'lastZeroReceived'
	for(uint16_t offset = 0; offset < 16; offset--) //ToDo: INSERT REAL PACKET SIZE LATER (16 now)
	{
		if((lastZeroReceived - offset) < 0)
			lastZeroReceived = RECEIVE_BUFFER_SIZE;

		data[offset] = buf_receive[lastZeroReceived - offset];
	}
	return 1;



}
