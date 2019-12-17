#ifndef __UPDATAAPP_H__
#define __UPDATAAPP_H__ 

#include "main.h"

#define BOOT_ADDR  		(0x08000000)   //BootLoader地址，8k 
#define APP_ADDR   		(0x08002000)   //程序起始地址 ，250k
#define UPDATA_ADDR   (0x08040800)   //更新程序地址 ，250k
#define RESERVE_ADDR	(0x0807F000)   //预留地址 ，2k
#define PAGE_ADDR  		(0x0807F800)   //升级标志位地址 ，2k
#define MAX_ADDR  		(0x08080000)	 //最大地址范围

extern void TestWriteFlash(void);

#endif

