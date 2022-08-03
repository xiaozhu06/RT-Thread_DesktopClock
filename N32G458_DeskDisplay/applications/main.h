/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-30     xiaozhu06       the first version
 */
#ifndef APPLICATIONS_MAIN_H_
#define APPLICATIONS_MAIN_H_


#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "n32g45x.h"
#include "rtdef.h"
#include "mpu6xxx.h"
#include "mpu6xxx_reg.h"
#include "ak8963_reg.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"
#include "inv_mpu.h"
#include "stdio.h"
#include "Mpu6050.h"
#include <u8g2_port.h>
#include <rthw.h>
#include "Oled.h"
#include "aht10.h"
#include "date.h"


/* 邮箱控制块-oled与date之间的邮箱 */
extern struct rt_mailbox mb_oled_date;
extern char mb_pool_oled_date[128];


struct rt_mailbox mb_oled_mpu;
char mb_pool_oled_mpu[128];

struct rt_mailbox mb_oled_aht;
char mb_pool_oled_aht[128];





#endif /* APPLICATIONS_MAIN_H_ */
