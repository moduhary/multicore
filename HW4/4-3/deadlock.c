#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char** argv)
{
	int rank, size;
	int a[10], b[10];

	int tag = 10, i = 0;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for(i=0; i<10; i++)
	{
		a[i] = rank + i;
		b[i] = rank + 2i;
	}

	switch (rank)
	{
		case 0:
			MPI_Send(a, 5, MPI_INT, 1, tag, MPI_COMM_WORLD);
			MPI_Recv(b, 5, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
			break;
		
		case 1:
			MPI_Send(b, 5, MPI_INT, 0, tag, MPI_COMM_WORLD);
			MPI_Recv(a, 5, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
			break;

		case 2:
			MPI_Send(a, 5, MPI_INT, 3, tag, MPI_COMM_WORLD);
			MPI_Send(b, 5, MPI_INT, 3, tag+1, MPI_COMM_WORLD);
			break;

		case 3:
			MPI_Recv(a, 5, MPI_INT, 2, tag, MPI_COMM_WORLD, &status);
			MPI_Recv(b, 5, MPI_INT, 2, tag+1, MPI_COMM_WORLD, &status);
			break;

		case 4:
			MPI_Recv(b, 5, MPI_INT, 5, tag, MPI_COMM_WORLD, &status);
			MPI_Send(a, 5, MPI_INT, 5, tag, MPI_COMM_WORLD);
			break;

		case 5:
			MPI_Recv(a, 5, MPI_INT, 4, tag, MPI_COMM_WORLD, &status);
			MPI_Send(b, 5, MPI_INT, 4, tag, MPI_COMM_WORLD);
			break;

		default:
			break;

	}

	printf("Process %d done!\n", rank);

	MPI_Finalize();

	return 0;
}
