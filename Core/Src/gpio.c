/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "flash_storage.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, OLED_SCL_Pin|OLED_SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : OLED_SCL_Pin OLED_SDA_Pin */
  GPIO_InitStruct.Pin = OLED_SCL_Pin|OLED_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY1_Pin KEY2_Pin KEY3_Pin */
  GPIO_InitStruct.Pin = KEY1_Pin|KEY2_Pin|KEY3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : HC_SR505_Pin */
  GPIO_InitStruct.Pin = HC_SR505_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(HC_SR505_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 2 */
static uint32_t last_key1_time = 0;
static uint32_t last_key2_time = 0;
static uint32_t last_key3_time = 0;
#define KEY_DEBOUNCE_TIME 100

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    extern volatile uint8_t setting_mode;
    extern volatile uint8_t threshold_value;
    extern uint8_t last_threshold_value;
    extern void Flash_WriteThreshold(uint8_t threshold);
    
    uint32_t current_time = HAL_GetTick();
    
    if (GPIO_Pin == KEY1_Pin)
    {
        if (current_time - last_key1_time > KEY_DEBOUNCE_TIME)
        {
            if (setting_mode)
            {
                Flash_WriteThreshold(threshold_value);
                last_threshold_value = threshold_value;
            }
            setting_mode = !setting_mode;
            last_key1_time = current_time;
        }
    }
    else if (GPIO_Pin == KEY2_Pin)
    {
        if (current_time - last_key2_time > KEY_DEBOUNCE_TIME)
        {
            if (setting_mode)
            {
                threshold_value++;
                if (threshold_value > 10)
                {
                    threshold_value = 1;
                }
                last_key2_time = current_time;
            }
        }
    }
    else if (GPIO_Pin == KEY3_Pin)
    {
        if (current_time - last_key3_time > KEY_DEBOUNCE_TIME)
        {
            if (setting_mode)
            {
                threshold_value--;
                if (threshold_value < 1)
                {
                    threshold_value = 10;
                }
                last_key3_time = current_time;
            }
        }
    }
}
/* USER CODE END 2 */
