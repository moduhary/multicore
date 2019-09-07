#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>
#include "timer.h"

#define N 2048

bool print_matrix = false;
bool validation = false;
int thread_num = 0;
int sched_type = 0;

float a[N][N], b[N][N], c[N][N];

void print_help(const char* prog_name);

void mat_mul(char** argv) {
	
	int tid;
	omp_set_num_threads(thread_num);

	#pragma omp parallel shared(a,b,c,thread_num) private(tid)
	{
		tid = omp_get_thread_num();
		
		switch(sched_type)
		{
				case 1:
				#pragma omp for collapse(3) schedule(static, N*N/thread_num)
    			for (int i = 0; i < N; i++) {
        			for (int k = 0; k < N; k++) {
            			for (int j = 0; j < N; j++) {
                			c[i][j] += a[i][k] * b[k][j];
            			}
        			}
   	 			}
				break;
				
				case 2:
				#pragma omp for collapse(3) schedule(dynamic, N*N/thread_num)
    			for (int i = 0; i < N; i++) {
        			for (int k = 0; k < N; k++) {
            			for (int j = 0; j < N; j++) {
                			c[i][j] += a[i][k] * b[k][j];
            			}
        			}
   	 			}
				break;
				
				case 3:
				#pragma omp for collapse(3) schedule(guided,N*N/thread_num)
    			for (int i = 0; i < N; i++) {
        			for (int k = 0; k < N; k++) {
            			for (int j = 0; j < N; j++) {
                			c[i][j] += a[i][k] * b[k][j];
            			}
        			}
   	 			}
				break;
				
				case 4:
				#pragma omp for collapse(3) schedule(auto)
    			for (int i = 0; i < N; i++) {
        			for (int k = 0; k < N; k++) {
            			for (int j = 0; j < N; j++) {
                			c[i][j] += a[i][k] * b[k][j];
            			}
        			}
   	 			}
				break;

				default:
					printf("scheduler invalid\n");
					print_help(argv[0]);
					exit(1);
		}
	}
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
    printf("Usage: %s [-pvh(# of threads)]\n", prog_name);
    printf("OPTIONS\n");
    printf("  -p : print matrix data.\n");
    printf("  -v : validate matrix multiplication.\n");
    printf("  -h : print this page.\n");
	printf(" -# of threads : use specified number of threads when running this program.\n");
}

void parse_opt(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "pvhsdga0123456789")) != -1) {
        switch(opt) {
            case 'p':
                print_matrix = true;
                break;
            case 'v':
                validation = true;
                break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				thread_num = thread_num * 10 + (opt - 48);
				break;
			case 's':
				sched_type = 1;
				break;
			case 'd':
				sched_type = 2;
				break;
			case 'g':
				sched_type = 3;
				break;
			case 'a':
				sched_type = 4;
				break;
            case 'h':
            default:
                print_help(argv[0]);
                exit(1);
        }
    }
}

int main(int argc, char **argv) {
    parse_opt( argc, argv );

	printf("threads: %d\n", thread_num);
	
	if(thread_num <= 0)
	{
		printf("thread number should be greater than 0\n");
		exit(0);
	}

    generate_mat(a);
    generate_mat(b);

    printf("Calculating..."); fflush(stdout);
    timer_start(0);
    mat_mul(argv);
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
