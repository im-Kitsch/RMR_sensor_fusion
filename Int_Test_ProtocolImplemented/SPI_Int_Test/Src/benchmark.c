/*
 * benchmark.c
 *
 *  Created on: 09.06.2018
 *      Author: Malte
 */

#include "benchmark.h"
#include "protocol.h"
#include <stdint.h>

volatile static testResult testSeries[100]; //Maximum 100 testResults
static int8_t testCounter = -1;

void newBenchmark(uint8_t speed)
{
	testSeries[++testCounter].speed = speed;
	testSeries[testCounter].protocol_falseCHECKSUM_reality_false = 0;
	testSeries[testCounter].protocol_falseCOBS_reality_false = 0;
	testSeries[testCounter].protocol_falseCHECKSUM_reality_true = 0;
	testSeries[testCounter].protocol_falseCOBS_reality_true = 0;
	testSeries[testCounter].protocol_true_reality_false = 0;
	testSeries[testCounter].protocol_true_reality_true = 0;
}

void addTransfer(protocol_u *input, feedback feedback)
{
	//Expected protocol_u
	static protocol_u expected_protocolValues = {1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
	generateChecksum(&expected_protocolValues);
	/* --- Check Byte for Byte if protocol stream is correct for real --- */
	char RealErrorFlag = 0;
	for(int i = 0; i < num_sum; i++)
	{
		if(input->bytestream[i] != expected_protocolValues.bytestream[i])
		{
			RealErrorFlag = 1;
			break;
		}
	}

	switch(feedback)
	{
		case PROTOCOL_ACCEPT:
			if(RealErrorFlag)
				testSeries[testCounter].protocol_true_reality_false++;
			else
				testSeries[testCounter].protocol_true_reality_true++;
			break;

		case PROTOCOL_ERROR_DETECTED_COBS:
			if(RealErrorFlag)
				testSeries[testCounter].protocol_falseCOBS_reality_false++;
			else
				testSeries[testCounter].protocol_falseCOBS_reality_true++;
			break;

		case PROTOCOL_ERROR_DETECTED_CHECKSUM:
			if(RealErrorFlag)
				testSeries[testCounter].protocol_falseCHECKSUM_reality_false++;
			else
				testSeries[testCounter].protocol_falseCHECKSUM_reality_true++;
			break;
	}

}

/*
 * 		  {
			  changeMSB_LSB(&protocolstream);
			  if(checkChecksum(&protocolstream))
			  {
				  if(checkCorrect(&protocolstream))
					  successful_protocol_real++;
				  else
					  successful_protocol_unreal++;
			  } else
			  {
				unsuccessful_checksum++;
			  }
		  } else
			unsuccessful_COBS++;

		  //Check for real successful_real (ToDo: Use Hexa Code)
//			if(protocolstream.protocol_s.startByte == 0x01 && protocolstream.protocol_s.test_int_1 == 1000 && protocolstream.protocol_s.angle_yaw == -300 && protocolstream.protocol_s.test_int_11 == 11000)

			if(checkCorrect(&protocolstream))
				successful_real++;
			else
				unsuccessful_real++;
 */

