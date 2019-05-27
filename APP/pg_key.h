/***********************************************************************
�ļ����ƣ�pg_key.h
��	  �ܣ�
��дʱ�䣺2018.09.18
�� д �ˣ�wuliu
ע    �⣺
***********************************************************************/
#ifndef _PG_KEY_H_
#define _PG_KEY_H_

#include "includes.h"

#define MIN		0 //����
#define SEC   1 //��

extern u8 TestCompleteRf; //�������״̬�л�������״̬�ı�־
extern WIN last_win; //�ϴα�ѡ�еĴ���
extern WIN cur_win;

extern u8 SettingFlag; //��������

extern u8 cur_switch; //���Ӻ���ѡ��

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
