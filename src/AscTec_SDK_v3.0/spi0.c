/*
 * spi0.c
 *
 *  Created on: 18.05.2018
 *      Author: Malte
 */

#include "LPC214x.h"			/* LPC21XX Peripheral Registers	*/
#include "type.h"
#include "irq.h"
#include "spi0.h"
#include "main.h"
#include "system.h"
#include "sdk.h"
#include <stdint.h>
#include "ringbuffer.h"

void SPI0Handler (void) __irq
{

	IENABLE;				/* handles nested interrupt */
	transfer_inProgress = 1;

	/* Check for pointer Overruns */
	if(pRead_buf_transmit >= TRANSMIT_BUFFER_SIZE)
		pRead_buf_transmit = 0;
	if(pWrite_buf_receive >= RECEIVE_BUFFER_SIZE)
		pWrite_buf_receive = 0;

	/* Check for nothing more to transmit */
	if(pRead_buf_transmit == pWrite_buf_transmit)
	{
		//Read last received Value
		buf_receive[pWrite_buf_receive] = S0SPDR;
		pWrite_buf_receive = (pWrite_buf_receive + 1)%100;
		transfer_inProgress = 0;

		//Disable Interrupt, because there is nothing to transmit at this moment.
		//S0SPCR &= !(1<<7); //Reset SPIE - Bit
		IDISABLE;
		VICVectAddr = 0;	/* Acknowledge Interrupt */
		S0SPINT = 0x01;	/* clear interrupt Flag */
		//IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
		return;
	}


    unsigned short statusReg = S0SPSR;
    if(statusReg & (1<<7)) //SPI TRANSFER COMPLETE FLAG
    {

			//Read the last received Value
			buf_receive[pWrite_buf_receive++] = S0SPDR;

			//Write new Data into data register
			S0SPDR = buf_transmit[pRead_buf_transmit++];


    }else //Mode or write collision Flag
    {
    	uint8_t dummy = S0SPDR; // dummy read to clear flags
    }

    IDISABLE;
    VICVectAddr = 0;	/* Acknowledge Interrupt */
    S0SPINT = 0x01;	/* clear interrupt Flag */
}

/**
 * Initialize SPI periphery as master in default configuration.
 */
void SPI0_Master_Init(void)
{
	PINSEL0 = PINSEL0 | 0x00001500; /* Select P0.4 -> SCK0, P0.5 -> MISO0, P0.6 -> MOSI0, P0.7 -> GPIO(SS)*/
	S0SPCR = 0x00A0; /* Master mode, 8-bit frames, SPI0 mode, Interrupt enable */
	S0SPCCR = 0x0E; /* Even number, minimum value 8, pre scalar for SPI Clock */

	OverrunC = 0;
}

/**
 * Transmit a single Byte to slave.
 * @param data		Byte to be transmitted
 */
void SPI_Master_SingleWrite(uint8_t data)
{
	uint8_t flush; //Nothing to receive
	IOCLR0 = (1<<7);  /* SS(GPIO) to '0', enable SPI communication */
	S0SPDR = data;  /* Load data to be written into the data register */
	while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed, SPIF = 1? */
	flush = S0SPDR;
	IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
}

/**
 * Receive a single Byte from slave.
 * @return received Byte
 */
uint8_t SPI_Master_SingleRead(void)
{
	uint8_t flush; //Nothing to receive
	IOCLR0 = (1<<7);  /* SS(GPIO) to '0', enable SPI communication */
	S0SPDR = 0xFF;  /* Load data to be written into the data register */
	while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed, SPIF = 1? */
	flush = S0SPDR;
	IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
}

/**
 * Transmit a Byte array to slave.
 * @param *data 	Byte array to be transmitted
 * @param length 	Size of Byte array
 */
void SPI_Master_Write(uint8_t *data, uint8_t length)
{
	uint8_t flush; //Nothing to receive
	IOCLR0 = (1<<7);  /* SS(GPIO) to '0', enable SPI communication */

	for(int i = 0; i < length; i++)
	{
		S0SPDR = *(data + i);  /* Load data to be written into the data register */
		while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed , SPIF = 1?*/
		flush = S0SPDR;
	}

	IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
}

/**
 * Receive a Byte array from slave.
 * @param *data		 Byte array to store received data
 * @param length	 Length of Byte array
 */
void SPI_Master_Read(uint8_t *dst, uint8_t length)
{
	uint8_t flush = 0xFF; //Nothing to transmit
	IOCLR0 = (1<<7);  /* SS(GPIO) to '0', enable SPI communication */

	for(int i = 0; i < length; i++)
	{
		S0SPDR = flush;  /* Nothing to transmit */
		while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed , SPIF = 1?*/
		*(dst + i) = S0SPDR;
	}

	IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
}

/**
 * Transmit and receive data from/to slave, return error code.
 * @param *data 	Pointer to data-array to be transmitted
 * @param *dst 		Pointer to destination-array to be received
 * @param length 	Length of data and destination array
 * @return '1' for successful transfer/receive, '0' for Read/Write overrun
 */
char SPI_Master_WriteRead(uint8_t *data ,uint8_t *dst, uint8_t length)
{
	char feedback = 1;
	IOCLR0 = (1<<7);  /* SS(GPIO) to '0', enable SPI communication */

	for(int i = 0; i < length; i++)
	{
		if(S0SPSR & 0xE0)
			feedback = 0; //Remember Read/Write overrun
		S0SPDR = *(data + i);  /* Nothing to transmit */
		while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed , SPIF = 1?*/
		*(dst + i) = S0SPDR;
	}

	IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
	return feedback;
}

/**
 * Transmit data to slave, check returned values. Slave needs to answer the exact received value immediately.
 * @param *data 	Pointer to data-array to be transmitted
 * @param length 	Length of data array
 * @return '0' for successful transfer/receive, '1' for Read/Write overrun, '2' for error detected in transmission
 */
char SPI_Master_WriteCheck(uint8_t *data, uint8_t length)
{
	char feedback = 0;
	uint8_t checkValue = 0;

	IOCLR0 = (1<<7);  /* SS(GPIO) to '0', enable SPI communication */

	for(int i = 0; i < length; i++)
	{
		if(S0SPSR & 0xE0)
			feedback = 1; //Remember Read/Write overrun
		if(i > 0 && checkValue != *(data + i - 1))
			feedback = 2; //Remember Error in transmission
		S0SPDR = *(data + i);
		while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed , SPIF = 1?*/
		checkValue = S0SPDR; //Read received value
	}

	IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
	return feedback;
}

/**
 * Receive data from slave, send values back for checking reasons.
 * @param *dst 		Pointer to destination array to be received
 * @param length 	Length of destination array
 * @return '0' for successful transfer/receive, '1' for Read/Write overrun
 */
char SPI_Master_ReadCheck(uint8_t *dst, uint8_t length)
{
	char feedback = 0;

	IOCLR0 = (1<<7);  /* SS(GPIO) to '0', enable SPI communication */

	for(int i = 0; i < length; i++)
	{
		if(S0SPSR & 0xE0)
			feedback = 1; //Remember Read/Write overrun

		S0SPDR = *(dst + i - 1);  /* Transmit last received Value */
		while ( (S0SPSR & 0x80) == 0 );  /* Wait till data transmission is completed , SPIF = 1?*/
		*(dst + i) = S0SPDR; //Read received value
	}

	IOSET0 = (1<<7);  /* SS(GPIO) to '1', disable SPI communication */
	return feedback;
}





