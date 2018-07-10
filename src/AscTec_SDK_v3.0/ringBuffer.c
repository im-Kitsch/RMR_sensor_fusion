/*
 * ringBuffer.c
 *
 *  Created on: 16.06.2018
 *      Author: Malte
 */

#include "ringbuffer.h"
#include "protocol.h"
#include "LPC214x.h"
#include <stdint.h>
#include "spi0.h"

extern uint16_t pWrite_buf_transmit;
extern uint8_t buf_transmit[];


void pushToTXBuffer(uint8_t *data, uint16_t length)
{
	IOCLR0 = (1<<7);  /* SS(GPIO) to '0', enable SPI communication */

		int32_t pWrite = pWrite_buf_transmit;
		int32_t pRead = pRead_buf_transmit;
		int32_t len = length;

		//Flush last message of too slow down transfer (increase transmittBuffer!)
		if(pWrite > pRead && (pRead - pWrite + TRANSMIT_BUFFER_SIZE) < len)
		{
			if((pWrite-len) < 0)
				pWrite_buf_transmit = TRANSMIT_BUFFER_SIZE - (pWrite-len);
			else
				pWrite_buf_transmit = (pWrite-len);
			FlushC++;
		}
		else if(pWrite < pRead && (pRead - pWrite) < len)
		{
			if((pWrite-len) < 0)
				pWrite_buf_transmit = TRANSMIT_BUFFER_SIZE - (pWrite-len);
			else
				pWrite_buf_transmit = (pWrite-len);
			FlushC++;
		}

	for(uint16_t index_data = 0; index_data < length; index_data++)
	{
		if(pWrite_buf_transmit == pRead_buf_transmit-1)
		{
			/* --- Interrupt is not working anymore (Interrupt Nesting failed) -> Reset the SPI Peripheral --- */
			if(!transfer_inProgress)
			{
				SPI0_Master_Init();
				uint8_t statusReg = S0SPSR;
				uint8_t flush = S0SPDR;
				S0SPDR = 0xFF;  /* Load data to be written into the data register */

			}
			OverrunC++; //Should be '0' all the time
		}

		buf_transmit[pWrite_buf_transmit++] = data[index_data];
		if(pWrite_buf_transmit >= TRANSMIT_BUFFER_SIZE)
		{
			pWrite_buf_transmit = 0;
		}
	}

	//Enable TX Interrupt & Activate SPI Master again
	//S0SPCR |= (1<<5); //Returning to Master mode
	//S0SPCR |= (1<<7); //Set SPIE - Bit

	//Initial Read / Write
	//SPI_Master_SingleWrite(0xEE);

	//uint8_t flush; //Nothing to receive
	//uint8_t flush = S0SPDR;

	if(!transfer_inProgress)
	{
		//Enable the transfer again if it is stopped
		uint8_t statusReg = S0SPSR;
		uint8_t flash = S0SPDR;
		S0SPDR = 0xFF;  /* Load data to be written into the data register */
	}

	//S0SPDR = 0xFF;  /* Load data to be written into the data register */
	//uint32_t timeout = 0;
	//while ( ((S0SPSR & 0x80) == 0) && (timeout++ < (1<<16)));  /* Wait till data transmission is completed, SPIF = 1? */
	//flush = S0SPDR;
	//IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
	//IOCLR0 = (1<<7); /* SS(GPIO) to '0', enable SPI communication */
	//Read the last received Value
	//buf_receive[pWrite_buf_receive++] = S0SPDR;
	//Write new Data into data register
	//S0SPDR = buf_transmit[pRead_buf_transmit++];
}

/**
 * Writes the last message into *data.
 * @param *data Requested Message is inserted here
 * @return '0' for no message (0x00) found, '1' for message (0x00) found
 */
uint8_t ReadLastMessageFromRXBuffer(uint8_t *data, uint8_t length)
{
	//Look for last zero in RX Buffer
	uint16_t lastZeroReceived = pWrite_buf_receive - 1;
	if(!pWrite_buf_receive)
		lastZeroReceived = RECEIVE_BUFFER_SIZE - 1;

	uint8_t packageComplete = 1;
	while(packageComplete)
	{
		while(buf_receive[lastZeroReceived] != 0x00)
		{
			if(lastZeroReceived == pWrite_buf_receive)
							return 0;
			if(lastZeroReceived == 0x0000)
				lastZeroReceived = RECEIVE_BUFFER_SIZE;

			lastZeroReceived--;
		}
		uint16_t endOfMessage = lastZeroReceived;
		//Zero was found and is stored in 'lastZeroReceived'


		for(uint16_t offset = 0; offset < length; offset++)
		{
			if((endOfMessage - offset) < 0)
				endOfMessage = RECEIVE_BUFFER_SIZE + offset - 1;

			if(!(data[length - offset - 1] = buf_receive[endOfMessage - offset]))
				packageComplete = 0; //No Zero should be placed inside the Message
		}
		if(lastZeroReceived != 0)
			lastZeroReceived--; //Look for next Zero Position
		else
			lastZeroReceived = RECEIVE_BUFFER_SIZE - 1;
	}
	return 1;



}
