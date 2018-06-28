/*
 * protocol.h
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>
#include "sdk.h"

/*	--- Protocol Parameter --- */
#define num_8b 1	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define num_16b 11	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, (unsigned) short*, ...) | Default: 0
#define num_32b 10	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define num_64b 0	//Number of 64 Bit - variables in protocol (for example: long*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!, DON'T use doubles, use float instead!

#define num_sum ( num_8b + 2 * num_16b + 4 * num_32b + 8 * num_64b)

typedef union {
	struct{
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...
		uint64_t timeStamp_IMU_Sensory;

		/*** 	--- Sensory Data --- ***/

	//remote control data
		unsigned short channel[8];
		/*
		 * channel[0]: pitch
		 * channel[1]: roll
		 * channel[2]: thrust
		 * channel[3]: yaw
		 * channel[4]: serial interface enable/disable (>2048 enabled)
		 * channel[5]: manual / height control / GPS + height control (0 -> manual mode; 2048 -> height mode; 4095 -> GPS mode)
		 *
		 * range of each channel: 0..4095
		 */

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

	//RPM measurements (0..200)
	/*
	 * Hexcopter (AscTec Firefly)
	 *
	 * measurement equals a real rpm of motor_rpm*64
	 *
	 * motor[0]: front-left
	 * motor[1]: left
	 * motor[2]: rear-left
	 * motor[3]: rear-right
	 * motor[4]: right
	 * motor[5]: front-right
	 *
	 */
	    unsigned char motor_rpm[6];

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
		//uint32_t timeStamp;

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
