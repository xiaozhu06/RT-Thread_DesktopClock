/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * E-mail : xiaozhu06@nyist.edu.cn
 * Date           Author       Notes
 * 2022-07-31     xiaozhu06       the first version
 */


#include "main.h"

#define DBG_TAG "aht10"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


#include "sensor_asair_aht10.h"
#define AHT10_I2C_BUS  "i2c4"
int rt_hw_aht10_port(void)
{
    struct rt_sensor_config cfg;
    cfg.intf.dev_name  = AHT10_I2C_BUS;
    cfg.intf.user_data = (void *)AHT10_I2C_ADDR;
    rt_hw_aht10_init("aht10", &cfg);
    return RT_EOK;
}
INIT_APP_EXPORT(rt_hw_aht10_port);
//MSH_CMD_EXPORT(rt_hw_aht10_port,rere);




void aht_show_data(rt_sensor_t sensor, struct rt_sensor_data *sensor_data)
{
    switch (sensor->info.type)
    {
    case RT_SENSOR_CLASS_TEMP:

        LOG_I(" temp:%3d.%d C, timestamp:%5d", sensor_data->data.temp / 10, (rt_uint32_t)sensor_data->data.temp % 10, sensor_data->timestamp);
        break;
    case RT_SENSOR_CLASS_HUMI:

        LOG_I("humi:%3d.%d%%, timestamp:%5d", sensor_data->data.humi / 10, sensor_data->data.humi % 10, sensor_data->timestamp);
        break;
    default:
        break;
    }



}


void thread_entry_aht10(void *param)
{
    static rt_device_t dev1 = RT_NULL, dev2 = RT_NULL;
    struct rt_sensor_data data;
    rt_sensor_t sensor1, sensor2;

    struct mesg_aht *mes;
    mes = (struct mesg_aht*)rt_malloc(sizeof(struct mesg_aht));

    dev1 = rt_device_find("temp_aht");
    dev2 = rt_device_find("humi_aht");
    if (dev1 == RT_NULL)
    {
        rt_kprintf("Can't find device");
        return;
    }
    if (rt_device_open(dev1, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return;
    }
    if (rt_device_open(dev2, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return;
    }

    sensor1 = (rt_sensor_t)dev1;
    sensor2 = (rt_sensor_t)dev2;

    while(1)
    {
        rt_device_read(dev1, 0, &data, 1);
        sprintf(mes->temp, "temp:%3d.%dC", (&data)->data.temp / 10, (rt_uint32_t)(&data)->data.temp % 10);

        rt_device_read(dev2, 0, &data, 1);
        sprintf(mes->humi, "humi:%3d.%d%%", (&data)->data.humi / 10, (&data)->data.humi % 10);

        mes->temp[11] = '\0';
        mes->humi[11] = '\0';

        rt_mb_send(&mb_oled_aht, (rt_uint32_t)mes);

        rt_thread_mdelay(100);
    }
}










