#ifndef SGP30_H__
#define SGP30_H__

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
		                 
}SGP30_Data_TypeDef;

uint8_t Read_SGP30(SGP30_Data_TypeDef *SGP30_Data);

#endif