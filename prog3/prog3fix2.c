#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct s
{
  float value;
  int pad[NUMPAD];
} Array[4];

int main() {

  omp_set_num_threads( NUMT );

  double max_perf;
  double sum_perf;

  const int large_num = 100000000;
  int k;

  for (k = 0; k < 10; k++) {
    int i;
    unsigned int j;
    double time_b = omp_get_wtime();

    #pragma omp parallel for default(none), shared(Array), private(j)
    for (i = 0; i < 4; i++ ) {
      unsigned int seed = 0;
      float sum = Array[i].value;
      for (j = 0; j < large_num; j++) {
        sum += (float)rand_r( &seed );
      }
      Array[i].value = sum;
    }

    double time_e = omp_get_wtime();
    double perf = (double)(4*large_num)/(time_e-time_b)/1000000;

    if (perf > max_perf)
      max_perf = perf;
    sum_perf += perf;
  }

  double avg_perf = sum_perf / 10;

  fprintf(stderr, "   Peak Performance = %8.2lf\n", max_perf);
  fprintf(stderr, "Average Performance = %8.2lf\n", avg_perf);

  printf("%8.2lf", max_perf);
  fflush(stdout);

  return 0;
}
