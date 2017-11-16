#ifndef TINYOS_H
#define TINYOS_H

#include <stdint.h>											//The official data type defination head file

#define NVIC_INIT_CTRL				0xE000ED04	//The address of NVIC_INIT_CTRL register
#define NVIC_INIT_PENDSVSET		0x10000000	//The value to be written to the NVIC_INIT_CTRL

#define NVIC_INIT_SYSPRI2			0xE000ED22	//The address of NVIC_INIT_SYSPRI2 register
#define NVIC_INIT_PENDSV_PRI	0x000000FF	//The value to be written to the NVIC_INIT_PENDSV_PRI

#define MEM8(addr)		*(volatile unsigned char *)addr
#define MEM32(addr)		*(volatile unsigned long *)addr

typedef uint32_t tTaskStack;		//Task stack data type
typedef struct _tTask					//Task structure data type
{
	uint32_t *stack;				//Task stack pointer
	uint32_t delayTicks;		//Task delay counter
}tTask;

extern tTask *currentTask;		//Current task's structure pointer
extern tTask *nextTask;				//Next task's structure pointer
extern tTask *idleTask;			//Idle task's structure pointer
extern tTask *taskTable[2];		//Task list

void tTaskRunFirst(void);	//This function'll be called when the tinyOS ran at the first time
void tTaskSched(void);			//Task schedule function
void tTaskSwitch(void);		//Task switch function

#endif
