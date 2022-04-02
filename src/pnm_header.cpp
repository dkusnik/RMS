
/** 
 * @file pnm_header.c
 * Routines for reading/writing PNM file headers
 */

#include "image.h"

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Reads the header of a raw PBM file
 *
 * @param[in,out] file_ptr File pointer
 * @param[out] num_rows # rows
 * @param[out] num_cols # columns
 *
 * @return E_SUCCESS or an appropriate error code
 *
 * @ref http://netpbm.sourceforge.net/doc/pbm.html

 * @author John Burkardt
 * @date 06.21.1999
 */

int
read_pbmb_header ( FILE * file_ptr, int *num_rows, int *num_cols )
{
 int c_val;
 int count;
 int flag;
 int nchar;
 int state;
 char string[MAX_LINE_LEN];

 /* Initialize the output parameters to impossible values */
 *num_rows = INT_MIN;
 *num_cols = INT_MIN;

 if ( IS_NULL ( file_ptr ) )
  {
   return E_FOPEN;
  }

 state = 0;
 nchar = 0;

 while ( 1 )
  {
   c_val = fgetc ( file_ptr );
   if ( c_val == EOF )
    {
     return E_FEOF;
    }

   /* If not whitespace, add the character to the current string. */
   flag = isspace ( c_val );

   /* Skip comment lines */
   if ( c_val == '#' )
    {
     while ( ( c_val = fgetc ( file_ptr ) ) != NEW_LINE )
      {
       /*empty */ ;
      }
     continue;
    }

   if ( !flag )
    {
     string[nchar] = ( char ) c_val;
     nchar = nchar + 1;
    }

   /* See if we have finished an old item, or begun a new one. */
   if ( state == 0 )
    {
     if ( !flag )
      {
       state = 1;
      }
     else
      {
       return E_FEOF;
      }
    }
   else if ( state == 1 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       if ( strcmp ( string, "P4" ) != 0 && strcmp ( string, "p4" ) != 0 )
	{
	 return E_UNFMT;
	}

       nchar = 0;
       state = 2;
      }
    }
   else if ( state == 2 )
    {
     if ( !flag )
      {
       state = 3;
      }
    }
   else if ( state == 3 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       count = sscanf ( string, "%d", num_cols );
       if ( count == EOF )
	{
	 return E_FEOF;
	}

       nchar = 0;
       state = 4;
      }
    }
   else if ( state == 4 )
    {
     if ( !flag )
      {
       state = 5;
      }
    }
   else if ( state == 5 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       count = sscanf ( string, "%d", num_rows );
       if ( count == EOF )
	{
	 return E_FEOF;
	}

       nchar = 0;

       return E_SUCCESS;
      }
    }
  }

 /*@notreached@ */
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

void
write_pbmb_header ( const int num_rows, const int num_cols, FILE * file_ptr )
{
 fprintf ( file_ptr, "P4\n%d %d\n", num_cols, num_rows );
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Reads the header of a raw PGM file
 *
 * @param[in,out] file_ptr File pointer
 * @param[out] num_rows # rows
 * @param[out] num_cols # columns
 * @param[out] max_gray Max gray value
 *
 * @return E_SUCCESS or an appropriate error code
 *
 * @note Only 8-bit raw PGM files are supported
 * @ref http://netpbm.sourceforge.net/doc/pgm.html

 * @author John Burkardt
 * @date 06.21.1999
 */

int
read_pgmb_header ( FILE * file_ptr, int *num_rows, int *num_cols,
		   int *max_gray )
{
 int c_val;
 int count;
 int flag;
 int nchar;
 int state;
 char string[MAX_LINE_LEN];

 /* Initialize the output parameters to impossible values */
 *num_rows = INT_MIN;
 *num_cols = INT_MIN;
 *max_gray = INT_MIN;

 if ( IS_NULL ( file_ptr ) )
  {
   return E_FOPEN;
  }

 state = 0;
 nchar = 0;

 while ( 1 )
  {
   c_val = fgetc ( file_ptr );

   if ( c_val == EOF )
    {
     return E_FEOF;
    }

   /* If not whitespace, add the character to the current string. */
   flag = isspace ( c_val );

   /* Skip comment lines */
   if ( c_val == '#' )
    {
     while ( ( c_val = fgetc ( file_ptr ) ) != NEW_LINE )
      {
       /*empty */ ;
      }
     continue;
    }

   if ( !flag )
    {
     string[nchar] = ( char ) c_val;
     nchar = nchar + 1;
    }

   /* See if we have finished an old item, or begun a new one. */
   if ( state == 0 )
    {
     if ( !flag )
      {
       state = 1;
      }
     else
      {
       return E_FEOF;
      }
    }
   else if ( state == 1 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       if ( strcmp ( string, "P5" ) != 0 && strcmp ( string, "p5" ) != 0 )
	{
	 return E_UNFMT;
	}

       nchar = 0;
       state = 2;
      }
    }
   else if ( state == 2 )
    {
     if ( !flag )
      {
       state = 3;
      }
    }
   else if ( state == 3 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       count = sscanf ( string, "%d", num_cols );
       if ( count == EOF )
	{
	 return E_FEOF;
	}

       nchar = 0;
       state = 4;
      }
    }
   else if ( state == 4 )
    {
     if ( !flag )
      {
       state = 5;
      }
    }
   else if ( state == 5 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       count = sscanf ( string, "%d", num_rows );
       if ( count == EOF )
	{
	 return E_FEOF;
	}

       nchar = 0;
       state = 6;
      }
    }
   else if ( state == 6 )
    {
     if ( !flag )
      {
       state = 7;
      }
    }
   else if ( state == 7 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       count = sscanf ( string, "%d", max_gray );
       if ( count == EOF )
	{
	 return E_FEOF;
	}

       nchar = 0;

       return E_SUCCESS;
      }
    }
  }

 /*@notreached@ */
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

void
write_pgmb_header ( const int num_rows, const int num_cols, const int max_gray,
		    FILE * file_ptr )
{
 fprintf ( file_ptr, "P5\n%d %d\n%d\n", num_cols, num_rows, max_gray );
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Reads the header of a raw PPM file
 *
 * @param[in,out] file_ptr File pointer
 * @param[out] num_rows # rows
 * @param[out] num_cols # columns
 * @param[out] max_rgb Max { R, G, B } value
 *
 * @return E_SUCCESS or an appropriate error code
 *
 * @note Only 24-bit raw PPM files are supported
 * @ref http://netpbm.sourceforge.net/doc/ppm.html

 * @author John Burkardt
 * @date 06.21.1999
 */

int
read_ppmb_header ( FILE * file_ptr, int *num_rows, int *num_cols, int *max_rgb )
{
 int c_val;
 int count;
 int flag;
 int nchar;
 int state;
 char string[MAX_LINE_LEN];

 /* Initialize the output parameters to impossible values */
 *num_rows = INT_MIN;
 *num_cols = INT_MIN;
 *max_rgb = INT_MIN;

 if ( IS_NULL ( file_ptr ) )
  {
   return E_FOPEN;
  }

 state = 0;
 nchar = 0;

 while ( 1 )
  {
   c_val = fgetc ( file_ptr );
   if ( c_val == EOF )
    {
     return E_FEOF;
    }

   /* If not whitespace, add the character to the current string. */
   flag = isspace ( c_val );

   /* Skip comment lines */
   if ( c_val == '#' )
    {
     while ( ( c_val = fgetc ( file_ptr ) ) != NEW_LINE )
      {
       /*empty */
      }
     continue;
    }

   if ( !flag )
    {
     string[nchar] = ( char ) c_val;
     nchar = nchar + 1;
    }

   /* See if we have finished an old item, or begun a new one. */
   if ( state == 0 )
    {
     if ( !flag )
      {
       state = 1;
      }
     else
      {
       return E_FEOF;
      }
    }
   else if ( state == 1 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       if ( strcmp ( string, "P6" ) != 0 && strcmp ( string, "p6" ) != 0 )
	{
	 return E_UNFMT;
	}

       nchar = 0;
       state = 2;
      }
    }
   else if ( state == 2 )
    {
     if ( !flag )
      {
       state = 3;
      }
    }
   else if ( state == 3 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       count = sscanf ( string, "%d", num_cols );
       if ( count == EOF )
	{
	 return E_FEOF;
	}

       nchar = 0;
       state = 4;
      }
    }
   else if ( state == 4 )
    {
     if ( !flag )
      {
       state = 5;
      }
    }
   else if ( state == 5 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       count = sscanf ( string, "%d", num_rows );
       if ( count == EOF )
	{
	 return E_FEOF;
	}

       nchar = 0;
       state = 6;
      }
    }
   else if ( state == 6 )
    {
     if ( !flag )
      {
       state = 7;
      }
    }
   else if ( state == 7 )
    {
     if ( flag )
      {
       string[nchar] = '\0';
       nchar = nchar + 1;

       count = sscanf ( string, "%d", max_rgb );
       if ( count == EOF )
	{
	 return E_FEOF;
	}

       nchar = 0;

       return E_SUCCESS;
      }
    }
  }

 /*@notreached@ */
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

void
write_ppmb_header ( const int num_rows, const int num_cols, const int max_rgb,
		    FILE * file_ptr )
{
 fprintf ( file_ptr, "P6\n%d %d\n%d\n", num_cols, num_rows, max_rgb );
}

/** @endcond INTERNAL_FUNCTION */
