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

	proStream->protocol_s.channel[0] = RO_ALL_Data.channel[0];
	proStream->protocol_s.channel[1] = RO_ALL_Data.channel[1];
	proStream->protocol_s.channel[2] = RO_ALL_Data.channel[2];
	proStream->protocol_s.channel[3] = RO_ALL_Data.channel[3];
	proStream->protocol_s.channel[4] = RO_ALL_Data.channel[4];
	proStream->protocol_s.channel[5] = RO_ALL_Data.channel[5];
	proStream->protocol_s.channel[6] = RO_ALL_Data.channel[6];
	proStream->protocol_s.channel[7] = RO_ALL_Data.channel[7];
	proStream->protocol_s.angle_pitch = RO_ALL_Data.angle_pitch;
	proStream->protocol_s.angle_roll = RO_ALL_Data.angle_roll;
	proStream->protocol_s.angle_yaw = RO_ALL_Data.angle_yaw;
	proStream->protocol_s.angvel_pitch = RO_ALL_Data.angvel_pitch;
	proStream->protocol_s.angvel_roll = RO_ALL_Data.angvel_roll;
	proStream->protocol_s.angvel_yaw = RO_ALL_Data.angvel_yaw;
	proStream->protocol_s.acc_x = RO_ALL_Data.acc_x;
	proStream->protocol_s.acc_y = RO_ALL_Data.acc_y;
	proStream->protocol_s.acc_z = RO_ALL_Data.acc_z;
	proStream->protocol_s.Hx = RO_ALL_Data.Hx;
	proStream->protocol_s.Hy = RO_ALL_Data.Hy;
	proStream->protocol_s.Hz = RO_ALL_Data.Hz;
	proStream->protocol_s.motor_rpm[0] = RO_ALL_Data.motor_rpm[0];
	proStream->protocol_s.motor_rpm[1] = RO_ALL_Data.motor_rpm[1];
	proStream->protocol_s.motor_rpm[2] = RO_ALL_Data.motor_rpm[2];
	proStream->protocol_s.motor_rpm[3] = RO_ALL_Data.motor_rpm[3];
	proStream->protocol_s.motor_rpm[4] = RO_ALL_Data.motor_rpm[4];
	proStream->protocol_s.motor_rpm[5] = RO_ALL_Data.motor_rpm[5];
	

}

/**
 * Generate a checksum calculated from sensory data, stored at the end of protocol's stream. 
 * @param proStream 	protocol stream the checksum will be calculated for
 */
void generateChecksum(protocol_u *proStream)
{
	//ToDo: Implement a CRC or a Fletchers's Checksum
	
	//Now used: Fletcher's Checksum (optimized, source: "http://www.drdobbs.com/database/fletchers-checksum/")
	register unsigned char *ptr = proStream->bytestream;
    register short int sum1, len = num_sum-4;
    register unsigned int sum2 ;

    sum1 = sum2 = 0;
    while (len--)
    {
         sum1 += *ptr++;
         if (sum1 >= 255) sum1 -= 255;
         sum2 += sum1;
    }
    sum2 %= 255;
	
	
	
	//Store checksum in protocolByteStream
	proStream->protocol_s.checksum = (uint32_t)sum1 + ((uint32_t)sum2<<16);
}

/**
 * Generate a checksum calculated from sensory data, stored at the end of protocol's stream. 
 * @param proStream 	protocol stream to be checked for correct checksum
 * @return '1' for correct checksum, '0' for incorrect checksum
 */
char checkChecksum(protocol_u *proStream)
{
	//ToDo: Implement a CRC or a Fletchers's Checksum
	
	//Now used: Fletcher's Checksum (optimized, source: "http://www.drdobbs.com/database/fletchers-checksum/")
	register unsigned char *ptr = proStream->bytestream;
    register short int sum1, len = num_sum-4;
    register unsigned int sum2 ;

    sum1 = sum2 = 0;
    while (len--)
    {
         sum1 += *ptr++;
         if (sum1 >= 255) sum1 -= 255;
         sum2 += sum1;
    }
    sum2 %= 255;
	
	//Check if the calculated checksum equals the received checksum
	if(((uint32_t)sum1 + ((uint32_t)sum2<<16)) == proStream->protocol_s.checksum)
		return 1;
	else
		return 0;
}

void generateChecksum_C(Cprotocol_u *proStream)
{
	//ToDo: Implement a CRC or a Fletchers's Checksum
	
	//Now used: Fletcher's Checksum (optimized, source: "http://www.drdobbs.com/database/fletchers-checksum/")
	register unsigned char *ptr = proStream->bytestream;
    register short int sum1, len = Cnum_sum-4;
    register unsigned int sum2 ;

    sum1 = sum2 = 0;
    while (len--)
    {
         sum1 += *ptr++;
         if (sum1 >= 255) sum1 -= 255;
         sum2 += sum1;
    }
    sum2 %= 255;
	
	
	
	//Store checksum in protocolByteStream
	proStream->protocol_s.checksum = (uint32_t)sum1 + ((uint32_t)sum2<<16);
}

/**
 * Generate a checksum calculated from sensory data, stored at the end of protocol's stream.
 * @param proStream 	protocol stream to be checked for correct checksum
 * @return '1' for correct checksum, '0' for incorrect checksum
 */
char checkChecksum_C(Cprotocol_u *proStream)
{
	//ToDo: Implement a CRC or a Fletchers's Checksum

	//Now used: Fletcher's Checksum (optimized, source: "http://www.drdobbs.com/database/fletchers-checksum/")
	register unsigned char *ptr = proStream->bytestream;
    register short int sum1, len = Cnum_sum-4;
    register unsigned int sum2 ;

    sum1 = sum2 = 0;
    while (len--)
    {
         sum1 += *ptr++;
         if (sum1 >= 255) sum1 -= 255;
         sum2 += sum1;
    }
    sum2 %= 255;
	
	//Check if the calculated checksum equals the received checksum
	if(((uint32_t)sum1 + ((uint32_t)sum2<<16)) == proStream->protocol_s.checksum)
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

