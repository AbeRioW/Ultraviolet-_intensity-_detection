#include "direct_8266.h"
#include "stdio.h"
#include "string.h"
#include "oled_1315.h"
#include "stdbool.h"
#include "flash_storage.h"

bool device_connect=false;
bool hand_shack=false;
bool wifi_closed_flag=false;

bool set_flag = false;
bool start_esp8266(void)
{ 
	
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_MODE2),13,0xffff);
			HAL_Delay(1000);
      HAL_UART_Transmit(&huart1,(uint8_t*)(AT_SET),38,0xffff);
			HAL_Delay(1000);
			
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_RST),8,0xffff);
			HAL_Delay(4000);
			
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_CIPMUX),13,0xffff);
			HAL_Delay(1000);
			
		  HAL_UART_Transmit(&huart1,(uint8_t*)(AT_CIPSERVER),21,0xffff);
			HAL_Delay(1000);
			
	
			__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);  
			 HAL_UART_Receive_DMA(&huart1,uart1_rx,1000);   
			 return true;
			 
}

void send_wifi(char *data,int size)
{
	  int send_size=0;
	  if(size<10)
		{
			 send_size=1; 
		}
		
		if(size>=10&&size<100)
		{
				send_size=2;
		}
		
	  if(device_connect)
		{
			char send_data[50]={0};
			sprintf(send_data,"AT+CIPSEND=0,%d\r\n",size);
			HAL_UART_Transmit(&huart1,(uint8_t*)(send_data),15+send_size,0xffff);

			HAL_Delay(100);
			HAL_UART_Transmit(&huart1,(uint8_t*)data,size,0xffff);
		}
}


void handle_esp8266(void)
{
	static int i=0;
	char *wifi_connect = "0,CONNECT";
   char *wifi_disconnect = "CLOSED";
	
	 char *set_th = "\r\n+IPD,0,6:SET";
	
	char *lay1on = "\r\n+IPD,0,6:lay1on";
	char *lay1off = "\r\n+IPD,0,7:lay1off";	
	
	char *lay2on = "\r\n+IPD,0,6:lay2on";
	char *lay2off = "\r\n+IPD,0,7:lay2off";		
	
	char *fanon = "\r\n+IPD,0,5:fanon";
	char *fanoff = "\r\n+IPD,0,6:fanoff";

	if(rx1_end_flag)
	{
			rx1_end_flag = false;
		  if(strstr((char*)uart1_rx,wifi_connect)!=NULL)  //wifi连接			
				{
						device_connect=true;
			  }
		  if(strstr((char*)uart1_rx,wifi_disconnect)!=NULL)  //wifi断开			
				{
						device_connect=false;
			  }
			
			if(strstr((char*)uart1_rx,set_th)!=NULL)  //设置阈值			
				{
						set_flag = true;
						char *colon = strchr((char*)uart1_rx, ':');
						if(colon != NULL)
						{
								if(strncmp(colon+1, "SET", 3) == 0)
								{
										uint8_t new_threshold = atoi(colon + 4);
										if(new_threshold >= 1 && new_threshold <= 10)
										{
												threshold_value = new_threshold;
												Flash_WriteThreshold(threshold_value);
												
												char response[30];
												sprintf(response, "TH_OK:%d", threshold_value);
												send_wifi(response, strlen(response));
										}
										else
										{
												send_wifi("TH_ERR:RANGE", 12);
										}
								}
						}
						
						HAL_Delay(100);
			  }
				
			
			// 在OLED上显示收到的数据
			if (!strstr((char*)uart1_rx, "OK") && !strstr((char*)uart1_rx, "ok"))
			{
				OLED_ShowString(0, 48, (uint8_t*)"Received:", 8);
				OLED_ShowString(30, 48, (uint8_t*)uart1_rx, 8);
				OLED_Refresh();
			}
			
			rx1_count=0;
			memset(uart1_rx,0,1000);
			HAL_UART_Receive_DMA(&huart1,uart1_rx,1000);  //重新启动DMA
	} 			
}
