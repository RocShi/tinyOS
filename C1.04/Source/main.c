int flag;
void delay(int count);

int main(void)
{
	for(;;)
	{
		flag=1;
		delay(100);
		flag=0;
		delay(100);
	}
}

void delay(int count)    //delay function
{
	while(--count>0);
}
