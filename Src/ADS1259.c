#include "main.h"
#include "ADS1259.h"
#include "delay.h"

#define  AD_CS    GPIO_PIN_4
#define  AD_PWN   GPIO_PIN_1	// PB1
#define  AD_DRY   GPIO_PIN_0	// PB0
#define  AD_CLK   GPIO_PIN_5
#define  AD_SDI   GPIO_PIN_7
#define  AD_SDO   GPIO_PIN_6

#define  AD_CS_H      HAL_GPIO_WritePin(GPIOA, AD_CS, GPIO_PIN_SET)
#define  AD_CS_L      HAL_GPIO_WritePin(GPIOA, AD_CS, GPIO_PIN_RESET)
#define  AD_PWN_H	  	HAL_GPIO_WritePin(GPIOB, AD_PWN, GPIO_PIN_SET)
#define  AD_PWN_L	  	HAL_GPIO_WritePin(GPIOB, AD_PWN, GPIO_PIN_RESET)
#define  AD_CLK_H     HAL_GPIO_WritePin(GPIOA, AD_CLK, GPIO_PIN_SET)
#define  AD_CLK_L     HAL_GPIO_WritePin(GPIOA, AD_CLK, GPIO_PIN_RESET)
#define  AD_SDI_H     HAL_GPIO_WritePin(GPIOA, AD_SDI, GPIO_PIN_SET)
#define  AD_SDI_L     HAL_GPIO_WritePin(GPIOA, AD_SDI, GPIO_PIN_RESET)
#define  AD_SDO_R     HAL_GPIO_ReadPin(GPIOA,AD_SDO)
#define  AD_DRY_R	  	HAL_GPIO_ReadPin(GPIOB,AD_DRY)

u8 InitData[9]={0x05,0x18,0x22,0xff,0xff,0x7f,0x00,0x00,0x40};

u8 RecData[9];

static void AD_Start(void)
{
	//IO输出
//	PGA_WrCmd(0x45,0x18);
	//启动CS选择
	AD_CS_L;
//	AD_SPI_WR_Byte(0xc5);
}

static void AD_Stop(void)
{
	AD_CS_H;
	delay_us(5);
//	PGA_WrCmd(0x45,0x08);
}

static void AD_SPI_WR_Byte(unsigned char dat)
{
   unsigned char i;
//		AD_Start();
   AD_CLK_L;
   delay_us(5);
   for(i=0;i<8;i++)
   {
	   if(dat&0x80)  AD_SDI_H;
	   else  		 AD_SDI_L;
	   delay_us(5);
	   dat<<=1;
	   AD_CLK_H;
	   delay_us(5);
	   AD_CLK_L;
	   delay_us(5);
   }
//	 AD_Stop();
}

static unsigned char AD_SPI_RD_Byte(void)
{
	unsigned char i,dat;
	dat=0;
//	AD_Start();
	AD_CLK_L;
	delay_us(5);
	for(i=0;i<8;i++)
	{
		dat<<=1;
		AD_CLK_H;
		delay_us(5);
		if(AD_SDO_R) dat|=1;
		AD_CLK_L;
		delay_us(5);	
	}
//	AD_Stop();
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
	u16 i;
	char count;
	unsigned long dat[10];
	unsigned long result;
	AD_Start();  
	AD_SPI_WR_Byte(0x10);	//连续模式下发送要读的数据
	delay_us(5000);

    for(i=0;i<10;i++) 
	{
		dat[i]=0;		  
		while(AD_DRY_R);
		for(count=0;count<3;count++)
		{
			dat[i]=dat[i]<<8;
			dat[i]=dat[i]|AD_SPI_RD_Byte();
		}
		dat[i]=dat[i]&0x7fffff;	
    } 	
	AD_SPI_WR_Byte(0x11);	//连续模式下停止读数据
	AD_Stop();
	delay_us(5000);
	result=0;
	for(i=5;i<9;i++) 
	{
		result=result+dat[i];
	}
	result=result/4;
//	result=result>>9;
//	if(result < 1)
//	result =  5;
	return result;
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
		HAL_GPIO_Init(GPIOB, &gpioinitstruct);

		gpioinitstruct.Pin    = AD_DRY;
		gpioinitstruct.Mode   = GPIO_MODE_INPUT;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &gpioinitstruct);

//	for(int i = 0; i < 9; i++)
//	{
//		printf("** RecData%d = %x. ** \n\r", i, RecData[i]);
//	}
	
		AD_PWN_L;
		delay_us(2);
		AD_PWN_H;
		
	//启动时需延时一段时间待电压稳定
	delay_ms(2*50);	
	
	AD_Start();

	//ADS1259 INIT
//	AD_SPI_WR_Byte(0x11);	//在连续模式下停止发送要读的数据
	AD_WriteToRes(0x40,0x08,InitData);
	AD_ReadFrRes(0x20,0x08,RecData);
//	RecData[8] = 0xff;
	for(int i = 0; i < 9; i++)
	{
		printf("** RecData%d = %x. ** \n\r", i, RecData[i]);
	}
	printf("** unsgined int length is %d. ** \n\r", sizeof(unsigned int));
	printf("** unsgined long length is %d. ** \n\r", sizeof(unsigned long));
}
