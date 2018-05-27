/*
 * protocol.h
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>
#include "sdk.h"

/*	--- Protocol Parameter --- */
#define num_8b 0
#define num_16b 0
#define num_32b 3
#define num_64b 0

#define num_sum ( num_8b + 2 * num_16b + 4 * num_32b + 8 * num_64b + 3)

#pragma pack(push,1)
typedef union {
	struct {
		/*	--- Protocol head --- */
		uint8_t startByte;

		/*	--- Sensory data --- */
		int angle_pitch;
		int angle_roll;
		int angle_yaw;

		/*	--- Protocol end --- */
		uint8_t checksum;
		uint8_t endByte;

	} protocol_s;

	uint8_t bytestream[num_sum];
}protocol_u;
#pragma pack(pop)

void refreshProtocolStream(protocol_u *proStream);


#endif /* PROTOCOL_H_ */
