#include "direct_8266.h"
#include "stdio.h"
#include "string.h"

static uint8_t rx_buf[ESP8266_RX_BUF_SIZE];
static uint16_t rx_len = 0;
static ESP8266_StatusTypeDef esp_status = ESP8266_IDLE;

extern UART_HandleTypeDef huart1;

void ESP8266_Init(void)
{
    rx_len = 0;
    esp_status = ESP8266_IDLE;
    memset(rx_buf, 0, ESP8266_RX_BUF_SIZE);
    HAL_UART_Receive_IT(&huart1, &rx_buf[rx_len], 1);
}

void ESP8266_Process(void)
{
    if (rx_len > 0)
    {
        if (rx_buf[rx_len - 1] == '\n' || rx_len >= ESP8266_RX_BUF_SIZE - 1)
        {
            rx_buf[rx_len] = '\0';

            if (strstr((char*)rx_buf, "OK") != NULL)
            {
                if (strstr((char*)rx_buf, "CONNECT") != NULL)
                {
                    esp_status = ESP8266_CONNECTED;
                }
            }

            if (strstr((char*)rx_buf, "IPD") != NULL)
            {
                esp_status = ESP8266_CONNECTED;
            }

            if (strstr((char*)rx_buf, "CIFSR") != NULL)
            {
                esp_status = ESP8266_CONNECTED;
            }

            rx_len = 0;
            memset(rx_buf, 0, ESP8266_RX_BUF_SIZE);
        }
    }
}

void ESP8266_SendData(uint8_t* data, uint16_t len)
{
    HAL_UART_Transmit(&huart1, data, len, 100);
}

void ESP8266_SendString(char* str)
{
    uint16_t len = strlen(str);
    HAL_UART_Transmit(&huart1, (uint8_t*)str, len, 100);
}

ESP8266_StatusTypeDef ESP8266_GetStatus(void)
{
    return esp_status;
}

void ESP8266_ConnectWiFi(char* ssid, char* password)
{
    char cmd[100];
    
    // 设置为Station+AP模式
    ESP8266_SendString("AT+CWMODE=3\r\n");
    HAL_Delay(1000);
    
    // 连接WiFi
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
    ESP8266_SendString(cmd);
    HAL_Delay(5000);
}

void ESP8266_ConnectServer(char* ip, uint16_t port)
{
    char cmd[100];
    
    // 设置为单连接模式
    ESP8266_SendString("AT+CIPMUX=0\r\n");
    HAL_Delay(1000);
    
    // 连接到服务器
    sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip, port);
    ESP8266_SendString(cmd);
    HAL_Delay(3000);
}

void ESP8266_SetTransparentMode(void)
{
    // 开启透传模式
    ESP8266_SendString("AT+CIPMODE=1\r\n");
    HAL_Delay(1000);
    
    // 开始透传
    ESP8266_SendString("AT+CIPSEND\r\n");
    HAL_Delay(1000);
    
    esp_status = ESP8266_TRANSPARENT;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (rx_len < ESP8266_RX_BUF_SIZE - 1)
        {
            rx_len++;
        }
        HAL_UART_Receive_IT(&huart1, &rx_buf[rx_len], 1);
    }
}
