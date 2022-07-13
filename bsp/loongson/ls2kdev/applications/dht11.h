#ifndef DHT11_H__
#define DHT11_H__

#include <rtthread.h>
#include <rthw.h>

#define HIGH  1
#define LOW   0

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
	uint8_t  data;	 	//校验和
		                 
}DHT11_Data_TypeDef;

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);

#endif