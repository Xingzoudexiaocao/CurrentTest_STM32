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
//  ���Ӻ�������Ҫ���������ں�������չ���û����Ը����Լ�����Ҫ�����������صĲ��Ժ�����
#define configUSE_PREEMPTION                    1		// 1ʹ����ռʽ��������0ʹ�ܺ���ʽ��������
#define configUSE_IDLE_HOOK                     0		// 1ʹ�ܿ�������Ĺ��Ӻ���  0���ܿ��������Ӻ���
#define configUSE_TICK_HOOK                     0		// 1ʹ�ܵδ�ʱ���ж�����ִ�еĹ��Ӻ���  0����
#define configCPU_CLOCK_HZ                      ( SystemCoreClock )			// �˲������ڶ���CPU����Ƶ����λHz��
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )	// �˲������ڶ���ϵͳʱ�ӽ���������λHz��һ��ȡ1000Hz���ɡ����ߵĵ�ϵͳʱ�ӽ��Ľ�ʹ��FreeRTOS�ں�����ռ�ù����ʱ�䣬����ϵͳ���ɡ�
#define configMAX_PRIORITIES                    ( 7 )	//�˲������ڶ���ɹ��û�ʹ�õ�������ȼ������������������5����ô�û�����ʹ�õ����ȼ�����0,1,2,3,4��������5��������һ�㣬��ѧ��Ҫ�ر��ע�⡣
#define configMINIMAL_STACK_SIZE                ( ( uint16_t ) 128 )	// �˲������ڶ�����������ջ�ռ��С����λ�֣���4�ֽڡ�
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 5 * 1024 ) )	// ����Ѵ�С��FreeRTOS�ںˣ��û���̬�ڴ����룬����ջ�����񴴽����ź�����������Ϣ���д����ȶ���Ҫ������ռ䡣
#define configMAX_TASK_NAME_LEN                 ( 16 )		// ���������������ַ�����ĩβ�Ľ����� '\0'ҲҪ�������ڡ�
#define configUSE_TRACE_FACILITY                1		// ���ó�1��ʾ�������ӻ����ٵ��ԣ��ἤ��һЩ���ӵĽṹ���Ա�ͺ�����
/*
configUSE_16_BIT_TICKS ϵͳʱ�ӽ��ļ���ʹ��TickType_t�������Ͷ���ġ�
����û�ʹ���˺궨�� configUSE_16_BIT_TICKS����ôTickType_t����ľ���16λ�޷����������û��ʹ�ܣ���ôTickType_t����ľ���32λ�޷�������
����32λ�ܹ��Ĵ�������һ��Ҫ��ֹ�˺궨�壬�����ô˺궨����ֵΪ0���ɡ���16λ�޷�����������Ҫ����8λ��16λ�ܹ��Ĵ�������*/
#define configUSE_16_BIT_TICKS                  0
/*
configIDLE_SHOULD_YIELD �˲�������ʹ�����������ͬ���ȼ�������ֻ������������������ʱ���˲�������Ч����
1.     ʹ����ռʽ��������
2.     �д������������ͬ���ȼ�������
����Ϊ1���Ϳ���ʹ�ܴ������ˣ�ʵ��Ӧ���в������û�ʹ�ô˹��ܣ���������Ϊ0���ɡ�*/
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_MUTEXES                       1			// 1ʹ�ܻ����ź�����0���ܻ����ź�����
#define configQUEUE_REGISTRY_SIZE               8
/*
FreeRTOS��ջ������֧�����ַ�����Ϊ�˷����������������ｫ���֮Ϊ����һ�ͷ�������
����Ϊ2ջ������ʹ�÷�������
����Ϊ1ջ������ʹ�÷���һ��
����Ϊ0��ֹջ�����⡣*/
#define configCHECK_FOR_STACK_OVERFLOW          0
#define configUSE_RECURSIVE_MUTEXES             1			// 1ʹ�ܵݹ黥���ź���   0���ܵݹ黥���ź���
/*
configUSE_MALLOC_FAILED_HOOK �����������ź���������Ϣ����ʱ��FreeRTOSͨ������pvPortMalloc()���붯̬�ڴ档
����Ϊ1ʹ�ܶ�̬�ڴ�����ʧ��ʱ�Ĺ��Ӻ���
����Ϊ0���ܶ�̬�ڴ�����ʧ��ʱ�Ĺ��Ӻ���*/
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           1			// 1ʹ�ܼ����ź��� 0���ܼ����ź���
#define configGENERATE_RUN_TIME_STATS           0			// 1ʹ����������״̬����ͳ�ơ� 0����

#define configENABLE_BACKWARD_COMPATIBILITY 0

/* Co-routine definitions. */
/*CO_ROUTINES������÷��룬���϶���Эͬ���򣬻��ߺ����������ΪЭͬһ��ʹ�õĳ���
Э�̣���Co-routines����Ҫ������Դ���ǳ����޵�Ƕ��ʽϵͳ��RAM�ǳ��٣���ͨ����������32λ΢��������*/
#define configUSE_CO_ROUTINES                   0				// 1ʹ��   0��ʹ��
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )		// CO_ROUTINE���ȼ�

/* Software timer definitions. */
/*TIMER����ʱ������RTOS�е�TIMER���������ʱ��FreeRTOS�Ķ�ʱ�����Ȳ��ߣ������Ŷ�ʱ�����Ӷ��ı䣬�ر���TIMER�������ȼ��ϵͣ������ȼ�ռ����Դ������¡�
��Ҫʹ�ø߾��ȵĶ�ʱ���������ʹ��Ӳ���Ķ�ʱ�������ڴ�����һ�㶼�ж��Ӳ��TIMER����
�����ʱ����ʵҲ����Ҫ����һ�����񣬴�����ʽ�����ǳ����һ����ֻ��������ϵͳ�ں���ɣ����������Լ�д����������롣*/
#define configUSE_TIMERS                        0				// 1ʹ��   0��ʹ��
#define configTIMER_TASK_PRIORITY               ( 2 )
#define configTIMER_QUEUE_LENGTH                10			// �����ʱ��������г���
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
 * Cortex-M�ں�ʹ��8bit���������ȼ�������STM32ֻʹ���˸�4bit����ֵԽС�����ȼ�Խ�ߡ�
 * �����Ĵ�������д��ֵ���õ�ʱ���ǰ���8bit��д�ģ���������д��ʱ����Ҫ����ת������ʽΪ��
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)�����е�priority�����������õ����������ȼ�
 */
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
 /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS                        __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS                        4        /* 15 priority levels */		// 4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority" function. */
/*============================================== SysTick�ж����ȼ����� ============================================*/
/*SysTick�����ȼ�����һ������Ϊ��ͣ���0xf �������������ϵͳ��ʵʱ��Ӧ���������������ⲿ�жϿ��Լ�ʱ�ĵõ���Ӧ��*/
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY     0xf				// 0xf

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
/*===========================================�����ε��ж����ȼ�����====================================================*/
/*��������STM32������Ĵ���basepri�Ĵ�����ֵ�����������жϣ�������basepriֵ�����ȼ����жϽ���ȫ�����Ρ�basepriֻ��4bit��Ч��
 * Ĭ��ֻΪ0����ȫ���ж϶�û�б����Ρ�configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY����Ϊ��5����˼�����ж����ȼ�����5���ж϶������Ρ�
 * �������úõ����ȼ�д���Ĵ�����ʱ���ǰ���8bit��д�ģ���������д��ʱ����Ҫ����ת������ʽΪ��
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)�����е�priority�����������õ����������ȼ������������ʽ֮��õ��������������꣺
 * configMAX_SYSCALL_INTERRUPT_PRIORITY
 * ��FreeRTOS�У����ж���ͨ������basepri�Ĵ�����ʵ�ֵģ��ص����ж������õ�basepri��ֵ������С��basepriֵ��
 * �ж�FreeRTOS�ǹز����ģ��������ĺô��ǿ���ϵͳ����߿�����Ϊ�Ŀ�����Щ�ǳ���Ҫ���жϲ��ܱ��رգ��ڽ�Ҫ�Ĺ�ͷ���뱻��Ӧ��
 * ����UCOS�У����ж���ͨ������PRIMASK��ʵ�ֵģ�PRIMASK��һ����1�Ķ�����λ��д1����ܳ���NMI��Ӳ fault�������жϡ���UCOS�ر�
 * �ж�֮�󣬼�ʹ������ϵͳ����Ƶķǳ��������ж����˶�����������Ӧ����Ӵ����ж��ӳٵ�ʱ�䣬������������صĳ��ϣ��Ǻ������ͦ���ء�
 * ���UCOS�Ĺ��жϵ���ƣ�FreeRTOS��������Ե����Ի��ܶࡣ*/
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

