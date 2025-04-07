/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-12-27     Rick       the first version
 */
#include <rtthread.h>
#include <agile_button.h>
#include "pin_config.h"
#include "water_work.h"

#define DBG_TAG "key"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

uint8_t key_on_count,key_on_long_click = 0;

extern enum Device_Status DeviceStatus;

agile_btn_t *key_sw_btn = RT_NULL;
agile_btn_t *water_leak_btn = RT_NULL;
agile_btn_t *water_lost_btn = RT_NULL;

void key_sw_click_handle(void)
{
    rt_kprintf("key_sw_click_handle\r\n");
    switch(DeviceStatus)
    {
    case MasterSensorLost:
        beep_stop();
        break;
    case MasterSensorLeak:
        beep_stop();
        break;
    case MasterSensorAbnormal:
        warning_all_clear();
        valve_open();
        break;
    case ValveLeftFail:
        break;
    case ValveRightFail:
        break;
    default:
        break;
    }
}

void key_sw_long_hold_handle(void)
{
    if(key_on_count < 4)
    {
        key_on_count ++;
        rt_kprintf("key_sw_long_hold_handle %d\r\n",key_on_count);
    }
    else
    {
        if(key_on_long_click == 0)
        {
            key_on_long_click = 1;
            valve_check();
        }
    }
}

void key_sw_long_free_handle(void)
{
    key_on_count = 0;
    key_on_long_click = 0;
}

void water_leak_up_callback(agile_btn_t *btn)
{
    MasterStatusChangeToDeAvtive();
    rt_kprintf("water_leak_up_callback\r\n");
}

void water_leak_down_callback(agile_btn_t *btn)
{
    extern WariningEvent MasterSensorLeakEvent;
    warning_enable(MasterSensorLeakEvent);
    rt_kprintf("water_leak_down_callback\r\n");
}

void water_lost_plugin_callback(agile_btn_t *btn)
{
    warning_lost_clear();
    rt_kprintf("water_lost_up_callback\r\n");
}

void water_lost_plugout_callback(agile_btn_t *btn)
{
    extern WariningEvent MasterSensorLostEvent;
    warning_enable(MasterSensorLostEvent);
    rt_kprintf("water_lost_down_callback\r\n");
}

void button_init(void)
{
    key_sw_btn = agile_btn_create(KEY_SW_PIN, PIN_LOW, PIN_MODE_INPUT);
    water_leak_btn = agile_btn_create(SENSOR_LEAK_PIN, PIN_LOW, PIN_MODE_INPUT);
    water_lost_btn = agile_btn_create(SENSOR_LOST_PIN, PIN_HIGH, PIN_MODE_INPUT);

    agile_btn_set_hold_cycle_time(key_sw_btn,1000);

    agile_btn_set_event_cb(key_sw_btn, BTN_PRESS_UP_EVENT, key_sw_click_handle);
    agile_btn_set_event_cb(key_sw_btn, BTN_HOLD_EVENT, key_sw_long_hold_handle);
    agile_btn_set_event_cb(key_sw_btn, BTN_HOLD_FREE_EVENT, key_sw_long_free_handle);

    agile_btn_set_event_cb(water_leak_btn, BTN_PRESS_UP_EVENT, water_leak_up_callback);
    agile_btn_set_event_cb(water_leak_btn, BTN_PRESS_DOWN_EVENT, water_leak_down_callback);
    agile_btn_set_event_cb(water_lost_btn, BTN_PRESS_UP_EVENT, water_lost_plugin_callback);
    agile_btn_set_event_cb(water_lost_btn, BTN_PRESS_DOWN_EVENT, water_lost_plugout_callback);

    agile_btn_start(key_sw_btn);

    agile_btn_start(water_leak_btn);
    agile_btn_start(water_lost_btn);
}
