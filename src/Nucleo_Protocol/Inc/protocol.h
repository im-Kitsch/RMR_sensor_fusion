/*
 * protocol.h
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>

/*	--- Protocol Parameter --- */
#define num_8b 1	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define num_16b 2	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, ...) | Default: 0
#define num_32b 25	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define num_64b 0	//Number of 64 Bit - variables in protocol (for example: long*, double*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!

#define num_sum ( num_8b + 2 * num_16b + 4 * num_32b + 8 * num_64b)

#pragma pack(push,1) //Isn't working on HLP
typedef union {
	struct {
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...

		uint64_t timeStamp_IMU_Sensory;

		/*** 	--- Sensory Data --- ***/

	//remote control data
		unsigned short channel[8];

	//angles derived by integration of gyro_outputs, drift compensated by data fusion; -90000..+90000 pitch(nick) and roll, 0..360000 yaw; 1000 = 1 degree
		int angle_pitch;
		int angle_roll;
		int angle_yaw;

	//angular velocities, bias free, in 0.0154 °/s (=> 64.8 = 1 °/s)
		int angvel_pitch;
		int angvel_roll;
		int angvel_yaw;

	//acc-sensor outputs, calibrated: -10000..+10000 = -1g..+1g, body frame coordinate system
		short acc_x;
		short acc_y;
		short acc_z;

	//magnetic field sensors output, offset free and scaled to +-2500 = +- earth field strength;
		int Hx;
		int Hy;
		int Hz;
		unsigned char motor_rpm[6];

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
#define Cnum_8b 5	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define Cnum_16b 0	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, ...) | Default: 0
#define Cnum_32b 1	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define Cnum_64b 0	//Number of 64 Bit - variables in protocol (for example: long*, double*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!

#define Cnum_sum ( Cnum_8b + 2 * Cnum_16b + 4 * Cnum_32b + 8 * Cnum_64b)

#pragma pack(push,1)
typedef union {
	struct {
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...

		/*** 	--- Control Data --- ***/

		/*  Example for adding a new variable: */
		// uint8_t newVariable;
		// Increase in '#define num_8b X'  X by one
		unsigned char pitch;
		unsigned char roll;
		unsigned char yaw;
		unsigned char thrust;


		/***	--- Protocol end --- ***/
		uint32_t checksum;
	} Cprotocol_s;

	uint8_t bytestream[Cnum_sum];
}Cprotocol_u;
#pragma pack(pop)

void generateChecksum(protocol_u *proStream);
char checkChecksum(protocol_u *proStream);
void generateChecksum_C(Cprotocol_u *proStream);
char checkChecksum_C(Cprotocol_u *proStream);

void update_sensory_struct(void);
uint8_t unpack_message(void);
void pack_message(void);

/* --- Protocol Variables --- */
uint8_t latestMessage_received[Cnum_sum+2];
protocol_u protocol_received;
Cprotocol_u protocol_transmit;
uint8_t cobsByteStream[num_sum+2];

/* --- Protocol Flags --- */
uint8_t messagePacking;

#define TIMEOUT 100000


#endif /* PROTOCOL_H_ */
