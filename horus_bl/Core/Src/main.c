/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FILENAME "horus_fw.bin"
#define APPLICATION_ADDRESS 0x08008000 // Address after 32KB (0x8000)
#define BUFFER_SIZE 2048 // 2KB buffer
#define START_SECTOR FLASH_SECTOR_2 // use FLASH_SECTOR_2 for release and FLASH_SECTOR_3 for debug
#define NR_OF_SECTORS 4 // End sector is 5. This is the number of sectors to erase
#define MAX_FILE_SIZE 229376 //224KBs
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
typedef enum {
    PIN_ERROR,
    PIN_WARN,
    PIN_INFO,
	PIN_SDIO
} PinType;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SDIO_SD_Init(void);
/* USER CODE BEGIN PFP */
static void goto_application(void);
int isBoot1High(void);
int flashFirmwareInChunks(FIL *file, uint32_t fileSize);
void toggleIndicator(PinType pinType, uint16_t delay, uint8_t times);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int isBoot1High(void) {
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_SET;
}

void toggleIndicator(PinType pinType, uint16_t delay, uint8_t times) {
    uint16_t pin;
    GPIO_TypeDef *port = GPIOC;

    // Determine which pin to toggle based on pinType
    switch (pinType) {
	case PIN_ERROR:
		pin = ErrorInd_Pin;
		break;
	case PIN_WARN:
		pin = WarnInd_Pin;
		break;
	case PIN_INFO:
		pin = InfoInd_Pin;
		break;
	default:
		return; // Invalid pinType, exit function
    }
    for (uint8_t i = 0; i < times * 2; i++) {
        HAL_GPIO_TogglePin(port, pin);
        HAL_Delay(delay);
    }
    HAL_Delay(delay * 3);
}

int flashFirmwareInChunks(FIL *file, uint32_t fileSize) {
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SectorError;
	uint8_t buffer[BUFFER_SIZE];
	uint32_t bytesRead = 0;
	UINT readBytes;
	uint32_t address = APPLICATION_ADDRESS;

	toggleIndicator(PIN_WARN, 30, 3);
	HAL_FLASH_Unlock();

	//Erase the application sectors in the flash memory
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = START_SECTOR; //Specify start sector number
	EraseInitStruct.NbSectors = NR_OF_SECTORS; //Specify num of sectors
	int res = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);

	if (res != HAL_OK) {
		HAL_FLASH_Lock();
		return HAL_ERROR;
	}

	// Write the new firmware in chunks
	while (bytesRead < fileSize) {
		FRESULT res = f_read(file, buffer, BUFFER_SIZE, &readBytes);
		if (res != FR_OK || readBytes == 0) {
			HAL_FLASH_Lock();
			return HAL_ERROR;
		}

		for (uint32_t i = 0; i < readBytes; i += 4) {
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, *(uint32_t *)(buffer + i)) != HAL_OK) {
				HAL_FLASH_Lock();
				return HAL_ERROR;
			}
		}

		address += readBytes;
		bytesRead += readBytes;
		toggleIndicator(PIN_WARN, 30, 3);
	}

	HAL_FLASH_Lock();
	return HAL_OK;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
  if (!isBoot1High()) {
	goto_application();
  }

  // Turn on the SD Card Led before accessing the SD Card.
  HAL_GPIO_WritePin(SDIOInd_GPIO_Port, SDIOInd_Pin, GPIO_PIN_SET);


  FIL file;
  uint32_t fileSize;
  FATFS FatFs;

  if (f_mount(&FatFs, SDPath, 1) == FR_OK) {
	  if (f_open(&file, FILENAME, FA_READ) == FR_OK) {
		  fileSize = f_size(&file);
		  if (fileSize <= MAX_FILE_SIZE) {
			  toggleIndicator(PIN_INFO, 150, 5);
			  if (flashFirmwareInChunks(&file, fileSize) == HAL_OK) {
				  f_close(&file);
				  // Show the Success LED Sequence.
				  toggleIndicator(PIN_INFO, 150, 5);
				  HAL_Delay(1000);
				  // Turn the SD Card LED off.
				  HAL_GPIO_WritePin(SDIOInd_GPIO_Port, SDIOInd_Pin, GPIO_PIN_RESET);
			  }
		  }
		  f_close(&file);
	  }

  }

  // Reaching this code means there was an error.
  toggleIndicator(PIN_ERROR, 150, 5);
  // Turn the SD Card LED off.
  HAL_GPIO_WritePin(SDIOInd_GPIO_Port, SDIOInd_Pin, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */
		hsd.Instance = SDIO;
		hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
		hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
		hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
		hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
		hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
		hsd.Init.ClockDiv = 0;
		if (HAL_SD_Init(&hsd) == HAL_OK) {
			HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B);
		}
		return;
  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_4B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ErrorInd_Pin|WarnInd_Pin|InfoInd_Pin|SDIOInd_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ErrorInd_Pin WarnInd_Pin InfoInd_Pin SDIOInd_Pin */
  GPIO_InitStruct.Pin = ErrorInd_Pin|WarnInd_Pin|InfoInd_Pin|SDIOInd_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void goto_application(void) {
	void (*app_reset_handler) (void) = (void*) ( *(volatile uint32_t *) (0x08008000 + 4));
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	  __set_MSP( ( *(volatile uint32_t *) (0x08008000 + 4)) );
//	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	  app_reset_handler();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
