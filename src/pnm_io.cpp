
/** 
 * @file pnm_io.c
 * Routines for reading/writing PNM pixel data
 */

#include "image.h"

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Reads pixel data of a raw PBM file
 *
 * @param[in] num_rows # rows
 * @param[in] num_cols # columns
 * @param[in,out] file_ptr File pointer
 *
 * @return Pointer to the image or NULL
 *
 * @ref http://netpbm.sourceforge.net/doc/pbm.html

 * @author M. Emre Celebi
 * @date 10.15.2006
 */

Image *
read_pbmb_data ( const int num_rows, const int num_cols, FILE * file_ptr )
{
 static int bit_masks[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
 byte one_byte;
 byte *tmp_data_1d;
 byte *data_1d;
 int num_bytes;
 int ir, ic, ik;
 int pos1;
 int pos2;
 Image *img;
 div_t div_res;

 img = alloc_img ( PIX_BIN, num_rows, num_cols );
 if ( IS_NULL ( img ) )
  {
   return NULL;
  }

 data_1d =(byte*)  get_img_data_1d ( img );

 num_bytes = num_rows * num_cols;
 tmp_data_1d = ( byte * ) calloc ( ( size_t ) num_bytes, sizeof ( byte ) );
 if ( IS_NULL ( tmp_data_1d ) )
  {
   return NULL;
  }

 /* Read the pixel data into a temporary 1d array */
 ( void ) fread ( tmp_data_1d, 1, ( size_t ) num_bytes, file_ptr );

 /* Determine the # bytes in a row */
 div_res = div ( num_cols, 8 );

 pos1 = 0;
 pos2 = 0;
 for ( ir = 0; ir < num_rows; ir++ )
  {
   for ( ic = 0; ic < div_res.quot; ic++ )
    {
     /* Get one byte */
     one_byte = tmp_data_1d[pos1++];

     /* Unpack it and store the bits */
     data_1d[pos2++] = !( one_byte & 0x80 );
     data_1d[pos2++] = !( one_byte & 0x40 );
     data_1d[pos2++] = !( one_byte & 0x20 );
     data_1d[pos2++] = !( one_byte & 0x10 );
     data_1d[pos2++] = !( one_byte & 0x08 );
     data_1d[pos2++] = !( one_byte & 0x04 );
     data_1d[pos2++] = !( one_byte & 0x02 );
     data_1d[pos2++] = !( one_byte & 0x01 );
    }

   if ( div_res.rem )
    {
     /* Get the last byte of this row */
     one_byte = tmp_data_1d[pos1++];

     /* Unpack it and store the bits */
     for ( ik = 0; ik < div_res.rem; ik++ )
      {
       data_1d[pos2++] = !( one_byte & bit_masks[7 - ik] );
      }
    }
  }

 free ( tmp_data_1d );

 return img;
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Writes pixel data of a raw PBM file
 *
 * @param[in] img Image pointer
 * @param[in,out] file_ptr File pointer
 *
 * @return E_SUCCESS or NULL
 *
 * @ref http://netpbm.sourceforge.net/doc/pbm.html
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
write_pbmb ( const Image * img, FILE * file_ptr )
{
 byte one_byte;
 byte *tmp_data_1d;
 byte *data_1d;
 int ir, ic, ik;
 int num_rows, num_cols;
 int num_cols_m1;
 int num_bytes;
 int col_size;
 int pos1, pos2;
 div_t div_res;

 num_rows = get_num_rows ( img );
 num_cols = get_num_cols ( img );
 num_cols_m1 = num_cols - 1;

 div_res = div ( num_cols, 8 );
 /* Determine column size in bytes */
 col_size = div_res.quot + ( div_res.rem > 0 );
 /* Determine total data size in bytes */
 num_bytes = num_rows * col_size;

 data_1d = (byte *) get_img_data_1d ( img );

 /* Allocate a linear array for storing pixel data contiguously */
 tmp_data_1d = ( byte * ) calloc ( ( size_t ) num_bytes, sizeof ( byte ) );
 if ( IS_NULL ( tmp_data_1d ) )
  {
   return E_NOMEM;
  }

 /* Unpack the pixels and store them into the image arrays */
 pos1 = 0;
 pos2 = 0;
 for ( ir = 0; ir < num_rows; ir++ )
  {
   for ( ic = 0; ic < div_res.quot; ic++ )
    {
     /* Pack 8 bits into one byte and store */
     tmp_data_1d[pos1++] = 255 - ( ( data_1d[pos2] << 7 ) |
				   ( data_1d[pos2 + 1] << 6 ) |
				   ( data_1d[pos2 + 2] << 5 ) |
				   ( data_1d[pos2 + 3] << 4 ) |
				   ( data_1d[pos2 + 4] << 3 ) |
				   ( data_1d[pos2 + 5] << 2 ) |
				   ( data_1d[pos2 + 6] << 1 ) |
				   ( data_1d[pos2 + 7] ) );

     pos2 += 8;
    }

   if ( div_res.rem )
    {
     /* Pack the last bits of this row into a byte */
     one_byte = 0;
     for ( ik = 0; ik < div_res.rem; ik++ )
      {
       one_byte |= ( ( data_1d[pos2] > 0 ) << ( 7 - ik ) );
       pos2++;
      }

     /* Store the last byte */
     tmp_data_1d[pos1++] = 255 - one_byte;
    }
  }

 /* Write image header */
 write_pbmb_header ( num_rows, num_cols, file_ptr );
 /* Write pixel data */
 ( void ) fwrite ( tmp_data_1d, 1, ( size_t ) num_bytes, file_ptr );

 free ( tmp_data_1d );

 return E_SUCCESS;
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Reads pixel data of a raw PGM file
 *
 * @param[in] num_rows # rows
 * @param[in] num_cols # columns
 * @param[in,out] file_ptr File pointer
 *
 * @return Pointer to the image or NULL
 *
 * @note Only 8-bit raw PGM files are supported
 * @ref http://netpbm.sourceforge.net/doc/pgm.html

 * @author M. Emre Celebi
 * @date 10.15.2006
 */

Image *
read_pgmb_data ( const int num_rows, const int num_cols, FILE * file_ptr )
{
 byte *data_1d;
 int num_bytes;
 Image *img;

 img = alloc_img ( PIX_GRAY, num_rows, num_cols );
 if ( IS_NULL ( img ) )
  {
   return NULL;
  }

 data_1d = (byte*) get_img_data_1d ( img );

 num_bytes = num_rows * num_cols;
 /* read the pixel data into the 1d array */
 ( void ) fread ( data_1d, 1, ( size_t ) num_bytes, file_ptr );

 return img;
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Writes pixel data of a raw PGM file
 *
 * @param[in] img Image pointer
 * @param[in,out] file_ptr File pointer
 *
 * @return E_SUCCESS if operation or E_INVBPP if image is not 8-bit
 *
 * @note Only 8-bit raw PGM files are supported
 * @ref http://netpbm.sourceforge.net/doc/pgm.html
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
write_pgmb ( const Image * img, FILE * file_ptr )
{
 byte *data_1d;
 int num_rows;
 int num_cols;
 int num_bytes;
 int max_gray;

 num_rows = get_num_rows ( img );
 num_cols = get_num_cols ( img );
 num_bytes = num_rows * num_cols;

 max_gray = img->max_pix_val;
 if ( !IS_BYTE ( max_gray ) )
  {
   return E_INVBPP;
  }

 data_1d = (byte *)get_img_data_1d ( img );

 /* Write image header */
 write_pgmb_header ( num_rows, num_cols, max_gray, file_ptr );
 /* Write pixel data */
 ( void ) fwrite ( data_1d, 1, ( size_t ) num_bytes, file_ptr );

 return E_SUCCESS;
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Reads pixel data of a raw PPM file
 *
 * @param[in] num_rows # rows
 * @param[in] num_cols # columns
 * @param[in,out] file_ptr File pointer
 *
 * @return Pointer to the image or NULL
 *
 * @note Only 24-bit raw PPM files are supported
 * @ref http://netpbm.sourceforge.net/doc/ppm.html

 * @author M. Emre Celebi
 * @date 10.15.2006
 */

Image *
read_ppmb_data ( const int num_rows, const int num_cols, FILE * file_ptr )
{
 byte *data_1d;
 int num_bytes;
 Image *img;

 img = alloc_img ( PIX_RGB, num_rows, num_cols );
 if ( IS_NULL ( img ) )
  {
   return NULL;
  }

 data_1d = (byte*)get_img_data_1d ( img );

 num_bytes = 3 * num_rows * num_cols;
 /* read the pixel data into the 1d array */
 ( void ) fread ( data_1d, 1, ( size_t ) num_bytes, file_ptr );

 return img;
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/** 
 * @brief Writes pixel data of a raw PPM file
 *
 * @param[in] img Image pointer
 * @param[in,out] file_ptr File pointer
 *
 * @return E_SUCCESS or E_INVBPP if image is not 24-bit
 *
 * @note Only 24-bit raw PPM files are supported
 * @ref http://netpbm.sourceforge.net/doc/ppm.html
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
write_ppmb ( const Image * img, FILE * file_ptr )
{
 byte *data_1d;
 int num_rows;
 int num_cols;
 int num_bytes;
 int max_rgb;

 num_rows = get_num_rows ( img );
 num_cols = get_num_cols ( img );
 num_bytes = 3 * num_rows * num_cols;

 max_rgb = img->max_pix_val;
 if ( !IS_BYTE ( max_rgb ) )
  {
   return E_INVBPP;
  }

 data_1d = (byte*) get_img_data_1d ( img );

 /* Write image header */
 write_ppmb_header ( num_rows, num_cols, max_rgb, file_ptr );
 /* Write pixel data */
 ( void ) fwrite ( data_1d, 1, ( size_t ) num_bytes, file_ptr );

 return E_SUCCESS;
}

/** @endcond INTERNAL_FUNCTION */
