/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * E-mail : xiaozhu06@nyist.edu.cn
 * Date           Author       Notes
 * 2022-07-31     xiaozhu06       the first version
 */
#ifndef APPLICATIONS_AHT10_H_
#define APPLICATIONS_AHT10_H_

static char thread_stack_aht10[2048];
static struct rt_thread thread_aht10;



void thread_entry_aht10(void *param);
void aht_show_data(rt_sensor_t sensor, struct rt_sensor_data *sensor_data);

struct mesg_aht
{
    rt_uint8_t temp[12];
    rt_uint8_t humi[12];
};



#endif /* APPLICATIONS_AHT10_H_ */
