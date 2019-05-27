
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
#include "Flash.h"


MemoryType MemoryData = {500,200,500,200};

// static uc32 	_FC_BeginBase =  0x8000000;	//数据的基地址
static uc32 	_FC_AddBase =  0x8004000;	//数据的基地址，有400K左右的容量 0x801B800
// static uc32   _FC_MaxAdd = 0x807ffff;		//最大地址


#define FLASH_SIZE 512          //所选MCU的FLASH容量大小(单位为K)

#if FLASH_SIZE<256
  #define SECTOR_SIZE           1024    //??
#else 
  #define SECTOR_SIZE           2048    //??
#endif

//?????????????
void MemoryWrite(u32 startAddress,void *writeData,u32 lenght)
{
		uint8_t status = 0;
		uint8_t index = 0;
		uint32_t *date = (uint32_t*)writeData;
	  //1???FLASH
		HAL_FLASH_Unlock();

    //2???FLASH
    //???FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = startAddress;
    f.NbPages = 1;
    //??PageError
    uint32_t PageError = 0;
    //??????
    HAL_FLASHEx_Erase(&f, &PageError);

	  for (index = 0; index < lenght/4; index++)
    {
			  //3??FLASH??
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startAddress + (4 * index), *(date + index));

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

      /* If the program operation is completed, disable the PG Bit */
      CLEAR_BIT(FLASH->CR, FLASH_CR_PG);

      /* In case of error, stop programation procedure */
      if (status != HAL_OK)
      {
        break;
      }
    }

    //4???FLASH
		HAL_FLASH_Lock();
}

void MemoryRead(const void* source ,s32 num,s32 offset,void *des)
{
   const u8* s = (const u8*)source;
	 u8 *d = (u8*)des;
	 while(offset>0)
	 {
	    s++;
		offset--;
	 }
	 while(num>0)
	 {
	    *d = *s;
		d++;
		s++;
		num--; 
	 }
}

/*----------------------------- SUB ROUTINES --------------------------------*/
void FlashInit(void)
{
		u8 *add = (u8*)_FC_AddBase;
		MemoryRead(add ,sizeof(MemoryData),0,&MemoryData);
// 		MemoryRead(add ,sizeof(MemoryData.out1_V),0,&MemoryData.out1_V);
// 		MemoryRead(add ,sizeof(MemoryData.out1_A),4,&MemoryData.out1_A);
	
// 		 MemoryData.out1_V = 500;			// 输出1电压
// 		 MemoryData.out1_A = 200;			// 输出1电流
// 		 MemoryData.out2_V = 500;			// 输出2电压
// 		 MemoryData.out2_A = 200;			// 输出2电流
}

void FlashLoop(void)
{
		static u8 cnt100ms = 0;
		static u32 buf_V = 0;
		static u32 buf_A = 0;
		if(SysTime)
		{
				cnt100ms++;
				if(cnt100ms >= 20)
				{
						cnt100ms = 0;
						if(buf_V != MemoryData.out1_V)
						{
								buf_V = MemoryData.out1_V;
								MemoryWrite(_FC_AddBase,&MemoryData,sizeof(MemoryData));
						}
						if(buf_A != MemoryData.out1_A)
						{
								buf_A = MemoryData.out1_A;
								MemoryWrite(_FC_AddBase,&MemoryData,sizeof(MemoryData));
						}
				}
		}
}
