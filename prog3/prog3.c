#include <cstdio>
#include <cstdlib>
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

  for (int k = 0; k < 10; k++) {

    double time_b = omp_get_wtime();

    #pragma omp parallel for default(none), shared(Array)
    for (int i = 0; i < 4; i++ ) {
      unsigned int seed = 0;
      for (unsigned int j = 0; j < large_num; j++) {
        Array[i].value = Array[i].value + (float)rand_r( &seed );
      }
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
