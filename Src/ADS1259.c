#include "main.h"
#include "ADS1259.h"
#include "delay.h"
#include "system.h"

#define MASTER_BOARD

//u8 InitData[9]={0x05,0x18,0x22,0xff,0xff,0x7f,0x00,0x00,0x40};
u8 InitData[9]={0x05,0x40,0x07,0x00,0x00,0x00,0x00,0x00,0x40};		// �Ĵ�����ֵ, FSC  ox4BBC50
u8 RecData[9];

//unsigned long curSendTmp[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//unsigned long curSendData = 0;
//u8 curSendCnt = 0;

//void delayNop()
//{
//	int i = 10;
//	while(i--);
//}

static void AD_Start(void)
{
	//IO���
//	PGA_WrCmd(0x45,0x18);
	//����CSѡ��
	AD_CS_L;
//	AD_SPI_WR_Byte(0xc5);
}

static void AD_Stop(void)
{
	AD_CS_H;
	delay_us(1);
//	delayNop();
//	PGA_WrCmd(0x45,0x08);
}

static void AD_SPI_WR_Byte(unsigned char dat)
{
   unsigned char i;
		AD_Start();
   AD_CLK_L;
   delay_us(1);
//	delayNop();
   for(i=0;i<8;i++)
   {
	   if(dat&0x80)  AD_SDI_H;
	   else  		 AD_SDI_L;
	   delay_us(1);
//		 delayNop();
	   dat<<=1;
	   AD_CLK_H;
	   delay_us(1);
//		 delayNop();
	   AD_CLK_L;
	   delay_us(1);
//		 delayNop();
   }
	 AD_Stop();
}

static unsigned char AD_SPI_RD_Byte(void)
{
	unsigned char i,dat;
	dat=0;
	AD_Start();
	AD_CLK_L;
	delay_us(1);
//	delayNop();
	for(i=0;i<8;i++)
	{
		dat<<=1;
		AD_CLK_H;
		delay_us(1);
//		delayNop();
		if(AD_SDO_R) dat|=1;
		AD_CLK_L;
		delay_us(1);	
//		delayNop();
	}
	AD_Stop();
	return dat;		
}
static void AD_WriteToRes(u8 cmd1,u8 len,u8 *dat)
{
	u8 i;
	AD_SPI_WR_Byte(cmd1);
	AD_SPI_WR_Byte(len);
	for(i=0;i<=len;i++)
	AD_SPI_WR_Byte(*(dat+i));	
}

static void AD_ReadFrRes(u8 cmd1,u8 len,u8 *dat)
{
	u8 i;
	AD_SPI_WR_Byte(cmd1);
	AD_SPI_WR_Byte(len);
	for(i=0;i<=len;i++)
	*(dat+i)=AD_SPI_RD_Byte();
}


unsigned long ADS1259_Read(void)
{
//	char count = 0;
//	unsigned long result = 0;
//	//����ģʽ�·���Ҫ��������
//	while(AD_DRY_R);

//	for(count=0;count<4;count++)
//	{
//		result = result << 8;
//		result = result | AD_SPI_RD_Byte();
//	}

//	return result;
	
	u8 recBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//	char count = 0;
	unsigned long result = 0;

//	if(AD_DRY_R)
//			return 0x80000000U + 100;
	while(AD_DRY_R);
	
  HAL_StatusTypeDef recState =	HAL_SPI_Receive(&SpiHandle, (uint8_t*)recBuf, 4, 50);	// ����4������
	if(recState != HAL_OK)
		return  0x80000000U + recState;		// �������ݴ��󣬷��ش������
	
//	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);		// PA1����
	
	result = (unsigned long)recBuf[0] << 24 | (unsigned long)recBuf[1] << 16 | (unsigned long)recBuf[2] << 8 | (unsigned long)recBuf[3]; 
	
	
	if(recBuf[3] == ((recBuf[0] + recBuf[1] + recBuf[2] + 0x9B) & 0xFF))
		return  result;
	else
		return 0x80000000U + 5;	// ����У��ʹ���
}

unsigned long AD_Read(void)
{
	unsigned long dat=0,temp[10];
	int i;
	for(i=0;i<4;i++)
	{
		temp[i]=ADS1259_Read();
	}
	for(i=1;i<4;i++)
	{
		dat+=temp[i];
	}
	dat=dat/3;
	return dat;
}

void AD_Init(void)
{
		GPIO_InitTypeDef  gpioinitstruct;
	  gpioinitstruct.Pin    = AD_CS | AD_CLK | AD_SDI;
		gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpioinitstruct);
	
		gpioinitstruct.Pin    = AD_SDO;
		gpioinitstruct.Mode   = GPIO_MODE_INPUT;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpioinitstruct);

	  gpioinitstruct.Pin    = AD_PWN;
		gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOE, &gpioinitstruct);

		gpioinitstruct.Pin    = AD_DRY;
		gpioinitstruct.Mode   = GPIO_MODE_INPUT;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOE, &gpioinitstruct);

//	for(int i = 0; i < 9; i++)
//	{
//		printf("** RecData%d = %x. ** \n\r", i, RecData[i]);
//	}
	
		AD_PWN_L;
		delay_us(2);
		AD_PWN_H;		// RESET/PWDN Pin ��������ʱΪ�ߵ�ƽ
		
	//����ʱ����ʱһ��ʱ�����ѹ�ȶ�
	delay_ms(2*50);	
	
	AD_Start();

	//ADS1259 INIT
//	AD_SPI_WR_Byte(0x06);	// ��λ
//	delay_ms(2*50);	
	AD_SPI_WR_Byte(0x02);	// ��˯��ģʽ�ָ�
	AD_SPI_WR_Byte(0x08);	// ��ʼת��
	delay_ms(100);	
	AD_SPI_WR_Byte(0x11);	//������ģʽ��ֹͣ����Ҫ��������
	delay_ms(5);	
	AD_WriteToRes(0x40,0x08,InitData);
	delay_ms(5);	
	AD_ReadFrRes(0x20,0x08,RecData);
	delay_ms(5);	
	AD_SPI_WR_Byte(0x10);	//����ģʽ�·���Ҫ��������
//	RecData[8] = 0xff;
	for(int i = 0; i < 9; i++)
	{
		printf("** RecData%d = %x. ** \n\r", i, RecData[i]);
	}
	printf("** unsgined int length is %d. ** \n\r", sizeof(unsigned int));
	printf("** unsgined long length is %d. ** \n\r", sizeof(unsigned long));
}
static void Error_Handler(void)
{
  while(1)
  {
		HAL_GPIO_TogglePin(LED_2_PORT, LED_2_PIN);		// LED2 ÿ����˸һ��
    HAL_Delay(1000);
  }
}

void AD_SPI_DMA_INIT(void)
{
		GPIO_InitTypeDef  gpioinitstruct;
		gpioinitstruct.Pin    = AD_PWN;
		gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOE, &gpioinitstruct);

		gpioinitstruct.Pin    = AD_DRY;
		gpioinitstruct.Mode   = GPIO_MODE_AF_INPUT;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOE, &gpioinitstruct);
	
	/*##-1- Configure the SPI peripheral #######################################*/
  /* Set the SPI parameters */
  SpiHandle.Instance               = SPIx;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;			// �½�����Ч
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.CRCPolynomial     = 7;
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;

#ifdef MASTER_BOARD
  SpiHandle.Init.Mode = SPI_MODE_MASTER;
#else
  SpiHandle.Init.Mode = SPI_MODE_SLAVE;
#endif /* MASTER_BOARD */

  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
	/* SPI block is enabled prior calling SPI transmit/receive functions, in order to get CLK signal properly pulled down.
     Otherwise, SPI CLK signal is not clean on this board and leads to errors during transfer */
  __HAL_SPI_ENABLE(&SpiHandle);
	
	AD_PWN_L;
//	delay_us(2);
	delay_ms(5);	
	AD_PWN_H;		// RESET/PWDN Pin ��������ʱΪ�ߵ�ƽ
		
	//����ʱ����ʱһ��ʱ�����ѹ�ȶ�
	delay_ms(100);	
//	AD_Start();
	//ADS1259 INIT
//	AD_SPI_WR_Byte(0x06);	// ��λ
//	delay_ms(2*50);	
//	while(SpiHandle.State != HAL_SPI_STATE_READY);
	u8 tem[20];
//	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 4);
	tem[0] = 0x02;
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 1);	// ��˯��ģʽ�ָ�
	tem[0] = 0x08;
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 1);	// ��ʼת��
	delay_ms(100);	
	tem[0] = 0x11;
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 1);	//������ģʽ��ֹͣ����Ҫ��������
	delay_ms(5);	
	tem[0] = 0x40;
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 1);
	tem[0] = 0x08;
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 1);
	memcpy(tem, InitData, 9);
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem , 9);	// ��9���Ĵ�����ֵ
	delay_ms(5);	
	tem[0] = 0x20;
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 1);
	tem[0] = 0x08;
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 1);
	HAL_SPI_Receive_DMA(&SpiHandle, (uint8_t*)RecData, 9);	// ����9���Ĵ�����ֵ
	delay_ms(5);
	tem[0] = 0x10;
	HAL_SPI_Transmit_DMA(&SpiHandle, (uint8_t*)tem, 1);	//����ģʽ�·���Ҫ��������
	
	memcpy(USB_Send_Buf + 16, RecData, sizeof(RecData));	// �Ĵ�����ֵ
}
/**
  * @brief  TxRx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of DMA TxRx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Turn LED2 on: Transfer in transmission/reception process is complete */
//	HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_RESET);
}
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
//	HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_RESET);
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
//	HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_RESET);	// ON
}
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
//	HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_SET);	// OFF
}
// �ж��Ƿ񻻵�����
void isChangeLevel(unsigned long adValue)
{
//	unsigned char i = 0;
	unsigned long sendBuf = adValue;
	adValue = adValue >> 8;		// �Ƴ�У��λ
	if(adValue > LEVEL_MAX)
	{
		cntLevMin = 0;
		cntLevMax ++;
		if(cntLevMax >= 1)			// ����һ�Σ�ֱ������ CNT_TOTAL
		{
			cntLevMax = 0;
			if(level == 4)
			{
				// ���ͳ��������ݣ���������
				USB_Send_Buf[4] = sendBuf; USB_Send_Buf[5] = sendBuf >> 8; USB_Send_Buf[6] = sendBuf >> 16; USB_Send_Buf[7] = sendBuf >> 24;
				USB_Send_Buf[25] = 0x02; USB_Send_Buf[26] = 0x02;	USB_Send_Buf[27] = level;		// 0x0202��ʾ��������
//		HAL_ADC_Start(&AdcHandle);		// ����ADת��
		sendBuf = adValue << 8;
		sendBuf |= level;
		ListAddOne(sendBuf, (unsigned long)aADCxConvertedValues[0]);
//				osSignalSet( USB_ThreadHandle, BIT_1 | BIT_2 );
			}
//			level++;
//			if(level >= 4)
				level = 4;
			SetCurrentLevel(level);	// ����Ĭ�ϵ�λ,����ֱ��������ߵ�4��

		}
	}
	else if(adValue <= LEVEL_MAX && adValue >= LEVEL_MIN)
	{
		cntLevMax = 0;
		cntLevMin = 0;
//		// ����n�ε�ƽ��ֵ
//		curSendTmp[curSendCnt] = adValue;
//		curSendCnt++;
//		if(curSendCnt >= 10)
//		{
//			curSendData = 0;
//			for(unsigned char i = 0; i < curSendCnt; i++)
//			{
//				curSendData += curSendTmp[i];
//			}
//			curSendData = curSendData / curSendCnt;		// ��ƽ��ֵ
//			curSendCnt = 0;		// �����
//			if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
//			{
//					USB_Send_Buf[4] = 0; USB_Send_Buf[5] = curSendData; USB_Send_Buf[6] = curSendData >> 8; USB_Send_Buf[7] = curSendData >> 16;
//					USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Send_Buf, 32);
//			}
//		}
////		HAL_ADC_Start(&AdcHandle);		// ����ADת��
//		sendBuf = adValue << 8;
//		sendBuf |= level;
//		ListAddOne(sendBuf, (unsigned long)aADCxConvertedValues[0]);
//			// ����ÿ�ε�����
			USB_Send_Buf[4] = sendBuf; USB_Send_Buf[5] = sendBuf >> 8; USB_Send_Buf[6] = sendBuf >> 16; USB_Send_Buf[7] = sendBuf >> 24;
		  USB_Send_Buf[25] = 0x00; USB_Send_Buf[26] = 0x00;			// 0x0000��ʾ��������
			USB_Send_Buf[27] = level;
//			if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
//				USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Send_Buf, 32);		// û��10ms��������
//			osSignalSet( USB_ThreadHandle, BIT_1 | BIT_2 );
	}
	else if(adValue < LEVEL_MIN)
	{
		cntLevMax = 0;
		cntLevMin ++;
		if(cntLevMin >= 5)	// ����һ�Σ�ֱ�ӽ��� CNT_TOTAL
		{
			cntLevMin = 0;
			level--;
			if(level <= 1)
				level = 1;
			SetCurrentLevel(level);	// ����Ĭ�ϵ�λ
		}
		if(level == 1)
		{
			// ����ÿ�ε�����
			USB_Send_Buf[4] = sendBuf; USB_Send_Buf[5] = sendBuf >> 8; USB_Send_Buf[6] = sendBuf >> 16; USB_Send_Buf[7] = sendBuf >> 24;
			USB_Send_Buf[25] = 0x01; USB_Send_Buf[26] = 0x01;	USB_Send_Buf[27] = level;		// 0x0101��ʾ������С
//			osSignalSet( USB_ThreadHandle, BIT_1 | BIT_2 );
		}
	}
}

void ADLoop(void const *argument)
{
//	unsigned long test = 0;
//	static unsigned char lastLevel = 4;
	(void) argument;
	
	for (;;)
	{
//			delay_ms(1000);	// ������ʱ�����Ƿ����
//			osDelay(0);			// ������ʱ���ܽ����߳����ȼ���Ϊ���
			
		  unsigned long	testAD = ADS1259_Read();
		
//			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);		// PA1����
		
			if(testAD >= 0xF0000000 && level == 1)
				testAD = 0x9B;		// С��0��ƫ�����ݣ�Ĭ��Ϊ0 + 0x9B
			
//			if(testAD == 0x80000000U + 100)
//			{
//			}
//			else 
			if(testAD >= 0x80000000)
			{
				uint8_t send_Buf[32];
				if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
				{
						memset(send_Buf, 0, sizeof(send_Buf));
						send_Buf[0] = 0xaa; send_Buf[1] = 0xbb; send_Buf[2] = 0xcc; send_Buf[3] = 0xdd;
						send_Buf[4] = testAD; send_Buf[5] = testAD >> 8; send_Buf[6] = testAD >> 16; send_Buf[7] = testAD >> 24;
						// 0xFF**��ʾSIP��ȡAD����,����ƫ�Ƴ�����Сֵ
						send_Buf[28] = 0x59; send_Buf[29] = 0x3E; send_Buf[30] = 0xBD; send_Buf[27] = level;
						USBD_CUSTOM_HID_SendReport(&USBD_Device, send_Buf, 32);
				}
			}
			else
			{
				isChangeLevel(testAD);	// ���û�������
				
//				if(lastLevel == level)
//				{
////						USB_Send_Buf[8] = lastADValue >> 8; USB_Send_Buf[9] = lastADValue;
////						USB_Send_Buf[10] = aADCxConvertedValues[1] >> 8; USB_Send_Buf[11] = aADCxConvertedValues[1];
////						USB_Send_Buf[12] = aADCxConvertedValues[2] >> 8; USB_Send_Buf[13] = aADCxConvertedValues[2];
//								USB_Send_Buf[27] = level;
//								if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
//									USBD_CUSTOM_HID_SendReport(&USBD_Device, USB_Send_Buf, 32);		// û��10ms��������
//				}
//				else
//					lastLevel = level;		// �����ͻ���֮�����һ������
				
				
//				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);		// PA1����
			}
			
//			while(!AD_DRY_R);		// �ȴ���һ������OK
//			osDelay(0);			// ������ʱ���ܽ����߳����ȼ���Ϊ���	
	}
}
