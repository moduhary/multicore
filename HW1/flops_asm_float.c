#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/time.h>

int main(void)
{
	//FILE* file_result;
	int i=0,j=0;
	float f[8]={0.,0.,0.,0.,0.,0.,0.,0.},fresult[4]={0.,0.,0.,0.};
	double d1=0,d2=0,dresult=0, totaltime=0;
	struct timeval before, after;

	//file_result = fopen("result.txt","rw+");

	srand(time(NULL));

	for(i=0;i<10000000;i++)
	{
		for(j=0;j<8;j++)
		{
			f[j] = ((float)rand())/INT_MAX*1000000;
		}

		asm("movaps -0x30(%rbp),%xmm0;"
			"movaps -0x20(%rbp),%xmm1;");
		gettimeofday(&before, NULL);
		asm("addps %xmm1,%xmm0;");
		gettimeofday(&after, NULL);
		asm("movaps %xmm0,-0x40(%rbp)");

		totaltime += (double)(after.tv_sec - before.tv_sec)*1000000;
		totaltime += (double)(after.tv_usec - before.tv_usec);

		/*fprintf(file_result,"result %d\n",i+1);
		fprintf(file_result,"f:%f,%f,%f,%f,%f,%f,%f,%f\n",f[0],f[4],f[1],f[5],f[2],f[6],f[3],f[7]);
		fprintf(file_result,"fresult:%f,%f,%f,%f\n",fresult[0],fresult[1],fresult[2],fresult[3]);*/
	}

	printf("Addition Execution time: %lf (sec)\n",totaltime/1000000);
	printf("%lf FLOPS\n",(1000000/totaltime)*40000000);

	totaltime = 0;

	for(i=0;i<10000000;i++)
	{
		for(j=0;j<8;j++)
		{
			f[j] = ((float)rand())/INT_MAX*1000000;
		}

		asm("movaps -0x30(%rbp),%xmm0;"
			"movaps -0x20(%rbp),%xmm1;");
		gettimeofday(&before, NULL);
		asm("mulps %xmm1,%xmm0;");
		gettimeofday(&after, NULL);
		asm("movaps %xmm0,-0x40(%rbp)");

		totaltime += (double)(after.tv_sec - before.tv_sec)*1000000;
		totaltime += (double)(after.tv_usec - before.tv_usec);

		/*fprintf(file_result,"result %d\n",i+1);
		fprintf(file_result,"f:%f,%f,%f,%f,%f,%f,%f,%f\n",f[0],f[4],f[1],f[5],f[2],f[6],f[3],f[7]);
		fprintf(file_result,"fresult:%f,%f,%f,%f\n",fresult[0],fresult[1],fresult[2],fresult[3]);*/
	}

	printf("Multiplication Execution time: %lf (sec)\n",totaltime/1000000);
	printf("%lf FLOPS\n",(1000000/totaltime)*40000000);

	//fclose(file_result);

	return 0;
}
