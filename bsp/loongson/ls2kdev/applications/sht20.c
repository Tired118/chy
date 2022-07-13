#include <drivers/pin.h>
#include "sht20.h"

#define SHT20_CLK (7)
#define SHT20_SDA (8)

void SHT20_GPIO_Config(void)
{		
    rt_pin_mode(SHT20_CLK, PIN_MODE_OUTPUT); //CLK
    rt_pin_mode(SHT20_SDA, PIN_MODE_OUTPUT); //SDK
}

void SHT20_Mode_Out_PP(void)
{
	rt_pin_mode(SHT20_SDA, PIN_MODE_OUTPUT); //SDK
}

int SHT20_DATA_IN(void)
{
    return rt_pin_read(SHT20_SDA);
}

void SHT20_Mode_IPU(void)
{
	rt_pin_mode(SHT20_SDA, PIN_MODE_INPUT); //SDK
}

void SHT20_DATA_OUT(int a)
{
	rt_pin_write(SHT20_SDA, a);
}


static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		while(SHT20_DATA_IN()==LOW);

		/*SHT20 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		delay_us(40); // 延时x us 这个延时需要大于数据0持续的时间即可	   	  

		if(SHT20_DATA_IN()==HIGH)/* x us后仍为高电平表示数据“1” */
		{
			/* 等待数据1的高电平结束 */
			while(SHT20_DATA_IN()==HIGH);

			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 // x us后为低电平表示数据“0”
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return temp;
}


uint8_t Read_SHT20(SHT20_Data_TypeDef *SHT20_Data)
{  
	/*输出模式*/
	SHT20_Mode_Out_PP();
	/*主机拉低*/
	SHT20_DATA_OUT(LOW);
	/*延时18ms*/
	rt_thread_mdelay(18);

	/*总线拉高 主机延时30us*/
	SHT20_DATA_OUT(HIGH); 

	delay_us(30);   // 延时30us

	/*主机设为输入 判断从机响应信号*/ 
	SHT20_Mode_IPU();

	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(SHT20_DATA_IN() == LOW)     
	{
		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
		while(SHT20_DATA_IN()==LOW);

		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
		while(SHT20_DATA_IN()==HIGH);

		/*开始接收数据*/   
		SHT20_Data->humi_int= Read_Byte();

		SHT20_Data->humi_deci= Read_Byte();

		SHT20_Data->temp_int= Read_Byte();

		SHT20_Data->temp_deci= Read_Byte();

		SHT20_Data->check_sum= Read_Byte();


		/*读取结束，引脚改为输出模式*/
		SHT20_Mode_Out_PP();
		/*主机拉高*/
		SHT20_DATA_OUT(HIGH);

		/*检查读取的数据是否正确*/
		if(SHT20_Data->check_sum == SHT20_Data->humi_int + SHT20_Data->humi_deci + SHT20_Data->temp_int+ SHT20_Data->temp_deci)
			return 0;
		else 
			return 1;
	}
	else
	{		
		return 1;
	}   
}
