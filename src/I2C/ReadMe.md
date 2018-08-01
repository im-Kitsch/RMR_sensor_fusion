### Note

For more details, see  /data/sensor/PX4FLOW note, there are detailed introduction.

### Content

Read sensor data of PX4FLOW via I2C

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

 

