/*
 * Copyright (c) , xiaozhu06
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * E-mail : xiaozhu06@nyist.edu.cn
 * Date           Author       Notes
 * 2022-07-30     xiaozhu06       the first version
 */


#include "main.h"


struct rt_mailbox mb_oled_date;
char mb_pool_oled_date[128];


void thread_entry_oled(void *param)
{
    u8g2_t u8g2;

    char  *test = RT_NULL;

    int num;

    struct mesg_date * str_date;
    struct mesg_aht * str_aht;
    str_aht = (struct mesg_aht*)rt_malloc(sizeof(struct mesg_aht));

    // Initialization
    u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_rtthread);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, OLED_I2C_PIN_SCL);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, OLED_I2C_PIN_SDA);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);


    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenR18_tf  );
    u8g2_DrawStr(&u8g2, 0, 48, "Loading...");
    u8g2_SendBuffer(&u8g2);

    rt_kprintf("ole start ok\n");

    while(1)
    {
        if (rt_mb_recv(&mb_oled_mpu, (rt_uint32_t *) &test, RT_WAITING_NO) == RT_EOK)
        {
            rt_kprintf("test = %d\n",test);
            num = (int)test;
        }

        if(num == 0)
        {
            if (rt_mb_recv(&mb_oled_date, (rt_uint32_t *) &str_date, RT_WAITING_FOREVER) == RT_EOK)
            {
//                rt_kprintf("received mb_oled_date\n");
                u8g2_ClearBuffer(&u8g2);
                u8g2_SetFont(&u8g2, u8g2_font_fub20_tf  );
                u8g2_DrawStr(&u8g2, 10, 55, str_date->hour);
                u8g2_SetFont(&u8g2, u8g2_font_helvR14_te );
                u8g2_DrawStr(&u8g2, 5, 18, str_date->mouth);
                u8g2_SetFont(&u8g2, u8g2_font_helvR14_te );
                u8g2_DrawStr(&u8g2, 80, 18, str_date->week);
                u8g2_SendBuffer(&u8g2);

                rt_thread_mdelay(1);
            }
        }

        if(num == 1)
        {
            if (rt_mb_recv(&mb_oled_aht, (rt_uint32_t *) &str_aht, RT_WAITING_FOREVER) == RT_EOK)
            {
//                rt_kprintf("received mb_oled_aht\n");

                u8g2_ClearBuffer(&u8g2);
                u8g2_SetFont(&u8g2, u8g2_font_helvR14_te );
                u8g2_DrawStr(&u8g2, 10, 55, str_aht->temp);
                u8g2_DrawStr(&u8g2, 10, 22, str_aht->humi);
                u8g2_SendBuffer(&u8g2);

                rt_thread_mdelay(1);
            }

        }
        if(num == 2)
        {
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetFont(&u8g2, u8g2_font_t0_15b_mr  );
            u8g2_DrawStr(&u8g2, 10, 16, "Weather:Sun");
            u8g2_DrawStr(&u8g2, 10, 32, "NextWea:Sun");
            u8g2_DrawStr(&u8g2, 10, 48, "temp:27-32C");
            u8g2_SendBuffer(&u8g2);
            rt_thread_mdelay(1);
        }


    }





    //    u8g2_SetFont(&u8g2, u8g2_font_unifont_ t_symbols);
    //    u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603 );
    //    u8g2_SendBuffer(&u8g2);
}






