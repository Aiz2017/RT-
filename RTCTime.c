/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-04     TRF       the first version
 */
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <DataBase.h>

#define THREAD_PRIORITY         20
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        10

static rt_thread_t Time_tid = RT_NULL;

extern SensorData Sensor_Data;

static void rtc_sample(void *parameter)
{
    rt_err_t ret = RT_EOK;
    time_t now;
    struct tm *p;

    /* set data */
    ret = set_date(2020, 7, 5);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
    }

    /* set time */
    ret = set_time(16, 17, 00);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
    }

    /* delay 3s */
    rt_thread_mdelay(3000);

    while(1)
    {
        /* get time */
        now = time(RT_NULL);

        p=gmtime((const time_t*) &now);
        Sensor_Data.Hour   = p->tm_hour;
        Sensor_Data.Min    = p->tm_min;
        Sensor_Data.Sec    = p->tm_sec;

        //rt_kprintf(ctime(&now));
        rt_thread_mdelay(1000);
    }

}

static int RTCTime_collect(void)
{
    /* 创建线程*/
    Time_tid = rt_thread_create("OLED_collect",
                             rtc_sample,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);

     /* 如果获得线程控制块，启动这个线程 */
     if (Time_tid != RT_NULL)
         rt_thread_startup(Time_tid);

     return 0;
}

INIT_APP_EXPORT(RTCTime_collect);


