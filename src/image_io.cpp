
/** 
 * @file image_io.c
 * Routines for reading/writing BMP or raw PNM files
 */

#include "image.h"

/** 
 * @brief Reads a BMP, JPEG, or raw PNM file
 *
 * @param[in] file_name File name
 *
 * @return Pointer to the image or NULL
 *
 * @note The following file formats are supported: 1) raw PBM 2) 8-bit raw PGM 
         3) 24-bit raw PPM 4) 1,8, or 24-bit uncompressed BMP 5) JPEG
 * @todo Add raw image file support
 *
 * @author M. Emre Celebi
 * @date 04.30.2008
 */

Image *
read_img ( const char *file_name )
{
 SET_FUNC_NAME ( "read_img" );
 int is_bottom_up;		/* is the BMP file stored bottom-up or top-down ? */
 int ret_code;
 int num_rows, num_cols;
 int max_pix_val;
 FILE *file_ptr;
 ImageFormat img_format;
 PixelType pix_type;
 Image *img;

 file_ptr = fopen ( file_name, "rb" );
 if ( IS_NULL ( file_ptr ) )
  {
   ERROR ( "Cannot open file ( %s ) !", file_name );
   return NULL;
  }

 ret_code = get_img_format ( file_ptr, &img_format );
 if ( ret_code )
  {
   fclose ( file_ptr );
   ERROR_RET ( "Unsupported image file format !", NULL );
  }

 rewind ( file_ptr );

 img = NULL;			/* initialize return value */
 switch ( img_format )
  {
   case FMT_PBM:

    ret_code = read_pbmb_header ( file_ptr, &num_rows, &num_cols );
    if ( ret_code )
     {
      fclose ( file_ptr );
      ERROR ( "Cannot read header for raw PBM file ( %s ) !", file_name );
      return NULL;
     }

    img = read_pbmb_data ( num_rows, num_cols, file_ptr );
    if ( IS_NULL ( img ) )
     {
      fclose ( file_ptr );
      ERROR ( "Cannot read raw PBM file ( %s ) { %s } !",
	      file_name, error_str ( E_NOMEM ) );
     }
    break;

   case FMT_PGM:

    ret_code =
     read_pgmb_header ( file_ptr, &num_rows, &num_cols, &max_pix_val );
    if ( ret_code )
     {
      fclose ( file_ptr );
      ERROR ( "Cannot read header for raw PGM file ( %s ) !", file_name );
      return NULL;
     }

    if ( !IS_BYTE ( max_pix_val ) )
     {
      fclose ( file_ptr );
      ERROR
       ( "Cannot read raw PGM file ( %s ) having pixel depth other than 8 bits { %s } !",
	 file_name, error_str ( E_UNIMPL ) );
      return NULL;
     }

    img = read_pgmb_data ( num_rows, num_cols, file_ptr );
    if ( IS_NULL ( img ) )
     {
      fclose ( file_ptr );
      ERROR ( "Cannot read raw PGM file ( %s ) { %s } !",
	      file_name, error_str ( E_NOMEM ) );
     }
    break;

   case FMT_PPM:

    ret_code =
     read_ppmb_header ( file_ptr, &num_rows, &num_cols, &max_pix_val );
    if ( ret_code )
     {
      fclose ( file_ptr );
      ERROR ( "Cannot read header for raw PPM file ( %s ) !", file_name );
      return NULL;
     }

    if ( !IS_BYTE ( max_pix_val ) )
     {
      fclose ( file_ptr );
      ERROR
       ( "Cannot read raw PPM file ( %s ) having pixel depth other than 24 bits { %s } !",
	 file_name, error_str ( E_UNIMPL ) );
      return NULL;
     }

    img = read_ppmb_data ( num_rows, num_cols, file_ptr );
    if ( IS_NULL ( img ) )
     {
      fclose ( file_ptr );
      ERROR ( "Cannot read raw PPM file ( %s ) { %s } !",
	      file_name, error_str ( E_NOMEM ) );
     }
    break;
   case FMT_PNG:
	   img = read_png_file(file_ptr);
	   break;

   case FMT_UNKNOWN:		/*@fallthrough@ */

   default:
    fclose ( file_ptr );
    ERROR_RET ( "Unsupported image file format !", NULL );
  }

 fclose ( file_ptr );

 return img;
}

/** 
 * @brief Writes a BMP or raw PNM file
 *
 * @param[in] img Image pointer { binary, grayscale, rgb }
 * @param[in] file_name File name 
 * @param[in] img_format File format code { FMT_PBM, FMT_PGM, FMT_PPM, FMT_BMP }
 *
 * @return E_SUCCESS or an appropriate error code
 *
 * @note The following file formats are supported: 1) raw PBM 2) 8-bit raw PGM 
         3) 24-bit raw PPM 4) 1,8, or 24 bit uncompressed BMP
 * @todo Add raw image file support
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
write_img ( const Image * img, const char *file_name,
	    const ImageFormat img_format )
{
 SET_FUNC_NAME ( "write_img" );
 int ret_code;
 FILE *file_ptr;
 PixelType pix_type;

 if ( !IS_VALID_OBJ ( img ) )
  {
   ERROR_RET ( "Invalid image object !", E_INVOBJ );
  }

 if ( !is_byte_img ( img ) )
  {
   ERROR_RET ( "Not a byte image !", E_INVOBJ );
  }

 file_ptr = fopen ( file_name, "wb" );
 if ( IS_NULL ( file_ptr ) )
  {
   ERROR ( "Cannot open file ( %s ) !", file_name );
   return E_FOPEN;
  }

 pix_type = get_pix_type ( img );

 switch ( img_format )
  {
   case FMT_PBM:

    if ( pix_type != PIX_BIN )
     {
      fclose ( file_ptr );
      ERROR_RET ( "Not a binary image !", E_INVARG );
     }

    ret_code = write_pbmb ( img, file_ptr );
    if ( ret_code )
     {
      if ( ret_code == E_INVOBJ )
       {
	fclose ( file_ptr );
	ERROR_RET ( "Invalid image object !", E_INVOBJ );
       }
      else if ( ret_code == E_NOMEM )
       {
	fclose ( file_ptr );
	ERROR ( "Cannot write image to raw PBM file ( %s ) { %s } !",
		file_name, error_str ( E_NOMEM ) );
       }
      else
       {
	fclose ( file_ptr );
	ERROR ( "Cannot write image to raw PBM file ( %s ) !", file_name );
       }
     }

    return ret_code;

   case FMT_PGM:

    if ( pix_type != PIX_GRAY )
     {
      fclose ( file_ptr );
      ERROR_RET ( "Not a grayscale image !", E_INVARG );
     }

    ret_code = write_pgmb ( img, file_ptr );
    if ( ret_code )
     {
      fclose ( file_ptr );
      ERROR_RET ( "Invalid image object !", E_INVOBJ );
     }

    return ret_code;

   case FMT_PPM:

    if ( pix_type != PIX_RGB )
     {
      fclose ( file_ptr );
      ERROR_RET ( "Not an RGB image !", E_INVARG );
     }

    ret_code = write_ppmb ( img, file_ptr );
    if ( ret_code )
     {
      fclose ( file_ptr );
      ERROR_RET ( "Invalid image object !", E_INVOBJ );
     }

    return ret_code;
   case FMT_PNG:
	   ret_code = write_png_file(img, file_ptr);
	   if (ret_code)
	   {
		   fclose(file_ptr);
		   ERROR_RET("Invalid image object !", E_INVOBJ);
	   }
	   return ret_code;
   case FMT_UNKNOWN:		/*@fallthrough@ */

   default:
    fclose ( file_ptr );
    ERROR_RET ( "Unsupported image file format !", E_UNFMT );
  }

 /*@notreached@ */
}
