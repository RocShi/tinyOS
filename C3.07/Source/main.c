#include "tinyOS.h"
#include "ARMCM3.h"

int task1Flag;												//Task1 flag
int task2Flag;												//Task2 flag
int task3Flag;												//Task3 flag

tTask *currentTask;										//Current task's structure pointer
tTask *nextTask;											//Next task's structure pointer
tTask *idleTask;											//Idle task's structure pointer
tList taskTable[TINYOS_PRO_COUNT];		//Task list
tTask tTask1;													//Task1 structure
tTask tTask2;													//Task2 structure
tTask tTask3;													//Task2 structure
tTask tTaskIdle;											//Idle task structure
tTaskStack task1Env[1024];						//Task1 stack
tTaskStack task2Env[1024];						//Task2 stack
tTaskStack task3Env[1024];						//Task3 stack
tTaskStack idleTaskEnv[1024];					//Idle task stack

uint8_t schedLockCount;								//Schedule lock counter
tBitmap taskPrioBitmap;								//The bitmap of the tasks' priorities
tList tTaskDelayedList;								//Task delayed list

void tTaskDelayInit(void);																				//Task delayed list initial function
void tTaskDelay(uint32_t ms);																			//Task delay function1
void delay(void);																								//Task delay function2
void tTaskInit(tTask *task, void(*entry)(void *), void *param,
								uint32_t prio, tTaskStack *stack);								//Task initial function
void task1Entry(void *param);																		//Task1 function
void task2Entry(void *param);																		//Task2 function
void task3Entry(void *param);																		//Task3 function

void idleTaskEntry(void *param);																	//Idle task function
void tSetSysTickPeriod(uint32_t ms);															//SysTick timer initial function

int main(void)
{
	tTaskSchedInit();
	tTaskDelayInit();																												  //Task delayed list initiate
	
	tTaskInit(&tTask1, task1Entry, (void *)0x11111111, 0, &task1Env[1024]);  //Task1 initiate
	tTaskInit(&tTask2, task2Entry, (void *)0x22222222, 1, &task2Env[1024]);  //Task2 initiate
	tTaskInit(&tTask3, task3Entry, (void *)0x33333333, 1, &task3Env[1024]);  //Task3 initiate
	tTaskInit(&tTaskIdle, idleTaskEntry, (void *)0, TINYOS_PRO_COUNT-1, 
						&idleTaskEnv[1024]);						//Idle task initiate
	
	idleTask=&tTaskIdle;
	
	nextTask=tTaskHighestReady();							//The first task to be ran has the highest priority
	tTaskRunFirst();
}

void tTaskDelayInit(void)									//Task delayed list initial function
{
	tListInit(&tTaskDelayedList);
}

void tTaskDelay(uint32_t ms)								//Task delay function, the parameter ms must be integral multiple of 10 
{
	uint32_t status=tTaskEnterCritical();
	
	tTimeTaskWait(currentTask, ms);
	
	tTaskSchedUnRdy(currentTask);
	
	tTaskExitCritical(status);
	
	tTaskSched();
}

void delay(void)														//Task delay function2
{
	int i;
	for(i=0; i<0xFF; i++){}
}

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
	
	tListAddFirst(&taskTable[prio], &(task->linkNode));

	tBitmapSet(&taskPrioBitmap,prio);
}

void task1Entry(void *param)						//Task1 function
{
	tSetSysTickPeriod(10);								//Timing cycle is 10ms	
	for(;;)
	{	
		task1Flag=1;
		tTaskDelay(10);	
		task1Flag=0;
		tTaskDelay(10);
	}
}

void task2Entry(void *param)						//Task2 function
{
	for(;;)
	{		
		task2Flag=1;
		delay();
		task2Flag=0;
		delay();
	}
}

void task3Entry(void *param)						//Task3 function
{
	for(;;)
	{		
		task3Flag=1;
		delay();
		task3Flag=0;
		delay();
	}
}

void idleTaskEntry(void *param)				//Idele task function
{
	for(;;)
	{
	}
}

void tSetSysTickPeriod(uint32_t ms)			//SysTick Timer initial function
{
	SysTick->LOAD=ms*SystemCoreClock/1000-1;									//Set the reload register
	NVIC_SetPriority(SysTick_IRQn,1<<(__NVIC_PRIO_BITS-1));		//The priority of "-" is higher than that of "<<"
	SysTick->VAL=0;
	SysTick->CTRL=SysTick_CTRL_CLKSOURCE_Msk |
				  SysTick_CTRL_TICKINT_Msk |
				  SysTick_CTRL_ENABLE_Msk;													//Enable SysTick IRQ and SysTick Time
}

void SysTick_Handler (void)
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
