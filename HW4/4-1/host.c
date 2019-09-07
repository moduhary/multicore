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
#include <string.h>

#define N 8192
#define dev_num 4
#define GPUnum dev_num

bool print_matrix = false;
bool validation = false;

float a[N][N], b[N][N], c[N][N];


size_t fsize(const char *filename) 
{
	struct stat st;

	if (stat(filename, &st) == 0)
		return st.st_size;

	fprintf(stderr, "Cannot determine size of %s\n", filename);
	return -1;
}

void mat_mul() {

	int size, err, mat_size;

	FILE* kernel;
	char* kernel_source, *file_name = "kernel\0";

	int sizeA, sizeB, sizeC;
	float *hostA, *hostB, *hostC;
	int i = 0;

	sizeA = sizeB = sizeC = N*N;
	hostA = calloc(sizeA, 4);
	hostB = calloc(sizeB, 4);
	hostC = calloc(sizeC, 4);

	memcpy(hostA, a, sizeA*4);
	memcpy(hostB, b, sizeA*4);

	kernel = fopen("kernel","r");
	size = fsize(file_name);
	if(size <= 0)
	{
		printf("kernel file not found\n");
		exit(1);
	}

	kernel_source = calloc(size+2,sizeof(char));
	err = fread(kernel_source, size+1, sizeof(char), kernel);

	if(err < 0)
	{
		printf("kernel file cannot be read\n");
		exit(1);
	}

	int ndev;

	cl_platform_id platform;
	cl_device_id device[dev_num];
	cl_context context;
	cl_command_queue command_queue[dev_num];

	cl_mem bufferA[dev_num], bufferB[dev_num], bufferC[dev_num];
	cl_program mat_mul;
	cl_kernel kernel_prog[dev_num];
	cl_int err_build;

	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, (unsigned int*)&ndev);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, GPUnum, device, NULL);
	context = clCreateContext(0, GPUnum, device, NULL, NULL, NULL);
	
	printf("ndev: %d\n",GPUnum);

	for(i=0; i<GPUnum; i++)
		command_queue[i] = clCreateCommandQueue(context, device[i], 0 , NULL);

	for(i=0; i<GPUnum; i++)
	{
		bufferA[i] = clCreateBuffer(context, CL_MEM_READ_WRITE, N*N*sizeof(float)/GPUnum, NULL, NULL);
		bufferB[i] = clCreateBuffer(context, CL_MEM_READ_WRITE, N*N*sizeof(float), NULL, NULL);
		bufferC[i] = clCreateBuffer(context, CL_MEM_READ_WRITE, N*N*sizeof(float)/GPUnum, NULL, NULL);
	}

	size_t kernel_src_len = strlen(kernel_source);

	mat_mul = clCreateProgramWithSource(context, 1, (const char**) &kernel_source, &kernel_src_len, NULL);

	err_build = clBuildProgram(mat_mul, GPUnum, device, "", NULL, NULL);
	if (err_build == CL_BUILD_PROGRAM_FAILURE) 
	{
		printf("Build Error Occured!\n");
		exit(1);
	}	

	for(i=0; i<GPUnum; i++)
		kernel_prog[i] = clCreateKernel(mat_mul, "mat_mul", NULL);

	mat_size = N;

	for(i=0; i<GPUnum; i++)
	{
		clSetKernelArg(kernel_prog[i], 0, sizeof(cl_mem), (void*) &bufferA[i]);
		clSetKernelArg(kernel_prog[i], 1, sizeof(cl_mem), (void*) &bufferB[i]);
		clSetKernelArg(kernel_prog[i], 2, sizeof(cl_mem), (void*) &bufferC[i]);
		clSetKernelArg(kernel_prog[i], 3, sizeof(cl_int), (int*) &mat_size);
		clSetKernelArg(kernel_prog[i], 4, sizeof(cl_int), (int*) &mat_size);
	}

	//kernel_prog = clCreateKernel(mat_mul, "mat_mul", NULL);
	
	for(i=0; i<GPUnum; i++)
	{
		switch(clEnqueueWriteBuffer(command_queue[i], bufferA[i], CL_FALSE, 0, sizeA*4/GPUnum, (void*)((size_t)hostA+(sizeA*4/GPUnum)*i), 0, NULL, NULL))
		{
			case CL_SUCCESS:
				break;

			case CL_INVALID_COMMAND_QUEUE:
				printf("Invalid command\n");
				exit(1);

			case CL_INVALID_CONTEXT:
				printf("Invalid context\n");
				exit(1);

			case CL_INVALID_MEM_OBJECT:
				printf("Invalild memory object\n");
				exit(1);

			case CL_INVALID_VALUE:
				printf("Invalid pointer value\n");
				exit(1);

			default:
				break;
		}
	
		switch(clEnqueueWriteBuffer(command_queue[i], bufferB[i], CL_FALSE, 0, sizeB*4, hostB, 0, NULL, NULL))
		{
			case CL_SUCCESS:
				break;

			case CL_INVALID_COMMAND_QUEUE:
				printf("Invalid command\n");
				exit(1);

			case CL_INVALID_CONTEXT:
				printf("Invalid context\n");
				exit(1);

			case CL_INVALID_MEM_OBJECT:
				printf("Invalild memory object\n");
				exit(1);

			case CL_INVALID_VALUE:
				printf("Invalid pointer value\n");
				exit(1);

			default:
				break;
		}
	}

	size_t global[2] = { N, N/GPUnum };
	size_t local[2] = { 16, 16 };

	int e;


	for(i=0; i<GPUnum; i++)
	{
		e = clEnqueueNDRangeKernel(command_queue[i], kernel_prog[i], 2, NULL, global, local, 0, NULL, NULL);
		switch(e)
		{
			case CL_SUCCESS:
				break;

			default:
				printf("Error Occured! 3 - errno: %d\n",e);
				exit(1);
		}
	}

	for(i=0; i<GPUnum; i++)
	{
		e = clEnqueueReadBuffer(command_queue[i], bufferC[i], CL_TRUE, 0, sizeC*4/GPUnum, (void*)((size_t)hostC+(sizeC*4/GPUnum)*i), 0, NULL, NULL);
		switch(e)
		{
			case CL_SUCCESS:
				break;

			default:
				printf("Error Occured! 4 - errno: %d\n",e);
				exit(1);
		}
	}

	memcpy(c, hostC, sizeC*4);

	for(i=0; i<GPUnum; i++)
	{
		clReleaseMemObject(bufferA[i]);
		clReleaseMemObject(bufferB[i]);
		clReleaseMemObject(bufferC[i]);
		clReleaseCommandQueue(command_queue[i]);
		clReleaseKernel(kernel_prog[i]);
	}

	clReleaseProgram(mat_mul);
	clReleaseContext(context);

	free(kernel_source);
	free(hostA);
	free(hostB);
	free(hostC);

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
