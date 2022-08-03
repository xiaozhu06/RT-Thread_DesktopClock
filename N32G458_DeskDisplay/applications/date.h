/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * E-mail : xiaozhu06@nyist.edu.cn
 * Date           Author       Notes
 * 2022-08-01     xiaozhu06       the first version
 */
#ifndef APPLICATIONS_DATE_H_
#define APPLICATIONS_DATE_H_


#include "ntp.h"


static char thread_stack_date[2048];
static struct rt_thread thread_date;


struct mesg_date
{
    char hour[9];
    char week[3];
    char year[4];
    char mouth[6];
};


void thread_entry_date(void *param);




#endif /* APPLICATIONS_DATE_H_ */
