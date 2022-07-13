#ifndef ESP8266_H__
#define ESP8266_H__

#include <rtthread.h>
#include <rthw.h>

#define AT_RST_RESPONSE "OK"
#define AT_RST "AT+RST\r\n"
#define AT_CWMODE_RESPONSE "OK"
#define AT_CWMODE_1 "AT+CWMODE=1\r\n"
#define AT_CWJAP_RESPONSE "WIFI CONNECTED\r\n"
#define AT_CWJAP "AT+CWJAP=\"%s\",\"%s\"\r\n"
#define AT_CWLAP_RESPONSE "OK"
#define AT_CWLAP "AT+CWLAP\r\n"
#define AT_CIFSR_RESPONSE "OK"
#define AT_CIFSR "AT+CIFSR\r\n"
#define AT_CIPSTART_RESPONSE "OK"
#define AT_CIPSTART "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n"
#define AT_CIPSEND_RESPONSE "OK"
#define AT_CIPSEND "AT+CIPSEND=%d\r\n"
#define AT_CIPMODE_RESPONSE "OK"
#define AT_CIPMODE "AT+CIPMODE=%d\r\n"
#define AT_RESPONSE "OK"
#define AT "AT\r\n"
#define AT_GMR_RESPONSE "version:"
#define AT_GMR "AT+GMR\r\n"
#define AT_CIPMUX_RESPONSE "OK"
#define AT_CIPMUX "AT+CIPMUX=%d\r\n"
#define AT_CIPAPMAC_RESPONSE "OK"
#define AT_CIPAPMAC "AT+CIPAPMAC?\r\n"
#define RECEIVE_FILTER "+IPD,"

//8266的工作模式
typedef enum
{
    STA,
    AP,
    STA_AP  
}ENUM_Net_ModeTypeDef;

//网络传输层协议，枚举类型
typedef enum{
     enumTCP,
     enumUDP,
} ENUM_NetPro_TypeDef;

//连接号，指定为该连接号可以防止其他计算机访问同一端口而发生错误
typedef enum{
    Multiple_ID_0 = 0,
    Multiple_ID_1 = 1,
    Multiple_ID_2 = 2,
    Multiple_ID_3 = 3,
    Multiple_ID_4 = 4,
    Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;

#define FunctionalState int

uint8_t ESP8266_Send_AT_Cmd(uint8_t *cmd, uint8_t *ack1, uint8_t *ack2, uint32_t time);
void ESP8266_Reset(void);
void ESP8266_AT_RESTORE(void);
uint8_t ESP8266_AT_Test(void);
void ESP8266_AT_Reset(void);
void ESP8266_AT_ATE(uint8_t state);
uint8_t ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
uint8_t ESP8266_JoinAP( char * pSSID, char * pPassWord);
uint8_t ESP8266_Enable_MultipleId (FunctionalState enumEnUnvarnishTx);
uint8_t ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
uint8_t ESP8266_UnvarnishSend (void);
uint8_t ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId);
void ESP8266_ExitUnvarnishSend (void);
uint8_t ESP8266_Get_LinkStatus (void);
void ESP8266_TCP_CLOSE(void);
uint8_t ESP8266_Net_Mode_Get(void);

#endif