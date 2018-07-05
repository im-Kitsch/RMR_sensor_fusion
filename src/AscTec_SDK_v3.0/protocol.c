/*
 * protocol.c
 */

#include "sdk.h"
#include "LPC214x.h"
#include "benchmark.h"
#include <stdint.h>
#include "protocol.h"
#include "ringBuffer.h"
#include "COBS.h"
#include "timeStamp.h"

/**
 * Refresh protocolStream with current sensor data.
 * @param proStream 	Protocol stream to be refreshed
 */
void update_message_with_IMU_Sensory(void)
{

	//TETSTING SEQUENCE
	protocol_transmit.protocol_s.startByte = 0x01;
	protocol_transmit.protocol_s.timeStamp_IMU_Sensory = getTimeStamp();
	protocol_transmit.protocol_s.channel[0] = RO_ALL_Data.channel[0];
	protocol_transmit.protocol_s.channel[1] = RO_ALL_Data.channel[1];
	protocol_transmit.protocol_s.channel[2] = RO_ALL_Data.channel[2];
	protocol_transmit.protocol_s.channel[3] = RO_ALL_Data.channel[3];
	protocol_transmit.protocol_s.channel[4] = RO_ALL_Data.channel[4];
	protocol_transmit.protocol_s.channel[5] = RO_ALL_Data.channel[5];
	protocol_transmit.protocol_s.channel[6] = RO_ALL_Data.channel[6];
	protocol_transmit.protocol_s.channel[7] = RO_ALL_Data.channel[7];
	protocol_transmit.protocol_s.angle_pitch = RO_ALL_Data.angle_pitch;
	protocol_transmit.protocol_s.angle_roll = RO_ALL_Data.angle_roll;
	protocol_transmit.protocol_s.angle_yaw = RO_ALL_Data.angle_yaw;
	protocol_transmit.protocol_s.angvel_pitch = RO_ALL_Data.angvel_pitch;
	protocol_transmit.protocol_s.angvel_roll = RO_ALL_Data.angvel_roll;
	protocol_transmit.protocol_s.angvel_yaw = RO_ALL_Data.angvel_yaw;
	protocol_transmit.protocol_s.acc_x = RO_ALL_Data.acc_x;
	protocol_transmit.protocol_s.acc_y = RO_ALL_Data.acc_y;
	protocol_transmit.protocol_s.acc_z = RO_ALL_Data.acc_z;
	protocol_transmit.protocol_s.Hx = RO_ALL_Data.Hx;
	protocol_transmit.protocol_s.Hy = RO_ALL_Data.Hy;
	protocol_transmit.protocol_s.Hz = RO_ALL_Data.Hz;
	protocol_transmit.protocol_s.motor_rpm[0] = RO_ALL_Data.motor_rpm[0];
	protocol_transmit.protocol_s.motor_rpm[1] = RO_ALL_Data.motor_rpm[1];
	protocol_transmit.protocol_s.motor_rpm[2] = RO_ALL_Data.motor_rpm[2];
	protocol_transmit.protocol_s.motor_rpm[3] = RO_ALL_Data.motor_rpm[3];
	protocol_transmit.protocol_s.motor_rpm[4] = RO_ALL_Data.motor_rpm[4];
	protocol_transmit.protocol_s.motor_rpm[5] = RO_ALL_Data.motor_rpm[5];

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
	proStream->Cprotocol_s.checksum = (uint32_t)sum1 + ((uint32_t)sum2<<16);
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
	if(((uint32_t)sum1 + ((uint32_t)sum2<<16)) == proStream->Cprotocol_s.checksum)
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

/**
 * Looks for the latest message inside the RX BUFFER and stores in inside 'protocol_received'.
 * @return '0' no message found. '1' for message found
 *
 */
uint8_t unpack_message(void)
{
	uint8_t found = ReadLastMessageFromRXBuffer(latestMessage_received, Cnum_sum + 2);

	/*
	 * found = 1: Message found; found = 0: No message found
	 */
	if (found) //Message found in RX Buffer
	{

		uint8_t COBS_len= decode_COBS(latestMessage_received, Cnum_sum + 2, protocol_received.bytestream);
		/* --- COBS - FORMAT passed --- */
		if((COBS_len == Cnum_sum+1) && (checkChecksum_C(&protocol_received)))
		{
				/* --- Checksum is correct -> Received Data seems to be correct --- */
				return 1;
		}
	}

	protocol_received.Cprotocol_s.startByte &= !(1); //Clear valid flag, because transfer was unvalid
	return 0; //Message not found!
}

void apply_message_to_UAV(void)
{
	//Check if current Data is valid
	if(protocol_received.Cprotocol_s.startByte & 0x01U)
	{
		//Forward the received data to the LLP
		WO_Direct_Motor_Control.pitch = protocol_received.Cprotocol_s.pitch;
		WO_Direct_Motor_Control.roll = protocol_received.Cprotocol_s.roll;
		WO_Direct_Motor_Control.yaw = protocol_received.Cprotocol_s.yaw;
		WO_Direct_Motor_Control.thrust = protocol_received.Cprotocol_s.thrust;
	}
}

void pack_message(void)
{
	messagePacking = 1; //Set Flag
	generateChecksum(&protocol_transmit);

	/* --- Encode the message --- */
	uint8_t cobs_len = encode_COBS(protocol_transmit.bytestream, num_sum, cobsByteStream);
	cobsByteStream[num_sum+1] = 0x00; //manually set the delimiter at the end of the encoded message
	pushToTXBuffer(cobsByteStream, cobs_len+1);

	messagePacking = 0; //Reset Flag
}

