#include "tinyOS.h"
#include "ARMCM3.h"

__asm void PendSV_Handler (void)	//PendSV service funtion
{
	IMPORT currentTask
	IMPORT nextTask
	
	MRS R0, PSP
	CBZ R0, PendSVHandler_NoSave
	
	STMDB R0!, {R4-R11}	//Push stack
	
	LDR R1, =currentTask
	LDR R1, [R1]
	STR R0, [R1]	//Update task structure stack pointer
	
PendSVHandler_NoSave
	
	LDR R0, =currentTask
	LDR R1, =nextTask
	LDR R1, [R1]
	STR R1, [R0]
	
	LDR R0, [R1]
	LDMIA R0!, {R4-R11}	//Pop stack
	
	MSR PSP, R0
	ORR LR, LR, #0X04		//Using PSP
	
	BX LR								//Exit PendSV
}

void tTaskRunFirst(void)	//This function'll be called when the tinyOS ran at the first time
{
	__set_PSP(0);
	MEM8(NVIC_INIT_SYSPRI2)=NVIC_INIT_PENDSV_PRI;	//Configure the priority of Pend_SVC->the lowest
	MEM32(NVIC_INIT_CTRL)=NVIC_INIT_PENDSVSET;		//Trigger the Pend_SVC
}

void tTaskSched(void)	//Task schedule function
{
	if(currentTask==taskTable[0])
			nextTask=taskTable[1];
	else
			nextTask=taskTable[0];
	tTaskSwitch();
}

void tTaskSwitch(void)	//Task switch function
{
	MEM32(NVIC_INIT_CTRL)=NVIC_INIT_PENDSVSET;			//Directly trigger PendSV
}
