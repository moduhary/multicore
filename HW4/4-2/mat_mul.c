#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "timer.h"
#include "mpi.h"

#define N 2048

bool print_matrix = false;
bool validation = false;

float a[N][N], b[N][N], c[N][N];

void mat_mul(int size, int rank) {

	MPI_Bcast(a, N*N, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Bcast(b, N*N, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = N*rank/size; i < N*(rank+1)/size; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

	MPI_Gather(&c[rank*N/size][0], N*N/size, MPI_FLOAT, c, N*N/size, MPI_FLOAT, 0, MPI_COMM_WORLD);
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

void generate_mat(float (*a)[N], int mode) {
	if(mode == 0)
	{
    	for (int i = 0; i < N; ++i) { 
        	for (int j = 0; j < N; ++j) {
            	a[i][j] = /*(float)rand() / RAND_MAX - 0.5*/ i + j;
        	}
    	}
	}
	else
	{	
		for (int i = 0; i < N; ++i) { 
        	for (int j = 0; j < N; ++j) {
            	a[i][j] = /*(float)rand() / RAND_MAX - 0.5*/ 1;
        	}
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
    parse_opt( argc, &argv[3] );

	int rank, size, flag; 
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0)
		printf("Size : %d\n", size);

	MPI_Initialized(&flag);

	if(flag != 0)
	{
	}

	if(rank == 0)
    {
		generate_mat(a, 0);
		generate_mat(b, 1);
	
    	printf("Calculating..."); fflush(stdout);
	}
	else
	{
	}

    timer_start(0);
    mat_mul(size, rank);
    double elapsed_time = timer_stop(0);
    
	if(rank == 0)
		printf(" done!\n");

    if (print_matrix && rank == 0) {
        printf("MATRIX A:\n"); print_mat(a);
        printf("MATRIX B:\n"); print_mat(b);
        printf("MATRIX C:\n"); print_mat(c);
    }

    if (rank == 0 && validation == 1) 
	{
        check_mat_mul();
    } 
	else if(validation == 0 && rank == 0) 
	{
        printf("Validation is skipped.\n");
    }

	if(rank == 0)
    	printf("Elapsed time: %f sec\n", elapsed_time);

	MPI_Finalize();

    return 0;
}
