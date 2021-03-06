/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    14-April-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "system.h"
// #include "SendData.h"
#include "LED.h"
#include "KeyBoard_Drv.h"
#include "SceneHome.h"
#include "Flash.h"
#include "UpdataApp.h"
#include "ADS1259.h"
#include "AD_spi.h"
#include "delay.h"

#include "lcd.h"
#include "UI.h"	

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
//extern PCD_HandleTypeDef hpcd;
/* Buffer used for transmission */
uint8_t aTxBuffer[] = "\n\r ****UART-Hyperterminal communication based on DMA****\n\r Enter 10 characters using keyboard :\n\r";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
void SystemClock_Config(void);
void USART_Config(void);
 void Error_Handler(void);
static void LED_Thread(void const *argument);
/* Private functions ---------------------------------------------------------*/

void FlashProtectCheck(void)
{
	FLASH_OBProgramInitTypeDef OBInit;
	HAL_FLASH_Unlock();		// ????
	HAL_FLASH_OB_Unlock();
	
	HAL_FLASHEx_OBGetConfig(&OBInit);
	if(OBInit.RDPLevel != OB_RDP_LEVEL_1)
	{
		OBInit.OptionType = OPTIONBYTE_RDP;
		OBInit.RDPLevel = OB_RDP_LEVEL_1;
		HAL_FLASHEx_OBProgram(&OBInit);
	}
	
	HAL_FLASH_OB_Lock();
	HAL_FLASH_Lock();		// ????
}

int main(void)
{
	SCB->VTOR = ((uint32_t)0x8000000) | (0x2000 & (uint32_t)0x1FFFFF80);
//	__enable_irq();
  /* STM32F103xB HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
	
  /* Configure the system clock to 64 MHz */
  SystemClock_Config();
	
  /* Add your application code here
     */
  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, &USBD_CUSTOM_HID);

  /* Add Custom HID callbacks */
  USBD_CUSTOM_HID_RegisterInterface(&USBD_Device, &USBD_CustomHID_fops);

  /* Start Device Process */
  USBD_Start(&USBD_Device);
	
		SysTimInit();
//		USART_Config();
		
//		LED_Init();
//		KeyBoard_Drv_Init();
//	
//		FlashInit();
//		SceneHomeInit();
//		AD_Init();
	AD_SPI_DMA_INIT();
//	Init_ADS1259();

//  /* Output a message on Hyperterminal using printf function */
//  printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");
//  printf("** Test finished successfully. ** \n\r");
	
//	LCD_Init();
//	LCD_ShowString(30,40,200,24,24, (u8 *)"Mini STM32 ^_^");	
//	LCD_ShowString(50,64,200,24,24, (u8 *)"Hello World");			
//	UI_Init();
  /* Infinite loop */
	osThreadDef(THREAD_LED, LED_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	LED_ThreadHandle = osThreadCreate(osThread(THREAD_LED), NULL);
	osThreadDef(THREAD_USB, TimerLoop, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	USB_ThreadHandle = osThreadCreate(osThread(THREAD_USB), NULL);
	osThreadDef(THREAD_AD, ADLoop, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	AD_ThreadHandle = osThreadCreate(osThread(THREAD_AD), NULL);

	
//	  /* Set thread 2 in suspend state */
//  osThreadSuspend(LED_Thread1Handle); 
  /* Start scheduler */
  osKernelStart();
//	void const *argument;
		
	FlashProtectCheck();		// FLASH???????? 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);	 //????????????????????????RCC_APB2Periph_AFIO

// GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//????????SWD??JTAG 
// GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //????JTAG
	/* JTAG-DP Disabled and SW-DP Disabled */
	__HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();           	//
	__HAL_RCC_GPIOF_CLK_ENABLE();           	//
  __HAL_AFIO_REMAP_SWJ_DISABLE();			//????????SWD??JTAG 
	__HAL_AFIO_REMAP_SWJ_NOJTAG();				//????JTAG
	
  while (1)
  {
//		LED_Thread(argument);
//		ADLoop(argument);
//			TimerLoop(argument);
		
//			if(SysTime)
//			{
//				UI_Poll();
//			}
		
//			KeyLoop();
//			LEDLoop();

//			FlashLoop();
//			SceneHomeLoop();
//// 			SendLoop();	
  }
//while(1){}
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
//	uint8_t cntNum[1];
//	cntNum[0] = *(uint8_t *)&ch;
//  if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)cntNum, 1) != HAL_OK)
//  {
//    /* Transfer error in transmission process */
//    Error_Handler();
//  }
//	while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
//  {
//  }
  return ch;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = 16
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  
  /* Configure PLL ------------------------------------------------------*/
  /* PLL configuration: PLLCLK = (HSI / 2) * PLLMUL = (8 / 2) * 16 = 64 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLLCLK / HSEPredivValue = 64 / 1 = 64 MHz */
  /* Enable HSI and activate PLL with HSi_DIV2 as source */
//   oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSI;
//   oscinitstruct.HSEState        = RCC_HSE_OFF;
//   oscinitstruct.LSEState        = RCC_LSE_OFF;
//   oscinitstruct.HSIState        = RCC_HSI_ON;
//   oscinitstruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//   oscinitstruct.HSEPredivValue    = RCC_HSE_PREDIV_DIV1;
//   oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
//   oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSI_DIV2;
//   oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL16;
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.LSEState        = RCC_LSE_OFF;
  oscinitstruct.HSIState        = RCC_HSI_OFF;
  oscinitstruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  oscinitstruct.HSEPredivValue    = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
	
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
	
	/* USB clock selection */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

void USART_Config(void)
{
	/*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) : 
	                  BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance          = USARTx;
  
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_9B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_RTS;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;

  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /*##-2- Start the transmission process #####################################*/
  /* User start transmission data through "TxBuffer" buffer */
  if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)aTxBuffer, TXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }
  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
  {
  }
  /*##-3- Put UART peripheral in reception process ###########################*/
  /* Any data received will be stored in "RxBuffer" buffer : the number max of
     data received is 10 */
  if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }

//  /*##-4- Wait for the end of the transfer ###################################*/
//  /*  Before starting a new communication transfer, you need to check the current
//      state of the peripheral; if it?s busy you need to wait for the end of current
//      transfer before starting a new one.
//      For simplicity reasons, this example is just waiting till the end of the
//      transfer, but application may perform other tasks while transfer operation
//      is ongoing. */
//  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
//  {
//  }

//  /*##-5- Send the received Buffer ###########################################*/
//  if (HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
//  {
//    /* Transfer error in transmission process */
//    Error_Handler();
//  }

//  /*##-6- Wait for the end of the transfer ###################################*/
//  /*  Before starting a new communication transfer, you need to check the current
//      state of the peripheral; if it?s busy you need to wait for the end of current
//      transfer before starting a new one.
//      For simplicity reasons, this example is just waiting till the end of the
//      transfer, but application may perform other tasks while transfer operation
//      is ongoing. */
//  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
//  {
//  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
 void Error_Handler(void)
{
	printf("** Error_Handler. ** \n\r");
  /* Toogle LED2 for error */
//  while(1)
//  {
////    BSP_LED_Toggle(LED2);
//    HAL_Delay(1000);
//  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  huart: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Toogle LED2 : Transfer in transmission process is correct */
//  BSP_LED_On(LED2);
}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Turn LED2 on: Transfer in reception process is correct */
//  BSP_LED_On(LED2);
	printf("** Receive Data %d. ** \n\r", COUNTOF(aRxBuffer) - 1);
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3);		// ??????????
	
	if (HAL_UART_Receive_DMA(huart, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
}

/**
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Turn LED2 off: Transfer error in reception/transmission process */
//  BSP_LED_Off(LED2);
	printf("** HAL_UART_ErrorCallback. ** \n\r");
	// ??????????????????????????
	if (HAL_UART_Receive_DMA(huart, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
}

// LED ????????
static void LED_Thread(void const *argument)
{
//  uint32_t count = 0;
	unsigned char LedFlag  = 1;
//	unsigned char receiveOnce[32];
  (void) argument;

  for (;;)
  {
//    count = osKernelSysTick() + 5000;
//    
//    while (count > osKernelSysTick())
//    {
      /* Toggle LED2 every 250ms*/
      osDelay(500);
//		delay_ms(1000);	// ????????????????????
//		memset(&receiveOnce, YMODEM_TIMEOUT, 32);
//			USBD_CUSTOM_HID_SendReport(&USBD_Device, receiveOnce, 32);
			//Led????????????
			if(LedFlag == 1)
			{
				LedFlag = 0;
//				HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_SET);		
//					HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_RESET);
				
			}
			else
			{
				LedFlag = 1;
//				HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_RESET);
////				TestWriteFlash();
//					HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);

			}
//    }
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
