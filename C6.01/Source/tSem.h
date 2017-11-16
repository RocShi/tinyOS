#ifndef TSEM_H
#define TSEM_H

#include "tEvent.h"

typedef struct _tSem		//The Counting Semaphores data type
{
	tEventControlBlock ECB;
	
	uint32_t Count;
	uint32_t maxCount;
}tSem;

void tSemInit(tSem *sem, uint32_t startCount, uint32_t maxCount);	//The Counting Semaphores initial function

#endif
