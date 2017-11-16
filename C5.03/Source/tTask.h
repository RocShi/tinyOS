#ifndef TTASK_H
#define TTASK_H

#include "stdint.h"
#include "tLib.h"

#define TINYOS_TASK_STATUS_RDY            		0			//Task is not delayed and ready
#define TINYOS_TASK_STATUS_DESTROYED			(1<<1)		//Task is destroyed 
#define TINYOS_TASK_STATUS_DELAYED				(1<<2)		//Task is delayed 
#define TINYOS_TASK_STATUS_SUSPENDED			(1<<3)		//Task is suspended 
#define TINYOS_TASK_WAIT_MASK					(0xFF<<16)	//Task is suspended 

struct _tEventControlBlock;									//Forward declaration of ECB

typedef uint32_t tTaskStack;								//Task stack data type
typedef struct _tTask										//Task structure data type
{
	uint32_t *stack;										//Task stack pointer
	tNode linkNode;								//Through this node, the task can be inserted to the taskTable
	uint32_t delayTicks;						//Task delay counter
	tNode delayNode;							//Through this node, the task can be inserted to the task delayed list 
	uint32_t prio;								//Task priority
	uint32_t state;								//Present some states of the task
	uint32_t slice;								//Time slice counter
	uint32_t suspendCount;						//Task suspended conunter
	
	void (*clean)(void *param);				//Callback function pointer
	void *cleanParam;							//Callback function parameter
	uint8_t requestDeleteFlag;					//The flag of requesting to delete task
	
	struct _tEventControlBlock *waitEvent;
	void *eventMsg;
	uint32_t waitEventResult;
	
}tTask;

typedef struct _tTaskInfo						//The structure of getting a task's information
{
	uint32_t delayTicks;						//Task delay counter
	uint32_t prio;								//Task priority
	uint32_t state;								//Present the delay state of the task
	uint32_t slice;								//Time slice counter
	uint32_t suspendCount;						//Task suspended conunter	
}tTaskInfo;

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
