/*
 * protocol.c
 */

#include "sdk.h"
#include "protocol.h"
#include "LPC214x.h"
#include "benchmark.h"

/**
 * Refresh protocolStream with current sensor data.
 * @param proStream 	Protocol stream to be refreshed
 */
void refreshProtocolStream(protocol_u *proStream)
{
	/* Sensor READ
	proStream->protocol_s.angle_pitch = RO_ALL_Data.angle_pitch;
	proStream->protocol_s.angle_roll = RO_ALL_Data.angle_roll;
	proStream->protocol_s.angle_yaw = RO_ALL_Data.angle_yaw;
	*/

	//TETSTING SEQUENCE
	proStream->protocol_s.startByte = 0x01;
	//proStream->protocol_s.timeStamp = T1TC;
	proStream->protocol_s.angle_pitch = 1;
	proStream->protocol_s.angle_roll = 2;
	proStream->protocol_s.angle_yaw = 3;
	proStream->protocol_s.test_int_1 = 4;
	proStream->protocol_s.test_int_2 = 5;
	proStream->protocol_s.test_int_3 = 6;
	proStream->protocol_s.test_int_4 = 7;
	proStream->protocol_s.test_int_5 = 8;
	proStream->protocol_s.test_int_6 = 9;
	proStream->protocol_s.test_int_7 = 10;
	proStream->protocol_s.test_int_8 = 11;
	proStream->protocol_s.test_int_9 = 12;
	proStream->protocol_s.test_int_10 = 13;
	proStream->protocol_s.test_int_11 = 14;
	proStream->protocol_s.test_int_12 = 15;

	proStream->protocol_s.test_u16_1 = 16;
	proStream->protocol_s.test_u16_2 = 17;
	proStream->protocol_s.f1 = 20.1;
	proStream->protocol_s.d1 = 55.6;
/*
	proStream->protocol_s.protocol_falseCHECKSUM_reality_false = testSeries[0].protocol_falseCHECKSUM_reality_false;
	proStream->protocol_s.protocol_falseCOBS_reality_false = testSeries[0].protocol_falseCOBS_reality_false;
	proStream->protocol_s.protocol_falseCHECKSUM_reality_true = testSeries[0].protocol_falseCHECKSUM_reality_true;
	proStream->protocol_s.protocol_falseCOBS_reality_true = testSeries[0].protocol_falseCOBS_reality_true;
	proStream->protocol_s.protocol_true_reality_false = testSeries[0].protocol_true_reality_false;
	proStream->protocol_s.protocol_true_reality_true = testSeries[0].protocol_falseCOBS_reality_true;
*/

}

/**
 * Generate a checksum calculated from sensory data, stored at the end of protocol's stream. 
 * @param proStream 	protocol stream the checksum will be calculated for
 */
void generateChecksum(protocol_u *proStream)
{
	//ToDo: Implement a CRC or a Fletchers's Checksum
	
	//Now used: Simple Additional Checksum
	uint32_t checksum = 0;
	for(uint8_t i = 0; i < num_sum-4; i++)
		checksum += proStream->bytestream[i]; //Overflow is accepted
	
	//Store checksum in protocolByteStream
	proStream->protocol_s.checksum = checksum;
}

/**
 * Generate a checksum calculated from sensory data, stored at the end of protocol's stream. 
 * @param proStream 	protocol stream to be checked for correct checksum
 * @return '1' for correct checksum, '0' for incorrect checksum
 */
char checkChecksum(protocol_u *proStream)
{
	//ToDo: Implement a CRC or a Fletchers's Checksum
	
	//Now used: Simple Additional Checksum
	uint32_t checksum = 0;
	for(uint8_t i = 0; i < num_sum-4; i++)
		checksum += proStream->bytestream[i]; //Overflow is accepted
	
	//Check if the calculated checksum equals the received checksum
	if(checksum == proStream->protocol_s.checksum)
		return 1;
	else
		return 0;
}

void generateChecksum_C(Cprotocol_u *proStream)
{
	//ToDo: Implement a CRC or a Fletchers's Checksum

	//Now used: Simple Additional Checksum
	uint32_t checksum = 0;
	for(uint8_t i = 0; i < Cnum_sum-4; i++)
		checksum += proStream->bytestream[i]; //Overflow is accepted

	//Store checksum in protocolByteStream
	proStream->Cprotocol_s.checksum = checksum;
}

/**
 * Generate a checksum calculated from sensory data, stored at the end of protocol's stream.
 * @param proStream 	protocol stream to be checked for correct checksum
 * @return '1' for correct checksum, '0' for incorrect checksum
 */
char checkChecksum_C(Cprotocol_u *proStream)
{
	//ToDo: Implement a CRC or a Fletchers's Checksum

	//Now used: Simple Additional Checksum
	uint32_t checksum = 0;
	for(uint8_t i = 0; i < Cnum_sum-4; i++)
		checksum += proStream->bytestream[i]; //Overflow is accepted

	//Check if the calculated checksum equals the received checksum
	if(checksum == proStream->Cprotocol_s.checksum)
		return 1;
	else
		return 0;
}

void changeMSB_LSB(protocol_u *proStream)
{
	/* --- 8Bit --- */
	//Nothing to change here

	/* --- 16Bit --- */
	for(int i = 0; i < num_16b; i++)
	{
		uint8_t nextPosition = (num_8b+(i*2));
		//swap
		uint8_t tmp = proStream->bytestream[nextPosition];
		proStream->bytestream[nextPosition] = proStream->bytestream[nextPosition+1];
		proStream->bytestream[nextPosition+1] = tmp;
	}

	/* --- 32Bit --- */
	for(int i = 0; i < num_32b; i++)
	{
		uint8_t nextPosition = (num_8b+2*num_16b+(i*4));
		//swap
		uint8_t tmp = proStream->bytestream[nextPosition];
		proStream->bytestream[nextPosition] = proStream->bytestream[nextPosition+3];
		proStream->bytestream[nextPosition+3] = tmp;
		tmp = proStream->bytestream[nextPosition+1];
		proStream->bytestream[nextPosition+1] = proStream->bytestream[nextPosition+2];
		proStream->bytestream[nextPosition+2] = tmp;
	}

	/* --- 64Bit --- */
	for(int i = 0; i < num_64b; i++)
	{
		uint8_t nextPosition = (num_8b+2*num_16b+4*num_32b+(i*8));
		//swap
		uint8_t tmp = proStream->bytestream[nextPosition];
		proStream->bytestream[nextPosition] = proStream->bytestream[nextPosition+7];
		proStream->bytestream[nextPosition+7] = tmp;
		tmp = proStream->bytestream[nextPosition+1];
		proStream->bytestream[nextPosition+1] = proStream->bytestream[nextPosition+6];
		proStream->bytestream[nextPosition+6] = tmp;
		tmp = proStream->bytestream[nextPosition+2];
		proStream->bytestream[nextPosition+2] = proStream->bytestream[nextPosition+5];
		proStream->bytestream[nextPosition+5] = tmp;
		tmp = proStream->bytestream[nextPosition+3];
		proStream->bytestream[nextPosition+3] = proStream->bytestream[nextPosition+4];
		proStream->bytestream[nextPosition+4] = tmp;
	}
}

