/*
 * protocol.h
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>
#include "sdk.h"

/*	--- Protocol Parameter --- */
#define num_8b 0
#define num_16b 2
#define num_32b 16
#define num_64b 0

#define num_sum ( num_8b + 2 * num_16b + 4 * num_32b + 8 * num_64b + 1)

#pragma pack(push,1)
typedef union {
	struct {
		/*	--- Protocol head --- */
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...

		uint16_t test_u16_1;
		uint16_t test_u16_2;

		/*	--- Sensory data --- */
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

		/*	--- Protocol end --- */
		uint32_t checksum;
	} protocol_s;

	uint8_t bytestream[num_sum];
}protocol_u;
#pragma pack(pop)

void refreshProtocolStream(protocol_u *proStream);
void generateChecksum(protocol_u *proStream);
char checkChecksum(protocol_u *proStream);


#endif /* PROTOCOL_H_ */
