#ifndef TMBOX_H
#define TMBOX_H

#include "tEvent.h"

typedef struct _tMbox				//The structure of mailbox
{
	tEventControlBlock ECB;
	uint32_t Count;					//Current amount of messages
	uint32_t maxCount;				//Permitted max amount of messages
	uint32_t read;					//Read index of message buffer
	uint32_t write;					//Writen index of message buffer
	void **msgBuffer;
	
}TMBOX_H;

#endif
