/*
*********************************************************************************************************
*                               MiniSTM32 Evaluation BOARD SUPPORT PACKAGE
*
*									(c) Copyright 2009; GXU CCIE ES 
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*********************************************************************************************************
*                                   BOARD SUPPORT PACKAGE
*
*            ST Microelectronics STM32	with the MiniSTM32 Evaluation Board
*
* Filename      : KeyBoard_Drv.h
* Version       : V1.0
* Programmer(s) : Doggie
*********************************************************************************************************/
#ifndef   __KEYBOARD_DRV_H__
#define   __KEYBOARD_DRV_H__


extern void  	KeyBoard_Drv_Init(void);
extern void 	KeyLoop (void);

extern unsigned int isKeyDown;
extern unsigned int isKeyLongDown;

extern unsigned int rotateCodeLeft;
extern unsigned int rotateCodeRight;

#define 	Key1  			(0x0001)   
#define 	Key2  			(0x0002)  
#define 	Key3  			(0x0004)  
#define 	Key4  			(0x0008)  
#define 	Key5  			(0x0010)  
#define 	Key6  			(0x0020)  
#define 	Key7  			(0x0040)  
#define 	UnDefine8  	(0x0080)   
#define 	UnDefine9  	(0x0100)  
#define 	UnDefine10  (0x0200)  
#define 	UnDefine11  (0x0400)  
#define 	UnDefine12  (0x0800)   
#define 	UnDefine13  (0x1000)
#define 	UnDefine14  (0x2000)
#define 	UnDefine15  (0x4000)
#define 	UnDefine16  (0x8000)
// 定义按键1
#define  	KeyShort_OnOff2			isKeyDown & Key1
#define 	ClearKey_OnOff2			isKeyDown = isKeyDown & (~Key1)
#define  	KeyLong_Set					isKeyLongDown & Key1
#define 	ClearKey_Set				isKeyLongDown = isKeyLongDown & (~Key1)
// 定义按键2
#define  	KeyShort_Shift			isKeyDown & Key2
#define 	ClearKey_Shift			isKeyDown = isKeyDown & (~Key2)
#define  	KeyLong_Lock				isKeyLongDown & Key2
#define 	ClearKey_Lock				isKeyLongDown = isKeyLongDown & (~Key2)
// 定义按键3
#define  	KeyShort_Left			isKeyDown & Key3
#define 	ClearKey_Left			isKeyDown = isKeyDown & (~Key3)
#define  	KeyLong_Null_3				isKeyLongDown & Key3
#define 	ClearKey_Null_3				isKeyLongDown = isKeyLongDown & (~Key3)
// 定义按键4
#define  	KeyShort_Right			isKeyDown & Key4
#define 	ClearKey_Right			isKeyDown = isKeyDown & (~Key4)
#define  	KeyLong_Null_4				isKeyLongDown & Key4
#define 	ClearKey_Null_4				isKeyLongDown = isKeyLongDown & (~Key4)
// 定义按键5
#define  	KeyShort_OnOff1			isKeyDown & Key5
#define 	ClearKey_OnOff1			isKeyDown = isKeyDown & (~Key5)
#define  	KeyLong_Null_5				isKeyLongDown & Key5
#define 	ClearKey_Null_5				isKeyLongDown = isKeyLongDown & (~Key5)
// 定义按键6
#define  	KeyShort_Current			isKeyDown & Key6
#define 	ClearKey_Current			isKeyDown = isKeyDown & (~Key6)
#define  	KeyLong_Null_6				isKeyLongDown & Key6
#define 	ClearKey_Null_6				isKeyLongDown = isKeyLongDown & (~Key6)
// 定义按键7
#define  	KeyShort_Value			isKeyDown & Key7
#define 	ClearKey_Value			isKeyDown = isKeyDown & (~Key7)
#define  	KeyLong_Null_7				isKeyLongDown & Key7
#define 	ClearKey_Null_7				isKeyLongDown = isKeyLongDown & (~Key7)
// 定义清楚所有按键信息
#define 	ClearAllKey				isKeyDown = 0;isKeyLongDown = 0;
#endif

