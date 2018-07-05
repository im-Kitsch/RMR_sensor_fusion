/*
 * protocol.h
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>

/*	--- Protocol Parameter --- */
#define num_8b 7	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define num_16b 11	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, ...) | Default: 0
#define num_32b 10	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define num_64b 1	//Number of 64 Bit - variables in protocol (for example: long*, double*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!

#define num_sum ( num_8b + 2 * num_16b + 4 * num_32b + 8 * num_64b)

typedef union {
	struct{
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...

		uint64_t timeStamp_IMU_Sensory;

		/*** 	--- Sensory Data --- ***/

	//remote control data
		uint16_t channel[8];

	//angles derived by integration of gyro_outputs, drift compensated by data fusion; -90000..+90000 pitch(nick) and roll, 0..360000 yaw; 1000 = 1 degree
	    int angle_pitch;
	    int angle_roll;
	    int angle_yaw;

	//angular velocities, bias free, in 0.0154 °/s (=> 64.8 = 1 °/s)
	    int angvel_pitch;
	    int angvel_roll;
	    int angvel_yaw;

	//acc-sensor outputs, calibrated: -10000..+10000 = -1g..+1g, body frame coordinate system
	    int16_t acc_x;
	    int16_t acc_y;
	    int16_t acc_z;

	//magnetic field sensors output, offset free and scaled to +-2500 = +- earth field strength;
	    int Hx;
	    int Hy;
	    int Hz;
	    uint8_t motor_rpm[6];
		/***	--- Protocol end --- ***/
		uint32_t checksum;
	}__attribute__ ((__packed__))protocol_s;

	uint8_t bytestream[num_sum];
}protocol_u;

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

typedef union {
	struct {
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...
		//uint32_t timeStamp;

		/*** 	--- Control Data --- ***/

		/*  Example for adding a new variable: */
		// uint8_t newVariable;
		// Increase in '#define num_8b X'  X by one

		uint8_t pitch;
		uint8_t roll;
		uint8_t yaw;
		uint8_t thrust;

		/***	--- Protocol end --- ***/
		uint32_t checksum;
	} __attribute__ ((__packed__))Cprotocol_s;

	uint8_t bytestream[Cnum_sum];
}Cprotocol_u;



void generateChecksum(protocol_u *proStream);
char checkChecksum(protocol_u *proStream);
void generateChecksum_C(Cprotocol_u *proStream);
char checkChecksum_C(Cprotocol_u *proStream);

uint8_t unpack_message(void);
void update_message_with_IMU_Sensory(void);
void apply_message_to_UAV(void);
void pack_message(void);


//Protocol Variables
uint8_t latestMessage_received[Cnum_sum+2];
Cprotocol_u protocol_received;
protocol_u protocol_transmit;
uint8_t cobsByteStream[num_sum+2];

//Protocol Flags
uint8_t messagePacking;


#endif /* PROTOCOL_H_ */
