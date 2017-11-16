#ifndef TEVENT_H
#define TEVENT_H

#include "tTask.h"

typedef enum _tEventType								//Type of event
{
	tEventTypeUnknown,
	tEventTypeTimeout,
}tEventType;

typedef struct _tEventControlBlock						//The structure of Event Control Block(ECB)
{
	tEventType type;
	tList waitList;
}tEventControlBlock;

void tEventInit(tEventControlBlock *ECB, tEventType eventtype);	//The Event Control Block initial function
void tEventWait(tEventControlBlock *ECB, tTask *task, 
				void *msg, uint32_t state, uint32_t timeout);	//The function of waiting event
tTask * tEventWakeUp(tEventControlBlock *ECB, 
					 void *msg, uint32_t result);				//The function of waking the ECB up
void tEventRemoveTask(tTask *task, void *msg, uint32_t result);//The function of remove the task from the waitList of its' ECB

#endif
