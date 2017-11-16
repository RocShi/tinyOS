#include "tinyOS.h"

void tEventInit(tEventControlBlock *ECB, tEventType eventtype)	//The Event Control Block initial function
{
	ECB->type=eventtype;
	tListInit(&(ECB->waitList));
}
