#include "tinyOS.h"

void tTaskInit(tTask *task, void(*entry)(void *), 
			   void *param, uint32_t prio, tTaskStack *stack)			//Task initial function
{
	//These data are pushed or popped using PSP pointer by the hardware automaticly
	*(--stack)=(unsigned long)(1<<24);		//xPSR
	*(--stack)=(unsigned long)entry;			//PC(R15)
	*(--stack)=(unsigned long)0x50C;			//R14
	*(--stack)=(unsigned long)0x12;			//R12
	*(--stack)=(unsigned long)0x3;				//R3
	*(--stack)=(unsigned long)0x2;				//R2
	*(--stack)=(unsigned long)0x1;				//R1
	*(--stack)=(unsigned long)param;			//param(R0)
	
	//These data are pushed or popped using task structure stack pointer by user manually
	*(--stack)=(unsigned long)0x11;			//R11
	*(--stack)=(unsigned long)0x10;			//R10
	*(--stack)=(unsigned long)0x9;				//R9
	*(--stack)=(unsigned long)0x8;				//R8
	*(--stack)=(unsigned long)0x7;				//R7
	*(--stack)=(unsigned long)0x6;				//R6
	*(--stack)=(unsigned long)0x5;				//R5
	*(--stack)=(unsigned long)0x4;				//R4
	
	task->stack=stack;										//Initiate the task stack pointer
	task->delayTicks=0;										//Initiate the task delay counter
	tNodeInit(&(task->delayNode));				//Initiate the task's delay node
	tNodeInit(&(task->linkNode));					//Initiate the task's link node
	task->prio=prio;											//Initiate the task priority
	task->state=TINYOS_TASK_STATUS_RDY;		//Initiate the task delay state: ready
	task->slice=TINYOS_SLICE_MAX;
	task->suspendCount=0;
	
	task->clean=(void(*)(void *))0; 		//Initiate the callback function pointer
	task->cleanParam=(void *)0;						//Initiate the callback function parameter
	task->requestDeleteFlag=0;						//Initiate the the flag of requesting to delete task
	
	tListAddFirst(&taskTable[prio], &(task->linkNode));

	tBitmapSet(&taskPrioBitmap,prio);
}

void tTaskSuspend(tTask *task)					//To suspend the task
{
	uint32_t status=tTaskEnterCritical();
	
	if(!(task->state & TINYOS_TASK_STATUS_DELAYED))			//The task isn't delayed
	{
		if(++task->suspendCount<=1)												//This is the first time to suspend the task
		{
			task->state |= TINYOS_TASK_STATUS_SUSPENDED;
			tTaskSchedUnRdy(task);
		}
		if(task==currentTask)
		{
			tTaskSched();
		}
	}
	
	tTaskExitCritical(status);
}

void tTaskWakeUp(tTask *task)						//To wake up the task
{
	uint32_t status=tTaskEnterCritical();
	
	if(task->state & TINYOS_TASK_STATUS_SUSPENDED)			//The task is suspended
	{
		if(--task->suspendCount==0)
		{
			task->state &= ~TINYOS_TASK_STATUS_SUSPENDED;
			tTaskSchedRdy(task);
			tTaskSched();
		}
	}
	
	tTaskExitCritical(status);
}

void tTaskSetCleanCallFunc(tTask *task, void (*clean)(void *param), void *param)		//Callback function
{
	task->clean=clean;
	task->cleanParam=param;
}

void tTaskForceDelete(tTask *task)			//Force to delete the task
{
	uint32_t status=tTaskEnterCritical();
	
	if(task->state & TINYOS_TASK_STATUS_DELAYED)
	{
		tTimeTaskRemove(task);
	}
	else if(!(task->state & TINYOS_TASK_STATUS_SUSPENDED))
	{
		tTaskSchedRemove(task);
	}
	if(task->clean)
	{
		task->clean(task->cleanParam);
	}
	if(currentTask==task)
	{
		tTaskSched();
	}
	
	tTaskExitCritical(status);
}

void tTaskRequestDelete(tTask *task)		//Request to delete the task
{
	uint32_t status=tTaskEnterCritical();
	
	task->requestDeleteFlag=1;
	
	tTaskExitCritical(status);
}

uint8_t tTaskIsRequestDeleted(void)			//Judge whether the task itself is requested to be deleted
{
	uint8_t deleteFlag;
	
	uint32_t status=tTaskEnterCritical();
	
	deleteFlag=currentTask->requestDeleteFlag;
	
	tTaskExitCritical(status);
	
	return deleteFlag;
}

void tTaskDeleteSelf(void)			//Delete the task itself
{
	uint32_t status=tTaskEnterCritical();
	
	tTaskSchedRemove(currentTask);
	if(currentTask->clean)
	{
		currentTask->clean(currentTask->cleanParam);
	}
	tTaskSched();
	
	tTaskExitCritical(status);
}

void tTaskGetInfo(tTask *task, tTaskInfo *taskinfo)		//Get a task's information
{
	taskinfo->delayTicks=task->delayTicks;					//Task delay counter
	taskinfo->prio=task->prio;									//Task priority
	taskinfo->state=task->state;									//Present the delay state of the task
	taskinfo->slice=task->slice;									//Time slice counter
	taskinfo->suspendCount=task->suspendCount;							//Task suspended conunter
}
