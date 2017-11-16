#define NVIC_INIT_CTRL				0xE000ED04	//The address of NVIC_INIT_CTRL register
#define NVIC_INIT_PENDSVSET		0x10000000	//The value to be written to the NVIC_INIT_CTRL

#define NVIC_INIT_SYSPRI2			0xE000ED22	//The address of NVIC_INIT_SYSPRI2 register
#define NVIC_INIT_PENDSV_PRI	0x000000FF	//The value to be written to the NVIC_INIT_PENDSV_PRI

#define MEM8(addr)		*(volatile unsigned char *)addr
#define MEM32(addr)		*(volatile unsigned long *)addr

void delay(int count);			//delay function
void triggerPendSVC(void);	//PendSVC trigger function

typedef struct _BlockType_t
{
	unsigned long * stackPtr;
} BlockType_t;

int flag;
unsigned long stackBuffer[1024];
BlockType_t * blockPtr;
BlockType_t block;

int main(void)
{
	block.stackPtr=&stackBuffer[1024];		//Point to the address of stackBuffer's last element
	blockPtr=&block;
	
	for(;;)
	{
		flag=1;
		delay(100);
		flag=0;
		delay(100);
		
		triggerPendSVC();		//Trigger PendSVC
	}
}

void delay(int count)    //delay function
{
	while(--count>0);
}

void triggerPendSVC(void)	//PendSVC trigger function
{
	MEM8(NVIC_INIT_SYSPRI2)=NVIC_INIT_PENDSV_PRI;		//Set the priority of PendSV
	MEM32(NVIC_INIT_CTRL)=NVIC_INIT_PENDSVSET;			//Directly trigger PendSV
}
