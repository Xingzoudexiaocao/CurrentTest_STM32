
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
#include "UpdataApp.h"
#include "Flash.h"
// 定义局部变量数组大小超过1024字节，调用menset/memcpy函数是会导致程序奔溃，解决方法：定义为静态static；
void TestWriteFlash(void)
{		
		u16 i = 0;
		u8 buf[1024];
		for(i = 0; i < 1024; i++)
			buf[i] = i & 0xFF;
//		MemoryRead((u8 *)BOOT_ADDR, sizeof(buf),0,&buf);
		MemoryWrite(UPDATA_ADDR, buf, 1024);
		MemoryWrite(UPDATA_ADDR + 1024, buf, 1024);
	
//		for(i = 0; i < 32; i++)
//				buf[i] = 0xBB;
//		MemoryWrite(UPDATA_ADDR, buf, 32);
//		for(i = 0; i < 32; i++)
//				buf[i] = 0xCC;
//		MemoryWrite(UPDATA_ADDR, buf, 32);
	
//		MemoryWrite(UPDATA_ADDR + 1024, buf, 1024);
//		MemoryWrite(UPDATA_ADDR + 1024, buf + 1, 1024);
//		MemoryWrite(UPDATA_ADDR + 512, buf + 512, 512);
//
//		for(i = 0; i < 32; i++)
//			MemoryWrite(UPDATA_ADDR + (32 * i), buf + (32 * i), 32);
	
//	uint8_t send_Buf[32];
//	uint32_t timeCnt = HAL_GetTick();			// 获取时间
//	i = 0;
//	while(i < 32)
//	{
//		memcpy(&send_Buf, &buf[i*32], sizeof(send_Buf));
//		timeCnt = HAL_GetTick();
//		send_Buf[0] = timeCnt >> 24; send_Buf[1] = timeCnt >> 16;
//		send_Buf[2] = timeCnt >> 8; send_Buf[3] = timeCnt;
//		if (USBD_CUSTOM_HID_SendReport(&USBD_Device, send_Buf, 32) == USBD_OK)
//		{
//				i++;
////				if(i >= 32)
////					return;
//		}
//	}
	
}
// 发送程序软件版本号和文件长度信息
void SendVersionLength(void)
{
	u8 buf[32];
	memset(&buf, YMODEM_VER_LEN, 32);	// Send YMODEM_VER_LEN
	MemoryRead((u8 *)(PAGE_ADDR + 4), sizeof(buf),0,buf + 4);	// 读取版本号和文件长度
	if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
		USBD_CUSTOM_HID_SendReport(&USBD_Device, buf, 32);	// 发送数据
	
}
// 设置校验值到Flash，同时发送校验值给上位机
void SetVerifyValue(u8 lev, u32 val)
{
	u8 buf[32];
	u8 flashValue[100];
	MemoryRead((u8 *)(VERIFY_ADDR), sizeof(flashValue),0,flashValue);	// 读取
	if(lev >= 1 && lev <= 8)
	{
		memcpy(flashValue + (lev - 1) * 4, &val, sizeof(val));
		MemoryWrite(VERIFY_ADDR, flashValue, sizeof(flashValue));		// 写入
	}
	
	memset(&buf, YMODEM_VALID_VALUE_1, 32);	// 
	memcpy(buf + 4, flashValue + 8, 24);					// 去除1档的校验数据
	if (USBD_Device.dev_state == USBD_STATE_CONFIGURED )
		USBD_CUSTOM_HID_SendReport(&USBD_Device, buf, 32);	// 发送数据
	
}
