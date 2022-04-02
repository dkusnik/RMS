
/** 
 * @file util.c
 * Routines for miscellaneous tasks
 */

#include "image.h"

/** 
 * @brief Returns the CPU time used so far as a clock_t
 *
 * @return CPU time (clock_t)
 *
 * @see #stop_timer
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

clock_t
start_timer ( void )
{
 return clock (  );
}

/** 
 * @brief Returns the CPU time (in seconds) used since START_TIME
 *
 * @param[in] start_time Return value of start_timer
 *
 * @return CPU time (in seconds)
 *
 * @see #start_timer
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

double
stop_timer ( const clock_t start_time )
{
 return ( ( double ) ( clock (  ) - start_time ) ) / CLOCKS_PER_SEC;
}

/** 
 * @brief Generates a 1D Gaussian mask
 *
 * @param[in] sigma Standard deviation { positive }
 * @param[out] mask_size Mask size
 *
 * @return Pointer to the mask or NULL
 *
 * @ref http://en.wikipedia.org/wiki/Normal_distribution
 *
 * @author M. Emre Celebi
 * @date 11.21.2006
 */

double *
gauss_1d ( const double sigma, int *mask_size )
{
 SET_FUNC_NAME ( "gauss_1d" );
 int ik;
 int center;			/* index of the mask center */
 double term;			/* constant */
 double mask_sum;		/* sum of the mask coefficients */
 double *mask;			/* mask array */

 if ( !IS_POS ( sigma ) )
  {
   ERROR ( "Sigma ( %f ) must be positive !", sigma );
   return NULL;
  }

 /* 
    99.73002039367% of the area under the curve 
    is within 3 standard deviations from the mean.
  */
 *mask_size = 1 + 2 * ( int ) ceil ( 3.0 * sigma );
 center = *mask_size / 2;

 /* Allocate storage for the mask */
 mask = ( double * ) malloc ( *mask_size * sizeof ( double ) );
 if ( IS_NULL ( mask ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 /* Constants in the Gaussian formula */
 term = -1.0 / ( 2.0 * sigma * sigma );

 /* Calculate the mask coefficients */
 mask_sum = 0.0;
 for ( ik = -center; ik <= center; ik++ )
  {
   mask[ik + center] = exp ( ik * ik * term );
   mask_sum += mask[ik + center];
  }

 /* Normalize the mask coefficients so that they add up to 1.0 */
 for ( ik = 0; ik < *mask_size; ik++ )
  {
   mask[ik] /= mask_sum;
  }

 return mask;
}

/** 
 * @brief Finds the Kth smallest of an array of numbers
 *
 * @param[in] num_elems # elements in the input
 * @param[in] k Value of K
 * @param[in,out] data Input array
 *
 * @return Kth smallest element in the input
 *
 * @note This routine modifies the input array !
 * @ref Fast median search: an ANSI C implementation
 *      http://ndevilla.free.fr/median/median/index.html 
 *
 * @author Nicolas Devillard
 * @date 11.21.2006
 */

int
select_kth_smallest ( const int num_elems, const int k, int *data )
{
 int tmp_var;
 int low, high;
 int middle, ll, hh;

 low = 0;
 high = num_elems - 1;
 while ( 1 )
  {
   if ( high <= low )
    {
     /* One element case */
     return data[k];
    }

   if ( high == low + 1 )
    {
     /* Two elements case */
     if ( data[low] > data[high] )
      {
       SWAP_INT ( data[low], data[high] );
      }

     return data[k];
    }

   /* Find median of low, middle and high items; swap into position low */
   middle = ( low + high ) / 2;
   if ( data[middle] > data[high] )
    {
     SWAP_INT ( data[middle], data[high] );
    }
   if ( data[low] > data[high] )
    {
     SWAP_INT ( data[low], data[high] );
    }
   if ( data[middle] > data[low] )
    {
     SWAP_INT ( data[middle], data[low] );
    }

   /* Swap low item (now in position middle) into position (low+1) */
   SWAP_INT ( data[middle], data[low + 1] );

   /* Nibble from each end towards middle, swapping items when stuck */
   ll = low + 1;
   hh = high;
   while ( 1 )
    {
     /* Scan up to find element > DATA[LOW] */
     do
      {
       ll++;
      }
     while ( data[low] > data[ll] );

     /* Scan down to find element < DATA[LOW] */
     do
      {
       hh--;
      }
     while ( data[hh] > data[low] );

     /* Pointer crossed. Partitioning complete. */
     if ( hh < ll )
      {
       break;
      }

     SWAP_INT ( data[ll], data[hh] );
    }

   /* Swap middle item (in position low) back into correct position */
   SWAP_INT ( data[low], data[hh] );

   /* Keep active the partition that contains the Kth element */
   if ( hh >= k )
    {
     high = hh - 1;
    }
   if ( hh <= k )
    {
     low = ll;
    }
  }
}

/** 
 * @brief Finds the median of an array of numbers
 *
 * @param[in] num_elems Number of elements in the input
 * @param[in,out] data Input array
 *
 * @return Median value in the input array
 *
 * @note 1) This routine modifies the input array !
 *       2) If NUM_ELEMS is even, the lower median (first  
 *          of the two middle values) is returned.
 * @ref Fast median search: an ANSI C implementation
 *      http://ndevilla.free.fr/median/median/index.html 
 *
 * @author Nicolas Devillard
 * @date 11.21.2006
 */

int
find_median ( const int num_elems, int *data )
{
 return select_kth_smallest ( num_elems, ( num_elems - 1 ) / 2, data );
}

/** 
 * @brief Sorts an integer array using Insertion Sort
 *
 * @param[in] num_elems Number of elements in the input
 * @param[in,out] data Input array
 *
 * @return Sorted array
 *
 * @note 1) This routine modifies the input array !
 *       2) Insertion Sort is preferable to Quicksort 
 *          for small input, i.e. num_elems < 100 
 *
 * @author M. Emre Celebi
 * @date 02.10.2008
 */

void
sort_int ( const int num_elems, int *data )
{
 int i, j;
 int key;

 for ( j = 1; j < num_elems; data[i + 1] = key, j++ )
  {
   for ( key = data[j], i = j - 1; i >= 0 && data[i] > key;
	 data[i + 1] = data[i], i-- );
  }
}
