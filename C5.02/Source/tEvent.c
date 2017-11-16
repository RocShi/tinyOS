#include "tinyOS.h"

void tEventInit(tEventControlBlock *ECB, tEventType eventtype)	//The Event Control Block initial function
{
	ECB->type=eventtype;
	tListInit(&(ECB->waitList));
}

void tEventWait(tEventControlBlock *ECB, tTask *task, 
				void *msg, uint32_t state, uint32_t timeout)	//The function of waiting event
{
	uint32_t status=tTaskEnterCritical();
	
	task->waitEvent=ECB;
	task->eventMsg=msg;
	task->state |= state;
	task->waitEventResult=tTaskNoError;
	
	tTaskSchedUnRdy(task);
	tListAddLast(&(ECB->waitList),&(task->linkNode));			//Insert the task into the wautList of ECB
	
	if(timeout!=0)
	{
		tTimeTaskWait(task, timeout);
	}
	
	tTaskExitCritical(status);
}

tTask * tEventWakeUp(tEventControlBlock *ECB, 
					 void *msg, uint32_t result)			//The function of waking the ECB up
{
	tNode *node=(tNode *)0;
	tTask *task=(tTask *)0;
	
	uint32_t status=tTaskEnterCritical();
	
	if((node=tListRemoveFirst(&(ECB->waitList))) != (tNode*)0)
	{
		task=tNodeParent(node, tTask, linkNode);
		task->waitEvent=(tEventControlBlock *)0;
		task->eventMsg=msg;
		task->state &= ~TINYOS_TASK_WAIT_MASK;
		task->waitEventResult=result;
	
		if(task->delayTicks!=0)
		{
			tTimeTaskWakeUp(task);
		}
		
		tTaskSchedRdy(task);
	}
	
	tTaskExitCritical(status);
	return task;
}

void tEventRemoveTask(tTask *task, void *msg, uint32_t result)		//The function of remove the task from the waitList of its' ECB
{
	uint32_t status=tTaskEnterCritical();
	
	tListRemoveNode(&(task->waitEvent->waitList), &(task->linkNode));
	
	task->waitEvent=(tEventControlBlock *)0;
	task->eventMsg=msg;
	task->state &= ~TINYOS_TASK_WAIT_MASK;
	task->waitEventResult=result;
	
	tTaskExitCritical(status);
}
