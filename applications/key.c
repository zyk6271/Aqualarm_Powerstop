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

extern enum Device_Status DeviceStatus;
extern WariningEvent MasterSensorLeakEvent;
extern WariningEvent MasterSensorLostEvent;

uint8_t water_lost_status = 0;
uint8_t water_leak_status = 0;

agile_btn_t *key_sw_btn = RT_NULL;
agile_btn_t *water_leak_btn = RT_NULL;
agile_btn_t *water_lost_btn = RT_NULL;

void key_sw_click_handle(void)
{
    rt_kprintf("key_sw_click_handle\r\n");
    switch(DeviceStatus)
    {
    case ValveClose:
        valve_check_start();
        break;
    case ValveOpen:
        valve_check_start();
        break;
    case MasterSensorLost:
        valve_check_start();
        break;
    case MasterSensorLeak:
        beep_stop();
        break;
    case MasterSensorAbnormal:
        warning_all_clear();
        valve_open();
        relay_control(1);
        if(water_lost_status == 1)
        {
            warning_enable(MasterSensorLostEvent);
        }
        break;
    case ValveLeftFail:
        valve_check_start();
        break;
    case ValveRightFail:
        valve_check_start();
        break;
    default:
        break;
    }
}

void water_leak_up_callback(agile_btn_t *btn)
{
    MasterStatusChangeToDeAvtive();
    rt_kprintf("water_leak_up_callback\r\n");
}

void water_leak_down_callback(agile_btn_t *btn)
{
    warning_enable(MasterSensorLeakEvent);
    rt_kprintf("water_leak_down_callback\r\n");
}

void water_lost_plugin_callback(agile_btn_t *btn)
{
    water_lost_status = 0;
    warning_lost_clear();
    rt_kprintf("water_lost_up_callback\r\n");
}

void water_lost_plugout_callback(agile_btn_t *btn)
{
    water_lost_status = 1;
    warning_enable(MasterSensorLostEvent);
    rt_kprintf("water_lost_down_callback\r\n");
}

void water_leak_pause(void)
{
    agile_btn_stop(water_leak_btn);
}

void water_leak_resume(void)
{
    agile_btn_start(water_leak_btn);
}

void water_lost_pause(void)
{
    agile_btn_stop(water_lost_btn);
}

void water_lost_resume(void)
{
    agile_btn_start(water_lost_btn);
}

void button_init(void)
{
    key_sw_btn = agile_btn_create(KEY_SW_PIN, PIN_LOW, PIN_MODE_INPUT);
    water_leak_btn = agile_btn_create(SENSOR_LEAK_PIN, PIN_LOW, PIN_MODE_INPUT);
    water_lost_btn = agile_btn_create(SENSOR_LOST_PIN, PIN_HIGH, PIN_MODE_INPUT);

    agile_btn_set_event_cb(key_sw_btn, BTN_PRESS_UP_EVENT, key_sw_click_handle);
    agile_btn_set_event_cb(water_leak_btn, BTN_PRESS_UP_EVENT, water_leak_up_callback);
    agile_btn_set_event_cb(water_leak_btn, BTN_PRESS_DOWN_EVENT, water_leak_down_callback);
    agile_btn_set_event_cb(water_lost_btn, BTN_PRESS_UP_EVENT, water_lost_plugin_callback);
    agile_btn_set_event_cb(water_lost_btn, BTN_PRESS_DOWN_EVENT, water_lost_plugout_callback);

    agile_btn_start(key_sw_btn);
    agile_btn_start(water_leak_btn);
    agile_btn_start(water_lost_btn);
}
