#include <omp.h>
#include "simd.p5.h"

float Randf(float low, float high) {
  float r = (float)rand();

  return (low + r * (high - low) / (float)RAND_MAX);

}

void Mul(float *A, float *B, float *C, int len) {
    for (int i = 0; i < len; i++) {
        C[i] = A[i] * B[i];
    }
}

float MulSum(float *A, float *B, int len) {
    float sum = 0.0;
    for (int i = 0; i < len; i++) {
        sum += A[i] * B[i];
    }
    return sum;
}

int main(int argc, char **argv) {

  double sum_perf = 0.,
         max_perf = 0.;

  float *A = new float[LEN],
        *B = new float[LEN],
        *C = new float[LEN];

  for (int i = 0; i < LEN; i++) {
    A[i] = Randf(0.1f, 1.f);
    B[i] = Randf(0.1f, 1.f);
  }

  for (int i = 0; i < 100; i++) {

    double t1 = omp_get_wtime();
    #if   defined SSE_MULT
    SimdMul(A, B, C, LEN);
    #elif defined MULT
    Mul(A, B, C, LEN);
    #elif defined SSE_REDUCE
    float result = SimdMulSum(A, B, LEN);
    #else 
    float result = MulSum(A, B, LEN);
    #endif
    double t2 = omp_get_wtime();

    double curr_perf = (double)LEN/(t2 - t1)/1000000;

    if (max_perf < curr_perf)
      max_perf = curr_perf;
    sum_perf += curr_perf;
  }

  double avg_perf = sum_perf / 100;

  fprintf(stderr, "   Peak Performance: %8.2lf\n", max_perf);
  fprintf(stderr, "Average Performance: %8.2lf\n", avg_perf); 

  printf("%8.2lf", max_perf);
}

