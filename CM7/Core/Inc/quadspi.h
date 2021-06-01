/**
  ******************************************************************************
  * @file    quadspi.h
  * @brief   This file contains all the function prototypes for
  *          the quadspi.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QUADSPI_H__
#define __QUADSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN Private defines */
uint8_t CSP_QUADSPI_Init( void );
/* USER CODE END Private defines */
uint8_t
CSP_QSPI_WriteMemory( uint8_t* buffer, uint32_t address, uint32_t buffer_size );

void MX_QUADSPI_Init(void);
uint8_t
CSP_QSPI_EraseSector( uint32_t EraseStartAddress, uint32_t EraseEndAddress );

uint8_t
CSP_QSPI_EnableMemoryMappedMode( void );

uint8_t
CSP_QSPI_Erase_Chip( void );

/* USER CODE BEGIN Prototypes */

#define MEMORY_FLASH_SIZE               0x4000000 /* 512 MBits*/
#define MEMORY_SECTOR_SIZE              0x1000   /* 64kBytes */
#define MEMORY_PAGE_SIZE                0x100     /* 256 bytes */
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __QUADSPI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
