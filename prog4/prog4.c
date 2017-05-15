#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int	NowYear;		// 2017 - 2022
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
float BlightPct;    // Percentage of grain thats blighted; poisonous to graindeer
int	NowNumDeer;		// number of deer in the current population

// Randomness function prototypes
float Ranf( unsigned int *seedp,  float low, float high );
int Ranf( unsigned int *seedp, int ilow, int ihigh );

// Thread Task Function prototypes
void GrainDeer();
void Grain();
void Watcher();
//void Blight();

int main() {
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

  const float BLIGHT_TEMP_MIN         =   60.0  // range of temps Blight prefers
  const float BLIGHT_TEMP_MAX         =   65.0
  const float BLIGHT_PRECIP_THRESH    =   8     // blight thrives in precip > 8

  // starting date and time:
  NowMonth                            =   0;
  NowYear                             =   2017;

  // starting state (feel free to change this if you want):
  NowNumDeer                          =   1;
  NowHeight                           =   1.;
  BlightPct                           =   0.;

  omp_set_num_threads( 4 );	// same as # of sections
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
/*
	#pragma omp section
	{
		MyAgent( );	// your own
	}*/
}       // implied barrier -- all functions must return in order
	// to allow any of them to get past here

}


float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}
