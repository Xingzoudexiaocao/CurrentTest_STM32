/*****************************************************************************
 *              MH-STM32_Project   
 *              Copyright (c) 2016
 *              All Rights Reserved
 *
 * File:UI.c
 * Description: 
 * Author: zsdshuai  
 *---------------------------------------------------------------------------
 * Last Changed By:  
 * Revision:         V1.00 
 * Content:          Creat the File
*******************************************************************************/

#include "main.h"
#include "UI.h"
#include "pg_ct.h"
#include "pg_rf.h"
#include "../lvgl/lvgl.h"
#include "lv_examples/lv_tests/lv_test_theme/lv_test_theme.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "lv_test_btn.h"
#include "lv_test_gauge.h"

/******************************/
u8 Littlev_Dly = 0;

/*
********************************************************************************
*                                   void Littlev_Tick(void)
* Description:  
* Arguments  : 
* Returns    :            
**********************************************************************************
*/
void Littlev_Tick(void)
{
	if(Littlev_Dly)
	{
		Littlev_Dly--;
	}
}

static void lv_key_task(void * param)
{
//	pg_key_poll();
}

void lv_key_init(void)
{    
	lv_task_create(lv_key_task, 1, LV_TASK_PRIO_HIGHEST, NULL);
}


/*
********************************************************************************
*                                void UI_Init(void)
* Description:  
* Arguments  : 
* Returns    :            
**********************************************************************************
*/
void UI_Init(void)
{
//  LCD_GPIO_init();
//	LCD_RST();  
//	DelayTimer(50);
//	LCD_init();
	
	lv_init();
	tft_init();
	
	page_create();
//	lv_key_init();
	
}
/*
********************************************************************************
*                                void UI_Poll(void)
* Description:  
* Arguments  : 
* Returns    :            
**********************************************************************************
*/
void UI_Poll(void)
{
//	refresh_poll();
	
  if(Littlev_Dly == 0)
		{
			Littlev_Dly = 10;
			lv_task_handler();
		}

}

/******************* (C) COPYRIGHT 2011  *****END OF FILE*******************************/
