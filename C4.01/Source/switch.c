#include "tinyOS.h"
#include "ARMCM3.h"

__asm void PendSV_Handler (void)	//PendSV service funtion
{
	IMPORT currentTask
	IMPORT nextTask
	
	MRS R0, PSP
	CBZ R0, PendSVHandler_NoSave
	
	STMDB R0!, {R4-R11}				//Push stack
	
	LDR R1, =currentTask
	LDR R1, [R1]
	STR R0, [R1]							//Update task structure stack pointer
	
PendSVHandler_NoSave
	
	LDR R0, =currentTask
	LDR R1, =nextTask
	LDR R1, [R1]
	STR R1, [R0]
	
	LDR R0, [R1]
	LDMIA R0!, {R4-R11}				//Pop stack
	
	MSR PSP, R0
	ORR LR, LR, #0X04					//Using PSP
	
	BX LR						    			//Exit PendSV
}

void tTaskRunFirst(void)		//This function'll be called when the tinyOS ran at the first time
{
	__set_PSP(0);
	MEM8(NVIC_INIT_SYSPRI2)=NVIC_INIT_PENDSV_PRI;		//Configure the priority of Pend_SVC->the lowest
	MEM32(NVIC_INIT_CTRL)=NVIC_INIT_PENDSVSET;			//Trigger the Pend_SVC
}

void tTaskSched(void)														//Task schedule function
{
	uint32_t status=tTaskEnterCritical();
	tTask *tempTask;
	
	if(schedLockCount>0)														//Task schedule has been locked
	{
		tTaskExitCritical(status);
		return;
	}
	
	tempTask=tTaskHighestReady();
	if(tempTask!=currentTask)
	{
		nextTask=tempTask;
		tTaskSwitch();
	}

	tTaskExitCritical(status);
}

void tTaskSwitch(void)														//Task switch function
{
	MEM32(NVIC_INIT_CTRL)=NVIC_INIT_PENDSVSET;			//Directly trigger PendSV
}

uint32_t tTaskEnterCritical(void)					//Enter critical section
{
	uint32_t status=__get_PRIMASK();				//Get current status of interrupt control register
	__disable_irq();												//Disable interrupt request
	return status;
}

void tTaskExitCritical(uint32_t status)		//Exit critical section
{
	__set_PRIMASK(status);									//Recover previous status of interrupt control register
}

void tTaskSchedInit(void)					//Task schedule initial function
{
	int i;
	schedLockCount=0;
	tBitmapInit(&taskPrioBitmap);

	for(i=0; i<TINYOS_PRO_COUNT; i++)
	{
		tListInit(&taskTable[i]);
	}
}

void tTaskSchedDisable(void)				//Task schedule disable function(LOCK)
{
	uint32_t status=tTaskEnterCritical();
	
	if(schedLockCount<255)
		schedLockCount++;
	
	tTaskExitCritical(status);
}

void tTaskSchedEnable(void)				//Task schedule enable function(UNLOCK)
{
	uint32_t status=tTaskEnterCritical();
	
	if(schedLockCount>0)
	{
		if(--schedLockCount==0)
			tTaskSched();
	}
	
	tTaskExitCritical(status);
}

tTask * tTaskHighestReady(void)			//Return the structure pointer of the task whose priority is the highest 
{
	uint32_t highest=tBitmapGetFirstSet(&taskPrioBitmap);
	return tNodeParent(tListFirst(&taskTable[highest]), tTask, linkNode);
}

void tTaskSchedRdy(tTask *task)			//Put the task into the taskTable and set the corresponding bit of taskPrioBitmap
{
	tListAddFirst(&taskTable[task->prio], &(task->linkNode));
	tBitmapSet(&taskPrioBitmap, task->prio);
}

void tTaskSchedUnRdy(tTask *task)		//Remove the task from the taskTable and clear the corresponding taskPrioBitmap
{
	tListRemoveNode(&taskTable[task->prio], &(task->linkNode));
	
	if(tListCount(&taskTable[task->prio])==0)
	{
		tBitmapClear(&taskPrioBitmap, task->prio);
	}
}

void tTimeTaskWait(tTask *task, uint32_t ms)				//Task wait function
{
	task->delayTicks=ms*0.1;
	tListAddLast(&tTaskDelayedList, &(task->delayNode));
	task->state |= TINYOS_TASK_STATUS_DELAYED;
}

void tTimeTaskWakeUp(tTask *task)										//Task wake up function
{
	tListRemoveNode(&tTaskDelayedList, &(task->delayNode));
	task->state &= ~TINYOS_TASK_STATUS_DELAYED;
}
