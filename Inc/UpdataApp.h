#ifndef __UPDATAAPP_H__
#define __UPDATAAPP_H__ 

#include "main.h"

#define BOOT_ADDR  		(0x08000000)   //BootLoader��ַ��8k 
#define APP_ADDR   		(0x08002000)   //������ʼ��ַ ��250k
#define UPDATA_ADDR   (0x08040800)   //���³����ַ ��250k
#define RESERVE_ADDR	(0x0807F000)   //Ԥ����ַ ��2k
#define PAGE_ADDR  		(0x0807F800)   //������־λ��ַ ��2k
#define MAX_ADDR  		(0x08080000)	 //����ַ��Χ

extern void TestWriteFlash(void);

#endif

