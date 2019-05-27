#include "AD_spi.h"
#include "delay.h"

uint32_t ADTempx,ADTempy,ADTempz;  //AD原码
//uint32_t TempT1,TempT2,TempT3;     //温度原码

//uint32_t ADI_address=0xA0F00000;

//extern char    GcRcvBuf[100];

/*-------------------------ADS1259-----------------------------*/
void ADS1259_Write_Byte(uint8_t Data_byte)
{ 
    uint8_t i;
    
    ADCLK_OFF();
  
   for(i = 0; i < 8; i++) 
   {
          
      if((Data_byte & 0x80)==0x80)
      {
           ADDIN_ON();
      }
      else
      {
           ADDIN_OFF();
      } 
      ADCLK_ON();  
      ADCLK_OFF();
      Data_byte <<= 1;
   }
 
    ADCLK_OFF();
 //  SET_ADS_CS;
  // SET_DIN; 
}

void ADS1259_Write(uint8_t reg,uint8_t data,uint8_t data1)
{               
      ADCS_OFF(); 
      //while((P2IN&BIT4)!=0); //DRDY信号高时才能写寄存器    
      ADS1259_Write_Byte(0x40+reg);//选择写入寄存器 
      ADS1259_Write_Byte(0x01);    //写入1个寄存器     
      ADS1259_Write_Byte(data);    //写入数据 
      
      ADS1259_Write_Byte(data1);    //写入数据
      
     ADCS_ON();      //SPI的CS=1,停止 
     //挺时1uS     //可更改。但读数据必须在要等待最小一个采集周期（大约90ms）后。 
} 

void Init_ADS1259(void)
{  
		GPIO_InitTypeDef  gpioinitstruct;
	  gpioinitstruct.Pin    = IO_ADCS | IO_ADCLK | IO_ADDIN;
		gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpioinitstruct);
	
		gpioinitstruct.Pin    = IO_ADDATA;
		gpioinitstruct.Mode   = GPIO_MODE_INPUT;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &gpioinitstruct);

	  gpioinitstruct.Pin    = IO_ADRST;
		gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
		gpioinitstruct.Pull   = GPIO_NOPULL;
		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &gpioinitstruct);

//		gpioinitstruct.Pin    = AD_DRY;
//		gpioinitstruct.Mode   = GPIO_MODE_INPUT;
//		gpioinitstruct.Pull   = GPIO_NOPULL;
//		gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
//		HAL_GPIO_Init(GPIOB, &gpioinitstruct);

//	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);
//	
//	LPC_IOCON->R_PIO1_0 &= ~0x07;
//	LPC_IOCON->R_PIO1_0 |= 0x01;
//	LPC_IOCON->R_PIO1_1 &= ~0x07;
//	LPC_IOCON->R_PIO1_1 |= 0x01;
//	LPC_IOCON->R_PIO1_2 &= ~0x07;
//	LPC_IOCON->R_PIO1_2 |= 0x01;
//	LPC_IOCON->SWDIO_PIO1_3 &= ~0x07;
//	LPC_IOCON->SWDIO_PIO1_3 |= 0x01;
//	LPC_IOCON->PIO1_4 &= ~0x07;
//	
//	LPC_IOCON->PIO0_3 &= ~0x07;
//	LPC_IOCON->PIO0_4 &= ~0x07;
//	LPC_IOCON->PIO0_5 &= ~0x07;
//	LPC_IOCON->PIO0_6 &= ~0x07;
//	LPC_IOCON->PIO0_7 &= ~0x07;
//	LPC_IOCON->PIO0_8 &= ~0x07;
//	LPC_IOCON->PIO0_9 &= ~0x07;
//	
//	LPC_GPIO1->DIR |= IO_G0;
//	LPC_GPIO1->DIR |= IO_G1;
//	LPC_GPIO1->DIR |= IO_G2;
//	LPC_GPIO1->DIR |= IO_G3;
//	LPC_GPIO1->DIR |= IO_G4; 
//	
//	LPC_GPIO0->DIR |= IO_ADRST;
//	LPC_GPIO0->DIR |= IO_ADCNV;
//	LPC_GPIO0->DIR |= IO_ADCS;
//	LPC_GPIO0->DIR |= IO_ADCLK;
//	LPC_GPIO0->DIR |= IO_ADDIN;
//	
//	LPC_GPIO0->DIR &=~IO_ADDATA;
//	LPC_GPIO0->DIR &=~IO_ADINT;
	
//	SET_GAIN_0_5;
	
	ADRST_ON();
	//delay_us(1000);
	ADRST_OFF();
	delay_us(2);
	ADRST_ON();
	
	//ADS1259_Write_Byte(0x02);
	
	//ADCNV_OFF();
	//ADCNV_ON();
	ADS1259_Write_Byte(0x08);
	ADCLK_OFF();
	ADS1259_Write(01,0x40,0x04);
}

uint32_t ADS1259_Read_2Word(void)
{ 
    uint8_t i;
    uint32_t Data_read;
    
    ADCS_OFF();
    Data_read=0;

    for(i = 0; i < 32; i++) 
    {
    	ADCLK_OFF();
    	//delay_us(1);
      Data_read <<= 1;
      ADCLK_ON();
      delay_us(1);            


      if(READ_ADDATA)
      {
      	Data_read |=0x01;
      }          

     }
     ADCLK_OFF();

     ADCS_ON();
	 //ADCNV_OFF();
	 //delay_us(10);
	 //ADCNV_ON();
     return(Data_read);
}

uint16_t ADS1259_Read_from_Rg(uint8_t reg)
{ 
    uint8_t i;
    uint8_t addata;
    uint16_t Data_read;
    
    ADCS_OFF();
    Data_read=0;
    //ADS1259_DOUT;//设置P2.3为输入
    ADS1259_Write_Byte(0x20+reg);//选择寄存器地址，寄存器个数
    ADS1259_Write_Byte(0x01);
     for(i = 0; i < 16; i++) 
    {      
          ADCLK_OFF();
      	  Data_read <<= 1;
          ADCLK_ON();
             addata=READ_ADDATA;
             if(addata)
             {
                Data_read |=0x01;
             }          
     }

     ADCLK_OFF();

    ADCS_ON();
    return(Data_read);
}

