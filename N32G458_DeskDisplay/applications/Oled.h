/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * E-mail : xiaozhu06@nyist.edu.cn
 * Date           Author       Notes
 * 2022-07-30     xiaozhu06       the first version
 */
#ifndef APPLICATIONS_OLED_H_
#define APPLICATIONS_OLED_H_


#define OLED_I2C_PIN_SCL                    22  // PB6
#define OLED_I2C_PIN_SDA                    23  // PB7

static char thread_stack_oled[2048];
static struct rt_thread thread_oled;



void thread_entry_oled(void *param);



#endif /* APPLICATIONS_OLED_H_ */
