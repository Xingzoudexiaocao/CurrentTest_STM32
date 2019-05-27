/***********************************************************************
文件名称：pg_key.h
功	  能：
编写时间：2018.09.18
编 写 人：wuliu
注    意：
***********************************************************************/
#ifndef _PG_KEY_H_
#define _PG_KEY_H_

#include "includes.h"

#define MIN		0 //分钟
#define SEC   1 //秒

extern u8 TestCompleteRf; //测试完成状态切换到空闲状态的标志
extern WIN last_win; //上次被选中的窗口
extern WIN cur_win;

extern u8 SettingFlag; //被设置了

extern u8 cur_switch; //分钟和秒选项

extern COMM Communicate;
extern u8 CommFlag;
extern u8 rrecord;
extern u8 WhichRecord;
extern u8 RecordNeedRefresh;

/*
**********************************************************************************************
*                                   void pg_key_poll(void) 
* Description: 
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void pg_key_poll(void);

#endif
