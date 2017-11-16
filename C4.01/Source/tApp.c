#include "tinyOS.h"

int task1Flag;												//Task1 flag
int task2Flag;												//Task2 flag
int task3Flag;												//Task3 flag

tTask tTask1;													//Task1 structure
tTask tTask2;													//Task2 structure
tTask tTask3;													//Task2 structure

tTaskStack task1Env[1024];						//Task1 stack
tTaskStack task2Env[1024];						//Task2 stack
tTaskStack task3Env[1024];						//Task3 stack

void task1Entry(void *param)					//Task1 function
{
	tSetSysTickPeriod(10);							//Timing cycle is 10ms	
	for(;;)
	{	
		task1Flag=1;
		tTaskSuspend(currentTask);	
		task1Flag=0;
		tTaskSuspend(currentTask);
	}
}

void task2Entry(void *param)					//Task2 function
{
	for(;;)
	{		
		task2Flag=1;
		tTaskDelay(10);
		tTaskWakeUp(&tTask1);
		task2Flag=0;
		tTaskDelay(10);
		tTaskWakeUp(&tTask1);
	}
}

void task3Entry(void *param)					//Task3 function
{
	for(;;)
	{		
		task3Flag=1;
		tTaskDelay(10);
		task3Flag=0;
		tTaskDelay(10);
	}
}

void tAppInit(void)										//Application initial function
{
	tTaskInit(&tTask1, task1Entry, (void *)0x11111111, 0, &task1Env[1024]);  //Task1 initiate
	tTaskInit(&tTask2, task2Entry, (void *)0x22222222, 1, &task2Env[1024]);  //Task2 initiate
	tTaskInit(&tTask3, task3Entry, (void *)0x33333333, 1, &task3Env[1024]);  //Task3 initiate
}
