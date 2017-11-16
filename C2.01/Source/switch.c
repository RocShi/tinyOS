__asm void PendSV_Handler (void)	//PendSV service funtion
{
	IMPORT blockPtr
	
	LDR R0, =blockPtr		//R0=&blockPtr,be careful of "="
	LDR R0, [R0]				//R0=*R0, equal to R0=blockPtr
	LDR R0, [R0]				//R0=*R0, equal to R0=blockPtr.stackPtr
	
	STMDB R0!, {R4-R11}	//Push stack
	
	LDR R1, =blockPtr		//R1=&blockPtr,be careful of "="
	LDR R1, [R1]				//R1=*R1, equal to R1=blockPtr
	STR R0, [R1]				//*R1=R0, equal to blockPtr.stackPtr=R1. To update stackPtr
	
	ADD R4, R4, #1
	ADD R5, R5, #1
	
	LDMIA R0!, {R4-R11}	//Pop stack
	
	BX LR								//Exit PendSV
}
