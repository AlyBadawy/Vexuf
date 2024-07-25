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
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "actuators.h"

#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define VREFINT      		1.22
#define ADC_RESOLUTION    	4096	// 12-bit ADC resolution


#define V25					0.76
#define CPU_TEMP_AVG_SLOPE  0.0025

#define adcR1               33000.0
#define adcR2               3300.0
#define adcRatio			(adcR2 / (adcR1 + adcR2))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t adcValues[5];	// buffer to store the AVs values from ADC, an array of 5 values
//  adcValues[0]  		<< value for VREFINT to calibrate VREF
//  adcValues[1]  		<< value for Internal temperature (CPU)
//  adcValues[2]  		<< value for AV1
//  adcValues[3]  		<< value for Av2
//  adcValues[4]  		<< value for Av3

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

float getVref(uint32_t adcValue) {
	float value = adcValue;
	return (VREFINT * ADC_RESOLUTION) / value;
}

float getCpuTempC(uint32_t vrefValue, uint32_t adcValue) {
	float vref = getVref(vrefValue);
	float temp_sense = (vref/ADC_RESOLUTION) * adcValue;
	return (((V25 - temp_sense) / CPU_TEMP_AVG_SLOPE) + 25.0);
}

float cToF(float c) {
	return (c * (9.0 / 5.0)) + 32.0;
}

float fToC(float f) {
	return (f - 32.0) * (5.0 / 9.0);
}

float adcToAv(uint32_t vrefValue, uint32_t adcValue) {
	float vref = getVref(vrefValue);
	float voltage = ((adcValue * vref) / ADC_RESOLUTION) / adcRatio;
	if (voltage < 0.05) { voltage = 0.0; }
	return voltage;
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
  MX_RTC_Init();
  MX_ADC1_Init();
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
  printf("Hello from VexUF\r\n");

  // ADCs test
  HAL_ADC_Start_DMA(&hadc1, adcValues, 5);

  uint32_t vrefValue = adcValues[0];
  float vrefValueFloat = adcValues[0];
  printf("value of VREF: %f which is %0.3fV\r\n", vrefValueFloat, getVref(vrefValue));

  float tempratureValue = adcValues[1];
  float cpuTempC = getCpuTempC(vrefValue, tempratureValue);
  float cpuTempF = cToF(cpuTempC);
  printf("value of temperature: %f, which is %0.3fC, and %0.3fF\r\n", tempratureValue, cpuTempC, cpuTempF);

  float av1 = adcValues[2];
  float av2 = adcValues[3];
  float av3 = adcValues[4];
  printf("value of av1: %f, which is %0.3fV\r\n", av1, adcToAv(vrefValue, av1));
  printf("value of av2: %f, which is %0.3fV\r\n" ,av2, adcToAv(vrefValue, av2));
  printf("value of av3: %f, which is %0.3fV\r\n", av3, adcToAv(vrefValue, av3));

  HAL_ADC_Stop_DMA(&hadc1);

  HAL_Delay(1000);
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
