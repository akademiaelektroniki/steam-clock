/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c

 * @brief          : Main program body
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2019 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/*
 * Bart's Steam clock v0.3
 * correct set enter time
 * added git repository
 *
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
RTC_TimeTypeDef gDate;
RTC_TimeTypeDef gTime;

RTC_TimeTypeDef sDate;
RTC_TimeTypeDef sTime;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//Common anode display
//segments. dot is not user, so we have 7 segments, from A to G.
//Segments are cathodes, so you can turn it on by pulling to ground.
#define SA_ON HAL_GPIO_WritePin(SA_GPIO_Port, SA_Pin, 0)
#define SB_ON HAL_GPIO_WritePin(SB_GPIO_Port, SB_Pin, 0)
#define SC_ON HAL_GPIO_WritePin(SC_GPIO_Port, SC_Pin, 0)
#define SD_ON HAL_GPIO_WritePin(SD_GPIO_Port, SD_Pin, 0)
#define SE_ON HAL_GPIO_WritePin(SE_GPIO_Port, SE_Pin, 0)
#define SF_ON HAL_GPIO_WritePin(SF_GPIO_Port, SF_Pin, 0)
#define SG_ON HAL_GPIO_WritePin(SG_GPIO_Port, SG_Pin, 0)

#define SA_OFF HAL_GPIO_WritePin(SA_GPIO_Port, SA_Pin, 1)
#define SB_OFF HAL_GPIO_WritePin(SB_GPIO_Port, SB_Pin, 1)
#define SC_OFF HAL_GPIO_WritePin(SC_GPIO_Port, SC_Pin, 1)
#define SD_OFF HAL_GPIO_WritePin(SD_GPIO_Port, SD_Pin, 1)
#define SE_OFF HAL_GPIO_WritePin(SE_GPIO_Port, SE_Pin, 1)
#define SF_OFF HAL_GPIO_WritePin(SF_GPIO_Port, SF_Pin, 1)
#define SG_OFF HAL_GPIO_WritePin(SG_GPIO_Port, SG_Pin, 1)

//digits. We have 4, from 1 to 4. Digits are anodes, so you can turn it on by pulling
//to supply voltage
#define D1_ON HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, 1)
#define D2_ON HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, 1)
#define D3_ON HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1)
#define D4_ON HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1)

#define D1_OFF HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, 0)
#define D2_OFF HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, 0)
#define D3_OFF HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 0)
#define D4_OFF HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 0)

//turning off
#define D_OFF D1_OFF; D2_OFF; D3_OFF; D4_OFF
#define S_OFF SA_OFF; SB_OFF; SC_OFF; SD_OFF; SE_OFF; SF_OFF; SG_OFF
#define ALL_OFF D_OFF; S_OFF

//display delay
#define WAIT HAL_Delay(1)

//buttons
#define CLEAR HAL_GPIO_ReadPin(CLEAR_GPIO_Port, CLEAR_Pin) == 0
#define SET HAL_GPIO_ReadPin(SET_GPIO_Port, SET_Pin) == 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
int hours;
int minutes;
int seconds;
int settings = 0;
int set_delay = 100;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void display(int position, int number);
void display_twice(int position, int number);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {


		//___________________SECTION 1 -setting clock

		if (CLEAR) { //clear button enter clock set.
			settings = 1;
			HAL_Delay(300);

		}
		//if set are bigger than 0, you are entering setting mode.
		while (settings >= 1) {

			if (CLEAR) {
				settings++;
				if (settings >= 4) {
					settings = 0;
				}
				HAL_Delay(300);
			}

			//reading from RTC also in settings mode
			HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
			HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
			hours = gTime.Hours;
			minutes = gTime.Minutes;
			seconds = gTime.Seconds;

			//display ony this, what you are setting
			if (settings == 1) {
				display_twice(1, hours);
			} else if (settings == 2) {
				display_twice(3, minutes);
			} else if (settings == 3) {
				display_twice(3, seconds);
			}

			ALL_OFF
			;

			if (settings == 1) {
				if (SET) {
					hours++;
					if (hours >= 24) {
						hours = 0;

					}

					sTime.Hours = hours;
					sTime.Minutes = minutes;
					sTime.Seconds = seconds;
					HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
					HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
					HAL_Delay(set_delay);

				}
			}

			if (settings == 2) {
				if (SET) {
					minutes++;
					if (minutes >= 60) {
						minutes = 0;

					}

					sTime.Hours = hours;
					sTime.Minutes = minutes;
					sTime.Seconds = seconds;
					HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
					HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
					HAL_Delay(set_delay);

				}
			}

			if (settings == 3) {
				if (SET) {
					seconds = 0;
					sTime.Hours = hours;
					sTime.Minutes = minutes;
					sTime.Seconds = seconds;
					HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
					HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
					HAL_Delay(set_delay);

				}

			}
		}

		//___________________SECTION 2 -display clock
		if (SET) {
			display_twice(3, seconds);
		} else {
			display_twice(1, hours);
			display_twice(3, minutes);
		}

		//___________________SECTION 3 -read from RTC

		HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
		HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
		hours = gTime.Hours;
		minutes = gTime.Minutes;
		seconds = gTime.Seconds;

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /**Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 4499;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /**Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 1;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /**Enable the Alarm A 
  */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D4_Pin|D3_Pin|D2_Pin|D1_Pin 
                          |SG_Pin|SA_Pin|SB_Pin|SC_Pin 
                          |SE_Pin|SF_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_GPIO_Port, SD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : D4_Pin D3_Pin D2_Pin D1_Pin */
  GPIO_InitStruct.Pin = D4_Pin|D3_Pin|D2_Pin|D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SG_Pin SA_Pin SB_Pin SC_Pin 
                           SE_Pin SF_Pin */
  GPIO_InitStruct.Pin = SG_Pin|SA_Pin|SB_Pin|SC_Pin 
                          |SE_Pin|SF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_Pin */
  GPIO_InitStruct.Pin = SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SET_Pin CLEAR_Pin */
  GPIO_InitStruct.Pin = SET_Pin|CLEAR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void display(int position, int number) {
	ALL_OFF
	;
	int ok = 1;
	switch (position) {
	case 1:
		D1_ON;
		break;
	case 2:
		D2_ON;
		break;
	case 3:
		D3_ON;
		break;
	case 4:
		D4_ON;
		break;
	default:
		ok = 0;		//secure. Dont turn LEDs, if position is different than 1-4
	}
	if (ok == 1) {
		switch (number) {
		case 0:
			SA_ON;
			SB_ON;
			SC_ON;
			SD_ON;
			SE_ON;
			SF_ON;
			break;
		case 1:
			SB_ON;
			SC_ON;
			break;
		case 2:
			SA_ON;
			SB_ON;
			SG_ON;
			SE_ON;
			SD_ON;
			break;
		case 3:
			SA_ON;
			SB_ON;
			SG_ON;
			SC_ON;
			SD_ON;
			break;

		case 4:
			SB_ON;
			SG_ON;
			SF_ON;
			SC_ON;
			break;
		case 5:
			SA_ON;
			SF_ON;
			SG_ON;
			SC_ON;
			SD_ON;
			break;

		case 6:
			SA_ON;
			SF_ON;
			SG_ON;
			SC_ON;
			SE_ON;
			SD_ON;
			break;

		case 7:
			SA_ON;
			SB_ON;
			SC_ON;
			break;

		case 8:
			SA_ON;
			SB_ON;
			SC_ON;
			SD_ON;
			SE_ON;
			SF_ON;
			SG_ON;
			break;

		case 9:
			SA_ON;
			SB_ON;
			SC_ON;
			SD_ON;
			SG_ON;
			SF_ON;
			break;

		}
	}
}

void display_twice(int position, int number) {
	int first_digit;
	int second_digit;
	ALL_OFF
	;
//brake a number into two digits
	if (number <= 9) {
		first_digit = 0;
		second_digit = number;
	}
	if ((number >= 10) && (number <= 19)) {		//check if number is from range
		first_digit = 1;
		second_digit = number - 10;
	}
	if ((number >= 20) && (number <= 29)) {		//check if number is from range
		first_digit = 2;
		second_digit = number - 20;
	}
	if ((number >= 30) && (number <= 39)) {		//check if number is from range
		first_digit = 3;
		second_digit = number - 30;
	}
	if ((number >= 40) && (number <= 49)) {		//check if number is from range
		first_digit = 4;
		second_digit = number - 40;
	}
	if ((number >= 50) && (number <= 59)) {		//check if number is from range
		first_digit = 5;
		second_digit = number - 50;
	}

	display(position, first_digit);
	WAIT;
	display(position + 1, second_digit);
	WAIT;

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
