#include "tinyOS.h"

#define NVIC_INIT_CTRL				0xE000ED04	//The address of NVIC_INIT_CTRL register
#define NVIC_INIT_PENDSVSET		0x10000000	//The value to be written to the NVIC_INIT_CTRL

#define NVIC_INIT_SYSPRI2			0xE000ED22	//The address of NVIC_INIT_SYSPRI2 register
#define NVIC_INIT_PENDSV_PRI	0x000000FF	//The value to be written to the NVIC_INIT_PENDSV_PRI

#define MEM8(addr)		*(volatile unsigned char *)addr
#define MEM32(addr)		*(volatile unsigned long *)addr

void delay(int count);			//delay function
void triggerPendSVC(void);	//PendSVC trigger function

void tTaskInit(tTask *task, void(*entry)(void *), void *param, tTaskStack *stack);	//Task initial function
void task1(void *param);	//Task1 function
void task2(void *param);	//Task2 function

typedef struct _BlockType_t
{
	unsigned long * stackPtr;
} BlockType_t;

int flag;
unsigned long stackBuffer[1024];
BlockType_t * blockPtr;
BlockType_t block;

tTask tTask1;	//Task1 structure
tTask tTask2;	//Task2 structure
tTaskStack task1Env[1014];	//Task1 stack
tTaskStack task2Env[1014];	//Task2 stack

int main(void)
{
	block.stackPtr=&stackBuffer[1024];		//Point to the address of stackBuffer's last element
	blockPtr=&block;
	
	tTaskInit(&tTask1, task1, (void *)0x11111111, &task1Env[1024]);  //Task1 initiate
	tTaskInit(&tTask2, task2, (void *)0x22222222, &task1Env[1024]);  //Task2 initiate
	
	for(;;)
	{
		flag=1;
		delay(100);
		flag=0;
		delay(100);
		
		triggerPendSVC();		//Trigger PendSVC
	}
}

void delay(int count)    //delay function
{
	while(--count>0);
}

void triggerPendSVC(void)	//PendSVC trigger function
{
	MEM8(NVIC_INIT_SYSPRI2)=NVIC_INIT_PENDSV_PRI;		//Set the priority of PendSV
	MEM32(NVIC_INIT_CTRL)=NVIC_INIT_PENDSVSET;			//Directly trigger PendSV
}

void tTaskInit(tTask *task, void(*entry)(void *), void *param, tTaskStack *stack)	//Task initial function
{
	task->stack=stack;
}

void task1(void *param)	//Task1 function
{
	for(;;)
	{
	}
}

void task2(void *param)	//Task2 function
{
	for(;;)
	{
	}
}
