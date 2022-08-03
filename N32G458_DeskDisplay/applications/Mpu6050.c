/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-28     xiaozhu06       the first version
 */


#include "main.h"


#define DBG_TAG "Mpu6050"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define Page_Max 3


#include "sensor_inven_mpu6xxx.h"
#define MPU6XXX_DEVICE_NAME  "i2c3"


struct mpu6xxx_device *dev;
float Pitch,Roll,Yaw;

rt_uint8_t flage_tim = 1;
rt_uint8_t page = 0;

void thread_entry_mpu(void *param)
{
    struct mpu6xxx_3axes accel, gyro;
    float pitch,roll,yaw;


    rt_uint8_t flage = 1;


    /* Initialize mpu6xxx, The parameter is RT_NULL, means auto probing for i2c*/
    dev = mpu6xxx_init(MPU6XXX_DEVICE_NAME, RT_NULL);

    if (dev == RT_NULL)
    {
        rt_kprintf("mpu6xxx init failed\n");
        return ;
    }
    rt_kprintf("mpu6xxx init succeed\n");


    //    mpu6xxx_set_param(dev, MPU6XXX_ACCEL_RANGE, MPU6XXX_GYRO_RANGE_2000DPS);  //陀螺仪范围配置
    //        mpu6xxx_set_param(dev, MPU6XXX_ACCEL_RANGE, MPU6XXX_ACCEL_RANGE_2G);     //加速度计，一般设置为±2G
    //        mpu6xxx_set_param(dev, MPU6XXX_SAMPLE_RATE, 50);                       //采样频率
    //        mpu6xxx_set_param(dev, MPU6XXX_DLPF_CONFIG, 25);                       //数字低通滤波器设置，一般为1/2采样率

    while(mpu_dmp_init())
    {
        rt_thread_mdelay(500);
        rt_kprintf("\r\nMPU6050 Error\r\n");
    }
    rt_kprintf("\r\nMPU6050 OK\r\n");

    rt_mb_send(&mb_oled_mpu, (rt_uint32_t)page);

    while(1)
    {
        //            Read_DMP(&Pitch,&Roll,&Yaw);
        mpu_dmp_get_data(&Pitch,&Roll,&Yaw);
        pitch = Pitch;
        roll = Roll;
        yaw = Yaw;

//        printf(" Pitch = %lf, Roll = %lf, Yaw = %lf\n", pitch, roll, yaw);

        if(roll > 40 && flage == 1)
        {
            page ++;
            if(page >= Page_Max)
                page = 0;
            flage = 0;
            flage_tim = 1;
            rt_mb_send(&mb_oled_mpu, (rt_uint32_t)page);
        }
        if(roll < -40 && flage == 1)
        {
            if(page <= 0)
                page = Page_Max;
             page --;
             flage = 0;
             flage_tim = 1;
             rt_mb_send(&mb_oled_mpu, (rt_uint32_t)page);
        }

        if(roll < 30 && roll > -30)
        {
            flage = 1;
        }



        rt_thread_mdelay(10);
    }

}




#define HWTIMER_DEV_NAME   "timer3"     /* 定时器名称 */

/* 定时器超时回调函数 */
static rt_err_t timeout_cb(rt_device_t dev, rt_size_t size)
{
    rt_kprintf("this is hwtimer timeout callback fucntion!\n");
    page = 0;
    flage_tim = 1;
    rt_mb_send(&mb_oled_mpu, (rt_uint32_t)page);

    return 0;
}

void thread_entry_tim(void *param)
{
    rt_err_t ret = RT_EOK;
    rt_hwtimerval_t timeout_s;      /* 定时器超时值 */
    rt_device_t hw_dev = RT_NULL;   /* 定时器设备句柄 */
    rt_hwtimer_mode_t mode;         /* 定时器模式 */
    rt_uint32_t freq = 10000;               /* 计数频率 */



    /* 查找定时器设备 */
    hw_dev = rt_device_find(HWTIMER_DEV_NAME);
    if (hw_dev == RT_NULL)
    {
        rt_kprintf("hwtimer sample run failed! can't find %s device!\n", HWTIMER_DEV_NAME);
        return ;
    }

    /* 以读写方式打开设备 */
    ret = rt_device_open(hw_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", HWTIMER_DEV_NAME);
        return ;
    }

    /* 设置超时回调函数 */
    rt_device_set_rx_indicate(hw_dev, timeout_cb);



    /* 设置计数频率(若未设置该项，默认为1Mhz 或 支持的最小计数频率) */
    rt_device_control(hw_dev, HWTIMER_CTRL_FREQ_SET, &freq);
    /* 设置模式为周期性定时器（若未设置，默认是HWTIMER_MODE_ONESHOT）*/
    mode = HWTIMER_MODE_ONESHOT;
    ret = rt_device_control(hw_dev, HWTIMER_CTRL_MODE_SET, &mode);
    if (ret != RT_EOK)
    {
        rt_kprintf("set mode failed! ret is :%d\n", ret);
        return ;
    }



//    rt_device_close(hw_dev);

    while(1)
    {
        if(Roll < 5 && Roll > -5 && page != 0)
        {
            rt_kprintf("in the tim\n");
            /* 设置定时器超时值为5s并启动定时器 */
            timeout_s.sec = 7;      /* 秒 */
            timeout_s.usec = 0;     /* 微秒 */
            if(flage_tim == 1)
            {
                flage_tim = 0;
                if (rt_device_write(hw_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s)    )
                {
                    rt_kprintf("set timeout value failed\n");

                }
            }
        }

        rt_thread_mdelay(500);
    }





}







void mpu_measurement_init(void)
{
    dev = (struct mpu6xxx_device *) mpu6xxx_init(MPU6XXX_DEVICE_NAME, RT_NULL); //初始化MPU6050，测量单位为角速度，加速度    while(count++)

    rt_int8_t res = 1;
    while (res)
    {
        res = mpu_dmp_init();
        rt_kprintf("\r\nRES = %d\r\n",res);

        rt_thread_mdelay(500);
        rt_kprintf("\r\nMPU6050 DMP init Error\r\n");
    }
    rt_kprintf("\r\nMPU6050 DMP init OK\r\n");
}



rt_uint8_t MPU_Write_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *databuf)
{
    rt_int8_t res = 0;
#ifdef RT_USING_I2C
    struct rt_i2c_msg msgs;
    rt_uint8_t buf[50] = {0};
#endif
    buf[0] = reg;

    for(int i = 0;i<len;i++)
    {
        buf[i+1]=databuf[i];
    }

    if (dev->bus->type == RT_Device_Class_I2CBUS)
    {
        msgs.addr  = dev->i2c_addr;    /* slave address */
        msgs.flags = RT_I2C_WR;        /* write flag */
        msgs.buf   = buf;              /* Send data pointer */
        msgs.len   = len+1;

        if (rt_i2c_transfer((struct rt_i2c_bus_device *)dev->bus, &msgs, 1) == 1)
        {
            res = RT_EOK;
        }
        else
        {
            res = -RT_ERROR;
        }
    }
}


rt_uint8_t MPU_Read_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf)
{
    rt_int8_t res = 0;
#ifdef RT_USING_I2C
    struct rt_i2c_msg msgs[2];
#endif
#ifdef RT_USING_SPI
    rt_uint8_t tmp;
#endif
    if (dev->bus->type == RT_Device_Class_I2CBUS)
    {
        msgs[0].addr  = dev->i2c_addr;    /* Slave address */
        msgs[0].flags = RT_I2C_WR;        /* Write flag */
        msgs[0].buf   = &reg;             /* Slave register address */
        msgs[0].len   = 1;                /* Number of bytes sent */

        msgs[1].addr  = dev->i2c_addr;    /* Slave address */
        msgs[1].flags = RT_I2C_RD;        /* Read flag */
        msgs[1].buf   = buf;              /* Read data pointer */
        msgs[1].len   = len;              /* Number of bytes read */

        if (rt_i2c_transfer((struct rt_i2c_bus_device *)dev->bus, msgs, 2) == 2)
        {
            res = RT_EOK;
        }
        else
        {
            res = -RT_ERROR;
        }
    }

    return res;
}


void show_data(rt_sensor_t sensor, struct rt_sensor_data *sensor_data)
{
    switch (sensor->info.type)
    {
    case RT_SENSOR_CLASS_ACCE:
        LOG_I(" x:%5d, y:%5d, z:%5d mg, timestamp:%5d", sensor_data->data.acce.x, sensor_data->data.acce.y, sensor_data->data.acce.z, sensor_data->timestamp);
        break;
    case RT_SENSOR_CLASS_GYRO:
        LOG_I(" x:%8d, y:%8d, z:%8d dps, timestamp:%5d", sensor_data->data.gyro.x / 1000, sensor_data->data.gyro.y / 1000, sensor_data->data.gyro.z / 1000, sensor_data->timestamp);
        break;
    case RT_SENSOR_CLASS_MAG:
        LOG_I(" x:%5d, y:%5d, z:%5d mGauss, timestamp:%5d", sensor_data->data.mag.x, sensor_data->data.mag.y, sensor_data->data.mag.z, sensor_data->timestamp);
        break;
    default:
        break;
    }
}


void mpu_read(int argc, char **argv)
{
    static rt_device_t dev = RT_NULL;
    struct rt_sensor_data data;
    rt_sensor_t sensor;


    rt_uint8_t reg = 0xFF;
    if (dev)
    {
        rt_device_close(dev);
    }

    dev = rt_device_find("gyro_mpu");
    if (dev == RT_NULL)
    {
        LOG_E("Can't find device:%s", argv[2]);
        return;
    }
    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        LOG_E("open device failed!");
        return;
    }
    rt_device_control(dev, RT_SENSOR_CTRL_GET_ID, &reg);
    LOG_I("device id: 0x%x!", reg);

    sensor = (rt_sensor_t)dev;


//    while(1)
//    {
        rt_device_read(dev, 0, &data, 1);
        show_data( sensor, &data);
        //        rt_thread_mdelay(100);
//    }
}
MSH_CMD_EXPORT(mpu_read,mpu_read);






