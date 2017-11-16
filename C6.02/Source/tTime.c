#include "tinyOS.h"

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
