
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
#include "UpdataApp.h"
#include "Flash.h"
#include "des.h"

/* Variable containing ADC conversions results */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];
// int32_t   wTemperature_DegreeCelsius = 0;		// 内部温度
 
osThreadId LED_ThreadHandle, AD_ThreadHandle, USB_ThreadHandle;

unsigned char _5ms;
unsigned char SysTime;
USBD_HandleTypeDef USBD_Device;	/* USB handler declaration */
SPI_HandleTypeDef SpiHandle;		/* SPI handler declaration */
ADC_HandleTypeDef    AdcHandle;		/* ADC handler declaration */
uint8_t USB_Receive_Buf[256];
uint8_t USB_Send_Buf[256];
uint8_t Random_Key[8];
volatile uint16_t USB_Receive_count = 0;
unsigned char level = 4;		// 定义测电流档位（1~4）,默认为4档
unsigned char cntLevMax = 0;		// 测量大于最大电压计数
unsigned char cntLevMin = 0;		// 测量小于最小电压计数

unsigned char verifyModeFlag = 0;							// 不为0为校准模式
unsigned long testUsbSendCnt = 0;			// 测试USB发送数据个数

Struct_SendData sSendData;
unsigned char HEADER_CODE[4] = {0xA7, 0x59, 0x3E, 0xBD};
unsigned char TAIL_CODE[4] = {0x59, 0x3E, 0xBD, 0x00};

unsigned char stepIndex = 0;

static const unsigned short crc16tab[256]= {
 0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
 0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
 0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
 0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
 0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
 0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
 0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
 0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
 0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
 0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
 0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
 0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
 0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
 0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
 0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
 0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
 0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
 0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
 0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
 0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
 0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
 0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
 0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
 0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
 0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
 0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
 0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
 0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
 0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
 0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
 0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
 0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

unsigned short YModemCRC(unsigned char *buf, int len)
{
		int counter;
		unsigned short crc = 0;
		for( counter = 0; counter < len; counter++)
				crc = (crc<<8) ^ crc16tab[((crc>>8) ^ (*buf++))&0x00FF];
		return crc;
}

static void Error_Handler(void)
{
  while(1)
  {
		HAL_GPIO_TogglePin(LED_2_PORT, LED_2_PIN);		// LED2 每秒闪烁一次
    HAL_Delay(1000);
  }
}
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  
  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCx;
  
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_ENABLE;               /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;       	//                /* Continuous mode disabled to have only 1 rank converted at each conversion trig, and because discontinuous mode is enabled */
  AdcHandle.Init.NbrOfConversion       = 3;                             /* Sequencer of regular group will convert the 3 first ranks: rank1, rank2, rank3 */
  AdcHandle.Init.DiscontinuousConvMode = ENABLE;                        /* Sequencer of regular group will convert the sequence in several sub-divided sequences */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Sequencer of regular group will convert ranks one by one, at each conversion trig */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Trig of conversion start done manually by software, without external event */

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each ADC conversion (IT by DMA end  */
  /*       of transfer), select sampling time and ADC clock with sufficient   */
  /*       duration to not create an overhead situation in IRQHandler.        */
  /* Note: Set long sampling time due to internal channels (VrefInt,          */
  /*       temperature sensor) constraints. Refer to device datasheet for     */
  /*       min/typ/max values.                                                */
  sConfig.Channel      = ADCx_CHANNELa;
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
	
	/* Configuration of channel on ADCx regular group on sequencer rank 2 */
  /* Replicate previous rank settings, change only channel and rank */
  sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank         = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
  /* Configuration of channel on ADCx regular group on sequencer rank 3 */
  /* Replicate previous rank settings, change only channel and rank */
  sConfig.Channel      = ADC_CHANNEL_VREFINT;
  sConfig.Rank         = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
}
/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
  /* Report to main program that ADC sequencer has reached its end */
	
}
///**
//  * @brief  Conversion DMA half-transfer callback in non blocking mode 
//  * @param  hadc: ADC handle
//  * @retval None
//  */
//void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
//{

//}
/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In case of ADC error, call main error handler */
  Error_Handler();
}
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

	// 配置PA1为测试口
	gpioinitstruct.Pin    = GPIO_PIN_1;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &gpioinitstruct);
	
	/* Configure PC.6 as LED function IO */
  gpioinitstruct.Pin    = LED_1_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_1_PORT, &gpioinitstruct);
	/* Configure PC.7 as LED function IO */
  gpioinitstruct.Pin    = LED_2_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_2_PORT, &gpioinitstruct);
	/* Reset PIN to switch off the LED */
  HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_SET);		// LED1 OFF
	HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);		// LED2 OFF
	/* Configure PE.3~6 as Level function IO */
  gpioinitstruct.Pin    = LELVE_1_PIN | LELVE_2_PIN | LELVE_3_PIN | LELVE_4_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LELVE_PORT, &gpioinitstruct);
	/* Reset PIN to switch off the LED */
	SetCurrentLevel(level);	// 设置默认档位
	memset(&sSendData, 0, sizeof(sSendData));	// 设置发送数据为0
	memset(USB_Send_Buf, 0, sizeof(USB_Send_Buf));	// 设置发送数据为0
	USB_Send_Buf[0] = 0xA7; USB_Send_Buf[1] = 0x59; USB_Send_Buf[2] = 0x3E; USB_Send_Buf[3] = 0xBD;		// 初始化头码
	USB_Send_Buf[28] = 0x59; USB_Send_Buf[29] = 0x3E; USB_Send_Buf[30] = 0xBD;		// 初始化尾码
	memset(&Random_Key, 0x00, sizeof(Random_Key));

	// 初始化内部AD
	/* Configure the ADC peripheral */
  ADC_Config();
  /* Run the ADC calibration */  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

	/* Start ADC conversion on regular group with transfer by DMA */
  if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedValues,
                        ADCCONVERTEDVALUES_BUFFER_SIZE
                       ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
	
//	/* Start ADC conversion */
//	HAL_ADC_Start(&AdcHandle);

}

void SetCurrentLevel(uint8_t lev)
{
		switch(lev)
		{
			case 1:	
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_1_PIN, GPIO_PIN_SET); 	// 先开1
				delay_us(10);
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_RESET);	// 后关2
				delay_us(10);
//				HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_SET);	// LED1  OFF	0x00
//				HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);	// LED2  OFF
			break;
			case 2:	
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_SET); 	// 先开2
				delay_us(10);
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_RESET);	// 后关3
				delay_us(10);
//				HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_RESET);	// LED1  ON	0x10
//				HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);	// LED2  OFF
			break;
			case 3:	
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_SET); 	// 先开3
				delay_us(10);
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_4_PIN, GPIO_PIN_RESET);	// 后关4
				delay_us(10);
//				HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_SET);	// LED1  OFF	0x01
//				HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_RESET);	// LED2  ON
			break;
			case 4:	
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_4_PIN, GPIO_PIN_SET); 		// 先开4
				delay_us(10);
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_1_PIN, GPIO_PIN_RESET);	// 后关 1，2，3
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_RESET);
				delay_us(10);
//				HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_RESET);	// LED1  ON	0x11
//				HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_RESET);	// LED2  ON
			break;
			default: break;
		}
		
//		if(lev == 1 || lev == 3)
//			HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_RESET);	// LED2  ON
//		else
//			HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);	// LED2  OFF
}

void TimerLoop(void const *argument)
{
	static uint32_t pageIndex = 0;
	static unsigned char receivePage[1056];
	static unsigned char receiveOnce[32];
	static unsigned char upUpdateOK[4] = {0x42, 0x01, 0x01, 0x00};
	static unsigned char upVersion[4] = {0, 0, 0, 0};
	static unsigned char upLength[4] = {0, 0, 0, 0};

	static unsigned int crcTmp = 0;
	static unsigned int crcValue = 0;
	static unsigned int timeOutCnt = 0;
	static unsigned int i = 0;

//	static uint16_t lastADValue = 0;
//	static unsigned char lastLevel = 4;
	static unsigned char cnt = 0;
//	static unsigned long testCnt = 0;
//	static uint32_t count = 0;
//	static unsigned char LedFlag  = 1;
	static uint16_t adValueBuf[2][ADCCONVERTEDVALUES_BUFFER_SIZE];
	static unsigned int j = 0;
	
	static unsigned int k = 0;
//	static unsigned char m_Value[3][8];
//	static unsigned char e_Value[3][8];
//	static unsigned char m_text[8];
//	static unsigned char e_text[8];
	
	(void) argument;
//	osEvent event;
	
	for (;;)
	{
//		if(USB_Receive_count > 0)
//		{
//	//							USB_Receive_Buf[USB_Receive_count] = USB_Receive_count;
//				USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Receive_Buf, USB_Receive_count);
//				USB_Receive_count = 0;
//		}
		osDelay(1);	// 延时10ms
		
//		delay_ms(10);	// 测试延时函数是否可用
//		count++;
//		if(count >= 100)
//		{
//			count = 0;
//			if(LedFlag == 1)
//			{
//				LedFlag = 0;
//				HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_SET);
////					HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_RESET);
//				
//			}
//			else
//			{
//				LedFlag = 1;
//				HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_RESET);
////				TestWriteFlash();
////					HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);

//			}
//		}
		
		 
		// 处理USB指令
		timeOutCnt++;
		if(USB_Receive_count > 0)
		{
				USB_Receive_count = 0;		// 清接收计数
			// 处理上位机发送的指令
				if(USB_Receive_Buf[0] == 0xa5 && USB_Receive_Buf[1] == 0xb7 
					&& USB_Receive_Buf[2] == 0xa5 && USB_Receive_Buf[3] == 0xb7 && stepIndex == 0)	// 非升级状态才处理指令
				{
						if(USB_Receive_Buf[4] <= 4)
						{
							verifyModeFlag = 0;		// 退出校准模式
							level = USB_Receive_Buf[4];
							HAL_GPIO_WritePin(LELVE_PORT,LELVE_1_PIN, GPIO_PIN_RESET);	// 关 1，2，3，4
							HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(LELVE_PORT,LELVE_4_PIN, GPIO_PIN_RESET);
							SetCurrentLevel(level);				// 再开对应档位
						}
						else if(USB_Receive_Buf[4] == 5 || USB_Receive_Buf[4] == 6)
						{
								uint8_t tem = 0x13 + USB_Receive_Buf[4];
								HAL_SPI_Transmit_DMA(&SpiHandle, &tem, 1);	// 0x18 偏移校正， 0x19 增益校正
						}
						else if(USB_Receive_Buf[4] == 7)
						{
							HAL_ADC_Stop(&AdcHandle);
//							/* Run the ADC calibration */  
//							if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
//							{
//								/* Calibration Error */
//								Error_Handler();
//							}
							HAL_ADC_Start(&AdcHandle);
						}
						else if(USB_Receive_Buf[4] == 8)
						{
//								TestWriteFlash();
								SendVersionLength();
						}
						else if(USB_Receive_Buf[4] >= 0x10 && USB_Receive_Buf[4] < 0x20)
						{
								verifyModeFlag = USB_Receive_Buf[4] - 0x10;		// 进入校准模式
								level = (verifyModeFlag - 1) / 2 + 1;		// 1/2对应1档；3/4对应2档；5/6对应3档；7/8对应4档
//								HAL_GPIO_WritePin(LELVE_PORT,LELVE_1_PIN, GPIO_PIN_RESET);	// 关 1，2，3，4
//								HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_RESET);
//								HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_RESET);
//								HAL_GPIO_WritePin(LELVE_PORT,LELVE_4_PIN, GPIO_PIN_RESET);
								SetCurrentLevel(level);				// 再开对应档位
						}
						else if(USB_Receive_Buf[4] == 0x20)
						{
								SetVerifyValue(9, 0);		// 读取各个档位的校验值并发送给上位机
								HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_RESET);	// LED2  ON
						}
						else if(USB_Receive_Buf[4] == 0x21)
						{
								HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);	// LED2  OFF
								memset(&Random_Key, 0x00, sizeof(Random_Key));
						}
						else if(USB_Receive_Buf[4] == 0x30)
						{
								memcpy(Random_Key, USB_Receive_Buf + 16, sizeof(Random_Key));		// 接收随机密钥
								SendRandomKeySuccess();
						}
						continue;
				}
				// 以下代码为AppUpdata代码
				memcpy(receiveOnce, USB_Receive_Buf, 32);
				if(stepIndex == 1)
				{
					timeOutCnt = 0;
					
						memcpy(receivePage + (unsigned int)(i * 32), receiveOnce, 32);
//					memcpy(receivePage, receiveOnce, 32);
//					memset(&receivePage, 0x00, sizeof(receivePage));
//					receivePage[1055] = 0xFF;
					
					i++;
					if(i >= 33)
					{
						i = 0;
						crcValue = (receivePage[1027] << 8) | receivePage[1028];
						crcTmp = YModemCRC(receivePage + 3, 1024);
						if((crcValue == crcTmp)
							&& (0xFF == (receivePage[1] + receivePage[2])) && (receivePage[0] == YMODEM_STX)
							&& (pageIndex == (receivePage[1] - 1)))
						{	// 接收一页成功，写Flash
							memset(&receiveOnce, YMODEM_ACK, 32);	// Send ACK
							memcpy(receiveOnce + 4, USB_Receive_Buf, 32 - 4);
							if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
								USBD_CUSTOM_HID_SendReport(&USBD_Device, receiveOnce, 32);
							
							MemoryWrite(UPDATA_ADDR + (uint32_t)pageIndex * 1024, receivePage + 3, 1024);
//							delay_ms(10);	// 测试延时函数是否可用
							
							pageIndex++;
							if(pageIndex >= 122)	// 不允许接收超过122k数据
							{
								pageIndex = 0;
								stepIndex = 0;
//								memset(&receiveOnce, YMODEM_TIMEOUT, 32);
//								USBD_CUSTOM_HID_SendReport(&USBD_Device, receiveOnce, 32);
							}
							
						}
						else
						{	// 接收不成功，重新接收改页数据
							memset(&receiveOnce, YMODEM_NAK, 32);	// Send NAK
							memcpy(receiveOnce + 4, USB_Receive_Buf, 32 - 4);
							receiveOnce[16] = receivePage[1027]; receiveOnce[17] = receivePage[1028];
							receiveOnce[18] = crcTmp >> 8; receiveOnce[19] = crcTmp & 0xFF;
							receiveOnce[20] = receivePage[0]; receiveOnce[21] = receivePage[1];receiveOnce[22] = receivePage[2]; receiveOnce[23] = receivePage[3];
							if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
								USBD_CUSTOM_HID_SendReport(&USBD_Device, receiveOnce, 32);

						}
					}
//					else
//					{
//						memset(&receiveOnce, YMODEM_ACK, 32);	// Send ACK
//						memcpy(receiveOnce + 4, USB_Receive_Buf, 32 - 4);
//						receiveOnce[30] = pageIndex; receiveOnce[31] = i;
//						USBD_CUSTOM_HID_SendReport(&USBD_Device, receiveOnce, 32);
//					}
				}
				if(receiveOnce[0] == YMODEM_SOH && receiveOnce[1] == 0x00 && receiveOnce[2] == 0xFF)		//  && stepIndex == 0
				{
					timeOutCnt = 0;
					crcValue = (receiveOnce[11] << 8) | receiveOnce[12];
					if(crcValue == YModemCRC(receiveOnce + 3, 8))
					{	// 校验正确，进入接收数据环节
						stepIndex = 1;
						i = 0;
						pageIndex = 0;
						
						memcpy(upVersion, receiveOnce + 3,   4);
						memcpy(upLength, receiveOnce + 7, 4);
						memset(receiveOnce, YMODEM_ACK, 32);	// Send ACK
//						memcpy(receiveOnce + 4, USB_Receive_Buf + 4, 32-4);
						if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
							USBD_CUSTOM_HID_SendReport(&USBD_Device, receiveOnce, 32);
					}
				}
				if(receiveOnce[0] == YMODEM_EOT && receiveOnce[1] == 0x00 && receiveOnce[2] == 0xFF)
				{
					timeOutCnt = 0;
					stepIndex = 0;		// 接收完成
					pageIndex = 0;
					
					crcValue = (receiveOnce[11] << 8) | receiveOnce[12];
					if(crcValue == YModemCRC(receiveOnce + 3, 8))
					{	// 校验正确，进入接收数据环节
						MemoryWrite(PAGE_ADDR , upUpdateOK, sizeof(upUpdateOK));
						MemoryWrite(PAGE_ADDR + sizeof(upUpdateOK), upVersion, sizeof(upVersion));
						MemoryWrite(PAGE_ADDR + sizeof(upUpdateOK) + sizeof(upVersion), upLength, sizeof(upLength));
					//	memcpy(upVersion, receiveOnce + 3,   4);
					//	memcpy(upLength, receiveOnce + 7, 4);
					// 比较upVersion，upLength是否正确，正确则返回ACK
						memset(&receiveOnce, YMODEM_ACK, 32);	// Send ACK
						if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
							USBD_CUSTOM_HID_SendReport(&USBD_Device, receiveOnce, 32);
					}
				}
		}
		// AppUpdate超时处理
		if(timeOutCnt >= 3000 && stepIndex != 0)		//	  
		{
			timeOutCnt = 0;
			stepIndex = 0;
			memset(&receiveOnce, YMODEM_TIMEOUT, 32);
			if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
				USBD_CUSTOM_HID_SendReport(&USBD_Device, receiveOnce, 32);
		}
		
//		HAL_ADCEx_Calibration_Start(&AdcHandle);
		HAL_ADC_Start(&AdcHandle);		// 启动AD转换
		memcpy(&adValueBuf[j], &aADCxConvertedValues, ADCCONVERTEDVALUES_BUFFER_SIZE);
		j++;
		if(j >= 2)
			j = 0;
////		/* Wait for conversion completion before conditional check hereafter */
////    HAL_ADC_PollForConversion(&AdcHandle, 1);
//		// uint32_t wTemperature_DegreeCelsius = HAL_ADC_GetValue(&AdcHandle);
//		lastADValue = aADCxConvertedValues[0];
//		uint16_t tmp = aADCxConvertedValues[0];
//		lastADValue += tmp;
//		lastADValue /= 2;
////		wTemperature_DegreeCelsius = COMPUTATION_TEMPERATURE_STD_PARAMS(aADCxConvertedValues[1]);		// 温度换算
		cnt++;
		if(cnt >= 2 && stepIndex == 0 && sSendData.index >= 2 && verifyModeFlag == 0)	// 非升级状态才处理指令,校准模式不发指令  
		{
				cnt = 0;		// 清计数
			USB_Send_Buf[4] = sSendData.value[sSendData.index - 1]; USB_Send_Buf[5] = sSendData.value[sSendData.index - 1] >> 8; 
			USB_Send_Buf[6] = sSendData.value[sSendData.index - 1] >> 16; USB_Send_Buf[7] = sSendData.value[sSendData.index - 1] >> 24;
			USB_Send_Buf[8] = adValueBuf[1][0] >> 8; USB_Send_Buf[9] = adValueBuf[1][0];
			USB_Send_Buf[10] = adValueBuf[1][1] >> 8; USB_Send_Buf[11] = adValueBuf[1][1];
			USB_Send_Buf[12] = adValueBuf[1][2] >> 8; USB_Send_Buf[13] = adValueBuf[1][2];
			
			if(sSendData.level[sSendData.index - 1] == sSendData.level[sSendData.index - 2])
			{
				USB_Send_Buf[14] = sSendData.value[sSendData.index - 2]; USB_Send_Buf[15] = sSendData.value[sSendData.index - 2] >> 8; 
				USB_Send_Buf[16] = sSendData.value[sSendData.index - 2] >> 16; USB_Send_Buf[17] = sSendData.value[sSendData.index - 2] >> 24;
				USB_Send_Buf[18] = adValueBuf[0][0] >> 8; USB_Send_Buf[19] = adValueBuf[0][0];
				USB_Send_Buf[20] = adValueBuf[0][1] >> 8; USB_Send_Buf[21] = adValueBuf[0][1];
				USB_Send_Buf[22] = adValueBuf[0][2] >> 8; USB_Send_Buf[23] = adValueBuf[0][2];
			}
			else
			{
				memcpy(USB_Send_Buf + 14, USB_Send_Buf + 4, 10);
			}

			USB_Send_Buf[25] = sSendData.index; USB_Send_Buf[26] = sSendData.tips[sSendData.index - 1];			// 0x0000表示电流正常
			USB_Send_Buf[27] = sSendData.level[sSendData.index - 1];
			
			// 使用随机密钥加密
			for(k = 0; k < 24; k ++)
			{
					USB_Send_Buf[4 + k] = USB_Send_Buf[4 + k] ^ Random_Key[k % 8];
			}
//			endes(m_text, Random_Key, m_text);
//			endes(m_Value[1], Random_Key, e_Value[1]);
//			endes(m_Value[2], Random_Key, e_Value[2]);
//			memcpy(USB_Send_Buf + 4, e_Value, sizeof(e_Value));
//			USB_Send_Buf[16] = testCnt; USB_Send_Buf[17] = testCnt >> 8; 
//			USB_Send_Buf[18] = testCnt >> 16; USB_Send_Buf[19] = testCnt >> 24;
//			USB_Send_Buf[20] = testUsbSendCnt; USB_Send_Buf[21] = testUsbSendCnt >> 8; 
//			USB_Send_Buf[22] = testUsbSendCnt >> 16; USB_Send_Buf[23] = testUsbSendCnt >> 24;
//			testCnt++;
			if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
				USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Send_Buf, 32);		// 
//			else
//				testCnt = 0;
			
			sSendData.index = 0;
			
//			memset(&sSendData, 0, sizeof(sSendData));	// 设置发送数据为0
		}
	}
}

