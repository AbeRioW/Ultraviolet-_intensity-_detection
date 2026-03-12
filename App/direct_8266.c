#include "direct_8266.h"
#include "stdio.h"
#include "string.h"
#include "oled_1315.h"
#include "stdbool.h"

bool device_connect=false;

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
		
		if(size>10&&size<100)
		{
				send_size=2;
		}
		
	  if(device_connect)
		{
			char send_data[50]={0};
			char send_data1[50]={0};
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

	char *lay1on = "\r\n+IPD,0,6:lay1on";
	char *lay1off = "\r\n+IPD,0,7:lay1off";	
	
	char *lay2on = "\r\n+IPD,0,6:lay2on";
	char *lay2off = "\r\n+IPD,0,7:lay2off";		
	
	char *fanon = "\r\n+IPD,0,5:fanon";
	char *fanoff = "\r\n+IPD,0,6:fanoff";

	if(rx1_end_flag)
	{
			rx1_end_flag = false;
		  if(memcmp(uart1_rx,wifi_connect,9)==0)  //wifiґÁ¬½ӊ			
				{
						device_connect=true;
					  OLED_ShowString(0, 1, (uint8_t*)"Client Connect", 8);
					   OLED_Refresh();
			  }
			rx1_count=0;
			memset(uart1_rx,0,1000);
			HAL_UART_Receive_DMA(&huart1,uart1_rx,1000);  //ШҪ֘Ђƴ¶¯DMA
	} 			
}
