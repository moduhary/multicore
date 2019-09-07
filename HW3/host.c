#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "timer.h"
#include <CL/cl.h>
#include <sys/stat.h>
#include <errno.h>

#define N 2048

bool print_matrix = false;
bool validation = false;

float a[N][N], b[N][N], c[N][N];


off_t fsize(const char *filename) 
{
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;

	fprintf(stderr, "Cannot determine size of %s: %s\n", filename, strerror(errno));
	return -1;
}

void mat_mul() {

	int size, err, mat_size;

	FILE* kernel;
	char* kernel_source;

	kernel = fopen("rw+,kernel");

	size = (int)fsize("kernel");
	if(size <= 0)
	{
		printf("kernel file not found\n");
		exit(1);
	}

	err = fread(kernel_source, 1, size, kernel);

	if(err < 0)
	{
		printf("kernel file cannot be read\n");
		exit(1);
	}

	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue command_queue;

	cl_mem bufferA, bufferB, bufferC;
	cl_program mat_mul;
	cl_kernel kernel;

	size_t kernel_src_len = strlen(kernel_source);

	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	context = clCreateContext(0, 1, &device, NULL, NULL, NULL);
	command_queue = clCreateCommandQueue(context, device, 0 , NULL);

	bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, N*N, NULL, NULL);
	bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, N*N, NULL, NULL);
	bufferC = clCreateBuffer(context, CL_MEM_READ_WRITE, N*N, NULL, NULL);

	kernel = clCreateKernel(mat_mul, "mat_mul", NULL);

	mat_size = N;

	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*) &bufferA);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*) &bufferB);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*) &bufferC);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*) &mat_size);
	clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*) &mat_size);
	clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*) &mat_size);

	clEnqueueWriteBuffer(command_queue, bufferA, CL_FALSE, 0, N*N, A, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, bufferB, CL_FALSE, 0, N*N, B, 0, NULL, NULL);

	size_t global[1] = { N, N };
	size_t local[1] = { 32, 32 };

	clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global, local, 0, NULL, NULL);

	clEnqueueReadBuffer(command_queue, bufferC, CL_TRUE, 0, N*N, C, 0, NULL, NULL);

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
