/*
 * protocol.h
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>

/*	--- Protocol Parameter --- */
#define num_8b 7	//Number of 8 Bit - variables in protocol (for example: char, uint8_t, int8_t, ...) | Default: 1
#define num_16b 15	//Number of 16 Bit - variables in protocol (for example: uint16_t, int16_t, (unsigned) short*, ...) | Default: 0
#define num_32b 29	//Number of 32 Bit - variables in protocol (for example: (unsigned) int, float*, uint32_t, int32_t, ...) | Default: 1
#define num_64b 4	//Number of 64 Bit - variables in protocol (for example: long*, uint64_t*, int64_t*, ...) | Default: 0
					// (*) not tested, yet!, DON'T use doubles, use float instead!

#define num_sum ( num_8b + 2 * num_16b + 4 * num_32b + 8 * num_64b)

typedef union {
	struct{
		/***	--- Protocol head --- ***/
		uint8_t startByte; //Status Flags; 0: Valid, 1: ...


		/*** 	--- Sensory Data --- ***/

		/* --- IMU --- */
			uint64_t IMU_timeStamp;

			//remote control data
			uint16_t IMU_channel[8];

			//angles derived by integration of gyro_outputs, drift compensated by data fusion; -90000..+90000 pitch(nick) and roll, 0..360000 yaw; 1000 = 1 degree
			int IMU_angle_pitch;
			int IMU_angle_roll;
			int IMU_angle_yaw;

			//angular velocities, bias free, in 0.0154 °/s (=> 64.8 = 1 °/s)
			int IMU_angvel_pitch;
			int IMU_angvel_roll;
			int IMU_angvel_yaw;

			//acc-sensor outputs, calibrated: -10000..+10000 = -1g..+1g, body frame coordinate system
			int16_t IMU_acc_x;
			int16_t IMU_acc_y;
			int16_t IMU_acc_z;

			//magnetic field sensors output, offset free and scaled to +-2500 = +- earth field strength;
			int IMU_Hx;
			int IMU_Hy;
			int IMU_Hz;

			//RPM measurements (0..200)
			int8_t IMU_motor_rpm[6];

		/* --- GPS Data --- */
			uint32_t GPS_timeStamp; //GPS_time_of_week;	[ms] (1 week = 604,800 s)

			//latitude/longitude in degrees * 10^7
			int GPS_latitude;
			int GPS_longitude;

			//GPS height in mm
			int GPS_height;

			//speed in x (E/W) and y(N/S) in mm/s
			int GPS_speed_x;
			int GPS_speed_y;

			//GPS heading in deg * 1000
			int GPS_heading;

			//accuracy estimates in mm and mm/s
			unsigned int GPS_position_accuracy;
			unsigned int GPS_height_accuracy;
			unsigned int GPS_speed_accuracy;

			//number of satellites used in NAV solution
			unsigned int GPS_sat_num;

			// GPS status information: Bit7...Bit3: 0; Bit 2: longitude direction; Bit1: latitude direction; Bit 0: GPS lock
			int GPS_status;

		/* --- PX4FLOW Data, optical flow measurement --- */
			uint64_t FLOW_timeStamp;
			int16_t FLOW_comp_m_x;// x velocity*1000 [meters/sec]
			int16_t FLOW_comp_m_y;// y velocity*1000 [meters/sec]
			int16_t FLOW_qual;// Optical flow quality / confidence [0: bad, 255: maximum quality]

		/* --- Lidar Lite V3 --- */
			uint64_t LIDAR_timeStamp;
			int16_t LIDAR_distance; // Distance to the ground, cm

		/* --- sensorData from Camera(via Mastermind) --- */
			uint64_t cam_timestamp;

			float cam_x;
			float cam_y;
			float cam_z;

			float cam_quanternion_l;
			float cam_quanternion_i;
			float cam_quanternion_j;
			float cam_quanternion_k;
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
