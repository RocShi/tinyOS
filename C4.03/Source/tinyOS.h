#ifndef TINYOS_H
#define TINYOS_H

#include <stdint.h>								//The official data type defination head file
#include "tLib.h"									//The head file of the tinyOS Library files
#include "tConfig.h"								//The head file of the tinyOS's priority configuration file

#define NVIC_INIT_CTRL				0xE000ED04		//The address of NVIC_INIT_CTRL register
#define NVIC_INIT_PENDSVSET			0x10000000		//The value to be written to the NVIC_INIT_CTRL

#define NVIC_INIT_SYSPRI2			0xE000ED22		//The address of NVIC_INIT_SYSPRI2 register
#define NVIC_INIT_PENDSV_PRI		0x000000FF		//The value to be written to the NVIC_INIT_PENDSV_PRI

#define MEM8(addr)		*(volatile unsigned char *)addr
#define MEM32(addr)		*(volatile unsigned long *)addr
	
#define TINYOS_TASK_STATUS_RDY            0					//Task is not delayed and ready
#define TINYOS_TASK_STATUS_DESTROYED			(1<<1)		//Task is destroyed 
#define TINYOS_TASK_STATUS_DELAYED				(1<<2)		//Task is delayed 
#define TINYOS_TASK_STATUS_SUSPENDED			(1<<3)		//Task is suspended 

typedef uint32_t tTaskStack;								//Task stack data type
typedef struct _tTask										//Task structure data type
{
	uint32_t *stack;										//Task stack pointer
	tNode linkNode;								//Through this node, the task can be inserted to the taskTable
	uint32_t delayTicks;						//Task delay counter
	tNode delayNode;							//Through this node, the task can be inserted to the task delayed list 
	uint32_t prio;								//Task priority
	uint32_t state;								//Present the delay state of the task
	uint32_t slice;								//Time slice counter
	uint32_t suspendCount;						//Task suspended conunter
	
	void (*clean)(void *param);				//Callback function pointer
	void *cleanParam;							//Callback function parameter
	uint8_t requestDeleteFlag;					//The flag of requesting to delete task
	
}tTask;

typedef struct _tTaskInfo						//The structure of getting a task's information
{
	uint32_t delayTicks;						//Task delay counter
	uint32_t prio;								//Task priority
	uint32_t state;								//Present the delay state of the task
	uint32_t slice;								//Time slice counter
	uint32_t suspendCount;						//Task suspended conunter	
}tTaskInfo;

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

void tAppInit(void);												//Application initial function
void tTaskSuspend(tTask *task);										//To suspend the task
void tTaskWakeUp(tTask *task);										//To wake up the task

void tTaskSetCleanCallFunc(tTask *task, void (*clean)(void *param), 
							void *param);							//Callback function
void tTaskForceDelete(tTask *task);			//Force to delete the task
void tTaskRequestDelete(tTask *task);		//Request to delete the task
uint8_t tTaskIsRequestDeleted(void);		//Judge whether the task is requested to be deleted

void tTimeTaskRemove(tTask *task);			//Remove the task from the tTaskDelayedList
void tTaskSchedRemove(tTask *task);			//Remove the task from the taskTable and clear the corresponding taskPrioBitmap
void tTaskDeleteSelf(void);				//Delete the task itself

void tTaskGetInfo(tTask *task, tTaskInfo *taskinfo);		//Get a task's information

#endif
