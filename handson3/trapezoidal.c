#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double f(double x); /* Function we're integrating */
void Trap(double a, double b, double n, double* global_result_p);

int main(int argc, char* argv[]) {
	double global_result = 0.0; /* Store result in global_result */
	double a, b; /* Left and right endpoints */
	double n; /* Total number of trapezoids */
	int thread_count;

	thread_count = strtol(argv[1], NULL, 10);

	printf("Enter a, b, and n\n");
	scanf("%lf %lf %lf", &a, &b, &n);

	#pragma omp parallel num_threads(thread_count)
	Trap(a, b, n, &global_result);

	printf("With n = %lf trapezoids, our estimate\n", n);
	printf("of the integral from %f to %f = %.14e\n",
	a, b, global_result);

	return 0;
}

double f(double x) {
	double return_val;
	return_val = x*x;
	return return_val;
}

void Trap(double a, double b, double n, double* global_result_p) {
	double h, x, my_result;
	double local_a, local_b;
	double i, local_n;
	int my_rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();
	
	h = (b-a)/n;
	local_n = n/thread_count;
	local_a = a + my_rank*local_n*h;
	local_b = local_a + local_n*h;
	
	my_result = (f(local_a) + f(local_b))/2.0;
	
	for (i = 1; i <= local_n-1; i++) {
		x = local_a + i*h;
		my_result += f(x);
	}
	my_result = my_result*h;
	
	#pragma omp critical
	*global_result_p += my_result;
}
