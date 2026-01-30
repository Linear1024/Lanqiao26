/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "stdio.h"
#include "i2c.h"
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
uint8_t ucSec;
uint8_t ucKey, ucDir;
uint8_t ucLed, ucSec1;
uint8_t ucLcd[21];
uint16_t usTlcd;
uint8_t ucUrx[20], ucSec2;
uint8_t ucCnt;
uint8_t ucMcp=0x0f;
uint16_t usAdc[3];
uint8_t ucTadc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void KEY_Proc(void);
void LED_Proc(void);
void LCD_Proc(void);
void UART_Proc(void);
void ADC_Proc(void);
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

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	HAL_Delay(100);
	LCD_Clear(Black);
	LCD_SetTextColor(White);
	LCD_SetBackColor(Black);
	
	MEM_Read((uint8_t *)&ucCnt, 0, 1);
	ucCnt++;
	MEM_Write((uint8_t *)&ucCnt, 0, 1);
	
	MCP_Write(ucMcp);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		KEY_Proc();
		LED_Proc();
		LCD_Proc();
		UART_Proc();
		ADC_Proc();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

/* USER CODE BEGIN 4 */
void KEY_Proc(void)
{
	uint8_t ucKey_Val = 0;
	ucKey_Val = KEY_Read();
	if(ucKey_Val != ucKey)
		ucKey = ucKey_Val;
	else
		ucKey_Val = 0;
	
	switch(ucKey_Val)
	{
		case 1:
			ucDir ^= 1;
			ucMcp += 0x10;
      if (ucMcp == 0x8f)
        ucMcp = 0x0f;
      MCP_Write(ucMcp);
			break;
		case 2:
			break;
	}
}

void LED_Proc(void)
{
	if(ucSec1 == ucSec)
		return;
	ucSec1 = ucSec;
	if(ucDir == 0)
	{
		ucLed <<= 1;
		if(ucLed == 0)

			ucLed = 1;
	} else{
		ucLed >>= 1;
		if(ucLed == 0)
			ucLed = 0x80;
	}
	LED_Disp(ucLed);
}

void LCD_Proc(void)
{
	if(usTlcd<500)
		return;
	usTlcd = 0;
	/*sprintf((char*)ucLcd, "         %03u        ", ucSec);
	LCD_DisplayStringLine(Line4, ucLcd);
	LCD_DisplayChar(Line5, Column9, ucLcd[9]);
	LCD_SetTextColor(Red);
	LCD_DisplayChar(Line5, Column10, ucLcd[10]);
	LCD_SetTextColor(White);
	LCD_DisplayChar(Line5, Column11, ucLcd[11]);
	LCD_SetTextColor(Red);
	LCD_DisplayStringLine(Line6,ucLcd);
	LCD_SetTextColor(White);*/
	sprintf((char*)ucLcd, " SEC:%03u   CNT:%03u ", ucSec, ucCnt);
	LCD_DisplayStringLine(Line2, ucLcd);
	
	sprintf((char *)ucLcd, " R37:%04u  B1: 0x%02X", usAdc[2], ucMcp);
  LCD_DisplayStringLine(Line4, ucLcd);
  sprintf((char *)ucLcd, " R38:%04u  MCP:%03u", usAdc[0], usAdc[1]);
  LCD_DisplayStringLine(Line5, ucLcd);
}

void UART_Proc(void)
{
	if(ucSec2 != ucSec)
	{
		ucSec2 = ucSec;
		
		/*UART_Transmit(ucLcd, 20);
		printf("%03u\r\n", ucSec);*/
		printf("%s %03u\r\n", ucLcd, ucSec);
	}
	
	if(UART_Receive(ucUrx, 2) == 0)
		ucSec = (ucUrx[0]-0x30)*10+ucUrx[1]-0x30;
}

int fputc(int ch, FILE *f)
{
	UART_Transmit((uint8_t *)&ch, 1);
	return ch;
}

void ADC_Proc(void)
{
  if (ucTadc < 100)
    return;
  ucTadc = 0;

  ADC1_Read(usAdc);
  usAdc[2] = ADC2_Read();
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
#ifdef USE_FULL_ASSERT
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
