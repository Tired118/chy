#include "esp8266.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define RET_TRUE  (1)
#define RET_FLASE (1)

uint8_t paser_data[];
uint8_t data_parse_flag;

//8266 发送数据
//cmd 发送的命令
//ack1 期待接收到的回复数据1
//ack2 期待接收到的回复数据2
//time 延时函数
uint8_t ESP8266_Send_AT_Cmd(uint8_t *cmd, uint8_t *ack1, uint8_t *ack2, uint32_t time)
{ 
    data_parse_flag = 0;
    memset(paser_data, 0, 512);
    printf("send date is %s\r\n", cmd);
    //send_to_wifi(cmd, strlen((const char *)cmd) + 1);

    if(ack1==0 && ack2==0)//no return
    {
        return RET_TRUE;
    }

    HAL_Delay(time);

    printf("%s\r\n", paser_data);
    if(ack1 != NULL&&ack2 != NULL)
    {
        if(strstr((const char *)paser_data, (const char *)ack1) || strstr((const char *)paser_data, (const char *)ack2))
        {
            return RET_TRUE;
        }
        else
        {
            return RET_FLASE;
        }
    }
    else if(ack1 != NULL)
        return (strstr((const char *)paser_data, (const char *)ack1) != NULL) ? RET_TRUE : RET_FLASE;

    else
        return (strstr((const char *)paser_data, (const char *)ack2) != NULL) ? RET_TRUE : RET_FLASE;

}

//硬件复位ESP8266
void ESP8266_Reset(void)
{
    //low
    //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_Delay(500);
    //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    //high
}

void ESP8266_AT_Reset(void)
{
    ESP8266_Send_AT_Cmd("AT+RST\r\n",NULL,NULL,500);
}

//发送恢复出厂默认设置指令将模块恢复成出厂设置
void ESP8266_AT_RESTORE(void)
{
    char count=0;
    while(count < 3)
    {
        if(ESP8266_Send_AT_Cmd("AT+RESTORE\r\n","OK",NULL,500)) 
        {
            printf("OK\r\n");
            memset(paser_data, 0, 512);
            return;
        }
        HAL_Delay(100);
        ++ count;
    }
    memset(paser_data, 0, 512);
    printf("AT RESTORE Error! plase check wifi reset pin\r\n");
}

//测试AT指令
uint8_t ESP8266_AT_Test(void)
{
    char count=0;
    while(count < 10)
    {
        if(ESP8266_Send_AT_Cmd("AT\r\n","OK",NULL,500)) 
        {
            printf("OK\r\n");
            return RET_TRUE;
        }
        HAL_Delay(1000);
        ++ count;
    }
    printf("AT Test Error! plase check wifi reset pin\r\n");
    return RET_FLASE;
}



//ESP8266回显操作
//state 0:关闭回显
//state 1:打开回显
void ESP8266_AT_ATE(uint8_t state)
{
    ESP8266_Send_AT_Cmd("ATE0\r\n","OK",NULL,500);
    // char count=0;
    // //while(count < 10)
    // {
    //     if(ESP8266_Send_AT_Cmd("ATE0\r\n","OK",NULL,500)) 
    //     {
    //         printf("OK\r\n");
    //         memset(paser_data, 0, 512);
    //         return;
    //     }
    //     HAL_Delay(100);
    //     ++ count;
    // }
    // memset(paser_data, 0, 512);
}

//查询当前的esp8266工作模式
// 1:sta 2:ap 3:sta_ap
uint8_t ESP8266_Net_Mode_Get(void)
{
    uint8_t ret = 0;
    char *date;
    uint8_t mode = 0;
    //AT+CWMODE?
    //+AT+CWMODE:2
    //OK
    ret = ESP8266_Send_AT_Cmd("AT+CWMODE?\r\n", "OK", NULL, 500);
    if(RET_TRUE == ret)
    {
        date = strstr(paser_data, "CWMODE:");
        //取第8个字符
        if(date != NULL)
        {
            mode = date[7];
            mode = mode - 48;
        }
    }
    //memset(paser_data, 0, 512);
    return mode;
}

//选择ESP8266的工作模式
//enumMode 模式类型
//成功返回true，失败返回false
uint8_t ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode)
{
    uint8_t ret = RET_FLASE;
    switch(enumMode)
    {
        case STA:
            ret = ESP8266_Send_AT_Cmd("AT+CWMODE=1\r\n", "OK", "no change", 2500); 
            break;
        case AP:
            ret = ESP8266_Send_AT_Cmd("AT+CWMODE=2\r\n", "OK", "no change", 2500); 
            break;
        case STA_AP:
            ret = ESP8266_Send_AT_Cmd("AT+CWMODE=3\r\n", "OK", "no change", 2500); 
            break;
        default:
            ret = RET_FLASE;
    }
    memset(paser_data, 0, 512);
    return ret;
}


//ESP8266连接外部的WIFI
//pSSID WiFi帐号
//pPassWord WiFi密码
//设置成功返回true 反之false
uint8_t ESP8266_JoinAP(char * pSSID, char * pPassWord)
{
    uint8_t ret = RET_FLASE;
    char cCmd [120];
	
    sprintf (cCmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", pSSID, pPassWord);
    ret =  ESP8266_Send_AT_Cmd((uint8_t *)cCmd, "CONNECTED", NULL, 5000);
    memset(paser_data, 0, 512);
    return ret;
}

//ESP8266 透传使能
//enumEnUnvarnishTx  是否多连接，bool类型
//设置成功返回true，反之false
uint8_t ESP8266_Enable_MultipleId(FunctionalState enumEnUnvarnishTx )
{
    uint8_t ret = RET_FLASE;
    char cStr [20];

    sprintf(cStr, "AT+CIPMUX=%d\r\n", (enumEnUnvarnishTx ? 1 : 0));

    ret = ESP8266_Send_AT_Cmd((uint8_t *)cStr, "OK", 0, 500);
    memset(paser_data, 0, 512);
    return ret;
}

//ESP8266 连接服务器
//enumE  网络类型
//ip ，服务器IP
//ComNum  服务器端口
//id，连接号，确保通信不受外界干扰
//设置成功返回true，反之fasle
uint8_t ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
    uint8_t ret = RET_FLASE;
    char cStr [100] = { 0 }, cCmd [120];

    switch(enumE)
    {
        case enumTCP:
          sprintf(cStr, "\"%s\",\"%s\",%s\r\n", "TCP", ip, ComNum);
          break;

        case enumUDP:
          sprintf(cStr, "\"%s\",\"%s\",%s\r\n", "UDP", ip, ComNum);
          break;

        default:
            break;
    }

    if (id < 5)
        sprintf(cCmd, "AT+CIPSTART=%d,%s\r\n", id, cStr);

    else
        sprintf(cCmd, "AT+CIPSTART=%s\r\n", cStr);

    ret = ESP8266_Send_AT_Cmd((uint8_t *)cCmd, "OK", "CONNECT", 4000);
    memset(paser_data, 0, 512);
    return ret;
}

//关闭连接
void ESP8266_TCP_CLOSE(void)
{
    ESP8266_Send_AT_Cmd("AT+CIPCLOSE\r\n", NULL, NULL, 500);
    memset(paser_data, 0, 512);
}


//ESP8266发送字符串
//enumEnUnvarnishTx是否使能透传模式
//pStr字符串
//ulStrLength字符串长度
//ucId 连接号
//设置成功返回true， 反之false
uint8_t ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId)
{
    char cStr [20];
    uint8_t bRet = RET_FLASE;


    if(enumEnUnvarnishTx)
    {
        send_to_wifi((uint8_t *)pStr, strlen((const char *)pStr) + 1);

        bRet = RET_TRUE;

    }

    else
    {
        if(ucId < 5)
            sprintf (cStr, "AT+CIPSEND=%d,%d\r\n", ucId, ulStrLength);

        else
            sprintf (cStr, "AT+CIPSEND=%d\r\n", ulStrLength);

        bRet = ESP8266_Send_AT_Cmd((uint8_t *)cStr, ">", NULL, 1500);
        HAL_Delay(300);
        if(bRet==RET_TRUE)
        {
            bRet = ESP8266_Send_AT_Cmd((uint8_t *)pStr, "SEND OK", NULL, 1000);
        }
        else
        {
            //ESP8266_Send_AT_Cmd((uint8_t *)cStr, ">", NULL, 1500);
            ESP8266_Send_AT_Cmd((uint8_t *)pStr, "SEND OK", NULL, 1000);
            ESP8266_Send_AT_Cmd("\r\n", "SEND OK", 0, 1000);
 
            ESP8266_Send_AT_Cmd("+++\r\n", NULL, NULL, 1500);
            HAL_Delay(2000);
        }
        // HAL_Delay(300);
        // bRet = ESP8266_Send_AT_Cmd((uint8_t *)cStr, ">", 0, 1000);
    }
    //memset(paser_data, 0, 512);
    //printf("bRet=%d\r\n", bRet);
    return bRet;

}


//ESP8266退出透传模式
void ESP8266_ExitUnvarnishSend(void)
{
    HAL_Delay(1000);
    send_to_wifi("+++\r\n", 4);
    HAL_Delay( 500 );    
}


//ESP8266 检测连接状态
//返回0：获取状态失败
//返回2：获得ip
//返回3：建立连接 
//返回4：失去连接 
uint8_t ESP8266_Get_LinkStatus(void)
{
    uint8_t ret = 0;
    if (ESP8266_Send_AT_Cmd("AT+CIPSTATUS\r\n", "OK", 0, 500))
    {
        if ( strstr((const char *)paser_data, "STATUS:2"))
            ret = 2;

        else if(strstr((const char *)paser_data, "STATUS:3"))
            ret = 3;

        else if(strstr((const char *)paser_data, "STATUS:4"))
            ret = 4;
    }
    return ret;

}

static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;

    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */