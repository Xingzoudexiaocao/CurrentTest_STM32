
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

/* Variable containing ADC conversions results */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];
// int32_t   wTemperature_DegreeCelsius = 0;		// �ڲ��¶�
 
osThreadId LED_ThreadHandle, AD_ThreadHandle, USB_ThreadHandle;

unsigned char _5ms;
unsigned char SysTime;
USBD_HandleTypeDef USBD_Device;	/* USB handler declaration */
SPI_HandleTypeDef SpiHandle;		/* SPI handler declaration */
ADC_HandleTypeDef    AdcHandle;		/* ADC handler declaration */
uint8_t USB_Receive_Buf[32];
uint8_t USB_Send_Buf[32];
volatile uint16_t USB_Receive_count = 0;
unsigned char level = 4;		// ����������λ��1~4��,Ĭ��Ϊ4��
unsigned char cntLevMax = 0;		// ������������ѹ����
unsigned char cntLevMin = 0;		// ����С����С��ѹ����
static void Error_Handler(void)
{
  while(1)
  {
		HAL_GPIO_TogglePin(LED_2_PORT, LED_2_PIN);		// LED2 ÿ����˸һ��
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
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 rank converted at each conversion trig, and because discontinuous mode is enabled */
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
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
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

//   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //����ʱ��Դ��8��ƵAHBʱ�ӣ�9MHz
// 	SysTick_SetReload(0xAFC8);			     	//����SysTick��װ��ֵ��45000 0xAFC8			9000 0x2328
// 	SysTick_CounterCmd(SysTick_Counter_Enable);	//ʹ�ܼ�����
// 	SysTick_ITConfig(ENABLE);					//ʹ��SysTick�ж�

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
  HAL_GPIO_WritePin(LED_1_PORT,LED_1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);
	/* Configure PA.0~3 as Level function IO */
  gpioinitstruct.Pin    = LELVE_1_PIN | LELVE_2_PIN | LELVE_3_PIN | LELVE_4_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LELVE_PORT, &gpioinitstruct);
	/* Reset PIN to switch off the LED */
	SetCurrentLevel(level);	// ����Ĭ�ϵ�λ
	memset(USB_Send_Buf, 0, sizeof(USB_Send_Buf));	// ���÷�������Ϊ0

	// ��ʼ���ڲ�AD
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
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_1_PIN, GPIO_PIN_SET); 	// �ȿ�1
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_RESET);	// ���2
			break;
			case 2:	
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_SET); 	// �ȿ�2
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_RESET);	// ���3
			break;
			case 3:	
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_SET); 	// �ȿ�3
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_4_PIN, GPIO_PIN_RESET);	// ���4
			break;
			case 4:	
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_4_PIN, GPIO_PIN_SET); 		// �ȿ�4
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_1_PIN, GPIO_PIN_RESET);	// ��� 1��2��3
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_RESET);
			break;
			default: break;
		}
		if(lev == 1 || lev == 3)
			HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_RESET);	// LED2  ON
		else
			HAL_GPIO_WritePin(LED_2_PORT,LED_2_PIN, GPIO_PIN_SET);	// LED2  OFF
}

void TimerLoop(void const *argument)
{
	uint16_t lastADValue = 0;
	unsigned char lastLevel = 4;
	unsigned char cnt = 0;
	(void) argument;
	
	for (;;)
	{
//		if(USB_Receive_count > 0)
//		{
//	//							USB_Receive_Buf[USB_Receive_count] = USB_Receive_count;
//				USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Receive_Buf, USB_Receive_count);
//				USB_Receive_count = 0;
//		}
		osDelay(1);	// ��ʱ10ms
		HAL_ADC_Start(&AdcHandle);		// ����ADת��
		lastADValue = aADCxConvertedValues[0];
//		uint16_t tmp = aADCxConvertedValues[0];
//		lastADValue += tmp;
//		lastADValue /= 2;
//		wTemperature_DegreeCelsius = COMPUTATION_TEMPERATURE_STD_PARAMS(aADCxConvertedValues[1]);		// �¶Ȼ���
		cnt++;
		if(cnt >= 10)
		{
				cnt = 0;		// �����
				if(lastLevel == level)
				{
						// ��ֵ�ڲ�AD����ֵ
						USB_Send_Buf[8] = lastADValue >> 8; USB_Send_Buf[9] = lastADValue;
						USB_Send_Buf[10] = aADCxConvertedValues[1] >> 8; USB_Send_Buf[11] = aADCxConvertedValues[1];
						USB_Send_Buf[12] = aADCxConvertedValues[2] >> 8; USB_Send_Buf[13] = aADCxConvertedValues[2];
		//			USB_Send_Buf[14] = wTemperature_DegreeCelsius >> 24; USB_Send_Buf[15] = wTemperature_DegreeCelsius >> 16;
		//			USB_Send_Buf[16] = wTemperature_DegreeCelsius >> 8; USB_Send_Buf[17] = wTemperature_DegreeCelsius;
						if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
							USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Send_Buf, 32);		// û��10ms��������
				}
				else
					lastLevel = level;		// �����ͻ���֮�����һ������
		}
		// ����USBָ��
		if(USB_Receive_count > 0)
		{
				if(USB_Receive_Buf[0] == 0xa5 && USB_Receive_Buf[1] == 0xb7 
					&& USB_Receive_Buf[2] == 0xa5 && USB_Receive_Buf[3] == 0xb7)
				{
						if(USB_Receive_Buf[4] <= 4)
						{
							level = USB_Receive_Buf[4];
							HAL_GPIO_WritePin(LELVE_PORT,LELVE_1_PIN, GPIO_PIN_RESET);	// �� 1��2��3��4
							HAL_GPIO_WritePin(LELVE_PORT,LELVE_2_PIN, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(LELVE_PORT,LELVE_3_PIN, GPIO_PIN_RESET);
							HAL_GPIO_WritePin(LELVE_PORT,LELVE_4_PIN, GPIO_PIN_RESET);
							SetCurrentLevel(level);				// �ٿ���Ӧ��λ
						}
						else if(USB_Receive_Buf[4] == 5 || USB_Receive_Buf[4] == 6)
						{
								uint8_t tem = 0x13 + USB_Receive_Buf[4];
								HAL_SPI_Transmit_DMA(&SpiHandle, &tem, 1);	// 0x18 ƫ��У���� 0x19 ����У��
						}
						else if(USB_Receive_Buf[4] == 7)
						{
							HAL_ADC_Stop(&AdcHandle);
							/* Run the ADC calibration */  
							if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
							{
								/* Calibration Error */
								Error_Handler();
							}
							HAL_ADC_Start(&AdcHandle);
						}
				}
				USB_Receive_count = 0;
		}
	}
}

