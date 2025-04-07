/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-03     Rick       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "signal_led.h"
#include "pin_config.h"

//定义信号灯对象句柄
static led_t *led_obj_off_red = RT_NULL;
static led_t *led_obj_loss_red = RT_NULL;
static led_t *led_obj_off_red_once = RT_NULL;
static led_t *led_obj_off_red_three = RT_NULL;
static led_t *led_obj_on_green = RT_NULL;

static led_t *led_obj_relay_on = RT_NULL;
static led_t *led_obj_relay_off = RT_NULL;

static led_t *beep_obj = RT_NULL;
static led_t *beep_obj_loss = RT_NULL;
static led_t *beep_obj_once = RT_NULL;
static led_t *beep_obj_three = RT_NULL;

static struct rt_thread led_thread;
static uint8_t led_thread_stack[512] = {0};

//定义内存操作函数接口
led_mem_opreation_t led_mem_opreation;

uint8_t led_valve_on_pause_flag = 0;

static void off_red_on(void *param)
{
    rt_pin_write(LED_OFF_RED_PIN, PIN_LOW);
}

static void off_red_off(void *param)
{
    rt_pin_write(LED_OFF_RED_PIN, PIN_HIGH);
}

static void on_green_on(void *param)
{
    rt_pin_write(LED_ON_GREEN_PIN, PIN_LOW);
}

static void on_green_off(void *param)
{
    rt_pin_write(LED_ON_GREEN_PIN, PIN_HIGH);
}

static void relay_on_on(void *param)
{
    rt_pin_write(RELAY_ON_PIN, PIN_HIGH);
}

static void relay_on_off(void *param)
{
    rt_pin_write(RELAY_ON_PIN, PIN_LOW);
}

static void relay_off_on(void *param)
{
    rt_pin_write(RELAY_OFF_PIN, PIN_HIGH);
}

static void relay_off_off(void *param)
{
    rt_pin_write(RELAY_OFF_PIN, PIN_LOW);
}

static void beep_on(void *param)
{
    beep_pwm_start();
}

static void beep_close(void *param)
{
    beep_pwm_stop();
}

static void led_run(void *parameter)
{
    while(1)
    {
        rt_thread_mdelay(LED_TICK_TIME);
        led_ticks();
    }
}

void beep_three_times(void)
{
    led_start(beep_obj_three);
    led_start(led_obj_off_red_three);
}

void led_valve_fail(void)
{
    led_set_mode(beep_obj, 3,"200,200,");
    led_start(beep_obj);
    led_set_mode(led_obj_off_red, 3,"200,200,");
    led_start(led_obj_off_red);
}

void led_moto_fail_start(void)
{
    led_set_mode(beep_obj, LOOP_PERMANENT,"200,200,200,200,200,200,200,200,200,200,200,10000,");
    led_start(beep_obj);
    led_set_mode(led_obj_off_red, LOOP_PERMANENT,"200,200,200,200,200,200,200,200,200,200,200,10000,");
    led_start(led_obj_off_red);
}

void led_master_lost_start(void)
{
    led_set_mode(beep_obj_loss, LOOP_PERMANENT,"200,200,200,5000,");
    led_start(beep_obj_loss);
    led_set_mode(led_obj_loss_red, LOOP_PERMANENT,"200,200,200,5000,");
    led_start(led_obj_loss_red);
}

void led_water_alarm_start(void)
{
    led_set_mode(beep_obj, LOOP_PERMANENT,"200,200,200,200,200,5000,");
    led_start(beep_obj);
    led_set_mode(led_obj_off_red, LOOP_PERMANENT,"200,200,200,200,200,5000,");
    led_start(led_obj_off_red);
}

void beep_stop(void)
{
    led_stop(beep_obj_loss);
    led_stop(beep_obj);
}

void beep_key_down(void)
{
    led_start(beep_obj_once);
    led_start(led_obj_off_red_once);
}

void beep_once(void)
{
    led_start(beep_obj_once);
}

void led_valve_on(void)
{
    led_stop(led_obj_off_red);
    led_set_mode(led_obj_on_green,LOOP_PERMANENT,"200,0,");
    led_start(led_obj_on_green);
}

void led_valve_off(void)
{
    led_stop(led_obj_on_green);
}

void led_warn_off(void)
{
    led_stop(led_obj_off_red);
}

void led_loss_off(void)
{
    led_stop(led_obj_loss_red);
    led_start(led_obj_on_green);
}

void relay_control(uint8_t value)
{
    if(value)
    {
        led_stop(led_obj_relay_off);
        led_start(led_obj_relay_on);
    }
    else
    {
        led_stop(led_obj_relay_on);
        led_start(led_obj_relay_off);
    }
}

void led_valve_on_pause(void)
{
    led_stop(led_obj_on_green);
    led_valve_on_pause_flag = 1;
}

void led_valve_on_resume(void)
{
    led_valve_on_pause_flag = 0;
    if(get_valve_status())
    {
        led_start(led_obj_on_green);
    }
}

int led_init(void)
{
    beep_pwm_hw_init();

    rt_pin_mode(LED_OFF_RED_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_ON_GREEN_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED_OFF_RED_PIN, PIN_HIGH);
    rt_pin_write(LED_ON_GREEN_PIN, PIN_HIGH);

    rt_pin_mode(RELAY_ON_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(RELAY_OFF_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(RELAY_ON_PIN, PIN_LOW);
    rt_pin_write(RELAY_OFF_PIN, PIN_LOW);

    led_mem_opreation.malloc_fn = (void* (*)(size_t))rt_malloc;
    led_mem_opreation.free_fn = rt_free;
    led_set_mem_operation(&led_mem_opreation);

    led_obj_off_red = led_create(off_red_on, off_red_off, NULL);
    led_set_mode(led_obj_off_red, LOOP_PERMANENT, "200,1,");

    led_obj_loss_red = led_create(off_red_on, off_red_off, NULL);
    led_set_mode(led_obj_loss_red, LOOP_PERMANENT, "200,200,");

    led_obj_off_red_once = led_create(off_red_on, off_red_off, NULL);
    led_set_mode(led_obj_off_red_once, 1, "200,1,");

    led_obj_off_red_three = led_create(off_red_on, off_red_off, NULL);
    led_set_mode(led_obj_off_red_three, 3, "200,200,");

    led_obj_on_green = led_create(on_green_on, on_green_off, NULL);
    led_set_mode(led_obj_on_green, 3, "200,200,");

    beep_obj = led_create(beep_on, beep_close, NULL);
    led_set_mode(beep_obj, LOOP_PERMANENT, "200,200,");

    beep_obj_loss = led_create(beep_on, beep_close, NULL);
    led_set_mode(beep_obj_loss, LOOP_PERMANENT, "200,200,");

    beep_obj_once = led_create(beep_on, beep_close, NULL);
    led_set_mode(beep_obj_once, 1, "200,1,");

    beep_obj_three = led_create(beep_on, beep_close, NULL);
    led_set_mode(beep_obj_three, 3, "200,200,");

    led_obj_relay_on = led_create(relay_on_on, relay_on_off, NULL);
    led_set_mode(led_obj_relay_on, 1, "50,1,");

    led_obj_relay_off = led_create(relay_off_on, relay_off_off, NULL);
    led_set_mode(led_obj_relay_off, 1, "50,1,");

    rt_thread_init(&led_thread,
                   "led",
                   led_run,
                   RT_NULL,
                   &led_thread_stack[0],
                   sizeof(led_thread_stack),
                   15,
                   100);
    rt_thread_startup(&led_thread);

    return RT_EOK;
}
