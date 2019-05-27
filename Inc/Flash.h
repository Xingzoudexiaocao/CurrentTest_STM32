#ifndef __FLASH_H__
#define __FLASH_H__ 

typedef struct{
 u32 out1_V;			// 输出1电压
 u32 out1_A;			// 输出1电流
 u32 out2_V;			// 输出2电压
 u32 out2_A;			// 输出2电流
}MemoryType;
extern MemoryType MemoryData;

extern void FlashInit(void);
extern void FlashLoop(void);

#endif

