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
uint32_t tSemWait(tSem *sem, uint32_t timeout);		//The Counting Semaphores wait function
uint32_t tSemNoWaitGet(tSem *sem);				//Get the status of the Counting Semaphores
void tSemNotify(tSem *sem);						//The Counting Semaphores notify function

#endif
