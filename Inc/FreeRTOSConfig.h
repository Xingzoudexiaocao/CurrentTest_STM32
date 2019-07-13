/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
 #include <stdint.h>
 extern uint32_t SystemCoreClock;
#endif
//  钩子函数的主要功能是用于函数的扩展，用户可以根据自己的需要往里面添加相关的测试函数。
#define configUSE_PREEMPTION                    1		// 1使能抢占式调度器。0使能合作式调度器。
#define configUSE_IDLE_HOOK                     0		// 1使能空闲任务的钩子函数  0禁能空闲任务钩子函数
#define configUSE_TICK_HOOK                     0		// 1使能滴答定时器中断里面执行的钩子函数  0禁能
#define configCPU_CLOCK_HZ                      ( SystemCoreClock )			// 此参数用于定义CPU的主频，单位Hz。
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )	// 此参数用于定义系统时钟节拍数，单位Hz，一般取1000Hz即可。过高的的系统时钟节拍将使得FreeRTOS内核运行占用过多的时间，增加系统负荷。
#define configMAX_PRIORITIES                    ( 7 )	//此参数用于定义可供用户使用的最大优先级数，如果这个定义的是5，那么用户可以使用的优先级号是0,1,2,3,4，不包含5，对于这一点，初学者要特别的注意。
#define configMINIMAL_STACK_SIZE                ( ( uint16_t ) 128 )	// 此参数用于定义空闲任务的栈空间大小，单位字，即4字节。
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 5 * 1024 ) )	// 定义堆大小，FreeRTOS内核，用户动态内存申请，任务栈，任务创建，信号量创建，消息队列创建等都需要用这个空间。
#define configMAX_TASK_NAME_LEN                 ( 16 )		// 定义任务名最大的字符数，末尾的结束符 '\0'也要计算在内。
#define configUSE_TRACE_FACILITY                1		// 设置成1表示启动可视化跟踪调试，会激活一些附加的结构体成员和函数。
/*
configUSE_16_BIT_TICKS 系统时钟节拍计数使用TickType_t数据类型定义的。
如果用户使能了宏定义 configUSE_16_BIT_TICKS，那么TickType_t定义的就是16位无符号数，如果没有使能，那么TickType_t定义的就是32位无符号数。
对于32位架构的处理器，一定要禁止此宏定义，即设置此宏定义数值为0即可。而16位无符号数类型主要用于8位和16位架构的处理器。*/
#define configUSE_16_BIT_TICKS                  0
/*
configIDLE_SHOULD_YIELD 此参数用于使能与空闲任务同优先级的任务，只有满足以下两个条件时，此参数才有效果：
1.     使能抢占式调度器。
2.     有创建与空闲任务同优先级的任务。
配置为1，就可以使能此特性了，实际应用中不建议用户使用此功能，将其配置为0即可。*/
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_MUTEXES                       1			// 1使能互斥信号量。0禁能互斥信号量。
#define configQUEUE_REGISTRY_SIZE               8
/*
FreeRTOS的栈溢出检测支持两种方法，为了方便描述，我们这里将其称之为方法一和方法二。
配置为2栈溢出检测使用方法二。
配置为1栈溢出检测使用方法一。
配置为0禁止栈溢出检测。*/
#define configCHECK_FOR_STACK_OVERFLOW          0
#define configUSE_RECURSIVE_MUTEXES             1			// 1使能递归互斥信号量   0禁能递归互斥信号量
/*
configUSE_MALLOC_FAILED_HOOK 当创建任务，信号量或者消息队列时，FreeRTOS通过函数pvPortMalloc()申请动态内存。
配置为1使能动态内存申请失败时的钩子函数
配置为0禁能动态内存申请失败时的钩子函数*/
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           1			// 1使能计数信号量 0禁能计数信号量
#define configGENERATE_RUN_TIME_STATS           0			// 1使能任务运行状态参数统计。 0禁能

#define configENABLE_BACKWARD_COMPATIBILITY 0

/* Co-routine definitions. */
/*CO_ROUTINES这个不好翻译，网上都叫协同程序，或者合作程序，理解为协同一起使用的程序
协程：（Co-routines）主要用于资源发非常受限的嵌入式系统（RAM非常少），通常不会用于32位微处理器。*/
#define configUSE_CO_ROUTINES                   0				// 1使用   0不使用
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )		// CO_ROUTINE优先级

/* Software timer definitions. */
/*TIMER即定时器，在RTOS中的TIMER属于软件定时。FreeRTOS的定时器精度不高，会随着定时的增加而改变，特别是TIMER任务优先级较低，高优先级占用资源的情况下。
若要使用高精度的定时，还是最后使用硬件的定时器（现在处理器一般都有多个硬件TIMER）。
软件定时器其实也是需要创建一个任务，创建方式和我们常规的一样，只是它是有系统内核完成，不用我们自己写创建任务代码。*/
#define configUSE_TIMERS                        0				// 1使用   0不使用
#define configTIMER_TASK_PRIORITY               ( 2 )
#define configTIMER_QUEUE_LENGTH                10			// 软件定时器命令队列长度
#define configTIMER_TASK_STACK_DEPTH            ( configMINIMAL_STACK_SIZE * 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xQueueGetMutexHolder            1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_eTaskGetState                   1

/*
 * Cortex-M内核使用8bit来配置优先级，但是STM32只使用了高4bit，数值越小，优先级越高。
 * 在往寄存器里面写数值配置的时候，是按照8bit来写的，所以真正写的时候需要经过转换，公式为：
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)，其中的priority就是我们配置的真正的优先级
 */
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
 /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS                        __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS                        4        /* 15 priority levels */		// 4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority" function. */
/*============================================== SysTick中断优先级配置 ============================================*/
/*SysTick的优先级我们一般配置为最低，即0xf 。这样可以提高系统的实时响应能力，即其他的外部中断可以及时的得到响应。*/
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY     0xf				// 0xf

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
/*===========================================可屏蔽的中断优先级配置====================================================*/
/*用于配置STM32的特殊寄存器basepri寄存器的值，用于屏蔽中断，当大于basepri值的优先级的中断将被全部屏蔽。basepri只有4bit有效，
 * 默认只为0，即全部中断都没有被屏蔽。configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY配置为：5，意思就是中断优先级大于5的中断都被屏蔽。
 * 当把配置好的优先级写到寄存器的时候，是按照8bit来写的，所以真正写的时候需要经过转换，公式为：
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)，其中的priority就是我们配置的真正的优先级。经过这个公式之后得到的是下面的这个宏：
 * configMAX_SYSCALL_INTERRUPT_PRIORITY
 * 在FreeRTOS中，关中断是通过配置basepri寄存器来实现的，关掉的中断由配置的basepri的值决定，小于basepri值的
 * 中断FreeRTOS是关不掉的，这样做的好处是可以系统设计者可以人为的控制那些非常重要的中断不能被关闭，在紧要的关头必须被响应。
 * 而在UCOS中，关中断是通过控制PRIMASK来实现的，PRIMASK是一个单1的二进制位，写1则除能除了NMI和硬 fault的所有中断。当UCOS关闭
 * 中断之后，即使是你在系统中设计的非常紧急的中断来了都不能马上响应，这加大了中断延迟的时间，如果是性命攸关的场合，那后果估计挺严重。
 * 相比UCOS的关中断的设计，FreeRTOS的设计则显得人性化很多。*/
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  5			// 5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY     ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
   standard names. */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler

/* IMPORTANT: This define MUST be commented when used with STM32Cube firmware,
              to prevent overwriting SysTick_Handler defined within STM32Cube HAL */
/* #define xPortSysTickHandler SysTick_Handler */

#endif /* FREERTOS_CONFIG_H */

