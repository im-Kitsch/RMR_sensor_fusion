/*
 * protocol.h
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>
#include "sdk.h"

/*	--- Protocol Parameter --- */
#define num_8b 1	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define num_16b 2	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, ...) | Default: 0
#define num_32b 18	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define num_64b 0	//Number of 64 Bit - variables in protocol (for example: long*, double*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!

#define num_sum ( num_8b + 2 * num_16b + 4 * num_32b + 8 * num_64b)

typedef union {
	struct{
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...

		/*** 	--- Sensory Data --- ***/

		/* --- 8Bit data --- */
		/*  Example for adding a new variable: */
		// uint8_t newVariable;
		// Increase in '#define num_8b X'  X by one



		/* --- 16Bit data --- */

		/* --- 32Bit data --- */
		int angle_pitch;
		int angle_roll;
		int angle_yaw;
		int test_int_1;
		int test_int_2;
		int test_int_3;
		int test_int_4;
		int test_int_5;
		int test_int_6;
		int test_int_7;
		int test_int_8;
		int test_int_9;
		int test_int_10;
		int test_int_11;
		int test_int_12;

		uint16_t test_u16_1;
		uint16_t test_u16_2;

		float f1;
		float d1;

		/***	--- Protocol end --- ***/
		uint32_t checksum;
	}__attribute__ ((__packed__))protocol_s;

	uint8_t bytestream[num_sum];
}protocol_u;

//---------------------------------------------------------------------------------------------------------------------------------------------------
/* --- Control Data ---*/
//---------------------------------------------------------------------------------------------------------------------------------------------------

/*	--- Protocol Parameter --- */
#define Cnum_8b 9	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define Cnum_16b 0	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, ...) | Default: 0
#define Cnum_32b 1	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define Cnum_64b 0	//Number of 64 Bit - variables in protocol (for example: long*, double*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!

#define Cnum_sum ( Cnum_8b + 2 * Cnum_16b + 4 * Cnum_32b + 8 * Cnum_64b)

typedef union {
	struct {
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...

		/*** 	--- Control Data --- ***/

		/*  Example for adding a new variable: */
		// uint8_t newVariable;
		// Increase in '#define num_8b X'  X by one

		uint8_t motor[8];

		/***	--- Protocol end --- ***/
		uint32_t checksum;
	} __attribute__ ((__packed__))Cprotocol_s;

	uint8_t bytestream[num_sum];
}Cprotocol_u;


void refreshProtocolStream(protocol_u *proStream);
void generateChecksum(protocol_u *proStream);
char checkChecksum(protocol_u *proStream);
void generateChecksum_C(Cprotocol_u *proStream);
char checkChecksum_C(Cprotocol_u *proStream);


#endif /* PROTOCOL_H_ */
