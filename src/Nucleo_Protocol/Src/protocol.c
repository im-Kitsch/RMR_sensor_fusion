/*
 * protocol.c
 */

#include "protocol.h"
#include "ringBuffer.h"
#include "COBS.h"



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

/**
 * Update the sensory struct 'protocol_received' using buffered received messages.
 */
void update_sensory_struct(void)
{
	uint32_t timeout = 0;
	while(1)
	{
		while(!(zeroDetected) && ((timeout++)<TIMEOUT)); //Wait until zero is detected, then try to unpack_message()
		TOTAL_REC++;
		zeroDetected = 0;
		if(unpack_message() || (timeout > TIMEOUT))
		{
			SUC_REC++;
			break;
		}
	}
}

/**
 * Looks for the latest message inside the RX BUFFER and stores it inside 'protocol_received'.
 * @return '0' no message found. '1' for message found
 *
 */
uint8_t unpack_message(void)
{
	uint8_t found = ReadLastMessageFromRXBuffer(latestMessage_received, num_sum+2);
	/*
	 * found = 1: Message found; found = 0: No message found
	 */

	  /* --- If message was found -> Decode (COBS) --- */
	  if(found) //If message is available
	  {
		  uint16_t length_COBS = decode_COBS(latestMessage_received, num_sum+2, protocol_received.bytestream);
		  if((length_COBS == num_sum+1) && (checkChecksum(&protocol_received))) //Decode COBS-Bytestream and check if length is how expected
		  {
			/* --- Checksum is correct -> Received Data seems to be correct --- */
			return 1;
		  }
	  }
	  protocol_received.protocol_s.startByte &= !(1); //Clear valid flag, because transfer was unvalid
	  return 0; //Message not found!
}

void pack_message(void)
{
	messagePacking = 1; //Set Flag
	generateChecksum_C(&protocol_transmit);

	/* --- Encode the message --- */
	uint8_t cobs_len = encode_COBS(protocol_transmit.bytestream, Cnum_sum, cobsByteStream);
	cobsByteStream[Cnum_sum+1] = 0x00; //manually set the delimiter at the end of the encoded message
	pushToTXBuffer(cobsByteStream, cobs_len+1);

	messagePacking = 0; //Reset Flag
}

