/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-02     TRF       the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <U8g2lib.h>
#include <DataBase.h>


// You may reference Drivers/drv_gpio.c for pinout
// In u8x8.h #define U8X8_USE_PINS
#define OLED_I2C_PIN_SCL                    24  // PB6
#define OLED_I2C_PIN_SDA                    25  // PB7

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       1024
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;

char   DATA[] = "U8g2 on RT-Thread";

extern SensorData Sensor_Data;

// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for all supported devices
static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,\
                                         /* clock=*/ OLED_I2C_PIN_SCL,\
                                         /* data=*/ OLED_I2C_PIN_SDA,\
                                         /* reset=*/ U8X8_PIN_NONE);
                                         // All Boards without Reset of the Display

static void Sensor_Display(void)
{
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(0, 63);
    u8g2.print(Sensor_Data.Temp);
    u8g2.setCursor(70, 63);
    u8g2.print(Sensor_Data.Humi);

    u8g2.setFont(u8g2_font_logisoso22_tf);
    u8g2.setCursor(38, 63);
    u8g2.print("*");
    u8g2.setCursor(48, 63);
    u8g2.print("C");
    u8g2.setCursor(110, 63);
    u8g2.print("%");
}

static void Hours_Display(void)
{
    u8g2.setFont(u8g2_font_logisoso18_tf);
    u8g2.setCursor(21, 22);
    u8g2.print(Sensor_Data.Hour);
    u8g2.setCursor(45, 22);
    u8g2.print(":");
    u8g2.setCursor(53, 22);
    u8g2.print(Sensor_Data.Min);
    u8g2.setCursor(87, 22);
    u8g2.print(Sensor_Data.Sec);
}

static void Sec_Flicker_ON(void)
{
    u8g2.setFont(u8g2_font_logisoso18_tf);
    u8g2.setCursor(79, 22);
    u8g2.print(":");
}

static void Sec_Flicker_OFF(void)
{
    u8g2.setFont(u8g2_font_logisoso18_tf);
    u8g2.setCursor(79, 22);
    u8g2.print(" ");
}

static void OLED_PartLine(void)
{
    u8g2.setFont(u8g2_font_u8glib_4_tf);
    u8g2.setCursor(0, 29);
    u8g2.print("--------------------------------");
    u8g2.setCursor(64, 34);
    u8g2.print("|");
    u8g2.setCursor(64, 42);
    u8g2.print("|");
    u8g2.setCursor(64, 50);
    u8g2.print("|");
    u8g2.setCursor(64, 58);
    u8g2.print("|");
    u8g2.setCursor(64, 66);
    u8g2.print("|");
}

static void ssd1306_12864(void *parameter)
{
    u8g2.begin();
    while(1)
    {
        u8g2.clearBuffer();                                 // clear the internal memory
        switch(Sensor_Data.Page)
        {
            case 0:
                Sec_Flicker_ON();
                break;
            case 1:
                Sec_Flicker_OFF();
                break;
        }
        Hours_Display();
        Sensor_Display();
        OLED_PartLine();
        u8g2.sendBuffer();
    }
}
static int OLED_collect(void)
{
    /* 创建线程*/
    tid1 = rt_thread_create("OLOED_collect",
                             ssd1306_12864,
                             RT_NULL,
                             THREAD_STACK_SIZE,
                             THREAD_PRIORITY,
                             THREAD_TIMESLICE);

     /* 如果获得线程控制块，启动这个线程 */
     if (tid1 != RT_NULL)
         rt_thread_startup(tid1);

     return 0;
}

INIT_APP_EXPORT(OLED_collect);
