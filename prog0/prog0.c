#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define ARRAYSIZE      (8 * 1024 * 1024)
#define NUMTRIES       100

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

int main(int argc, char **argv )
{
#ifndef _OPENMP
  fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
  return 1;
#endif
  if (argc < 2 || !isdigit(argv[1][0])) {
    printf("usage: prog [num threads]\n");
    return 1;
  }
  int numt = atoi(argv[1]);
  omp_set_num_threads( numt );
  fprintf( stderr, "Using %d threads\n", numt );

  double maxMegaMults = 0.;
  double sumMegaMults = 0.;

  for( int t = 0; t < NUMTRIES; t++ )
  {
    double time0 = omp_get_wtime( );

    #pragma omp parallel for
    for( int i = 0; i < ARRAYSIZE; i++ )
    {
      C[i] = A[i] * B[i];
    }

    double time1 = omp_get_wtime( );
    double megaMults = (double)ARRAYSIZE/(time1-time0)/1000000.;
    sumMegaMults += megaMults;
    if( megaMults > maxMegaMults )
      maxMegaMults = megaMults;
  }

  double avgMegaMults = sumMegaMults/(double)NUMTRIES;
  fprintf(stderr, "   Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults );
  fprintf(stderr, "Average Performance = %8.2lf MegaMults/Sec\n", avgMegaMults );
  printf("%8.2lf", avgMegaMults);
  // note: %lf stands for "long float", which is what printf calls a "double"
  //        %d stands for "decimal integer", not "double"

  return 0;
}
