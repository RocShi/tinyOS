#include "tinyOS.h"

void tSemInit(tSem *sem, uint32_t startCount, uint32_t maxCount)	//The Counting Semaphores initial function
{
	tEventInit(&(sem->ECB), tEventTypeSem);
	
	sem->maxCount=maxCount;
	if(maxCount==0)
	{
		sem->Count=startCount;
	}
	else
	{
		sem->Count=(startCount>maxCount) ? maxCount:startCount;
	}
}

uint32_t tSemWait(tSem *sem, uint32_t timeout)		//The Counting Semaphores wait function
{
	uint32_t status=tTaskEnterCritical();
	
	if(sem->Count>0)
	{
		sem->Count--;
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	else
	{
		tEventWait(&(sem->ECB), currentTask, (void *)0, tEventTypeSem, timeout);
		tTaskExitCritical(status);
		
		tTaskSched();
		return currentTask->waitEventResult;
	}
}

uint32_t tSemNoWaitGet(tSem *sem)				//Get the status of the Counting Semaphores
{
	uint32_t status=tTaskEnterCritical();
	
	if(sem->Count>0)
	{
		sem->Count--;
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	else
	{
		tTaskExitCritical(status);
		return tErrorResourceUnavailable;
	}
}

void tSemNotify(tSem *sem)						//The Counting Semaphores notify function
{
	tTask *task;
	uint32_t status=tTaskEnterCritical();
	
	if(tEventWaitCount(&(sem->ECB))>0)
	{
		task=tEventWakeUp(&(sem->ECB), (void *)0, tErrorNoError);
		if(task->prio<currentTask->prio)
		{
			tTaskSched();
		}
	}
	else
	{
		sem->Count++;
		if((sem->maxCount)!=0 && (sem->Count)>(sem->maxCount))
		{
			sem->Count=sem->maxCount;
		}
	}
	tTaskExitCritical(status);
}
