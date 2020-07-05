/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-04     TRF       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "sensor.h"
#include "sensor_dallas_dht11.h"
#include <board.h>
#include <drv_common.h>
#include <DataBase.h>

extern SensorData Sensor_Data;

/* Modify this pin according to the actual wiring situation */
#define DHT11_DATA_PIN    GET_PIN(C, 4)

static void read_temp_entry(void *parameter)
{
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data sensor_data;
    rt_size_t res;
    rt_uint8_t get_data_freq = 1; /* 1Hz */

    dev = rt_device_find("temp_dht11");
    if (dev == RT_NULL)
    {
        return;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!\n");
        return;
    }

    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)(&get_data_freq));

    while (1)
    {
        res = rt_device_read(dev, 0, &sensor_data, 1);

        if (res != 1)
        {
            rt_kprintf("read data failed! result is %d\n", res);
            rt_device_close(dev);
            return;
        }
        else
        {
            if (sensor_data.data.temp >= 0)
            {
                //uint8_t temp = (sensor_data.data.temp & 0xffff) >> 0;      // get temp
                //uint8_t humi = (sensor_data.data.temp & 0xffff0000) >> 16; // get humi
                //rt_kprintf("temp:%d, humi:%d\n" ,temp, humi);
                Sensor_Data.Temp = (sensor_data.data.temp & 0xffff) >> 0;      // get temp
                Sensor_Data.Humi = (sensor_data.data.temp & 0xffff0000) >> 16; // get humi
            }
        }

        rt_thread_delay(1000);
    }
}

static int dht11_read_temp_sample(void)
{
    rt_thread_t dht11_thread;

    dht11_thread = rt_thread_create("dht_tem",
                                     read_temp_entry,
                                     RT_NULL,
                                     1024,
                                     RT_THREAD_PRIORITY_MAX / 2,
                                     20);
    if (dht11_thread != RT_NULL)
    {
        rt_thread_startup(dht11_thread);
    }

    return RT_EOK;
}
INIT_APP_EXPORT(dht11_read_temp_sample);

static int rt_hw_dht11_port(void)
{
    struct rt_sensor_config cfg;

    cfg.intf.user_data = (void *)DHT11_DATA_PIN;
    rt_hw_dht11_init("dht11", &cfg);

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_dht11_port);
