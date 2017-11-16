#include "tinyOS.h"
#include "ARMCM3.h"

tTask *currentTask;		//Current task's structure pointer
tTask *nextTask;			//Next task's structure pointer
tTask *idleTask;			//Idle task's structure pointer
tTask *taskTable[2];	//Task list

void tTaskDelay(uint32_t ms);			//Task delay function

void tTaskInit(tTask *task, void(*entry)(void *), void *param, tTaskStack *stack);	//Task initial function
void task1Entry(void *param);					//Task1 function
void task2Entry(void *param);					//Task2 function
void idleTaskEntry(void *param);				//Idle task function
void tSetSysTickPeriod(uint32_t ms);		//SysTick timer initial function

int task1Flag;	//Task1 flag
int task2Flag;	//Task2 flag

tTask tTask1;									//Task1 structure
tTask tTask2;									//Task2 structure
tTask tTaskIdle;							//Idle task structure
tTaskStack task1Env[1024];		//Task1 stack
tTaskStack task2Env[1024];		//Task2 stack
tTaskStack idleTaskEnv[1024];	//Idle task stack

int main(void)
{
	tTaskInit(&tTask1, task1Entry, (void *)0x11111111, &task1Env[1024]);  //Task1 initiate
	tTaskInit(&tTask2, task2Entry, (void *)0x22222222, &task2Env[1024]);  //Task2 initiate
	tTaskInit(&tTaskIdle, idleTaskEntry, (void *)0, &idleTaskEnv[1024]);  //Idle task initiate
	
	taskTable[0]=&tTask1;
	taskTable[1]=&tTask2;
	idleTask=&tTaskIdle;
	
	nextTask=taskTable[0];	//The first task to be ran is task1
	tTaskRunFirst();
}


void tTaskDelay(uint32_t ms)		//Task delay function, the parameter ms must be integral multiple of 10 
{
	currentTask->delayTicks=ms*0.1;
	tTaskSched();
}

void tTaskInit(tTask *task, void(*entry)(void *), void *param, tTaskStack *stack)	//Task initial function
{
	//These data are pushed or popped using PSP pointer by the hardware automaticly
	*(--stack)=(unsigned long)(1<<24);		//xPSR
	*(--stack)=(unsigned long)entry;			//PC(R15)
	*(--stack)=(unsigned long)0x14;			//R14
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
	
	task->stack=stack;		//Initiate the task stack pointer
	task->delayTicks=0;		//Initiate the task delay counter
}

void task1Entry(void *param)	//Task1 function
{
	tSetSysTickPeriod(10);			//Timing cycle is 10ms
	for(;;)
	{
		task1Flag=1;
		tTaskDelay(100);
		task1Flag=0;
		tTaskDelay(100);
	}
}

void task2Entry(void *param)	//Task2 function
{
	for(;;)
	{
		task2Flag=1;
		tTaskDelay(10);
		task2Flag=0;
		tTaskDelay(10);
	}
}

void idleTaskEntry(void *param)	//Idele task function
{
	for(;;)
	{
	}
}

void tSetSysTickPeriod(uint32_t ms)		//SysTick Timer initial function
{
	SysTick->LOAD=ms*SystemCoreClock/1000-1;	//Set the reload register
	NVIC_SetPriority(SysTick_IRQn,1<<(__NVIC_PRIO_BITS-1));		//The priority of "-" is higher than that of "<<"
	SysTick->VAL=0;
	SysTick->CTRL=SysTick_CTRL_CLKSOURCE_Msk |
								SysTick_CTRL_TICKINT_Msk |
								SysTick_CTRL_ENABLE_Msk;		//Enable SysTick IRQ and SysTick Time
}

void SysTick_Handler (void)
{
	int i;
	for(i=0; i<2; i++)
	{
		if(taskTable[i]->delayTicks>0)
			taskTable[i]->delayTicks--;
	}
	
	tTaskSched();	//Schedule the task
}
