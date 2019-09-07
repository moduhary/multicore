#include <stdio.h>
#include <stdlib.h>

void tobinary(long* c, char* result);

int main(void)
{
	double c;
	long *mem;
	char* bin=malloc(sizeof(char)*65);
	printf("Enter a number: ");
	scanf("%lf",&c);
	
	bin[64]='\0';

	//printf("%lx\n",*((long*)(&c)));
	mem = (long*)&c;

	tobinary(mem, bin);

	printf("%s\n",bin);
	printf("%lf\n",c);

	free(bin);

	return 0;
}

void tobinary(long* c, char* result)
{
	int i=0;
	long a;
	a=1;

	for(i=0;i<64;i++)
	{
		if(((*c)&a)==a)
			result[i]=0x31;
		else
			result[i]=0x30;

		a=a*2;
	}

	return;
}
