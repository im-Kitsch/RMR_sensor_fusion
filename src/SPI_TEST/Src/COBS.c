/*
 * COBS.c
 *
 *  Created on: 16.05.2018
 *      Author: Malte
 */

#include "COBS.h"

#define StartBlock()	(code_ptr = dst++, code = 1) //Shift leading count, reset 'code'
#define FinishBlock()	(*code_ptr = code) // Setting the code_ptr to position of '0x00'

size_t encode_COBS(const uint8_t *ptr, size_t len, uint8_t *dst) {
	const uint8_t *start = dst; //Save startposition of destination array, dst as run pointer
	const uint8_t *end = ptr + len; //Save endposition of destination array, dst as run pointer
	uint8_t code, *code_ptr; // Where to insert the leading count

	StartBlock();
	while (ptr < end) {
		if (code != 0xFF) {
			uint8_t c = *ptr++;
			if (c != 0) { //Check if inputstream contains '0x00' at current position
				*dst++ = c;
				code++;
				continue; // don't run 'FinishBlock()' & 'StartBlock()'
			}
		}
		FinishBlock();
		StartBlock();
	}
	FinishBlock();
	return dst - start;
}

size_t decode_COBS(const uint8_t *ptr, size_t length, uint8_t *dst)
{
	const uint8_t *start = dst; //Save startposition of destination array, dst as run pointer
	const uint8_t *end = ptr + length; //Save endposition of destination array, dst as run pointer
	uint8_t code = 0xFF, copy = 0;

	for (; ptr < end; copy--) {
		if (copy != 0) {
			*dst++ = *ptr++;
		} else {
			if (code != 0xFF)
				*dst++ = 0; //Insert '0'
			copy = code = *ptr++; //Estimante next '0'
			if (code == 0)
				break; /* Source length too long */
		}
	}
	return dst - start;
}
