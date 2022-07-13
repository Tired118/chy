#ifndef SHT20_H__
#define SHT20_H__

#include <rtthread.h>
#include <rthw.h>

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
	uint8_t  data;	 	//校验和
		                 
}SHT20_Data_TypeDef;

uint8_t Read_SHT20(SHT20_Data_TypeDef *SHT20_Data);

#endif