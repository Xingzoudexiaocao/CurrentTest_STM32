#ifndef __SYSTEM_H__
#define __SYSTEM_H__ 

#include "main.h"

extern unsigned char _5ms;
extern unsigned char SysTime;
extern USBD_HandleTypeDef USBD_Device;
extern uint8_t USB_Receive_Buf[32];
extern uint8_t USB_Send_Buf[32];
extern volatile uint16_t USB_Receive_count; 
	
extern void SysTimInit(void);
extern void TimerLoop(void);

#endif

