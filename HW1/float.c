#include <stdio.h>
#include <stdlib.h>

void tobinary(int* c, char* result);

int main(void)
{
	float c;
	int *mem;
	char* bin=malloc(sizeof(char)*33);
	printf("Enter a number: ");
	scanf("%f",&c);
	
	bin[32]='\0';

	//printf("%x\n",*((int*)(&c)));
	mem = (int*)&c;

	tobinary(mem, bin);

	printf("%s\n",bin);
	printf("%f\n",c);

	free(bin);

	return 0;
}

void tobinary(int* c, char* result)
{
	int i=0;
	int a;
	a=1;

	for(i=0;i<32;i++)
	{
		if(((*c)&a)==a)
			result[31-i]=0x31;
		else
			result[31-i]=0x30;

		a=a*2;
	}

	return;
}
