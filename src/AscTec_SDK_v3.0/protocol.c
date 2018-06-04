/*
 * protocol.c
 */

#include "sdk.h"
#include "protocol.h"

/**
 * Refresh protocolStream with current sensor data.
 * @param proByteStream 	ByteStream to be refreshed
 * @param newData 			Struct including new data
 */
void refreshProtocolStream(protocol_u *proStream)
{
	proStream->protocol_s.angle_pitch = RO_ALL_Data.angle_pitch;
	proStream->protocol_s.angle_roll = RO_ALL_Data.angle_roll;
	proStream->protocol_s.angle_yaw = RO_ALL_Data.angle_yaw;
}

