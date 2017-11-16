#include "tinyOS.h"
#include "ARMCM3.h"

tTask *currentTask;										//Current task's structure pointer
tTask *nextTask;											//Next task's structure pointer
tTask *idleTask;											//Idle task's structure pointer
tList taskTable[TINYOS_PRO_COUNT];		//Task list
tTask tTaskIdle;											//Idle task structure
tTaskStack idleTaskEnv[TINYOS_IDLETASK_STACK_SIZE];					//Idle task stack

uint8_t schedLockCount;								//Schedule lock counter
tBitmap taskPrioBitmap;								//The bitmap of the tasks' priorities
tList tTaskDelayedList;								//Task delayed list

void idleTaskEntry(void *param);																	//Idle task function

int main(void)
{
	tTaskSchedInit();
	tTaskDelayInit();																								//Task delayed list initiate
	
	tAppInit();
	
	tTaskInit(&tTaskIdle, idleTaskEntry, (void *)0, TINYOS_PRO_COUNT-1, 
						&idleTaskEnv[TINYOS_IDLETASK_STACK_SIZE]);						//Idle task initiate
	
	idleTask=&tTaskIdle;
	
	nextTask=tTaskHighestReady();							//The first task to be ran has the highest priority
	tTaskRunFirst();
	return 0;
}

void idleTaskEntry(void *param)				//Idele task function
{
	for(;;)
	{
	}
}
