#ifndef __FLASH_H__
#define __FLASH_H__ 

typedef struct{
 u32 out1_V;			// ���1��ѹ
 u32 out1_A;			// ���1����
 u32 out2_V;			// ���2��ѹ
 u32 out2_A;			// ���2����
}MemoryType;
extern MemoryType MemoryData;

extern void FlashInit(void);
extern void FlashLoop(void);

#endif

