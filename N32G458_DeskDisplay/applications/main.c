/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *E-mail : xiaozhu06@nyist.edu.cn
 * Date           Author       Notes
 * 2021-12-18     BruceOu      first implementation
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include "main.h"





/* defined the LED3 pin: PB5 */
#define LED3_PIN GET_PIN(B, 5)

int main(void)
{

    rt_thread_init(&thread_mpu,
            "name_mpu",
            thread_entry_mpu,
            RT_NULL,
            &thread_stack_mpu[0],
            sizeof(thread_stack_mpu),
            22,
            5);
    rt_thread_startup(&thread_mpu);

    rt_thread_init(&thread_aht10,
            "name_aht",
            thread_entry_aht10,
            RT_NULL,
            &thread_stack_aht10[0],
            sizeof(thread_stack_aht10),
            24,
            5);
    rt_thread_startup(&thread_aht10);


    rt_thread_init(&thread_date,
            "nam_date",
            thread_entry_date,
            RT_NULL,
            &thread_stack_date[0],
            sizeof(thread_stack_date),
            26,
            10);
    rt_thread_startup(&thread_date);

    rt_thread_init(&thread_oled,
            "nam_oled",
            thread_entry_oled,
            RT_NULL,
            &thread_stack_oled[0],
            sizeof(thread_stack_oled),
            28,
            5);
    rt_thread_startup(&thread_oled);

    rt_thread_init(&thread_tim,
            "name_tim",
            thread_entry_tim,
            RT_NULL,
            &thread_stack_tim[0],
            sizeof(thread_stack_tim),
            23,
            5);
    rt_thread_startup(&thread_tim);




    rt_err_t result;

    /* 初始化一个 mailbox */
    result = rt_mb_init(&mb_oled_date, "mb_od",
            &mb_pool_oled_date[0], /* 邮箱用到的内存池是 mb_pool */
            sizeof(mb_pool_oled_date) / 4, /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
            RT_IPC_FLAG_FIFO); /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
        rt_kprintf("init mailbox failed.\n");
        return -1;
    }

    result = rt_mb_init(&mb_oled_mpu, "mb_om",
            &mb_pool_oled_mpu[0], /* 邮箱用到的内存池是 mb_pool */
            sizeof(mb_pool_oled_mpu) / 4, /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
            RT_IPC_FLAG_FIFO); /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
        rt_kprintf("init mailbox failed.\n");
        return -1;
    }


    result = rt_mb_init(&mb_oled_aht, "mb_oa",
            &mb_pool_oled_aht[0], /* 邮箱用到的内存池是 mb_pool */
            sizeof(mb_pool_oled_aht) / 4, /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
            RT_IPC_FLAG_FIFO); /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
        rt_kprintf("init mailbox failed.\n");
        return -1;
    }






    /* set LED3 pin mode to output */
    rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
