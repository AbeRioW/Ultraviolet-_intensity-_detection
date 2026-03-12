#include "flash_storage.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"

uint8_t Flash_ReadThreshold(void)
{
    uint8_t *p = (uint8_t *)FLASH_STORAGE_ADDR;
    uint8_t value = *p;
    
    if (value < 1 || value > 10)
    {
        return DEFAULT_THRESHOLD;
    }
    
    return value;
}

void Flash_WriteThreshold(uint8_t threshold)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError = 0;
    
    HAL_FLASH_Unlock();
    
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_STORAGE_ADDR;
    EraseInitStruct.NbPages = 1;
    
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, FLASH_STORAGE_ADDR, threshold);
    }
    
    HAL_FLASH_Lock();
}
