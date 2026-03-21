#ifndef __FLASH_STORAGE_H__
#define __FLASH_STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>

#define FLASH_STORAGE_ADDR    0x0800FC00
#define DEFAULT_THRESHOLD     1

uint8_t Flash_ReadThreshold(void);
void Flash_WriteThreshold(uint8_t threshold);

#ifdef __cplusplus
}
#endif

#endif
