#ifndef __UPDATAAPP_H__
#define __UPDATAAPP_H__ 

#include "main.h"

#define BOOT_ADDR  		(0x08000000)   //BootLoader��ַ��8k 
#define APP_ADDR   		(0x08002000)   //������ʼ��ַ ��122k(1E800)
#define UPDATA_ADDR   (0x08020800)   //���³����ַ ��122k
#define VERIFY_ADDR	  (0x0803F000)   //У׼���ݵ�ַ ��2k(800)
#define PAGE_ADDR  		(0x0803F800)   //������־λ��ַ ��2k
#define MAX_ADDR  		(0x08040000)	 //����ַ��Χ,256k

extern void TestWriteFlash(void);
extern void SendVersionLength(void);
extern void SetVerifyValue(u8 lev, u32 val);
extern void SendRandomKeySuccess(void);
	
#endif

