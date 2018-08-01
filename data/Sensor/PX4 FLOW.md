##  Read Px4Flow using Nucleo	

# General Information of Sensor
## Introduction
Important Information can be found mostly [here](https://pixhawk.org/modules/px4flow). It's quite useful when use this sensor.
## Using Ground Control Station To Get the Information
via: QGroundControl, [here](https://pixhawk.org/modules/px4flow)
via: MissionPlanner, [here](http://ardupilot.org/copter/docs/common-px4flow-overview.html)
# Data Structure 
There is 2 Data frame.  **I2C frame** and  **I2C integral frame**. 
send 0x0 to PX4FLOW module and receive back 22 Bytes data, internal address auto increments
send 0x16 (22) to PX4FLOW module and receive back 25 Bytes data, internal address auto increments
Read data achievement is easy and achievement code is [here.](https://github.com/im-Kitsch/RMR_sensor_fusion/blob/c734a15700bb0e91d6e7becde3f995a75a43a9c1/PX4FLOW%20I2C/Src/main.c#L117-L121)

```
    uint8_t *I2C_Receive = I2C_Info.arr;
    while ( HAL_I2C_Master_Transmit(&hi2c2, 0x84, send, 1, 0xff) != HAL_OK );
    while ( HAL_I2C_Master_Receive(&hi2c2, 0x84, I2C_Receive, 22,  0xfff) !=HAL_OK  );
    I2C_Receive = I2C_Integral_Info.arr;
    while ( HAL_I2C_Master_Transmit(&hi2c2, 0x84, receive, 1, 0xff) != HAL_OK );
    while ( HAL_I2C_Master_Receive(&hi2c2, 0x84, I2C_Receive, 25,  0xfff) !=HAL_OK  );
```
The Data Structure:
``` c
typedef struct i2c_frame
{
    uint16_t frame_count;// counts created I2C frames [#frames]
    int16_t pixel_flow_x_sum;// latest x flow measurement in pixels*10 [pixels]
    int16_t pixel_flow_y_sum;// latest y flow measurement in pixels*10 [pixels]
    int16_t flow_comp_m_x;// x velocity*1000 [meters/sec]
    int16_t flow_comp_m_y;// y velocity*1000 [meters/sec]
    int16_t qual;// Optical flow quality / confidence [0: bad, 255: maximum quality]
    int16_t gyro_x_rate; // latest gyro x rate [rad/sec]
    int16_t gyro_y_rate; // latest gyro y rate [rad/sec]
    int16_t gyro_z_rate; // latest gyro z rate [rad/sec]
    uint8_t gyro_range; // gyro range [0 .. 7] equals [50 deg/sec .. 2000 deg/sec] 
    uint8_t sonar_timestamp;// time since last sonar update [milliseconds]
    int16_t ground_distance;// Ground distance in meters*1000 [meters]. Positive value: distance known. Negative value: Unknown distance
} i2c_frame;
typedef struct i2c_integral_frame
{
    uint16_t frame_count_since_last_readout;//number of flow measurements since last I2C readout [#frames]
    int16_t pixel_flow_x_integral;//accumulated flow in radians*10000 around x axis since last I2C readout [rad*10000]
    int16_t pixel_flow_y_integral;//accumulated flow in radians*10000 around y axis since last I2C readout [rad*10000]
    int16_t gyro_x_rate_integral;//accumulated gyro x rates in radians*10000 since last I2C readout [rad*10000] 
    int16_t gyro_y_rate_integral;//accumulated gyro y rates in radians*10000 since last I2C readout [rad*10000] 
    int16_t gyro_z_rate_integral;//accumulated gyro z rates in radians*10000 since last I2C readout [rad*10000] 
    uint32_t integration_timespan;//accumulation timespan in microseconds since last I2C readout [microseconds]
    uint32_t sonar_timestamp;// time since last sonar update [microseconds]
    int16_t ground_distance;// Ground distance in meters*1000 [meters*1000]
    int16_t gyro_temperature;// Temperature * 100 in centi-degrees Celsius [degcelsius*100]
    uint8_t quality;// averaged quality of accumulated flow values [0:bad quality;255: max quality]
} __attribute__((packed)) i2c_integral_frame; 
```
# Important Notes of Hardwire
The wire sequence of official document is wrong. It's also discussed in Internet.
The wire of official document is:
![image](https://user-images.githubusercontent.com/33943000/41624015-c4118384-7414-11e8-99a9-6de4fad78baa.png)
![image](https://user-images.githubusercontent.com/33943000/41624044-d37d7f12-7414-11e8-8ced-96aa60d3b10e.png)

**But the sequence is wrong**. From Ground to 5V is not from 4 to 1 but 1 to 4. 
It has been proved. See the corresponding cable sequence of Pixhawk!
![image](https://user-images.githubusercontent.com/33943000/41624220-450f8382-7415-11e8-9716-383accf1e003.png)

Also the mentioned connector, DF13-4S-1.25C seems also not true. It can be found in inventory but doesn't match! Maybe Molex or some others......



## Problem 

It doesn't work in FireFly UAV, the achievement of I2C in UAV is not complex and not good.

# I2C via FireFly

### Tong Xun's Approach

See archive file laseri2c.zip

# Padding

If you use data structure you can find the problem of Padding. In PX4Flow it's solve as attribute word, you can find many information in Internet about this solution.

# Test Result
![image](https://user-images.githubusercontent.com/33943000/43514905-8517a8aa-9581-11e8-9d6b-e380339cd5f1.png)

# Connection Refine
![1](https://user-images.githubusercontent.com/33943000/43515092-f2448f38-9581-11e8-9460-f9222210aa96.jpg)
![default](https://user-images.githubusercontent.com/33943000/43515095-f42d6874-9581-11e8-8847-fc8c17b6feee.jpg)


