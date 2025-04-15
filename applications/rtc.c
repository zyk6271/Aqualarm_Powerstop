/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-26     Rick       the first version
 */
#include "rtthread.h"
#include "stm32g0xx.h"

uint32_t RTC_Hours = 1 ;

RTC_HandleTypeDef rtc_handle;

void RTC_TAMP_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_TAMP_IRQn 0 */

  /* USER CODE END RTC_TAMP_IRQn 0 */
  HAL_RTC_AlarmIRQHandler(&rtc_handle);
  /* USER CODE BEGIN RTC_TAMP_IRQn 1 */

  /* USER CODE END RTC_TAMP_IRQn 1 */
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *RtcHandle)
{
    RTC_TimeTypeDef sTime = {0};
    sTime.Hours = 0;
    sTime.Minutes = 0;
    sTime.Seconds = 0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(RtcHandle, &sTime, RTC_FORMAT_BIN);

    rt_kprintf("HAL_RTC_AlarmAEventCallback %d\r\n",RTC_Hours);

    if(RTC_Hours % 120 == 0 )
    {
        RTC_Hours = 1;
        valve_check_start();
    }
    else
    {
        RTC_Hours++;
    }
}

void rtc_init(void)
{

    /* USER CODE BEGIN RTC_Init 0 */

    /* USER CODE END RTC_Init 0 */

    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    RTC_AlarmTypeDef sAlarm = {0};

    /* USER CODE BEGIN RTC_Init 1 */

    /* USER CODE END RTC_Init 1 */

    /** Initialize RTC Only
    */
    rtc_handle.Instance = RTC;
    rtc_handle.Init.HourFormat = RTC_HOURFORMAT_24;
    rtc_handle.Init.AsynchPrediv = 127;
    rtc_handle.Init.SynchPrediv = 249;
    rtc_handle.Init.OutPut = RTC_OUTPUT_DISABLE;
    rtc_handle.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
    rtc_handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    rtc_handle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    rtc_handle.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
    if (HAL_RTC_Init(&rtc_handle) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN Check_RTC_BKUP */

    /* USER CODE END Check_RTC_BKUP */

    /** Initialize RTC and set the Time and Date
    */
    sTime.Hours = 0x0;
    sTime.Minutes = 0x0;
    sTime.Seconds = 0x0;
    sTime.SubSeconds = 0x0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&rtc_handle, &sTime, RTC_FORMAT_BCD) != HAL_OK)
    {
        Error_Handler();
    }
    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month = RTC_MONTH_JANUARY;
    sDate.Date = 0x1;
    sDate.Year = 0x0;

    if (HAL_RTC_SetDate(&rtc_handle, &sDate, RTC_FORMAT_BCD) != HAL_OK)
    {
        Error_Handler();
    }

    /** Enable the Alarm A
    */
    sAlarm.AlarmTime.Hours = 1;
    sAlarm.AlarmTime.Minutes = 0;
    sAlarm.AlarmTime.Seconds = 0;
    sAlarm.AlarmTime.SubSeconds = 0x0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = 0x1;
    sAlarm.Alarm = RTC_ALARM_A;
    if (HAL_RTC_SetAlarm_IT(&rtc_handle, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN RTC_Init 2 */

    /* USER CODE END RTC_Init 2 */
}
