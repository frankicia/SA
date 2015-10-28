//mvmmpi.c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h> //taking time
#include <time.h> //taking time

void print_times(char * str);

struct timeval start_green, end_green; //taking time
struct timeval start_orange, end_orange; //taking time

int main(int argc, char *argv[]) {
	double* local_A;
	double* local_x;
	double* local_y;
	int m, local_m, n, local_n;
	int my_rank, comm_sz;
	
	MPI_Comm comm;
	double* vec = NULL;
	double* v_vec = NULL;
	double* A = NULL;
	int i, j;
	double* x;
	int local_i, noreal_i;
	
	MPI_Init(NULL, NULL);
	
	comm = MPI_COMM_WORLD;
	
	MPI_Comm_size(comm, &comm_sz);
	MPI_Comm_rank(comm, &my_rank);
	
	if (my_rank == 0) {
		if (argc<2) {
			printf("the number of parameters is not correct\n");
			exit(-1);
		}
		n = atoi(argv[1]);
		m = n; // for simplicity nxn matrix
	}
	
	MPI_Bcast(&m, 1, MPI_INT, 0, comm);
	MPI_Bcast(&n, 1, MPI_INT, 0, comm);

	local_m = m/comm_sz;
	local_n = n/comm_sz;

	// Allocate storage for local parts of A, x, and y
	local_A = malloc(local_m*n*sizeof(double));
	local_x = malloc(local_n*sizeof(double));
	local_y = malloc(local_m*sizeof(double));

	//Read_matrix_vector
	if (my_rank == 0) {

		A = malloc(m*n*sizeof(double));
		v_vec = malloc(n*sizeof(double));

		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				A[i*n+j]=rand();

		for (i = 0; i < n; i++)
			v_vec[i]=rand();
	}

	//taking time
	if (my_rank == 0) { gettimeofday(&start_orange, NULL); }

	//Scatter
	MPI_Scatter(A, local_m*n, MPI_DOUBLE, local_A, local_m*n, MPI_DOUBLE,
	0, comm);
	MPI_Scatter(v_vec, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE,
	0, comm);

	x = malloc(n*sizeof(double));

	MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE,
	comm);

	// Mat_vect_mult
	if (my_rank == 0) { gettimeofday(&start_green, NULL); }

	for (noreal_i = 0; noreal_i < 10; noreal_i++ ){ // x100 iterations
		for (local_i = 0; local_i < local_m; local_i++) {
			local_y[local_i] = 0.0;
			for (j = 0; j < n; j++)
				local_y[local_i] += local_A[local_i*n+j]*x[j];
		}
	}

	if (my_rank == 0) { gettimeofday(&end_green, NULL); print_times("green"); }

	//taking time
	if (my_rank == 0) { vec = malloc(n*sizeof(double)); }

	//Vector Gather
	MPI_Gather(local_y, local_m, MPI_DOUBLE, vec, local_m, MPI_DOUBLE, 0,
	comm);

	//taking time
	if (my_rank == 0) { gettimeofday(&end_orange, NULL); print_times("orange"); }

	if (my_rank == 0) {
		free(vec);
		free(A);
		free(v_vec);
	}
	free(x);
	free(local_A);
	free(local_x);
	free(local_y);

	MPI_Finalize();

	return 0;
} /* main */

void print_times(char * str)
{
	int total_usecs_green, total_usecs_orange;
	float total_time, total_flops;
	total_usecs_green = (end_green.tv_sec-start_green.tv_sec) * 1000000 +
	(end_green.tv_usec-start_green.tv_usec);
	
	total_usecs_orange = (end_orange.tv_sec-start_orange.tv_sec) * 1000000 +
	(end_orange.tv_usec-start_orange.tv_usec);

	if ( str == "orange" )
		printf("Parallel %.2f mSec \n", ((float) total_usecs_orange) / 1000.0);
	if ( str == "green" )
		printf("Computation %.2f mSec \n", ((float) total_usecs_green) / 1000.0);

	//total_time = ((float) total_usecs) / 1000000.0;
}
