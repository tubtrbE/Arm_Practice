/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
int FF_JK(int cp, int j, int k, int q);
int FF_D(int cp_type, int cp_before, int cp_now, int d, int q);
int FF_JK_Edge(int type, int before, int now, int j, int k, int q);
int ttl7490(int cp_type, int cp_before, int cp_now, int Q_Before[], int Q_Now[]);
int ttl7492(int cp_type, int cp_before, int cp_now, int Q_Before[], int Q_Now[]);

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
	int cp, j[4] = { 1,0, }, k[4] = { 1,0, }, cp_type = 1;
	int cp_before = 0, cp_now = 0;
	int Q_Before[4] = { 0, };
	int Q_Now[4] = { 0, };
	int carry = 0;
	int sum = 0, flag = 0;



	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */


	// down_edge_inputfirst
	if (cp_type < 1) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);
	}
	// upper_edge_inputfirst
	else if (cp_type > 0) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);
	}

	while (1)
	{
		int mask = 0;
		//cp_edge??????
		cp_before = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7);
		for (int i = 0; i < 4; i++) {
			Q_Before[i] = Q_Now[i];
		}
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_7);
		HAL_Delay(250);
		cp_now = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7);

		//?????? ????
		//======================================================================
		int mask_random = 0, mask_temp = 0x0001;
		int Q = Q_Now[2] ^ Q_Now[3];
		Q_Now[0] = FF_D(cp_type, cp_before, cp_now, Q, Q_Before[0]);
		Q_Now[1] = FF_D(cp_type, cp_before, cp_now, Q_Before[0], Q_Before[1]);
		Q_Now[2] = FF_D(cp_type, cp_before, cp_now, Q_Before[1], Q_Before[2]);
		Q_Now[3] = FF_D(cp_type, cp_before, cp_now, Q_Before[2], Q_Before[3]);

		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1) {
			for (int i = 0; i < 4; i++) {
				Q_Now[i] = 1;
			}
		}

		for (int i = 0; i < 4; i++) {
			if (Q_Now[i] == 1) {
				mask_random += mask_temp;
			}
			mask_temp = mask_temp << 1;
		}
		//======================================================================
		GPIOD->ODR = 0;
		GPIOD->ODR |= mask_random;
		HAL_Delay(250);

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
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 180;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Activate the Over-Drive mode
	*/
	if (HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
int FF_JK(int cp, int j, int k, int q) {

	if (cp > 0) {

		if (j < 1 && k < 1) {
			q = q;
		}
		else if (j < 1 && k > 0) {
			q = 0;
		}
		else if (j > 0 && k < 1) {
			q = 1;
		}
		else if (j > 0 && k > 0) {
			if (q > 0) {
				q = 0;
			}
			else if (q < 1) {
				q = 1;
			}
		}
	}
	return q;
}

int FF_JK_Edge(int type, int before, int now, int j, int k, int q) {


	int cp = 0;
	// ???? ????
	if (type < 1) {
		if (before > 0 && now < 1) {
			cp = 1;
		}
	}
	// ???? ????
	else if (type > 0) {
		if (before < 1 && now > 0) {
			cp = 1;
		}
	}


	if (cp > 0) {

		if (j < 1 && k < 1) {
			q = q;
		}
		else if (j < 1 && k > 0) {
			q = 0;
		}
		else if (j > 0 && k < 1) {
			q = 1;
		}
		else if (j > 0 && k > 0) {
			if (q > 0) {
				q = 0;
			}
			else if (q < 1) {
				q = 1;
			}
		}
	}
	return q;
}

int FF_D(int cp_type, int cp_before, int cp_now, int d, int q) {

	int j = 0, k = 0;

	if (d == 0) {
		j = 0;
		k = 1;
	}
	else if (d == 1) {
		j = 1;
		k = 0;
	}

	q = FF_JK_Edge(cp_type, cp_before, cp_now, j, k, q);

	return q;
}

int ttl7490(int cp_type, int cp_before, int cp_now, int Q_Before[], int Q_Now[]) {

	int j[4] = { 1,0, }, k[4] = { 1,0, };
	int c;

	if (Q_Before[0] == 1 && Q_Before[3] == 0) {

		j[1] = 1;
		k[1] = 1;
	}

	else if (Q_Before[0] == 1 && Q_Before[3] == 1) {
		c = 1;
	}
	if (Q_Before[1] == 1 &&
		Q_Before[0] == 1) {

		j[2] = 1;
		k[2] = 1;
	}
	if (Q_Before[2] == 1 &&
		Q_Before[1] == 1 &&
		Q_Before[0] == 1) {

		j[3] = 1;
	}
	if (Q_Before[0] == 1) {
		k[3] = 1;
	}

	for (int i = 0; i < 4; i++) {
		Q_Now[i] = FF_JK_Edge(cp_type, cp_before, cp_now, j[i], k[i], Q_Before[i]);
	}

	for (int i = 1; i < 4; i++) {
		j[i] = 0;
		k[i] = 0;
	}
	return c;
}

int ttl7492(int cp_type, int cp_before, int cp_now, int Q_Before[], int Q_Now[]) {

	int j[4] = { 1,0, }, k[4] = { 1,0, };
	int c;

	if (Q_Before[0] == 1) {

		j[1] = 1;
		k[1] = 1;
	}
	if (Q_Before[1] == 1 &&
		Q_Before[0] == 1) {

		j[2] = 1;
		k[2] = 1;
	}
	if (Q_Before[2] == 1 &&
		Q_Before[1] == 1 &&
		Q_Before[0] == 1) {

		j[3] = 1;
		k[3] = 1;
	}
	for (int i = 0; i < 4; i++) {
		Q_Now[i] = FF_JK_Edge(cp_type, cp_before, cp_now, j[i], k[i], Q_Before[i]);

		// 6???? ????
		if (Q_Now[1] == 1 &&
			Q_Now[2] == 1) {
			c = 1;
			for (int j = 0; j < 4; j++) {
				Q_Now[j] = 0;
				Q_Before[j] = 0;
			}
		}
		//		12 ?? ????
		//		if (Q_Now[2] == 1 &&
		//			Q_Now[3] == 1) {
		//			c = 1;
		//			for (int j = 0; j < 4; j++) {
		//				Q_Now[j] = 0;
		//				Q_Before[j] = 0;
		//			}
		//		}
	}
	for (int i = 1; i < 4; i++) {
		j[i] = 0;
		k[i] = 0;
	}
	return c;
}

int doorlock(int input) {

	//security number
	int s_num[4] = { 3, 1, 9, 0 };


	if (s_num[0] == input) {

	}
	if (s_num[1] == input) {

	}
	if (s_num[2] == input) {

	}
	if (s_num[3] == input) {

	}


}

int make_random(int cp_type, int cp_before, int cp_now, int Q_Before[], int Q_Now[]) {

	int mask_random = 0, mask_temp = 0x0001;
	Q_Before[0] = Q_Before[2] ^ Q_Before[3];
	Q_Now[0] = FF_D(cp_type, cp_before, cp_now, Q_Before[0], Q_Before[0]);
	Q_Now[1] = FF_D(cp_type, cp_before, cp_now, Q_Now[0], Q_Before[1]);
	Q_Now[2] = FF_D(cp_type, cp_before, cp_now, Q_Now[1], Q_Before[2]);
	Q_Now[3] = FF_D(cp_type, cp_before, cp_now, Q_Now[2], Q_Before[3]);

	for (int i = 0; i < 4; i++) {
		if (Q_Now[i] == 1) {
			mask_random += mask_temp;
		}
		mask_temp = mask_temp << 1;
	}
	return mask_random;
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
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	   /* USER CODE END 6 */
}




#endif /* USE_FULL_ASSERT */
