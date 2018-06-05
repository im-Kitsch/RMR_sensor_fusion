/*
 * COBS.h
 *
 *  Created on: 16.05.2018
 *      Author: Malte
 */
#ifndef COBS_H_
#define COBS_H_

#include <stdio.h>
#include <stdlib.h>

size_t encode_COBS(const uint8_t *ptr, size_t len, uint8_t *dst);
size_t decode_COBS(const uint8_t *ptr, size_t len, uint8_t *dst);



#endif /* COBS_H_ */
