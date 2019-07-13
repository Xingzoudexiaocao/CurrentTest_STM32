#ifndef __SYSTEM_H__
#define __SYSTEM_H__ 

#include "main.h"

#define BIT_0	( 1 << 0 )
#define BIT_1	( 1 << 1 )
#define BIT_2	( 1 << 2 )

#define VDD_APPLI                      ((uint32_t) 3000)   /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_12BITS                   ((uint32_t) 4095)   /* Max value with a full range of 12 bits */
#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)    3)   /* Size of array containing ADC converted values: set to ADC sequencer number of ranks converted, to have a rank in each address */
/* Internal temperature sensor: constants data used for indicative values in  */
/* this example. Refer to device datasheet for min/typ/max values.            */
/* For more accurate values, device should be calibrated on offset and slope  */
/* for application temperature range.                                         */
#define INTERNAL_TEMPSENSOR_V25        ((int32_t)1430)         /* Internal temperature sensor, parameter V25 (unit: mV). Refer to device datasheet for min/typ/max values. */
#define INTERNAL_TEMPSENSOR_AVGSLOPE   ((int32_t)4300)         /* Internal temperature sensor, parameter Avg_Slope (unit: uV/DegCelsius). Refer to device datasheet for min/typ/max values. */                                                               /* This calibration parameter is intended to calculate the actual VDDA from Vrefint ADC measurement. */

/**
  * @brief  Computation of temperature (unit: degree Celsius) from the internal
  *         temperature sensor measurement by ADC.
  *         Computation is using temperature sensor standard parameters (refer
  *         to device datasheet).
  *         Computation formula:
  *         Temperature = (VTS - V25)/Avg_Slope + 25
  *         with VTS = temperature sensor voltage
  *              Avg_Slope = temperature sensor slope (unit: uV/DegCelsius)
  *              V25 = temperature sensor @25degC and Vdda 3.3V (unit: mV)
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param TS_ADC_DATA: Temperature sensor digital value measured by ADC
  * @retval None
  */
	// 换算方法有问题，应该不用X1000的
#define COMPUTATION_TEMPERATURE_STD_PARAMS(TS_ADC_DATA)                        \
  ((((int32_t)(INTERNAL_TEMPSENSOR_V25 - (((TS_ADC_DATA) * VDD_APPLI) / RANGE_12BITS)   \
     ) * 1000                                                                  \
    ) / INTERNAL_TEMPSENSOR_AVGSLOPE                                           \
   ) + 25                                                                      \
  )
/**
  * @brief  Computation of voltage (unit: mV) from ADC measurement digital
  *         value on range 12 bits.
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param ADC_DATA: Digital value measured by ADC
  * @retval None
  */
#define COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(ADC_DATA)                        \
  ( (ADC_DATA) * VDD_APPLI / RANGE_12BITS)

#define LED_1_PORT                 GPIOC
#define LED_1_PIN                  GPIO_PIN_6
#define LED_2_PORT                 GPIOC
#define LED_2_PIN                  GPIO_PIN_7

#define LELVE_PORT 								GPIOE
#define LELVE_1_PIN               GPIO_PIN_3
#define LELVE_2_PIN               GPIO_PIN_4
#define LELVE_3_PIN               GPIO_PIN_5
#define LELVE_4_PIN               GPIO_PIN_6

#define LEVEL_MAX   0x6B851D			// 换档最大电压 2.1v
#define LEVEL_MIN 	0x010000			// 换档最小电压 19.53mv
#define CNT_TOTAL		10  	// 连续出现n次超量程数据就换档

typedef enum
{
  THREAD_LED = 0,
  THREAD_AD = 1,
	THREAD_USB = 2,
} Thread_TypeDef;

extern osThreadId LED_ThreadHandle, AD_ThreadHandle, USB_ThreadHandle;

extern unsigned char _5ms;
extern unsigned char SysTime;
extern USBD_HandleTypeDef USBD_Device;
extern SPI_HandleTypeDef SpiHandle;
extern ADC_HandleTypeDef    AdcHandle;
extern uint8_t USB_Receive_Buf[32];
extern uint8_t USB_Send_Buf[32];
extern volatile uint16_t USB_Receive_count; 
extern unsigned char level;
extern unsigned char cntLevMax;		// 测量大于最大电压计数
extern unsigned char cntLevMin;		// 测量小于最小电压计数

extern void SysTimInit(void);
extern void SetCurrentLevel(uint8_t lev);
extern void TimerLoop(void const *argument);

#endif

