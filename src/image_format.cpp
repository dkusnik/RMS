
/** 
 * @file image_format.c
 * Routines for determining the format of an image file
 */

#include "image.h"

/** 
 * @brief Determines the format of an image file
 *
 * @param[in,out] file_ptr File pointer
 * @param[out] img_format File format code
 *
 * @return E_SUCCESS or an appropriate error code
 *         
 * @note If an error occurs, file format will be FMT_UNKNOWN
 * @see #img_format_str
 * @ref http://www.fileformat.info/format/cloud.htm
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
get_img_format ( FILE * file_ptr, ImageFormat * img_format )
{
 int magic[2];

 /* initialize the return value */
 *img_format = FMT_UNKNOWN;

 if ( IS_NULL ( file_ptr ) )
  {
   return E_FOPEN;
  }

 magic[0] = fgetc ( file_ptr );

 if ( magic[0] == 0 )		/* 00h */
  {
   *img_format = FMT_TGA;
   return E_SUCCESS;
  }
 else if ( magic[0] == 10 )	/* Ox0A */
  {
   *img_format = FMT_PCX;
   return E_SUCCESS;
  }
 else if ( magic[0] == EOF )
  {
   return E_FEOF;
  }

 magic[1] = fgetc ( file_ptr );
 if ( magic[1] == EOF )
  {
   return E_FEOF;
  }

 switch ( magic[0] )
  {
   case 'B':			/* BM */

    if ( magic[1] == 'M' )
     {
      *img_format = FMT_BMP;
      return E_SUCCESS;
     }
    break;

   case 255:			/* 0xffd8 */

    if ( magic[1] == 216 )
     {
      *img_format = FMT_JPG;
      return E_SUCCESS;
     }
    break;

   case 'I':			/* II */

    if ( magic[1] == 'I' )
     {
      *img_format = FMT_TIFF;
      return E_SUCCESS;
     }
    break;

   case 'M':			/* MM */

    if ( magic[1] == 'M' )
     {
      *img_format = FMT_TIFF;
      return E_SUCCESS;
     }
    break;

   case 'P':			/* P1 / P2 / P3 / P4 / P5 / P6 */

    switch ( magic[1] )
     {
      case '1':
       *img_format = FMT_PBMA;
       return E_SUCCESS;
      case '2':
       *img_format = FMT_PGMA;
       return E_SUCCESS;
      case '3':
       *img_format = FMT_PPMA;
       return E_SUCCESS;
      case '4':
       *img_format = FMT_PBM;
       return E_SUCCESS;
      case '5':
       *img_format = FMT_PGM;
       return E_SUCCESS;
      case '6':
       *img_format = FMT_PPM;
       return E_SUCCESS;
      default:
       break;
     }
    break;
   case 'G':			/* GIF */

    if ( magic[1] == 'I' && fgetc ( file_ptr ) == 'F' )
     {
      *img_format = FMT_GIF;
      return E_SUCCESS;
     }
    break;

   case '8':			/* 8BPS */

    if ( magic[1] == 'B' &&
	 fgetc ( file_ptr ) == 'P' && fgetc ( file_ptr ) == 'S' )
     {
      *img_format = FMT_PSD;
      return E_SUCCESS;
     }
    break;

   case 89:			/* 89 166 106 149 (59a66a95h) */

    if ( magic[1] == 166 &&
	 fgetc ( file_ptr ) == 106 && fgetc ( file_ptr ) == 149 )
     {
      *img_format = FMT_RAS;
      return E_SUCCESS;
     }
    break;

   case 137:			/* 137 80 78 71 13 10 26 10 */

    if ( magic[1] == 80 &&
	 fgetc ( file_ptr ) == 78 &&
	 fgetc ( file_ptr ) == 71 &&
	 fgetc ( file_ptr ) == 13 &&
	 fgetc ( file_ptr ) == 10 &&
	 fgetc ( file_ptr ) == 26 && fgetc ( file_ptr ) == 10 )
     {
      *img_format = FMT_PNG;
      return E_SUCCESS;
     }
    break;

   default:			/* return below */
    break;
  }

 return E_UNFMT;
}

/** 
 * @brief Returns the format string that corresponds to a file format code
 *
 * @param[in] img_format File format code
 *
 * @return File format (string)
 *
 * @see #get_img_format
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

const char *
img_format_str ( const ImageFormat img_format )
{
 switch ( img_format )
  {
   case FMT_BMP:
    return "BMP";
   case FMT_GIF:
    return "GIF";
   case FMT_JPG:
    return "JPG";
   case FMT_PBMA:
    return "Plain/ASCII PBM";
   case FMT_PBM:
    return "Raw/Binary PBM";
   case FMT_PCX:
    return "PCX";
   case FMT_PGMA:
    return "Plain/ASCII PGM";
   case FMT_PGM:
    return "Raw/Binary PGM";
   case FMT_PNG:
    return "PNG";
   case FMT_PPMA:
    return "Plain/ASCII PPM";
   case FMT_PPM:
    return "Raw/Binary PPM";
   case FMT_PSD:
    return "PSD";
   case FMT_RAS:
    return "RAS";
   case FMT_TGA:
    return "TGA";
   case FMT_TIFF:
    return "TIFF";
   case FMT_UNKNOWN:		/*@fallthrough@ */
   default:
    return "unknown";
  }

 /*@notreached@ */
}
