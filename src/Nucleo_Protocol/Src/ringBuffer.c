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
	int32_t pWrite = pWrite_buf_transmit;
	int32_t pRead = pRead_buf_transmit;
	int32_t len = length;
	//Flush last message (Should never occur on this side (Control data << Sensory data))
	if(pWrite > pRead && (pRead - pWrite + TRANSMIT_BUFFER_SIZE) < len)
	{
		if((pWrite-len) < 0)
			pWrite_buf_transmit = TRANSMIT_BUFFER_SIZE - (pWrite-len);
		else
			pWrite_buf_transmit = (pWrite-len);
		OV_Write++;
	}
	else if(pWrite < pRead && (pRead - pWrite) < len)
	{
		if((pWrite-len) < 0)
			pWrite_buf_transmit = TRANSMIT_BUFFER_SIZE - (pWrite-len);
		else
			pWrite_buf_transmit = (pWrite-len);
		OV_Write++;
	}


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
char ReadLastMessageFromRXBuffer(uint8_t *data, uint16_t length)
{
	//Look for last zero in RX Buffer
	int16_t lastZeroReceived = pWrite_buf_receive - 1;
	uint8_t packageComplete = 1;
	while(packageComplete)
	{
		while(buf_receive[lastZeroReceived] != 0x00)
		{
			if(lastZeroReceived < 0)
				lastZeroReceived = RECEIVE_BUFFER_SIZE;
			else if(lastZeroReceived == pWrite_buf_receive)
				return 0;
			lastZeroReceived--;
		}
		int16_t endOfMessage = lastZeroReceived;
		//Zero was found and is stored in 'lastZeroReceived'


		for(uint16_t offset = 0; offset < length; offset++)
		{
			if((endOfMessage - offset) < 0)
				endOfMessage = RECEIVE_BUFFER_SIZE + offset - 1;

			if(!(data[length - offset - 1] = buf_receive[endOfMessage - offset]))
				packageComplete = 0; //No Zero should be placed inside the Message
		}
		lastZeroReceived--; //Look for next Zero Position
	}
	return 1;



}
