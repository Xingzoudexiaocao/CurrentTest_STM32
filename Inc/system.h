#ifndef __SYSTEM_H__
#define __SYSTEM_H__ 

#include "main.h"

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
extern uint8_t USB_Receive_Buf[32];
extern uint8_t USB_Send_Buf[32];
extern volatile uint16_t USB_Receive_count; 
	
extern void SysTimInit(void);
extern void TimerLoop(void const *argument);

#endif

