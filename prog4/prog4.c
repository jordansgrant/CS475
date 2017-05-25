#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef BLIGHT
#define NUMT   4
#else
#define NUMT   3
#endif

int	NowYear;		// 2017 - 2022
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
float BlightPct;    // Percentage of grain thats blighted; poisonous to graindeer
int	NowNumDeer;		// number of deer in the current population

// Some constant variables that effect the state each month
const float GRAIN_GROWS_PER_MONTH   =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH    =		6.0;	// average
const float AMP_PRECIP_PER_MONTH    =		6.0;	// plus or minus
const float RANDOM_PRECIP           =		2.0;	// plus or minus noise

const float AVG_TEMP                =		50.0;	// average
const float AMP_TEMP                =		20.0;	// plus or minus
const float RANDOM_TEMP             =	  10.0;	// plus or minus noise

const float MIDTEMP                 =		40.0;
const float MIDPRECIP               =		10.0;

const float BLIGHT_TEMP_MIN         =   40.0;  // range of temps Blight prefers
const float BLIGHT_TEMP_MAX         =   60.0;
const float BLIGHT_FREEZE           =   32.0;
const float BLIGHT_PRECIP_THRESH    =   7;     // blight thrives in precip > 7

// Randomness function prototypes
float Ranf( unsigned int *seedp,  float low, float high );
int Rani( unsigned int *seedp, int ilow, int ihigh );
float SQR( float x );

unsigned int seed = 0;

// Thread Task Function prototypes
void GrainDeer();
void Grain();
void Watcher();
void Blight();

int main() {

  // starting date and time:
  NowMonth                            =   0;
  NowYear                             =   2017;

  // starting state (feel free to change this if you want):
  NowNumDeer                          =   1;
  NowHeight                           =   1.;
  BlightPct                           =   0.;

  #ifdef BLIGHT
  printf("Date\tPrecip\tTemp\tHeight\tDeer\tBlight\n");
  #else
  printf("Date\tPrecip\tTemp\tHeight\tDeer\n");
  #endif

  omp_set_num_threads( NUMT );	// same as # of sections
  #pragma omp parallel sections
  {
    #pragma omp section
    {
      GrainDeer( );
    }

    #pragma omp section
    {
      Grain( );
    }

    #pragma omp section
    {
      Watcher( );
    }
    #ifdef BLIGHT
    #pragma omp section
    {
      Blight( );
    }
    #endif
}       // implied barrier -- all functions must return in order
// to allow any of them to get past here

}

void GrainDeer() {
  while( NowYear < 2023 )
  {
    // compute a temporary next-value for this quantity
    // based on the current state of the simulation:
    float height = NowHeight;
    int deer = NowNumDeer;

    if (deer > height)
      deer--;
    else if (deer < height)
      deer++;

    // DoneComputing barrier:
    #pragma omp barrier
    NowNumDeer = deer;

    // DoneAssigning barrier:
    #pragma omp barrier


    // DonePrinting barrier:
    #pragma omp barrier

  }
}

void Grain() {
  while( NowYear < 2023 )
  {
    // compute a temporary next-value for this quantity
    // based on the current state of the simulation:
    float height = NowHeight;

    float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
    float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

    height += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
    height -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

    #ifdef BLIGHT
    height *= (1 - (BlightPct/100));
    #endif

    // DoneComputing barrier:
    #pragma omp barrier
    NowHeight = height;

    // DoneAssigning barrier:
    #pragma omp barrier


    // DonePrinting barrier:
    #pragma omp barrier

  }
}

void Watcher() {
  while( NowYear < 2023 )
  {
    // compute a temporary next-value for this quantity
    // based on the current state of the simulation:

    // DoneComputing barrier:
    #pragma omp barrier

    // DoneAssigning barrier:
    #pragma omp barrier
    // Print information
    #ifdef BLIGHT
    printf("%d-%d\t%6.2f\t%6.2f\t%6.2f\t%d\t%5.4f\n", NowMonth+1, NowYear, NowPrecip, NowTemp, NowHeight, NowNumDeer, BlightPct);
    #else
    printf("%d-%d\t%6.2f\t%6.2f\t%6.2f\t%d\n", NowMonth+1, NowYear, NowPrecip, NowTemp, NowHeight, NowNumDeer);
    #endif
    // Increment Month/Year
    if (NowMonth == 11) {
      NowMonth = 0;
      NowYear++;
    }
    else
      NowMonth++;
    // Calculate seasonal temp effect
    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
    // calculate temperature
    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    // Calculate Precipitation
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
      NowPrecip = 0.;

    // DonePrinting barrier:
    #pragma omp barrier
  }
}

void Blight() {
  while( NowYear < 2023 )
  {
    // compute a temporary next-value for this quantity
    // based on the current state of the simulation:
    float blight = BlightPct;

    if (NowTemp <= BLIGHT_FREEZE)
      blight = 0.0;
    else if (NowTemp >= BLIGHT_TEMP_MIN && NowTemp <= BLIGHT_TEMP_MAX && NowPrecip > BLIGHT_PRECIP_THRESH) {
      blight += 20.0;
    } else if (NowTemp >= BLIGHT_TEMP_MIN && NowPrecip > BLIGHT_PRECIP_THRESH){
      blight += 15.0;
    } else if (NowTemp < BLIGHT_TEMP_MIN && NowPrecip > BLIGHT_PRECIP_THRESH) {
      blight -= 10.0;
    } else {
      blight -= 20.0;
    }

    if (blight > 100.0)
      blight = 100.0;
    else if (blight < 0.0)
      blight = 0.0;

    // DoneComputing barrier:
    #pragma omp barrier
    BlightPct = blight;

    // DoneAssigning barrier:
    #pragma omp barrier


    // DonePrinting barrier:
    #pragma omp barrier

  }
}


float Ranf( unsigned int *seedp,  float low, float high )
{
  float r = (float) rand_r( seedp );              // 0 - RAND_MAX

  return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int Rani( unsigned int *seedp, int ilow, int ihigh )
{
  float low = (float)ilow;
  float high = (float)ihigh + 0.9999f;

  return (int)(  Ranf(seedp, low,high) );
}

float SQR( float x )
{
        return x*x;
}
