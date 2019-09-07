#include <stdio.h>
#include <immintrin.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>

int main(void)
{
	float a[8],b[8],c[8];
	double totaltime=0;
	int i=0,j=0;
	struct timeval begin,end;

	__m256 vec1 = _mm256_set_ps(1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0);
	__m256 vec2 = _mm256_set_ps(1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5);
	__m256 vec3 = _mm256_set_ps(3,4.5,6,7.5,9,10.5,12,13.5);
	__m256 vecres = _mm256_setzero_ps();

	srand(time(NULL));

	for(j=0;j<8;j++)
	{
		a[j] = ((float)rand())/INT_MAX*10000;
		b[j] = ((float)rand())/INT_MAX*10000;
		c[j] = ((float)rand())/INT_MAX*10000;
	}
			
	vec1 = _mm256_set_ps(rand(),rand(),rand(),rand(),rand(),rand(),rand(),rand());
	vec2 = _mm256_set_ps(rand(),rand(),rand(),rand(),rand(),rand(),rand(),rand());
	vec3 = _mm256_set_ps(rand(),rand(),rand(),rand(),rand(),rand(),rand(),rand());
	//vec2 = _mm256_set_ps(b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7]);
	//vec3 = _mm256_set_ps(c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7]);
	
	gettimeofday(&begin,NULL);

	for(i=0;i<10000000;i++)
	{
		/*for(j=0;j<8;j++)
		{
			a[j] = ((float)rand())/INT_MAX*10000;
			b[j] = ((float)rand())/INT_MAX*10000;
			c[j] = ((float)rand())/INT_MAX*10000;
		}
		
		vec1 = _mm256_set_ps(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
		vec2 = _mm256_set_ps(b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7]);
		vec3 = _mm256_set_ps(c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7]);*/
			
		//gettimeofday(&begin,NULL);	
		vec3 = _mm256_fmadd_ps(vec1,vec2,vec3);
		//gettimeofday(&end,NULL);

		//totaltime += (double)(end.tv_usec-begin.tv_usec);
		//totaltime += (double)(end.tv_sec-begin.tv_sec)*1000000;
	}

	gettimeofday(&end, NULL);

	totaltime += (double)(end.tv_usec-begin.tv_usec);
	totaltime += (double)(end.tv_sec-begin.tv_sec)*1000000;

	printf("Fused Multiply Add Execution Time: %lf (sec)\n",totaltime/1000000);
	printf("%lf GFLOPS\n",1000000/totaltime*0.16);

	_mm256_storeu_ps(a,vec3);
	printf("%f\n",a[0]);

	return 0;
}
