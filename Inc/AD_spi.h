/* LPC11xx外设寄存器            */
#include <stdio.h>
#include <string.h>
#include  <math.h> 

#include "main.h"

////gain
//#define  IO_G0   (1ul << 0)
//#define  IO_G1   (1ul << 1)
//#define  IO_G2   (1ul << 2)
//#define  IO_G3   (1ul << 3)
//#define  IO_G4   (1ul << 4)

//#define  G0_ON()   { LPC_GPIO1->DATA   |= IO_G0; }
//#define  G0_OFF()  { LPC_GPIO1->DATA   &=~IO_G0; }
//#define  G1_ON()   { LPC_GPIO1->DATA   |= IO_G1; }
//#define  G1_OFF()  { LPC_GPIO1->DATA   &=~IO_G1; }
//#define  G2_ON()   { LPC_GPIO1->DATA   |= IO_G2; }
//#define  G2_OFF()  { LPC_GPIO1->DATA   &=~IO_G2; }
//#define  G3_ON()   { LPC_GPIO1->DATA   |= IO_G3; }
//#define  G3_OFF()  { LPC_GPIO1->DATA   &=~IO_G3; }
//#define  G4_ON()   { LPC_GPIO1->DATA   |= IO_G4; }
//#define  G4_OFF()  { LPC_GPIO1->DATA   &=~IO_G4; }

//#define  SET_GAIN_0_125   { G0_OFF();G1_OFF();G2_OFF();G3_OFF();G4_OFF(); }
//#define  SET_GAIN_0_25    { G0_ON() ;G1_OFF();G2_OFF();G3_OFF();G4_OFF(); }
//#define  SET_GAIN_0_5     { G0_OFF();G1_ON() ;G2_OFF();G3_OFF();G4_OFF(); }
//#define  SET_GAIN_1       { G0_ON() ;G1_ON() ;G2_OFF();G3_OFF();G4_OFF(); }
//#define  SET_GAIN_2       { G0_OFF();G1_OFF();G2_ON() ;G3_OFF();G4_OFF(); }
//#define  SET_GAIN_4       { G0_ON() ;G1_OFF();G2_ON() ;G3_OFF();G4_OFF(); }
//#define  SET_GAIN_8       { G0_OFF();G1_ON() ;G2_ON() ;G3_OFF();G4_OFF(); }
//#define  SET_GAIN_16      { G0_ON() ;G1_ON() ;G2_ON() ;G3_OFF();G4_OFF(); }
//#define  SET_GAIN_32      { G0_OFF();G1_OFF();G2_OFF();G3_ON() ;G4_OFF(); }
//#define  SET_GAIN_64      { G0_ON() ;G1_OFF();G2_OFF();G3_ON() ;G4_OFF(); }
//#define  SET_GAIN_128     { G0_OFF();G1_ON() ;G2_OFF();G3_ON() ;G4_OFF(); }

//ADC
#define  IO_ADRST    GPIO_PIN_1
//#define  IO_ADCNV    (1ul << 4)
#define  IO_ADCS     GPIO_PIN_4
#define  IO_ADCLK    GPIO_PIN_5
#define  IO_ADDIN    GPIO_PIN_7
#define  IO_ADDATA   GPIO_PIN_6
//#define  IO_ADINT    (1ul << 9)

#define ADRST_ON()   HAL_GPIO_WritePin(GPIOB, IO_ADRST, GPIO_PIN_SET)
#define ADRST_OFF()  HAL_GPIO_WritePin(GPIOB, IO_ADRST, GPIO_PIN_RESET)
//#define ADCNV_ON()   { LPC_GPIO0->DATA   |= IO_ADCNV; }
//#define ADCNV_OFF()  { LPC_GPIO0->DATA   &=~IO_ADCNV; }
#define ADCS_ON()    HAL_GPIO_WritePin(GPIOA, IO_ADCS, GPIO_PIN_SET)
#define ADCS_OFF()   HAL_GPIO_WritePin(GPIOA, IO_ADCS, GPIO_PIN_RESET)
#define ADCLK_ON()   HAL_GPIO_WritePin(GPIOA, IO_ADCLK, GPIO_PIN_SET)
#define ADCLK_OFF()  HAL_GPIO_WritePin(GPIOA, IO_ADCLK, GPIO_PIN_RESET)
#define ADDIN_ON()   HAL_GPIO_WritePin(GPIOA, IO_ADDIN, GPIO_PIN_SET)
#define ADDIN_OFF()  HAL_GPIO_WritePin(GPIOA, IO_ADDIN, GPIO_PIN_RESET)
 
#define READ_ADDATA  HAL_GPIO_ReadPin(GPIOB,IO_ADDATA)

extern void Init_ADS1259(void);

//void ADS1259_Start();
//void ADS1259_Stop();

extern void ADS1259_Write_Byte(uint8_t Data_byte);
extern void ADS1259_Write(uint8_t reg,uint8_t data,uint8_t data1);//写寄存器。选择寄存器地址，写入的数据。
extern uint32_t ADS1259_Read_2Word(void);
extern uint16_t ADS1259_Read_from_Rg(uint8_t reg);
