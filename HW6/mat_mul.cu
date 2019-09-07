#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "timer.h"
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define N 8192
//#define M 16

bool print_matrix = false;
bool validation = false;

float a[N][N], b[N][N], c[N][N];

__global__ void cu_mat_mul(float *A, float *B, float *C, int n)
{
	int i, j ,k;
	float acm = 0;
	i = blockDim.x * blockIdx.x + threadIdx.x;
	j = blockDim.y * blockIdx.y + threadIdx.y;

	//printf("i - %d, j - %d\n", i, j);

	for(k = 0; k < n ; k++)
	{
		acm += *( A + j*n + k ) * *(B + k*n + i);
	}

	*(C + j*n + i) = acm;
}

void mat_mul() {

	int sizeA, sizeB, sizeC;
	float *d_a, *d_b, *d_c;

	sizeA = sizeB = sizeC = N*N;

	if(cudaMalloc(&d_a, sizeof(float)*sizeA) != cudaSuccess)
	{
		printf("Malloc A error!\n");
		exit(1);
	}

	if(cudaMalloc(&d_b, sizeof(float)*sizeB) != cudaSuccess)
	{
		printf("Malloc B error!\n");
		exit(1);
	}

	if(cudaMalloc(&d_c, sizeof(float)*sizeC) != cudaSuccess)
	{
		printf("Malloc C error!\n");
		exit(1);
	}

	if( cudaMemcpy(d_a, a, sizeA*4, cudaMemcpyHostToDevice) != cudaSuccess)
	{
		printf("Memcpy A error!\n");
		exit(1);
	}
	
	if( cudaMemcpy(d_b, b, sizeB*4, cudaMemcpyHostToDevice) != cudaSuccess)
	{
		printf("Memcpy B error!\n");
		exit(1);
	}

	dim3 grid( 16, 16, 1 );
	dim3 thread( N/grid.x, N/grid.y, 1 );

	cu_mat_mul <<< thread, grid >>> (d_a, d_b, d_c, N);

	if( cudaMemcpy(c, d_c, sizeof(float)*sizeC, cudaMemcpyDeviceToHost) != cudaSuccess)
	{
		printf("Memcpy C error!\n");
		exit(1);
	}

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

	return;
}
/*
 * ==================================================================
 *                      DO NOT EDIT BELOW THIS LINE
 * ==================================================================
 */

void check_mat_mul() {
    printf("Validating...\n");

    bool is_valid = true;
    float eps = 1e-3;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float s = 0;
            for (int k = 0; k < N; ++k) {
                s += a[i][k] * b[k][j];
            }
            if (fabsf(c[i][j] - s) > eps && (s == 0 || fabsf((c[i][j] - s) / s) > eps)) {
                printf("c[%d][%d] : correct_value = %f, your_value = %f\n", i, j, s, c[i][j]);
                is_valid = false;
            }
        }
    }

    if (is_valid) {
        printf("result: VALID\n");
    } else {
        printf("result: INVALID\n");
    }
}

void generate_mat(float (*a)[N]) {
    for (int i = 0; i < N; ++i) { 
        for (int j = 0; j < N; ++j) {
            a[i][j] = (float)rand() / RAND_MAX - 0.5;
        }
    }
}

void print_mat(float (*a)[N]) {
    for (int i = 0; i < N; ++i) { 
        for (int j = 0; j < N; ++j) {
            printf("%+.3f ", a[i][j]);
        }
        printf("\n");
    }
}

void print_help(const char* prog_name) {
    printf("Usage: %s [-pvh]\n", prog_name);
    printf("OPTIONS\n");
    printf("  -p : print matrix data.\n");
    printf("  -v : validate matrix multiplication.\n");
    printf("  -h : print this page.\n");
}

void parse_opt(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "pvh")) != -1) {
        switch(opt) {
            case 'p':
                print_matrix = true;
                break;
            case 'v':
                validation = true;
                break;
            case 'h':
            default:
                print_help(argv[0]);
                exit(0);
        }
    }
}

int main(int argc, char **argv) {
    parse_opt( argc, argv );

    generate_mat(a);
    generate_mat(b);

    printf("Calculating..."); fflush(stdout);
    timer_start(0);
    mat_mul();
    double elapsed_time = timer_stop(0);
    printf(" done!\n");

    if (print_matrix) {
        printf("MATRIX A:\n"); print_mat(a);
        printf("MATRIX B:\n"); print_mat(b);
        printf("MATRIX C:\n"); print_mat(c);
    }

    if (validation) {
        check_mat_mul();
    } else {
        printf("Validation is skipped.\n");
    }

    printf("Elapsed time: %f sec\n", elapsed_time);

    return 0;
}
