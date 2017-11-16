#include "tinyOS.h"
#include "ARMCM3.h"

void tSetSysTickPeriod(uint32_t ms)													//SysTick Timer initial function
{
	SysTick->LOAD=ms*SystemCoreClock/1000-1;									//Set the reload register
	NVIC_SetPriority(SysTick_IRQn,1<<(__NVIC_PRIO_BITS-1));		//The priority of "-" is higher than that of "<<"
	SysTick->VAL=0;
	SysTick->CTRL=SysTick_CTRL_CLKSOURCE_Msk |
				  SysTick_CTRL_TICKINT_Msk |
				  SysTick_CTRL_ENABLE_Msk;													//Enable SysTick IRQ and SysTick Time
}

void SysTick_Handler (void)																//SysTick interrupt service function
{
	tNode *node;
	tTask *task;
	uint32_t status=tTaskEnterCritical();
	
	for(node=tTaskDelayedList.firstnode; node!= &(tTaskDelayedList.headNode); node=node->nextNode)
	{
		task=tNodeParent(node, tTask, delayNode);
		if(--(task->delayTicks)==0)
		{
			tTimeTaskWakeUp(task);
			tTaskSchedRdy(task);
		}
	}
	
	if(--(currentTask->slice)==0)			//Time slice round-robin
	{
		if(tListCount(&taskTable[currentTask->prio])>0)
		{
			tListRemoveFirst(&taskTable[currentTask->prio]);
			tListAddLast(&taskTable[currentTask->prio], &(currentTask->linkNode));
			currentTask->slice=TINYOS_SLICE_MAX;
		}
	}

	tTaskExitCritical(status);
	
	tTaskSched();											//Schedule the task
}
