#ifndef __DIRECT_8266_H
#define __DIRECT_8266_H

#include "main.h"
#include "usart.h"

#define ESP8266_RX_BUF_SIZE  256
#define ESP8266_TX_BUF_SIZE  256

typedef enum {
    ESP8266_IDLE = 0,
    ESP8266_CONNECTED,
    ESP8266_TRANSPARENT
} ESP8266_StatusTypeDef;

void ESP8266_Init(void);
void ESP8266_Process(void);
void ESP8266_SendData(uint8_t* data, uint16_t len);
void ESP8266_SendString(char* str);
ESP8266_StatusTypeDef ESP8266_GetStatus(void);
void ESP8266_SetTransparentMode(void);
void ESP8266_ConnectWiFi(char* ssid, char* password);
void ESP8266_ConnectServer(char* ip, uint16_t port);

#endif
