/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-06     Rick       the first version
 */

#ifndef APPLICATIONS_PIN_CONFIG_H_
#define APPLICATIONS_PIN_CONFIG_H_

/*
 * SENSOR
 */
#define SENSOR_LEAK_PIN                 23
#define SENSOR_LOST_PIN                 22
/*
 * MOTO
 */
#define MOTO_LEFT_HALL_PIN              21
#define MOTO_LEFT_CONTROL_PIN           1
#define MOTO_RIGHT_HALL_PIN             20
#define MOTO_RIGHT_CONTROL_PIN          3
/*
 * KEY
 */
#define KEY_SW_PIN                      15
/*
 * LED
 */
#define LED_OFF_RED_PIN                 12
#define LED_ON_GREEN_PIN                11
/*
 * BUZZER
 */
#define BEEP_PIN                        24
/*
 * AC_RELAY
 */
#define RELAY_ON_PIN                    38
#define RELAY_OFF_PIN                   2

#endif /* APPLICATIONS_PIN_CONFIG_H_ */
