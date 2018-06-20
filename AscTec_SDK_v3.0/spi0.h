/*
 * spi0.h
 *
 *  Created on: 18.05.2018
 *      Author: Malte
 */

#ifndef SPI0_H_
#define SPI0_H_

#include <stdint.h>

void SPI0_Master_Init(void);
void SPI_Master_SingleWrite(uint8_t data);
uint8_t SPI_Master_SingleRead(void);
void SPI_Master_Write(uint8_t *data, uint8_t length);
void SPI_Master_Read(uint8_t *dst, uint8_t length);
char SPI_Master_WriteRead(uint8_t *data ,uint8_t *dst, uint8_t length);
char SPI_Master_WriteCheck(uint8_t *dst, uint8_t length);
char SPI_Master_ReadCheck(uint8_t *dst, uint8_t length);
extern void SPI0Handler (void);


#endif /* SPI0_H_ */
