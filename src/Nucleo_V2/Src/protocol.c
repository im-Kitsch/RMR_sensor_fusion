/*
 * protocol.c
 */

#include "protocol.h"

/**
 * Refresh protocolStream with current sensor data.
 * @param proStream 	Protocol stream to be refreshed
 */
void refreshProtocolStream(protocol_u *proStream)
{
	/*
	proStream->protocol_s.angle_pitch = RO_ALL_Data.angle_pitch;
	proStream->protocol_s.angle_roll = RO_ALL_Data.angle_roll;
	proStream->protocol_s.angle_yaw = RO_ALL_Data.angle_yaw;
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

void changeMSB_LSB(protocol_u *proStream)
{
	/* --- 8Bit --- */
	//Nothing to change here

	/* --- 16Bit --- */
	for(int i = 0; i < num_16b; i++)
	{
		uint8_t nextPosition = (1+num_8b+(i*2));
		//swap
		uint8_t tmp = proStream->bytestream[nextPosition];
		proStream->bytestream[nextPosition] = proStream->bytestream[nextPosition+1];
		proStream->bytestream[nextPosition+1] = tmp;
	}

	/* --- 32Bit --- */
	for(int i = 0; i < num_32b; i++)
	{
		uint8_t nextPosition = (1+num_8b+2*num_16b+(i*4));
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
		uint8_t nextPosition = (1+num_8b+2*num_16b+4*num_32b+(i*8));
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

