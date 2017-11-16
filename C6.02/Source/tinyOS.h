#ifndef TINYOS_H
#define TINYOS_H

#include <stdint.h>								//The official data type defination head file
#include "tLib.h"									//The head file of the tinyOS Library files
#include "tConfig.h"								//The head file of the tinyOS's priority configuration file
#include "tTask.h"
#include "tEvent.h"								//The head file about Event Control Block
#include "tSem.h"

#define NVIC_INIT_CTRL				0xE000ED04		//The address of NVIC_INIT_CTRL register
#define NVIC_INIT_PENDSVSET			0x10000000		//The value to be written to the NVIC_INIT_CTRL

#define NVIC_INIT_SYSPRI2			0xE000ED22		//The address of NVIC_INIT_SYSPRI2 register
#define NVIC_INIT_PENDSV_PRI		0x000000FF		//The value to be written to the NVIC_INIT_PENDSV_PRI

#define MEM8(addr)		*(volatile unsigned char *)addr
#define MEM32(addr)		*(volatile unsigned long *)addr

typedef  enum _tTaskError						//Errors of tinyOS
{
	tErrorNoError=0,
	tErrorTimeout,
	tErrorResourceUnavailable,
}tTaskError;

extern tTask *currentTask;						//Current task's structure pointer
extern tTask *nextTask;							//Next task's structure pointer
extern tTask *idleTask;							//Idle task's structure pointer
extern tList taskTable[TINYOS_PRO_COUNT];		//Task list
extern uint8_t schedLockCount;							//Schedule lock counter
extern tBitmap taskPrioBitmap;							//The bitmap of the tasks' priorities
extern tList tTaskDelayedList;							//Task delayed list

void tSetSysTickPeriod(uint32_t ms);					//SysTick Timer initial function
void tTaskRunFirst(void);								//This function'll be called when the tinyOS ran at the first time
void tTaskSched(void);									//Task schedule function
void tTaskSwitch(void);								//Task switch function
uint32_t tTaskEnterCritical(void);						//Enter critical section
void tTaskExitCritical(uint32_t status);				//Exit critical section
void tTaskSchedInit(void);								//Task schedule initial function
void tTaskSchedDisable(void);							//Task schedule disable function(LOCK)
void tTaskSchedEnable(void);							//Task schedule enable function(UNLOCK)
tTask * tTaskHighestReady(void);						//Return the structure pointer of the task whose priority is the highest 
void tTaskSchedRdy(tTask *task);						//Put the task into the taskTable and set the corresponding bit of taskPrioBitmap
void tTaskSchedUnRdy(tTask *task);						//Remove the task from the taskTable and clear the corresponding taskPrioBitmap
void tTimeTaskWait(tTask *task, uint32_t ms);						//Task wait function
void tTimeTaskWakeUp(tTask *task);									//Task wake up function
void tTaskDelayInit(void);																				//Task delayed list initial function
void tTaskDelay(uint32_t ms);																			//Task delay function
void tTaskInit(tTask *task, void(*entry)(void *), void *param,
								uint32_t prio, tTaskStack *stack);	//Task initial function

#endif
