#include "tinyOS.h"

tTask *currentTask;		//Current task's structure pointer
tTask *nextTask;			//Next task's structure pointer
tTask *taskTable[2];	//Task list

void delay(int count);			//delay function
void triggerPendSVC(void);	//PendSVC trigger function

void tTaskInit(tTask *task, void(*entry)(void *), void *param, tTaskStack *stack);	//Task initial function
void task1Entry(void *param);	//Task1 function
void task2Entry(void *param);	//Task2 function

int task1Flag;	//Task1 flag
int task2Flag;	//Task2 flag

tTask tTask1;	//Task1 structure
tTask tTask2;	//Task2 structure
tTaskStack task1Env[1014];	//Task1 stack
tTaskStack task2Env[1014];	//Task2 stack

int main(void)
{
	tTaskInit(&tTask1, task1Entry, (void *)0x11111111, &task1Env[1024]);  //Task1 initiate
	tTaskInit(&tTask2, task2Entry, (void *)0x22222222, &task2Env[1024]);  //Task2 initiate
	
	taskTable[0]=&tTask1;
	taskTable[1]=&tTask2;
	
	nextTask=taskTable[0];	//The first task to be ran is task1
	tTaskRunFirst();
}

void delay(int count)    //delay function
{
	while(--count>0);
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
	
	task->stack=stack;	//Initiate the task stack pointer
}

void task1Entry(void *param)	//Task1 function
{
	for(;;)
	{
		task1Flag=1;
		delay(100);
		task1Flag=0;
		delay(100);
		
		tTaskSched();	//Schedule the task
	}
}

void task2Entry(void *param)	//Task2 function
{
	for(;;)
	{
		task2Flag=1;
		delay(100);
		task2Flag=0;
		delay(100);
		
		tTaskSched();	//Schedule the task
	}
}


