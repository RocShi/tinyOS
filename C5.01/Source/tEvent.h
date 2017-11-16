#ifndef TEVENT_H
#define TEVENT_H

#include "tLib.h"

typedef enum _tEventType								//Type of event
{
	tEventTypeUnknown,
}tEventType;

typedef struct _tEventControlBlock						//The structure of Event Control Block(ECB)
{
	tEventType type;
	tList waitList;
}tEventControlBlock;

void tEventInit(tEventControlBlock *ECB, tEventType eventtype);	//The Event Control Block initial function

#endif
