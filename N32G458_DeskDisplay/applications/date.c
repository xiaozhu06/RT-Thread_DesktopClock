/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * E-mail : xiaozhu06@nyist.edu.cn
 * Date           Author       Notes
 * 2022-08-01     xiaozhu06       the first version
 */



#include "main.h"


#define DBG_TAG "date"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


struct rt_mailbox mb_oled_date;
char mb_pool_oled_date[128];


void thread_entry_date(void *param)
{
    time_t now = (time_t)0;
    char *str = RT_NULL;

    struct mesg_date * mes;

    mes = (struct mesg_date*)rt_malloc(sizeof(struct mesg_date));

//    struct timeval tv = { 0 };
//    struct timezone tz = { 0 };

//    char *host_name = RT_NULL;

    time_t cur_time = ntp_get_time(RT_NULL);
    rt_device_control(rt_device_find("rtc"), RT_DEVICE_CTRL_RTC_SET_TIME, &cur_time);
    LOG_I("Get local time from NTP server: %s", ctime((const time_t *) &cur_time));


    rt_kprintf("date start\n");

    while(1)
    {
        now = time(RT_NULL);

//        rt_kprintf("local time: %.*s", 25, ctime(&now));

        str = ctime(&now);

        memcpy(mes->hour, str+11, 8);
        mes->hour[8] = '\0';
        memcpy(mes->year, str+20,4);
        mes->year[4] = '\0';
        memcpy(mes->week, str, 3);
        mes->week[3] = '\0';
        memcpy(mes->mouth, str+4, 6);
        mes->mouth[6] = '\0';
//        rt_kprintf("time = %s,year = %s,week = %s,mouth = %s\n",mes->hour, mes->year,mes->week,mes->mouth);

        rt_mb_send(&mb_oled_date, (rt_uint32_t)mes);

//        rt_kprintf("send ok\n");

        rt_thread_mdelay(500);

    }

}














