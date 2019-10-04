#include <stdio.h>
#include <omp.h>
#include <sys/time.h>
#include <math.h>

double wtime(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double func(double x){
	return sqrt(x * (3 - x))/(x+1);
}

int main()
{
	double t = wtime();
	const double eps = 1E-5;
	const double a = 1.0;
	const double b = 1.2;
	const int n0 = 100000000;
	printf("Numerical integration: [%f, %f], n0 = %d, EPS = %f\n", a, b, n0, eps);
	double sq[2];
	
	#pragma omp parallel
	{
		int n = n0, k;
		double delta = 1;
		for(k = 0; delta > eps; n *= 2, k ^= 1){
			double h = (b - a) / n;
			double s = 0.0;
			sq[k] = 0;
			#pragma omp barrier
			
			#pragma omp for nowait
			for(int i = 0; i < n; i++)
				s += func(a + h * (i + 0.5));

			#pragma omp atomic
			sq[k] += s * h;

			#pragma omp barrier
			if(n > n0)
				delta = fabs(sq[k] - sq[k^1])/3.0;
		}
		#pragma omp master
		printf("Result: %.12f; EPS =  %e\n", sq[k], eps);
	}
	t = wtime() - t;
	printf	("Elapsed time (sec.): %.6f\n", t);
	
	return 0;
}
