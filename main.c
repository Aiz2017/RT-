/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-02     RT-Thread    first version
 */

#include <rtthread.h>
#include <board.h>
#include <drv_common.h>
#include <rtdevice.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "DataBase.h"

#define LED0  GET_PIN(C, 7)
#define LED1  GET_PIN(B, 7)
#define LED2  GET_PIN(B, 14)

extern SensorData Sensor_Data;

int main(void)
{
    int count = 1;
    Sensor_Data.Page = 0;
    rt_pin_mode(LED0, PIN_MODE_OUTPUT);
    rt_pin_mode(LED1, PIN_MODE_OUTPUT);
    rt_pin_mode(LED2, PIN_MODE_OUTPUT);

    while (count++)
    {
        //LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);

        Sensor_Data.Page ++;
        if(Sensor_Data.Page > 1)
            Sensor_Data.Page = 0;
        rt_pin_write(LED0, count % 2);
        rt_pin_write(LED1, count % 5);
        rt_pin_write(LED2, count % 7);
    }

    return RT_EOK;
}
