/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
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

/* Includes ------------------------------------------------------------------*/
#include "quadspi.h"


#ifdef MICRON_QSPI
#define CHIP_ERASE_CMD 0xC7
#define READ_STATUS_REG_CMD 0x05
#define WRITE_ENABLE_CMD 0x06
#define READ_CONFIGURATION_REG_CMD ( 0x15UL )
#define WRITE_STATUS_REG_CMD ( 0x01UL )
#define SECTOR_ERASE_CMD 0xD8
#define SECTOR_ERASE_CMD_QUAD 0x21
#define QUAD_IN_FAST_PROG_CMD ( 0x32UL )
#define FOUR_BYTE_QUAD_IN_FAST_PROG_CMD ( 0x34UL )
#define ENTER_QUAD_MODE ( 0x35UL )
#define ENTER_FOUR_BYTE ( 0xB7UL )
#define EXIT_QUAD_MODE ( 0xF5UL )
#define QUAD_OUT_FAST_READ_CMD ( 0x6BUL )
#define FOUR_BYTE_QUAD_OUT_FAST_READ_CMD ( 0x6CUL )
#define RESET_ENABLE_CMD 0x66
#define RESET_EXECUTE_CMD 0x99
#define DUMMY_CLOCK_CYCLES_READ_QUAD ( 8UL )

#else

#define CHIP_ERASE_CMD 0xC7
#define READ_STATUS_REG_CMD 0x05
#define WRITE_ENABLE_CMD 0x06
#define READ_CONFIGURATION_REG_CMD ( 0x15UL )
#define WRITE_STATUS_REG_CMD ( 0x01UL )
#define SECTOR_ERASE_CMD 0x20
#define SECTOR_ERASE_CMD_QUAD 0x21
#define QUAD_IN_FAST_PROG_CMD ( 0x32UL )
#define FOUR_BYTE_QUAD_IN_FAST_PROG_CMD ( 0xECUL )
#define ENTER_QUAD_MODE ( 0x35UL )
#define ENTER_FOUR_BYTE ( 0xB7UL )
#define EXIT_QUAD_MODE ( 0xF5UL )
#define QUAD_OUT_FAST_READ_CMD ( 0x6BUL )
#define FOUR_BYTE_QUAD_OUT_FAST_READ_CMD ( 0x6CUL )
#define RESET_ENABLE_CMD 0x66
#define RESET_EXECUTE_CMD 0x99
#define DUMMY_CLOCK_CYCLES_READ_QUAD ( 8UL )
#endif

/* USER CODE BEGIN 0 */


    #define READ_STATUS_REG1_CMD                  0x05
    #define READ_STATUS_REG2_CMD                  0x35
    #define READ_STATUS_REG3_CMD                  0x15

    #define WRITE_STATUS_REG1_CMD                 0x01
    #define WRITE_STATUS_REG2_CMD                 0x31
    #define WRITE_STATUS_REG3_CMD                 0x11


/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;

static uint8_t
QSPI_Enter4Bytes( void );
static uint8_t
QSPI_EnterQuad( void );
static uint8_t
QSPI_WriteDisable( void );
static uint8_t
QSPI_WriteEnable( void );
static uint8_t
QSPI_WriteEnable_Quad( void );
uint8_t QSPI_AutoPollingMemReady( void );
uint8_t QSPI_AutoPollingMemReady_Quad( void );
/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

#ifdef MICRON_QSPI
	 hqspi.Instance = QUADSPI;
	  hqspi.Init.ClockPrescaler = 1;
	  hqspi.Init.FifoThreshold = 1;
	  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
	  hqspi.Init.FlashSize = 28;
	  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_3_CYCLE;
	  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
	  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
	  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;

#else

	 hqspi.Instance = QUADSPI;
	  hqspi.Init.ClockPrescaler = 2;
	  hqspi.Init.FifoThreshold = 1;
	  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
	  hqspi.Init.FlashSize = 25;
	  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE;
	  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
	  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
	  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
#endif

  /* USER CODE END QUADSPI_Init 1 */

  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* QUADSPI clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**QUADSPI GPIO Configuration
    PG6     ------> QUADSPI_BK1_NCS
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF8     ------> QUADSPI_BK1_IO0
    PF9     ------> QUADSPI_BK1_IO1
    PB2     ------> QUADSPI_CLK
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }
}


uint8_t
CSP_QSPI_EraseSector( uint32_t EraseStartAddress, uint32_t EraseEndAddress )
{
    QSPI_CommandTypeDef sCommand;



    //QSPI_EnterQuad();



    EraseStartAddress = EraseStartAddress
                        - EraseStartAddress % MEMORY_SECTOR_SIZE;

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = SECTOR_ERASE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    while ( EraseEndAddress >= EraseStartAddress )
    {
        sCommand.Address = ( EraseStartAddress & 0x0FFFFFFF );


        if ( HAL_OK != QSPI_WriteEnable( ) )
        {
            return HAL_ERROR;
        }



        if ( HAL_OK !=
             HAL_QSPI_Command( &hqspi, &sCommand,
                               HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
        {
            return HAL_ERROR;
        }

        EraseStartAddress += MEMORY_SECTOR_SIZE;

        if ( HAL_OK != QSPI_AutoPollingMemReady( ) )
        {
            return HAL_ERROR;
        }
    }

    return HAL_OK;
}


void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /**QUADSPI GPIO Configuration
    PG6     ------> QUADSPI_BK1_NCS
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF8     ------> QUADSPI_BK1_IO0
    PF9     ------> QUADSPI_BK1_IO1
    PB2     ------> QUADSPI_CLK
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);

  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


static unsigned char
_Flash_Busy_Quad( void )
{
    unsigned char status_reg;
    QSPI_CommandTypeDef cmd;

    cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.Instruction = 0x05;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_24_BITS;
    cmd.Address = 0x00000000UL;
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytes = 0;
    cmd.AlternateBytesSize = 0;
    cmd.DataMode = QSPI_DATA_4_LINES;
    cmd.DummyCycles = 0;
    cmd.NbData = 1;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command( &hqspi, &cmd, 1000 );
    HAL_QSPI_Receive( &hqspi, &status_reg, 1000 );


    return ( status_reg & 0x01 );
}

static unsigned char
_Flash_Busy( void )
{
    unsigned char status_reg;
    QSPI_CommandTypeDef cmd;

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = 0x05;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.AddressSize = QSPI_ADDRESS_24_BITS;
    cmd.Address = 0x00000000UL;
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytes = 0;
    cmd.AlternateBytesSize = 0;
    cmd.DataMode = QSPI_DATA_1_LINE;
    cmd.DummyCycles = 0;
    cmd.NbData = 1;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command( &hqspi, &cmd, 1000 );
    HAL_QSPI_Receive( &hqspi, &status_reg, 1000 );


    return ( status_reg & 0x01 );
}


static uint8_t
QSPI_WriteEnable_Quad( void )
{
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    sCommand.Instruction = 0x06;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
        return HAL_ERROR;
    }


    return HAL_OK;
}



static uint8_t
QSPI_WriteDisable( void )
{
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction = 0x04;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
        return HAL_ERROR;
    }

    sConfig.Match = 0x02;
    sConfig.Mask = 0x02;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0xf0;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    sCommand.Instruction = 0x05UL;
    sCommand.DataMode = QSPI_DATA_1_LINE;

    if ( HAL_OK !=
         HAL_QSPI_AutoPolling( &hqspi, &sCommand, &sConfig,
                               HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}


static uint8_t
QSPI_Enter4Bytes( void )
{
    QSPI_CommandTypeDef sCommand;
    uint32_t end_addr, current_size, current_addr;

    current_addr = 0;


#ifdef MICRON_QSPI
	//Enter Quad mode
	    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	    sCommand.Instruction = ENTER_FOUR_BYTE;
	    sCommand.AddressMode = QSPI_ADDRESS_NONE ;
	    sCommand.DataMode = QSPI_DATA_NONE;
#else

	//Enter Quad mode
	    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
	    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
	    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	    sCommand.Instruction = ENTER_FOUR_BYTE;
	    sCommand.AddressMode = QSPI_ADDRESS_NONE ;
	    sCommand.DataMode = QSPI_DATA_NONE;
#endif




	    sCommand.DummyCycles = 0;
	    if ( HAL_OK !=
	         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
	    {
	        return HAL_ERROR;
	    }

}


static uint8_t
QSPI_EnterQuad( void )
{
    QSPI_CommandTypeDef sCommand;
    uint32_t end_addr, current_size, current_addr;

    current_addr = 0;

	//Enter Quad mode



#ifdef MICRON_QSPI
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = ENTER_QUAD_MODE;
    sCommand.AddressMode = QSPI_ADDRESS_NONE ;
    sCommand.DataMode = QSPI_DATA_NONE;

    sCommand.DummyCycles = 0;
    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
        return HAL_ERROR;
    }

    return HAL_OK;
#else

    return HAL_OK;
#endif


}


static uint8_t
QSPI_WriteEnable( void )
{
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction = 0x06;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
        return HAL_ERROR;
    }

    sConfig.Match = 0x02;
    sConfig.Mask = 0x02;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0xf0;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    sCommand.Instruction = 0x05UL;
    sCommand.DataMode = QSPI_DATA_1_LINE;

    if ( HAL_OK !=
         HAL_QSPI_AutoPolling( &hqspi, &sCommand, &sConfig,
                               HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

uint8_t
QSPI_ResetChip( void )
{
    QSPI_CommandTypeDef sCommand;



#ifdef MICRON_QSPI
    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = RESET_ENABLE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
       // return HAL_ERROR;
    }

    HAL_Delay( 1 );

    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = RESET_EXECUTE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
       // return HAL_ERROR;
    }

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = RESET_ENABLE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
       // return HAL_ERROR;
    }

    HAL_Delay( 1 );

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = RESET_EXECUTE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
       // return HAL_ERROR;
    }

#else




    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = RESET_ENABLE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
       // return HAL_ERROR;
    }

    HAL_Delay( 10 );

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = RESET_EXECUTE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.Address = 0;
    sCommand.DataMode = QSPI_DATA_NONE;
    sCommand.DummyCycles = 0;

    if ( HAL_OK !=
         HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
       // return HAL_ERROR;
    }
#endif




    return HAL_OK;
}

uint8_t
CSP_QSPI_EnableMemoryMappedMode( void )
{
    QSPI_CommandTypeDef sCommand;
    QSPI_MemoryMappedTypeDef sMemMappedCfg;


    if ( HAL_OK != QSPI_EnterQuad() )
     {
         return HAL_ERROR;   //  WREN failed
     }


    if ( HAL_OK != QSPI_Enter4Bytes())
     {
         return HAL_ERROR;   //  WREN failed
     }



#ifdef MICRON_QSPI
    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.NbData = 0;
    sCommand.Address = 0;
    sCommand.Instruction = FOUR_BYTE_QUAD_OUT_FAST_READ_CMD;
   // sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ_QUAD;
    sCommand.DummyCycles = 10;
    sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
#else

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.NbData = 0;
    sCommand.Address = 0;
    sCommand.Instruction = FOUR_BYTE_QUAD_OUT_FAST_READ_CMD;
   // sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_READ_QUAD;
    sCommand.DummyCycles = 8;
    sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
#endif



    if ( HAL_OK != HAL_QSPI_MemoryMapped( &hqspi, &sCommand, &sMemMappedCfg ) )
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

uint8_t
CSP_QSPI_WriteMemory( uint8_t* buffer, uint32_t address, uint32_t buffer_size )
{
    QSPI_CommandTypeDef sCommand;
    uint32_t end_addr, current_size, current_addr;

    current_addr = 0;

 //   if ( HAL_OK != QSPI_WriteDisable( ) )
  //  {
  //      return HAL_ERROR;   //  WREN failed
  //  }

  //  QSPI_EnterQuad();

   // if ( HAL_OK != QSPI_WriteEnable_Quad( ) )

    QSPI_Enter4Bytes();

    /* Calculation of the size between the write address and the end of the page */
    current_size = MEMORY_PAGE_SIZE - (address % MEMORY_PAGE_SIZE);

    if ( current_size > buffer_size )
    {
        current_size = buffer_size;
    }

    current_addr = address;
    end_addr = address + buffer_size;



#ifdef MICRON_QSPI
    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = FOUR_BYTE_QUAD_IN_FAST_PROG_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE ;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.NbData = buffer_size;
    sCommand.Address = address;
    sCommand.DummyCycles = 0;

#else

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.AddressSize = QSPI_ADDRESS_32_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = 0x34;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE ;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.NbData = buffer_size;
    sCommand.Address = address;
    sCommand.DummyCycles = 0;

#endif






    do
    {
        sCommand.Address = current_addr;
        sCommand.NbData = current_size;

        if ( current_size == 0 )
        {
            return HAL_OK;
        }


        if ( HAL_OK != QSPI_AutoPollingMemReady( ) )
        {
            return HAL_ERROR;   //  Status failed
        }

        if ( HAL_OK != QSPI_WriteEnable( ) )
        {
            return HAL_ERROR;   //  WREN failed
        }

        if ( HAL_OK !=
             HAL_QSPI_Command( &hqspi, &sCommand,
                              HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
        {
            return HAL_ERROR;   //  PP failed
        }

        if ( HAL_OK !=
             HAL_QSPI_Transmit( &hqspi, buffer,
                                HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
        {
            return HAL_ERROR;   //  Data failed
        }

        if ( HAL_OK != QSPI_AutoPollingMemReady( ) )
        {
            return HAL_ERROR;   //  Status failed
        }

        current_addr += current_size;
        buffer += current_size;
        current_size = ( ( current_addr + MEMORY_PAGE_SIZE ) > end_addr ) ?
                       ( end_addr - current_addr ) : MEMORY_PAGE_SIZE;

    } while ( current_addr <= end_addr );


    //Exit Quad mode
   /*     sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
        sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
        sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
        sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
        sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
        sCommand.Instruction = EXIT_QUAD_MODE;
        sCommand.AddressMode = QSPI_ADDRESS_NONE ;
        sCommand.DataMode = QSPI_DATA_NONE;
        sCommand.NbData = buffer_size;
        sCommand.Address = address;
        sCommand.DummyCycles = 0;
        if ( HAL_OK !=
             HAL_QSPI_Command( &hqspi, &sCommand,
                              HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
*/
    return HAL_OK;
}



uint8_t
CSP_QSPI_WriteMemory_Quad( uint8_t* buffer, uint32_t address, uint32_t buffer_size )
{
    QSPI_CommandTypeDef sCommand;
    uint32_t end_addr, current_size, current_addr;
    uint8_t *write_data;
    current_addr = 0;

 //   if ( HAL_OK != QSPI_WriteDisable( ) )
  //  {
  //      return HAL_ERROR;   //  WREN failed
  //  }

    QSPI_EnterQuad();





    /* Calculation of the size between the write address and the end of the page */
    current_size = MEMORY_PAGE_SIZE - (address % MEMORY_PAGE_SIZE);

    if ( current_size > buffer_size )
    {
        current_size = buffer_size;
    }

    current_addr = address;
    end_addr = address + buffer_size;
    write_data = buffer;

    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.Instruction = QUAD_IN_FAST_PROG_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_4_LINES ;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.NbData = buffer_size;
    sCommand.Address = address;
    sCommand.DummyCycles = 0;




    do
    {
        sCommand.Address = current_addr;
        sCommand.NbData = current_size;

        if ( current_size == 0 )
        {
            return HAL_OK;
        }


        if ( HAL_OK != QSPI_AutoPollingMemReady_Quad( ) )
        {
            return HAL_ERROR;   //  Status failed
        }

        if ( HAL_OK != QSPI_WriteEnable_Quad( ) )
        {
            return HAL_ERROR;   //  WREN failed
        }

        if ( HAL_OK !=
             HAL_QSPI_Command( &hqspi, &sCommand,
                              HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
        {
            return HAL_ERROR;   //  PP failed
        }

        if ( HAL_OK !=
             HAL_QSPI_Transmit( &hqspi, buffer,
                                HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
        {
            return HAL_ERROR;   //  Data failed
        }

        if ( HAL_OK != QSPI_AutoPollingMemReady_Quad( ) )
        {
            return HAL_ERROR;   //  Status failed
        }

        current_addr += current_size;
        buffer += current_size;
        current_size = ( ( current_addr + MEMORY_PAGE_SIZE ) > end_addr ) ?
                       ( end_addr - current_addr ) : MEMORY_PAGE_SIZE;

    } while ( current_addr <= end_addr );


    //Exit Quad mode
   /*     sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
        sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
        sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
        sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
        sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
        sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
        sCommand.Instruction = EXIT_QUAD_MODE;
        sCommand.AddressMode = QSPI_ADDRESS_NONE ;
        sCommand.DataMode = QSPI_DATA_NONE;
        sCommand.NbData = buffer_size;
        sCommand.Address = address;
        sCommand.DummyCycles = 0;
        if ( HAL_OK !=
             HAL_QSPI_Command( &hqspi, &sCommand,
                              HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
*/
    return HAL_OK;
}


uint8_t
QSPI_AutoPollingMemReady_Quad( void )
{
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;

    sCommand.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    sCommand.Instruction = READ_STATUS_REG1_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_4_LINES;
    sCommand.DummyCycles = 0UL;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 1;

    sConfig.Match = 0x00;
    sConfig.Mask = 0x01;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0x10;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    if ( HAL_OK !=
         HAL_QSPI_AutoPolling( &hqspi, &sCommand, &sConfig,
                               HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

uint8_t
QSPI_AutoPollingMemReady( void )
{
    QSPI_CommandTypeDef sCommand;
    QSPI_AutoPollingTypeDef sConfig;

    sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction = READ_STATUS_REG1_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_NONE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode = QSPI_DATA_1_LINE;
    sCommand.DummyCycles = 0UL;
    sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 1;

    sConfig.Match = 0x00;
    sConfig.Mask = 0x01;
    sConfig.MatchMode = QSPI_MATCH_MODE_AND;
    sConfig.StatusBytesSize = 1;
    sConfig.Interval = 0x10;
    sConfig.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    if ( HAL_OK !=
         HAL_QSPI_AutoPolling( &hqspi, &sCommand, &sConfig,
                               HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

uint8_t
CSP_QSPI_Erase_Chip( void )
{
    QSPI_CommandTypeDef sCommand;



#ifdef MICRON_QSPI
    if ( HAL_OK != QSPI_WriteEnable( ) )
       {
           return HAL_ERROR;
       }


       if ( HAL_OK != QSPI_AutoPollingMemReady( ) )
       {
           return HAL_ERROR;
       }


       sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;
         sCommand.AddressSize = QSPI_ADDRESS_24_BITS;
         sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
         sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;
         sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
         sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
         sCommand.Instruction = CHIP_ERASE_CMD;
         sCommand.AddressMode = QSPI_ADDRESS_NONE;
         sCommand.DataMode = QSPI_DATA_NONE;
         sCommand.DummyCycles = 0;


       if ( HAL_OK !=
            HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
       {
           return HAL_ERROR;
       }


       while ( _Flash_Busy( ) )
       {
           HAL_Delay( 10 );
       }
#else

       if ( HAL_OK != QSPI_WriteEnable( ) )
          {
              //return HAL_ERROR;
          }





       sCommand.Instruction = CHIP_ERASE_CMD;                      //
       sCommand.InstructionMode = QSPI_INSTRUCTION_1_LINE;         //
       sCommand.AddressSize = QSPI_ADDRESS_24_BITS;                //
       sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;     //
       sCommand.DdrMode = QSPI_DDR_MODE_DISABLE;                   //
       sCommand.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;      //
       sCommand.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;               //
       sCommand.AddressMode = QSPI_ADDRESS_NONE;                   //
       sCommand.Address = 0UL;                                     //
       sCommand.DataMode = QSPI_DATA_NONE;                         //
       sCommand.DummyCycles = 0UL;
       sCommand.AlternateBytes = 0;
       sCommand.AlternateBytesSize = 0;
       sCommand.NbData = 0;


          if ( HAL_OK !=
               HAL_QSPI_Command( &hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE ) )
          {
              return HAL_ERROR;
          }


          while ( _Flash_Busy( ) )
          {
              HAL_Delay( 10 );
          }

          if ( HAL_OK != QSPI_AutoPollingMemReady( ) )
          {
              return HAL_ERROR;
          }

#endif







    return HAL_OK;
}


uint8_t
CSP_QUADSPI_Init( void )
{
    //prepare QSPI peripheral for ST-Link Utility operations
	hqspi.Instance = QUADSPI;
    if (HAL_QSPI_DeInit(&hqspi) != HAL_OK) {
        return HAL_ERROR;
    }

    MX_QUADSPI_Init();

    if (QSPI_ResetChip() != HAL_OK) {
        return HAL_ERROR;
    }

    HAL_Delay(100);





    return HAL_OK;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
