/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-02-17     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

const uint8_t fw_main_ver = 0x00;
const uint8_t fw_sub_ver = 0x00;

int main(void)
{
    rt_kprintf("System Version is V1.%d.%d\r\n",fw_main_ver,fw_sub_ver);
    led_init();
    button_init();
    warning_init();
    rtc_init();
    valve_init();
    syswatch_init();
    while (1)
    {
        syswatch_feed();
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}
