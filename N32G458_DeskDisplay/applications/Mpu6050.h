/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-28     xiaozhu06       the first version
 */
#ifndef APPLICATIONS_MPU6050_H_
#define APPLICATIONS_MPU6050_H_


#include "n32g45x.h"
#include "rtdef.h"

static char thread_stack_mpu[2048];
static struct rt_thread thread_mpu;

static char thread_stack_tim[1024];
static struct rt_thread thread_tim;


rt_uint8_t MPU_Write_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *databuf);
rt_uint8_t MPU_Read_Len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf);



void thread_entry_mpu(void *param);
void timeout(void *param);
void thread_entry_tim(void *param);



#endif /* APPLICATIONS_MPU6050_H_ */
