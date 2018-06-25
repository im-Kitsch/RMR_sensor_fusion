/*
 * protocol.h
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>

/*	--- Protocol Parameter --- */
#define num_8b 1	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define num_16b 2	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, ...) | Default: 0
#define num_32b 19	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define num_64b 0	//Number of 64 Bit - variables in protocol (for example: long*, double*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!

#define num_sum ( num_8b + 2 * num_16b + 4 * num_32b + 8 * num_64b)

#pragma pack(push,1) //Isn't working on HLP
typedef union {
	struct {
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...
		uint32_t timeStamp ;

		/*** 	--- Sensory Data --- ***/

		/*  Example for adding a new variable: */
		// uint8_t newVariable;
		// Increase in '#define num_8b X'  X by one

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
/*
		uint32_t protocol_falseCOBS_reality_false;		//Protocol detects error in COBS (and there is an error in reality)
		uint32_t protocol_falseCHECKSUM_reality_false;	//Protocol detects error in CHECKSUM (and there is an error in reality)
		uint32_t protocol_falseCOBS_reality_true;		//Protocol detects error in COBS, but the values are like expected (FAIL)
		uint32_t protocol_falseCHECKSUM_reality_true;	//Protocol detects error in CHECKSUM, but the values are like expected (FAIL)
		uint32_t protocol_true_reality_false;			//Protocol says yes, but there is an error in reality
		uint32_t protocol_true_reality_true;			//Protocol says yes and in reality it's true (That's what we want!)
*/

		/***	--- Protocol end --- ***/
		uint32_t checksum;
	} protocol_s;

	uint8_t bytestream[num_sum];
}protocol_u;
#pragma pack(pop)

//---------------------------------------------------------------------------------------------------------------------------------------------------
/* --- Control Data ---*/
//---------------------------------------------------------------------------------------------------------------------------------------------------

/*	--- Protocol Parameter --- */
#define Cnum_8b 9	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define Cnum_16b 0	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, ...) | Default: 0
#define Cnum_32b 2	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define Cnum_64b 0	//Number of 64 Bit - variables in protocol (for example: long*, double*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!

#define Cnum_sum ( Cnum_8b + 2 * Cnum_16b + 4 * Cnum_32b + 8 * Cnum_64b)

#pragma pack(push,1)
typedef union {
	struct {
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...
		uint32_t timeStamp ;

		/*** 	--- Control Data --- ***/

		/*  Example for adding a new variable: */
		// uint8_t newVariable;
		// Increase in '#define num_8b X'  X by one

		uint8_t motor[8];

		/***	--- Protocol end --- ***/
		uint32_t checksum;
	} Cprotocol_s;

	uint8_t bytestream[Cnum_sum];
}Cprotocol_u;
#pragma pack(pop)

void refreshProtocolStream(protocol_u *proStream);
void generateChecksum(protocol_u *proStream);
char checkChecksum(protocol_u *proStream);
void changeMSB_LSB(protocol_u *proStream);
void generateChecksum_C(Cprotocol_u *proStream);
char checkChecksum_C(Cprotocol_u *proStream);


#endif /* PROTOCOL_H_ */
