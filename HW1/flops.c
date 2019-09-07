#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/time.h>

int main(void)
{
	int i=0,j=0;
	float f[16]={1.2,2.4,3.6,4.8,0.,0.,0.,0.,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, fresult[8]={1.0,2.0,13.0,4.0,5.0,6.0,7.0,8.0};
	double d[16]={1.2,2.4,3.6,4.8,0.,0.,0.,0.,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}, dresult[8]={1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0}, totaltime=0;
	struct timeval before, after;
	srand(time(NULL));

	for(j=0;j<8;j++)
	{
		d[j] = 1 + rand()/(RAND_MAX+1.0);
		f[j] = 1 + rand()/(RAND_MAX+1.0);

		printf("d[%d]: %f, f[%d]: %f\n", j, d[j], j, f[j]);
	}

	for(j=8;j<16;j++)
	{
		d[j] = 1/d[j-8];
		f[j] = 1/f[j-8];
	}

	gettimeofday(&before,NULL);

	for(i=0;i<100000000;i++)
	{
		/*for(j=0;j<4;j++)
		{
			d[j] = ((double)rand())/INT_MAX*1000000;
		}*/

		//gettimeofday(&before,NULL);
		dresult[0]+=d[0];
		dresult[1]+=d[1];
		dresult[2]+=d[2];
		dresult[3]+=d[3];
		dresult[4]+=d[4];
		dresult[5]+=d[5];
		dresult[6]+=d[6];
		dresult[7]+=d[7];
		//gettimeofday(&after, NULL);

		//totaltime += (double)(after.tv_sec - before.tv_sec);
		//totaltime += (double)(after.tv_usec - before.tv_usec)/1000000;
	}

	gettimeofday(&after,NULL);
	
	totaltime += (double)(after.tv_sec - before.tv_sec);
	totaltime += (double)(after.tv_usec - before.tv_usec)/1000000;

	printf("%lf, %lf, %lf, %lf\n",dresult[0],dresult[1],dresult[2],dresult[3]);
	printf("%lf, %lf, %lf, %lf\n",dresult[4],dresult[5],dresult[6],dresult[7]);

	printf("Double Add Execution time: %lf (sec)\n",totaltime);
	printf("%lf GFLOPS\n",(1/totaltime)*0.8);

	totaltime = 0;
	gettimeofday(&before,NULL);

	dresult[0] = 1.0;
	dresult[1] = 2.0;
	dresult[2] = 3.0;
	dresult[3] = 4.0;
	dresult[4] = 5.0;
	dresult[5] = 6.0;
	dresult[6] = 7.0;
	dresult[7] = 8.0;

	for(i=0;i<100000000;i++)
	{
		/*for(j=0;j<4;j++)
		{
			d[j] = ((double)rand())/INT_MAX*1000000;
		}*/

		//gettimeofday(&before,NULL);
		dresult[0]*=d[0];
		dresult[1]*=d[1];
		dresult[2]*=d[2];
		dresult[3]*=d[3];
		dresult[0]*=d[8];
		dresult[1]*=d[9];
		dresult[2]*=d[10];
		dresult[3]*=d[11];
		//gettimeofday(&after, NULL);

		//totaltime += (double)(after.tv_sec - before.tv_sec);
		//totaltime += (double)(after.tv_usec - before.tv_usec)/1000000;
	}

	gettimeofday(&after,NULL);
	
	totaltime += (double)(after.tv_sec - before.tv_sec);
	totaltime += (double)(after.tv_usec - before.tv_usec)/1000000;

	printf("%lf, %lf, %lf, %lf\n",dresult[0],dresult[1],dresult[2],dresult[3]);
	printf("%lf, %lf, %lf, %lf\n",dresult[4],dresult[5],dresult[6],dresult[7]);

	printf("Double Mul Execution time: %lf (sec)\n",totaltime);
	printf("%lf GFLOPS\n",(1/totaltime)*0.8);

	totaltime=0;
	gettimeofday(&before,NULL);

	for(i=0;i<100000000;i++)
	{
		/*for(j=0;j<4;j++)
		{
			d[j] = ((double)rand())/INT_MAX*1000000;
		}*/

		//gettimeofday(&before,NULL);
		fresult[0]+=f[0];
		fresult[1]+=f[1];
		fresult[2]+=f[2];
		fresult[3]+=f[3];
		fresult[4]+=f[4];
		fresult[5]+=f[5];
		fresult[6]+=f[6];
		fresult[7]+=f[7];

		//totaltime += (double)(after.tv_sec - before.tv_sec);
		//totaltime += (double)(after.tv_usec - before.tv_usec)/1000000;
	}

	gettimeofday(&after,NULL);
	
	totaltime += (double)(after.tv_sec - before.tv_sec);
	totaltime += (double)(after.tv_usec - before.tv_usec)/1000000;

	printf("%f, %f, %f, %f\n",fresult[0],fresult[1],fresult[2],fresult[3]);
	printf("%f, %f, %f, %f\n",fresult[4],fresult[5],fresult[6],fresult[7]);

	printf("float Add Execution time: %lf (sec)\n",totaltime);
	printf("%lf GFLOPS\n",(1/totaltime)*0.8);
	
	fresult[0]=1.0;
	fresult[1]=2.0;
	fresult[2]=3.0;
	fresult[3]=4.0;
	fresult[4]=5.0;
	fresult[5]=6.0;
	fresult[6]=7.0;
	fresult[7]=8.0;

	totaltime=0;
	gettimeofday(&before,NULL);

	for(i=0;i<100000000;i++)
	{
		/*for(j=0;j<4;j++)
		{
			d[j] = ((double)rand())/INT_MAX*1000000;
		}*/

		//gettimeofday(&before,NULL);
		fresult[0]*=f[0];
		fresult[1]*=f[1];
		fresult[2]*=f[2];
		fresult[3]*=f[3];
		fresult[0]*=f[8];
		fresult[1]*=f[9];
		fresult[2]*=f[10];
		fresult[3]*=f[11];
		//gettimeofday(&after, NULL);

		//totaltime += (double)(after.tv_sec - before.tv_sec);
		//totaltime += (double)(after.tv_usec - before.tv_usec)/1000000;
	}

	gettimeofday(&after,NULL);
	
	totaltime += (double)(after.tv_sec - before.tv_sec);
	totaltime += (double)(after.tv_usec - before.tv_usec)/1000000;

	printf("%f, %f, %f, %f\n",fresult[0],fresult[1],fresult[2],fresult[3]);
	printf("%f, %f, %f, %f\n",fresult[4],fresult[5],fresult[6],fresult[7]);

	printf("float Mul Execution time: %lf (sec)\n",totaltime);
	printf("%lf GFLOPS\n",(1/totaltime)*0.8);

	return 0;
}
