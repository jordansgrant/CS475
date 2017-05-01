#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <omp.h>

#define ARRAYSIZE 32 * 1024
#define NUMTRIES 10

float array[ARRAYSIZE];

float Randf(float low, float high) {
  float r = (float)rand();

  return (low + r * (high - low) / (float)RAND_MAX);

}


int main() {

  for (int i = 0; i < ARRAYSIZE; i++ ) 
    array[i] = Randf(-1.f, 1.f);

  omp_set_num_threads( NUMT );

  double maxMegaMults = 0.;
  double sumMegaMults = 0.;

  for (int j = 0; j < NUMTRIES; j++) { 

    double prod = 1.0;

    double time_b = omp_get_wtime();

    #pragma omp parallel for private(prod), schedule( SCHED_TYPE, CHUNK_SIZE )
    for (int i = 0; i < ARRAYSIZE - 1; i++ ) {
      for (int j = 0; j < i; j++) { 
        prod *= array[j];
      }
    }

    double time_e = omp_get_wtime();

    long int numMulted = (long int)ARRAYSIZE * (long int)(ARRAYSIZE+1) / 2;
    double megaMults = (double)numMulted/(time_e-time_b)/1000000.;

    if (megaMults > maxMegaMults)
      maxMegaMults = megaMults;
    sumMegaMults += megaMults;

  }

  double avgMegaMults = sumMegaMults / NUMTRIES;
  fprintf(stderr, "   Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
  fprintf(stderr, "Average Performance = %8.2lf MegaMults/Sec\n", avgMegaMults);

  printf("%8.2lf", maxMegaMults);
  fflush(stdout);
  return 0;
}

