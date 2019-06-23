#ifndef ADS1259_H__
#define ADS1259_H__

#define  AD_CS    GPIO_PIN_4
#define  AD_PWN   GPIO_PIN_0	// PB1 ¸Ä PE0
#define  AD_DRY   GPIO_PIN_2	// PB0     PE2
#define  AD_CLK   GPIO_PIN_5
#define  AD_SDI   GPIO_PIN_7
#define  AD_SDO   GPIO_PIN_6

#define  AD_CS_H      HAL_GPIO_WritePin(GPIOA, AD_CS, GPIO_PIN_SET)
#define  AD_CS_L      HAL_GPIO_WritePin(GPIOA, AD_CS, GPIO_PIN_RESET)
#define  AD_PWN_H	  	HAL_GPIO_WritePin(GPIOE, AD_PWN, GPIO_PIN_SET)
#define  AD_PWN_L	  	HAL_GPIO_WritePin(GPIOE, AD_PWN, GPIO_PIN_RESET)
#define  AD_CLK_H     HAL_GPIO_WritePin(GPIOA, AD_CLK, GPIO_PIN_SET)
#define  AD_CLK_L     HAL_GPIO_WritePin(GPIOA, AD_CLK, GPIO_PIN_RESET)
#define  AD_SDI_H     HAL_GPIO_WritePin(GPIOA, AD_SDI, GPIO_PIN_SET)
#define  AD_SDI_L     HAL_GPIO_WritePin(GPIOA, AD_SDI, GPIO_PIN_RESET)
#define  AD_SDO_R     HAL_GPIO_ReadPin(GPIOA,AD_SDO)
#define  AD_DRY_R	  	HAL_GPIO_ReadPin(GPIOE,AD_DRY)

extern void AD_Init(void);
extern void AD_SPI_DMA_INIT(void);
extern unsigned long ADS1259_Read(void);
extern unsigned long AD_Read(void);
extern void ADLoop(void const *argument);

#endif
