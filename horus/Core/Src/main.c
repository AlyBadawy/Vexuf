/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main VexUF:Horus program body
  ******************************************************************************
  * @attention
  *
  * Text within  USER CODE sections: Copyright (c) 2024 Aly Badawy
  * Text outside USER CODE sections: Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "sdio.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "93c86.h"
#include "adc_avs.h"
#include "actuators.h"
#include "i2c_Checker.h"
#include "i2c_aht20.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// This is used by printf(). We are redirecting that to ITM for debugging.
int _write(int file, char *ptr, int len) {
	for (int i=0; i < len; i++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}






/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{


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
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  // Actuators test
  Actuators_Lights(GPIO_PIN_SET);
  for(ActuatorPin pin = ACT_PIN_A1; pin <= ACT_PIN_A8; pin++) {
	Actuators_SetPin(pin);
	HAL_Delay(100);
	Actuators_ResetPin(pin);
	HAL_Delay(100);

  }
  Actuators_Lights(GPIO_PIN_RESET);


  // printf test

  char serial_number[25];
  generate_serial_number(serial_number);

  printf("===================================================\r\n\r\n");
  printf("   VexUF:Horus\r\n\r\n");
  printf("   Serial Number: %s\r\n\r\n", serial_number);
  printf("===================================================\r\n");

  uint32_t avsBuffer[5];
  testAvs(avsBuffer);

  // Test I2c
  printf("\r\n");
  I2C_Scan(&hi2c1);


  // Internal temp. Test
  printf("\r\n");
  // Initialize the AHT20 sensor
  if (AHT20_Init(&hi2c1) != HAL_OK) {
	  Error_Handler();
  }

  float temperature = 0.0f;
  float humidity = 0.0f;
	// Read temperature and humidity from AHT20
  printf("Internal AHT20 sensor:\r\n");
  if (AHT20_ReadTemperatureHumidity(&hi2c1, &temperature, &humidity) == HAL_OK) {
	  printf("  Temperature Internal C: %0.2f\r\n", temperature);
	  printf("  Temperature Internal F: %0.2f\r\n", cToF(temperature));
	  printf("  Humidity Internal %%: %0.2f\r\n", humidity);
  } else {
	  printf("  Error reading from AHT20\n");
  }

  	printf("\r\n");
  	printf("Testing SD Card Functionalities...\r\n");

    char TxBuffer[250];
  	FATFS FatFs;
    FIL Fil;
    FRESULT FR_Status;
    FATFS *FS_Ptr;
    UINT RWC, WWC; // Read/Write Word Counter
    DWORD FreeClusters;
    uint32_t TotalSize, FreeSpace;
    char RW_Buffer[200];
    do
    {
      //------------------[ Mount The SD Card ]--------------------
      FR_Status = f_mount(&FatFs, SDPath, 1);
      if (FR_Status != FR_OK)
      {
        sprintf(TxBuffer, "  Error! While Mounting SD Card, Error Code: (%i)\r\n", FR_Status);
        printf(TxBuffer);
        break;
      }
      sprintf(TxBuffer, "  SD Card Mounted Successfully! \r\n");
      printf(TxBuffer);
      //------------------[ Get & Print The SD Card Size & Free Space ]--------------------
      f_getfree("", &FreeClusters, &FS_Ptr);
      TotalSize = (uint32_t)((FS_Ptr->n_fatent - 2) * FS_Ptr->csize * 0.5);
      FreeSpace = (uint32_t)(FreeClusters * FS_Ptr->csize * 0.5);
      sprintf(TxBuffer, "  Total SD Card Size: %lu Bytes\r\n", TotalSize);
      printf(TxBuffer);
      sprintf(TxBuffer, "  Free SD Card Space: %lu Bytes\r\n", FreeSpace);
      printf(TxBuffer);
      //------------------[ Open A Text File For Write & Write Data ]--------------------
      //Open the file
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
      if(FR_Status != FR_OK)
      {
        sprintf(TxBuffer, "  Error! While Creating/Opening A New Text File, Error Code: (%i)\r\n", FR_Status);
        printf(TxBuffer);
        break;
      }
      sprintf(TxBuffer, "  Text File Created & Opened! Writing Data To The Text File..\r\n");
      printf(TxBuffer);
      // (1) Write Data To The Text File [ Using f_puts() Function ]
      f_puts("Hello! From STM32 To SD Card Over SDIO, Using f_puts()\n", &Fil);
      // (2) Write Data To The Text File [ Using f_write() Function ]
      strcpy(RW_Buffer, "Hello! From STM32 To SD Card Over SDIO, Using f_write().\r\n");
      f_write(&Fil, RW_Buffer, strlen(RW_Buffer), &WWC);
      // Close The File
      f_close(&Fil);
      //------------------[ Open A Text File For Read & Read Its Data ]--------------------
      // Open The File
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_READ);
      if(FR_Status != FR_OK)
      {
        sprintf(TxBuffer, "  Error! While Opening (MyTextFile.txt) File For Read.. \r\n");
        printf(TxBuffer);
        break;
      }
      // (1) Read The Text File's Data [ Using f_gets() Function ]
      f_gets(RW_Buffer, sizeof(RW_Buffer), &Fil);
      sprintf(TxBuffer, "  Data Read From (MyTextFile.txt) Using f_gets().\r\n");
      printf(TxBuffer);
      // (2) Read The Text File's Data [ Using f_read() Function ]
      f_read(&Fil, RW_Buffer, f_size(&Fil), &RWC);
      sprintf(TxBuffer, "  Data Read From (MyTextFile.txt) Using f_read().\r\n");
      printf(TxBuffer);
      // Close The File
      f_close(&Fil);
      sprintf(TxBuffer, "  File Closed! \r\n");
      printf(TxBuffer);
      //------------------[ Open An Existing Text File, Update Its Content, Read It Back ]--------------------
      // (1) Open The Existing File For Write (Update)
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_OPEN_EXISTING | FA_WRITE);
      FR_Status = f_lseek(&Fil, f_size(&Fil)); // Move The File Pointer To The EOF (End-Of-File)
      if(FR_Status != FR_OK)
      {
        sprintf(TxBuffer, "  Error! While Opening (MyTextFile.txt) File For Update.. \r\n");
        printf(TxBuffer);
        break;
      }
      // (2) Write New Line of Text Data To The File
      FR_Status = f_puts("  This New Line Was Added During File Update!\r\n", &Fil);
      f_close(&Fil);
      memset(RW_Buffer,'\0',sizeof(RW_Buffer)); // Clear The Buffer
      // (3) Read The Contents of The Text File After The Update
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_READ); // Open The File For Read
      f_read(&Fil, RW_Buffer, f_size(&Fil), &RWC);
      sprintf(TxBuffer, "  Data Read From (MyTextFile.txt) After Update.\r\n");
      printf(TxBuffer);
      f_close(&Fil);
      //------------------[ Delete The Text File ]--------------------
      // Delete The File
      /*
      FR_Status = f_unlink(MyTextFile.txt);
      if (FR_Status != FR_OK){
          sprintf(TxBuffer, "Error! While Deleting The (MyTextFile.txt) File.. \r\n");
          USC_CDC_Print(TxBuffer);
      }
      */
    } while(0);
    //------------------[ Test Complete! Unmount The SD Card ]--------------------
    FR_Status = f_mount(NULL, "", 0);
    if (FR_Status != FR_OK)
    {
        sprintf(TxBuffer, "  Error! While Un-mounting SD Card, Error Code: (%i)\r\n", FR_Status);
        printf(TxBuffer);
    } else{
        sprintf(TxBuffer, "  SD Card Un-mounted Successfully! \r\n");
        printf(TxBuffer);
    }

  // EEPROM READ TEST
    // Example usage
	uint16_t data = EEPROM_Read(0xFFEE);
	EEPROM_Write(0xFFEE, 0xABCD);
	data = EEPROM_Read(0xFFEE);
	printf("EEprom says: %i\r\n", data);



	uint16_t buffer[128];
	for (uint16_t address = 0; address < 128; address++) {
		buffer[address] = EEPROM_Read(address);
	}
    for (uint16_t address = 0; address < 128; address += 16) {
    	printf("%10X: ", address);
        // Print first column (8 words)
        for (uint16_t i = 0; i < 8; i++) {
            printf("%04X ", buffer[address + i]);
        }

        // Print separator
        printf("    ");

        // Print second column (8 words)
        for (uint16_t i = 8; i < 16; i++) {
            printf("%04X ", buffer[address + i]);
        }

        // New line
        printf("\r\n");



    }

    char writeData[25] = "Hello, EEPROM! Testing";
        char readData[25];
        uint16_t writeBuffer[13]; // 25 characters will be stored in 13 words (16-bit each)
        uint16_t readBuffer[13];

        // Convert char array to uint16_t array
        for (int i = 0; i < 12; i++) {
            writeBuffer[i] = (writeData[2 * i] << 8) | writeData[2 * i + 1];
        }
        // Handle the last character (if array size is odd)
        writeBuffer[12] = writeData[24];

        // Write to EEPROM
        EEPROM_WriteMultipleWords(0x0000, writeBuffer, 13);

        // Read from EEPROM
        EEPROM_ReadMultipleWords(0x0000, readBuffer, 13);

        // Convert uint16_t array back to char array
        for (int i = 0; i < 12; i++) {
            readData[2 * i] = (readBuffer[i] >> 8) & 0xFF;
            readData[2 * i + 1] = readBuffer[i] & 0xFF;
        }
        // Handle the last character (if array size is odd)
        readData[24] = readBuffer[12] & 0xFF;

        // Null-terminate the readData string
        readData[25] = '\0';

        // Print the read data
        printf("Read Data: %s\r\n", readData);


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
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
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

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
