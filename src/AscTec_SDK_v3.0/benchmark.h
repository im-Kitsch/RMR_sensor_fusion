/*
 * benchmark.h
 *
 *  Created on: 09.06.2018
 *      Author: Malte
 */

#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include "protocol.h"

typedef enum {
	PROTOCOL_ACCEPT,
	PROTOCOL_ERROR_DETECTED_COBS,
	PROTOCOL_ERROR_DETECTED_CHECKSUM
} feedback;

typedef struct {
	uint8_t speed;
	uint32_t protocol_falseCOBS_reality_false;		//Protocol detects error in COBS (and there is an error in reality)
	uint32_t protocol_falseCHECKSUM_reality_false;	//Protocol detects error in CHECKSUM (and there is an error in reality)
	uint32_t protocol_falseCOBS_reality_true;		//Protocol detects error in COBS, but the values are like expected (FAIL)
	uint32_t protocol_falseCHECKSUM_reality_true;	//Protocol detects error in CHECKSUM, but the values are like expected (FAIL)
	uint32_t protocol_true_reality_false;			//Protocol says yes, but there is an error in reality
	uint32_t protocol_true_reality_true;			//Protocol says yes and in reality it's true (That's what we want!)
} testResult;

testResult testSeries[100]; //Maximum 100 testResults

void newBenchmark(uint8_t speed);
void addTransfer(Cprotocol_u *input, feedback feedback);

#endif /* BENCHMARK_H_ */
