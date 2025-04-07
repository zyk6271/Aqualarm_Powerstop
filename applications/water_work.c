/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-12-25     Rick       the first version
 */
#include <rtthread.h>
#include <water_work.h>

#define DBG_TAG "water_work"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

enum Device_Status DeviceStatus = ValveOpen;

WariningEvent NowEvent;
WariningEvent ValveLeftFailEvent;
WariningEvent ValveRightFailEvent;
WariningEvent MasterSensorLostEvent;
WariningEvent MasterSensorLeakEvent;

void warning_enable(WariningEvent event)
{
    if(event.priority >= NowEvent.priority)
    {
        NowEvent.last_id = event.warning_id;
        NowEvent.priority = event.priority;
        if(event.callback!=RT_NULL)
        {
            NowEvent.callback = event.callback;
            NowEvent.callback(RT_NULL);
        }
        rt_kprintf("Warning_Enable Success,warning id is %d , now priority is %d\r\n",event.warning_id,event.priority);
    }
    else
    {
        rt_kprintf("Warning_Enable Fail last is %d Now is %d\r\n",NowEvent.priority,event.priority);
    }
}

void warning_event_bind(uint8_t warning_id,uint8_t priority,WariningEvent *event,void (*callback)(void*))
{
    rt_memset(event,0,sizeof(WariningEvent));
    event->warning_id = warning_id;
    event->priority = priority;
    event->callback = callback;
}

void MasterSensorLostEventCallback(void *parameter)
{
    led_valve_on_pause();
    led_master_lost_start();
    DeviceStatus = MasterSensorLost;
}

void MasterStatusChangeToDeAvtive(void)
{
    DeviceStatus = MasterSensorAbnormal;
}

void MasterSensorLeakEventCallback(void *parameter)
{
    valve_close();
    led_warn_off();
    led_water_alarm_start();
    DeviceStatus = MasterSensorLeak;
}

void ValveLeftFailEventCallback(void *parameter)
{
    led_valve_on_pause();
    led_moto_fail_start();
    DeviceStatus = ValveLeftFail;
}

void ValveRightFailEventCallback(void *parameter)
{
    led_valve_on_pause();
    led_moto_fail_start();
    DeviceStatus = ValveRightFail;
}

void valvefail_warning_disable(void)
{
    if(DeviceStatus == ValveLeftFail || DeviceStatus == ValveRightFail)
    {
        warning_all_clear();
    }
}

void warning_init(void)
{
    warning_event_bind(0,0,&NowEvent,RT_NULL);//本地存储器
    warning_event_bind(1,2,&ValveLeftFailEvent,ValveLeftFailEventCallback);
    warning_event_bind(2,2,&ValveRightFailEvent,ValveRightFailEventCallback);
    warning_event_bind(3,1,&MasterSensorLostEvent,MasterSensorLostEventCallback);
    warning_event_bind(4,3,&MasterSensorLeakEvent,MasterSensorLeakEventCallback);
}

uint8_t warning_leak_status_get(void)
{
    if(DeviceStatus == MasterSensorLeak || DeviceStatus == MasterSensorAbnormal)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


uint32_t warning_status_get(void)
{
    return DeviceStatus;
}

void warning_all_clear(void)
{
    beep_stop();
    led_warn_off();
    led_valve_on_resume();
    DeviceStatus = get_valve_status();
    rt_memset(&NowEvent, 0, sizeof(WariningEvent));
}

void warning_lost_clear(void)
{
    if(DeviceStatus == ValveClose || DeviceStatus == ValveOpen || DeviceStatus == MasterSensorLost)
    {
        warning_all_clear();
        led_loss_off();
    }
}
