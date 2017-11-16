#include "tinyOS.h"

void tSemInit(tSem *sem, uint32_t startCount, uint32_t maxCount)	//The Counting Semaphores initial function
{
	tEventInit(&(sem->ECB), tEventTypeSem);
	
	sem->maxCount=maxCount;
	if(maxCount==0)
	{
		sem->Count=startCount;
	}
	else
	{
		sem->Count=(startCount>maxCount) ? maxCount:startCount;
	}
}
