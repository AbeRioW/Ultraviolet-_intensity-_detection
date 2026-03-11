#ifndef __OLED_1315_H
#define __OLED_1315_H

#include "main.h"
#include "stdlib.h"

// OLED SSD1315 参数定义
#define OLED_WIDTH      128
#define OLED_HEIGHT     64
#define OLED_PAGES      8
#define OLED_ADDRESS    0x78  // I2C地址 (0x3C << 1)

// OLED控制命令
#define OLED_CMD        0x00
#define OLED_DATA       0x40

// 函数声明
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DisplayOn(void);
void OLED_DisplayOff(void);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
void OLED_Refresh(void);
void OLED_WriteCommand(uint8_t cmd);
void OLED_WriteData(uint8_t data);
void OLED_Test(void);

#endif
