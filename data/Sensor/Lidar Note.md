#Connection

![image](https://user-images.githubusercontent.com/33943000/43515377-9e0fe0ec-9582-11e8-804e-24f849eaf242.png)
![image](https://user-images.githubusercontent.com/33943000/43515393-ab1057c2-9582-11e8-9ecb-6f17e066e233.png)

# Performance

![image](https://user-images.githubusercontent.com/33943000/43515406-b544c4a8-9582-11e8-9cdc-d45dfae8827e.png)

# Using it

Attention: Just for understanding the logic.

```
int lidar_setReg(unsigned char addr, unsigned char data)

{

    unsigned char data[2];

    data[0] = addr;

    data[1] = data;

    if(i2c_wrtie_bytes(0xc4, data, 2) != 0)

        return 1;

    else

        return 0;

} 

int lidar_readReg(unsigned char addr)

{

    unsigned char data[2];

    data[0] = addr;

    if(i2c_wrtie_bytes(0xc4, data, 1) == 0)

        return 0;

    i2c_read_bytes (0xc4, data, 1);

    return data[0];

} 

int lidar_readMeaReg()

{

    unsigned char data[2];

    unsigned short mea;

    data[0] = 0x0f;

    if(i2c_wrtie_bytes(0xc4, data, 1) == 0)

        return 0;

    i2c_read_bytes (0xc4, data, 2);

    mea = (data[0]8) | data[1];

    return mea;

}

int isBitHigh(unsigned char data, unsigned char bit)

{

    if(data  (1bit))

    {

        return 1;

    }else

    {

        return 0;

    }

}

unsigned short readLidarSensor()

{

    unsigned short data;

    lidar_setReg(0x00, 0x04);

    while(isBitHigh(lidar_readReg(0x01),  0) ;

    data = lidar_readMeaReg();

    returen data;

}

```

