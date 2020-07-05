/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-03     TRF       the first version
 */
#ifndef APPLICATIONS_DATABASE_H_
#define APPLICATIONS_DATABASE_H_

/****************数据库****************/

#include <rtthread.h>


typedef struct
{
    uint16_t  Temp;            //储存温度

    uint32_t  Humi;            //存储湿度

    uint8_t   Page;            //存储需要显示的页面

    uint8_t   Hour;            //存储时钟-小时

    uint8_t   Min;             //存储时钟-分钟

    uint8_t   Sec;             //存储时间-秒

}SensorData;


#endif /* APPLICATIONS_DATABASE_H_ */
