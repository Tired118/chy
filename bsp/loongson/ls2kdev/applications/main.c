/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-05     bigmagic    first version
 */

#include <rtthread.h>
#include "dht11.h"
#include "sht20.h"
#include "sgp30.h"
#include "testdata.h"

/*

ifaddr syn0 192.168.12.100 
load tftp://192.168.12.35/rtthread.elf;
g

*/

rt_uint8_t recv_data[64];
int fan_state = 0;

void fan_on()
{
    rt_pin_write(10, 1);
}

void fan_off()
{
    rt_pin_write(10, 0);
}


void uart_send(char *str, int len)
{
    rt_kprintf("test send\n");
}

void read_esp8266_data(char* str)
{
    //接收到rt-thread的数据
    if((str[0] == 0xAA) && (str[1] == 0xBB))
    {
        fan_state = str[6];
    }
}

void send_esp8266_data(char* str)
{
    str[0] = 0xBB;
    str[1] = 0xAA;
    //发送数据到模块
    uart_send(str, rt_strlen(str));
}

void delay_us(int tick)
{
    while (tick--)
    {
        /* code */
    }
    
}

int main(int argc, char **argv)
{
    DHT11_Data_TypeDef DHT11_Data;
    SGP30_Data_TypeDef SGP30_Data;
    SHT20_Data_TypeDef SHT20_Data;
    rt_uint8_t sensor_data[8];

    int ii = 0;
    int pp = 0;

    while(1)
    {
#if 1
        Read_DHT11(&DHT11_Data);
        sensor_data[0] = DHT11_Data.data;
        Read_SGP30(&SGP30_Data);
        sensor_data[1] = SGP30_Data.data;
        Read_SHT20(&SHT20_Data);
        sensor_data[2] = SHT20_Data.data;
        send_esp8266_data(sensor_data);
    
        if(fan_state)
        {
            fan_on();
        }
#endif
        pp = pp + 1;
        ii = pp % 10;
        rt_kprintf("龙芯2k1000蔬菜大棚监控系统!\n");
        rt_kprintf("当前环境温度     :%d\n", test_data1[ii]);
        rt_kprintf("当前环境湿度     :%d\n", test_data2[ii]);
        rt_kprintf("当前二氧化碳浓度 :%d\n", test_data3[ii]);
        rt_kprintf("当前光照强度     :%d\n", test_data4[ii]);
        rt_kprintf("当前土壤温度     :%d\n", test_data5[ii]);
        rt_kprintf("当前土壤湿度     :%d\n", test_data6[ii]);
        rt_kprintf("\n\r");
        rt_thread_mdelay(3000);
    }

    return 0;
}

