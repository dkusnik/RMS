
/** 
 * @file alloc_nd.c
 * Routines for multi-dimensional array allocation/deallocation
 */

/*
* All questions regarding the software should be addressed to
* 
*       Prof. Charles A. Bouman
*       Purdue University
*       School of Electrical and Computer Engineering
*       1285 Electrical Engineering Building
*       West Lafayette, IN 47907-1285
*       USA
*       +1 765 494 0340
*       +1 765 494 3358 (fax)
*       email:  bouman@ecn.purdue.edu
*       http://www.ece.purdue.edu/~bouman
* 
* Copyright (c) 1995 The Board of Trustees of Purdue University.
*
* Permission to use, copy, modify, and distribute this software and its
* documentation for any purpose, without fee, and without written agreement is
* hereby granted, provided that the above copyright notice and the following
* two paragraphs appear in all copies of this software.
*
* IN NO EVENT SHALL PURDUE UNIVERSITY BE LIABLE TO ANY PARTY FOR DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
* USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF PURDUE UNIVERSITY HAS
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* PURDUE UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS,
* AND PURDUE UNIVERSITY HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
* UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#include "image.h"

/** 
 * @brief Allocates a DIM_COUNT dimensional array whose 
 *        dimensions are stored in a comma separated list
 *
 * @param[in] elem_size Size of an array element in bytes
 * @param[in] dim_count # dimensions
 * @param[in] ... Size of each dimension (comma separated)
 *
 * @return Pointer to the allocated memory or NULL
 *
 * @see #free_nd
 *
 * @author Charles A. Bouman
 * @date 6.29.1995
 */

void *
alloc_nd ( const size_t elem_size, const int dim_count, ... )
{
 char *pos_next;		/* see below */
 char *base_ptr;		/* base pointer to beginning of first array */
 char **tmp_ptr;		/* pointer to beginning of the array of the pointers for a dimension */
 char **pos_cur;		/* see below */
 int max;			/* size of array to be declared */
 int i, j;			/* loop counters */
 int *dims;			/* dimension list */
 int *dims_ptr;			/* pointer to dimension list */
 va_list arg_ptr;		/* varargs list traverser */

 if ( ( elem_size < 1 ) || ( dim_count < 1 ) )
  {
   return NULL;
  }

 va_start ( arg_ptr, dim_count );

 dims = ( int * ) calloc ( ( size_t ) dim_count, sizeof ( int ) );
 if ( IS_NULL ( dims ) )
  {
   return NULL;
  }

 for ( i = 0; i < dim_count; i++ )
  {
   dims[i] = va_arg ( arg_ptr, int );
  }

 /* Take care of 1-D case separately */
 if ( dim_count == 1 )
  {
   base_ptr = ( char * ) malloc ( dims[0] * elem_size * sizeof ( char ) );
   free ( dims );
   if ( IS_NULL ( base_ptr ) )
    {
     return NULL;
    }

   /* Return base pointer */
   return ( ( void * ) base_ptr );
  }

 tmp_ptr = &base_ptr;
 dims_ptr = dims;		/* First dimension */
 max = 1;

 /* For each of the dimensions but the last */
 for ( i = 0; i < dim_count - 1; i++ )
  {
   max *= ( *dims_ptr );
   tmp_ptr[0] = ( char * ) malloc ( max * sizeof ( char ** ) );
   if ( IS_NULL ( tmp_ptr ) )
    {
     free ( dims );
     return NULL;
    }

   /* Step through to beginning of next dimension array */
   tmp_ptr = ( char ** ) tmp_ptr[0];
   dims_ptr++;
  }

 /* Grab actual array memory */
 max *= elem_size * ( *dims_ptr );
 tmp_ptr[0] = ( char * ) calloc ( ( size_t ) max, sizeof ( char ) );
 if ( IS_NULL ( tmp_ptr[0] ) )
  {
   return NULL;
  }

 /*
  * tmp_ptr is now set to point to the beginning of each array so that we can
  * use it to scan down each array rather than having to go across and
  * then down 
  */
 tmp_ptr = ( char ** ) base_ptr;	/* Back to the beginning of list of arrays */
 dims_ptr = dims;		/* Back to the first dimension */
 max = 1;
 /* Deal with the last array of pointers later on */
 for ( i = 0; i < dim_count - 2; i++ )
  {
   max *= ( *dims_ptr );	/* Number of elements in this dimension */

   /* Scans down array for first and subsequent elements */
   pos_cur = tmp_ptr + 1;
   pos_next = tmp_ptr[0];
   for ( j = 1; j < max; j++ )
    {
     /* 
      * Modify each of the pointers so that it points to
      * the correct position (sub-array) of the next
      * dimension array. pos_cur is the current position in the
      * current array. pos_next is the current position in the
      * next array. pos_next is incremented before pos_cur is, 
      * but it starts off one behind. *( dims_ptr + 1 ) is the 
      * dimension of the next array. 
      */

     *pos_cur = ( pos_next += sizeof ( char ** ) * *( dims_ptr + 1 ) );
     pos_cur++;
    }

   /* Step through to begining of next dimension array */
   tmp_ptr = ( char ** ) tmp_ptr[0];
   dims_ptr++;
  }

 /* max is total number of elements in the last pointer array */
 max *= ( *dims_ptr );

 /* Same as previous loop, but different size factor */
 pos_cur = tmp_ptr + 1;
 pos_next = tmp_ptr[0];
 for ( j = 1; j < max; j++ )
  {
   *pos_cur++ = ( pos_next += elem_size * *( dims_ptr + 1 ) );
  }

 va_end ( arg_ptr );
 free ( dims );

 /* Return base pointer */
 return ( ( void * ) base_ptr );
}

/** 
 * @brief Deallocates a DIM_COUNT dimensional array that was 
 *        allocated using alloc_nd
 *
 * @param[in] base_ptr Pointer to the array to be freed
 * @param[in] dim_count # dimensions { positive }
 *
 * @see #alloc_nd
 *
 * @author Charles A. Bouman
 * @date 6.29.1995
 */

void
free_nd ( void *base_ptr, const int dim_count )
{
 void *next_ptr = NULL;
 void **tmp_ptr;
 int i;

 tmp_ptr = ( void ** ) base_ptr;
 for ( i = 0; i < dim_count; i++ )
  {
   if ( !IS_NULL ( tmp_ptr ) )
    {
     next_ptr = *tmp_ptr;
     free ( ( void * ) tmp_ptr );
    }

   tmp_ptr = ( void ** ) next_ptr;
  }
}
