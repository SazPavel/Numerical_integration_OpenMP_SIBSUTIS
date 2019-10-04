#define _POSIX_C_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>
#include <math.h>

const double PI = 3.14159265358979323846;
const int n = 10000000;

double wtime(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double getrand(unsigned int *seed){
	return (double)rand_r(seed) / RAND_MAX;
}

double func(double x, double y){
	return exp(x-y);	
}

int main(){
	printf("Numerical integration by Monte Carlo method: n = %d\n", n);
	int in = 0;
	double s = 0;
	#pragma omp parallel
	{
		double s_loc = 0;
	//	int in_loc = 0;
		unsigned int seed = omp_get_thread_num();
		#pragma omp for nowait
		for(int i = 0; i < n; i++){
			double x = getrand(&seed) - 1;
			double y = getrand(&seed);
	//		in_loc++;
			s_loc += func(x, y);
			
		}
	#pragma omp atomic
	s += s_loc;
	}
	double res = s / n;
	printf("Result: %.12f, n %d\n", res, n);
	return 0;
}
