
/* THIS SAMPLE CODE IS PROVIDED AS IS AND IS SUBJECT TO ALTERATIONS. FUJITSU */
/* MICROELECTRONICS ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR */
/* ELIGIBILITY FOR ANY PURPOSES.                                             */
/*            (C) Fujitsu Microelectronics (Shanghai) Co., LTD.              */
/*---------------------------------------------------------------------------
  SYSTEM.C
  - description
  - System init.watch dog init.timebase init...
-----------------------------------------------------------------------------*/

#include "main.h"
#include "system.h"
#include "LED.h"
#include "ADS1259.h"
#include "AD_spi.h"
#include "delay.h"

osThreadId LED_ThreadHandle, AD_ThreadHandle, USB_ThreadHandle;

unsigned char _5ms;
unsigned char SysTime;
USBD_HandleTypeDef USBD_Device;
uint8_t USB_Receive_Buf[32];
uint8_t USB_Send_Buf[32];
volatile uint16_t USB_Receive_count = 0;
/*----------------------------- SUB ROUTINES --------------------------------*/
void SysTimInit(void)
{
	GPIO_InitTypeDef  gpioinitstruct;
  
  /* Enable the GPIO_LED Clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

//   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //设置时钟源，8分频AHB时钟，9MHz
// 	SysTick_SetReload(0xAFC8);			     	//设置SysTick重装载值，45000 0xAFC8			9000 0x2328
// 	SysTick_CounterCmd(SysTick_Counter_Enable);	//使能计数器
// 	SysTick_ITConfig(ENABLE);					//使能SysTick中断

	/* Configure PA.8 as LED function IO */
  gpioinitstruct.Pin    = GPIO_PIN_2;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gpioinitstruct);
	/* Configure PD.12 as LED function IO */
  gpioinitstruct.Pin    = GPIO_PIN_3;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gpioinitstruct);
	/* Reset PIN to switch off the LED */
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, GPIO_PIN_SET);
//	// 发送USB测试数据
//	delay_ms(4*50);	
//	USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Send_Buf, 32);
}

void TimerLoop(void const *argument)
{
	static unsigned long testAD = 0;
	static unsigned int Cnt500ms = 100;
	static unsigned char LedFlag  = 1;
	(void) argument;
	
	for (;;)
	{
		if(USB_Receive_count > 0)
		{
	//							USB_Receive_Buf[USB_Receive_count] = USB_Receive_count;
				USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Receive_Buf, USB_Receive_count);
				USB_Receive_count = 0;
		}
	}

	SysTime = 0;
	if(_5ms)
	{
		_5ms = 0;
		SysTime=1;
		if(SysTime)
		{
			// Led闪烁代码
//			Cnt500ms--;			// 不执行计时
			if(Cnt500ms == 0)		//500ms时间到
			{

//					if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
//					{
//							USB_Send_Buf[0] = 0xA7; USB_Send_Buf[1] = 0x59; USB_Send_Buf[2] = 0x3E; USB_Send_Buf[3] = 0xBD;
//							USB_Send_Buf[4] += 1; USB_Send_Buf[5] += 2; USB_Send_Buf[6] += 3; USB_Send_Buf[7] += 4;
//							USB_Send_Buf[12] += 1; USB_Send_Buf[13] += 2; USB_Send_Buf[14] += 3; USB_Send_Buf[15] += 4;
//							USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Send_Buf, 32);
//					}

				
//				printf("** Get AD Data = %x. ** \n\r", ADS1259_Read_2Word());
//				ADS1259_Write_Byte(0x08);
				testAD = ADS1259_Read();
				printf("** Get AD Data = %x. ** \n\r", (unsigned int)testAD);
//				printf("** Get AD Data = %x. ** \n\r", 0xff);
				Cnt500ms = 100;
				//Led闪烁控制部分
				if(LedFlag == 1)
				{
					LedFlag = 0;
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, GPIO_PIN_RESET);
					
				}
				else
				{
					LedFlag = 1;
					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, GPIO_PIN_RESET);
//					HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, GPIO_PIN_SET);

				}
			}	
		}
	}
}

