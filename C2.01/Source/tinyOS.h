#ifndef TINYOS_H
#define TINYOS_H

#include <stdint.h>						//The official data type defination head file

typedef uint32_t tTaskStack;		//Task stack data type
typedef struct _tTask					//Task structure data type
{
	uint32_t *stack;
}tTask;

#endif
