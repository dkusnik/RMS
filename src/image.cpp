
/** 
 * @file image.c
 * Routines for allocating/deallocation images, accessing image attributes,
   couting the # colors in an RGB image, RGB-to-luminance conversion, and 
   image negation
 */

#include "image.h"
extern "C" {
#include "iqa.h"
}
static int alloc_img_data ( Image * img );

/** 
 * @brief Checks whether or not the object is a binary image
 *
 * @param[in] img Image pointer
 *
 * @return true if object is a binary image;
 *         false otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
is_bin_img ( const Image * img )
{
 return ( !IS_NULL ( img ) && img->type == PIX_BIN );
}

/** 
 * @brief Checks whether or not the object is a grayscale image
 *
 * @param[in] img Image pointer
 *
 * @return true if object is a grayscale image;
 *         false otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
is_gray_img ( const Image * img )
{
 return ( !IS_NULL ( img ) && img->type == PIX_GRAY );
}

/** 
 * @brief Checks whether or not the object is an RGB image
 *
 * @param[in] img Image pointer
 *
 * @return true if object is an RGB image;
 *         false otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
is_rgb_img ( const Image * img )
{
 return ( !IS_NULL ( img ) && img->type == PIX_RGB );
}

/** 
 * @brief Checks whether or not the object is a byte image
 *
 * @param[in] img Image pointer
 *
 * @return true if object is a byte { binary, grayscale, rgb } image;
 *         false otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
is_byte_img ( const Image * img )
{
 return ( !IS_NULL ( img ) &&
	  ( ( img->type == PIX_BIN ) ||
	    ( img->type == PIX_GRAY ) || ( img->type == PIX_RGB ) ) );
}

/** 
 * @brief Checks whether or not the object is a label image
 *
 * @param[in] img Image pointer
 *
 * @return true if object is a label image;
 *         false otherwise
 *
 * @author M. Emre Celebi
 * @date 06.19.2007
 */

int
is_label_img ( const Image * img )
{
 return ( !IS_NULL ( img ) && ( img->type == PIX_INT_1B ) &&
	  ( img->num_cc > 0 ) );
}

/** 
 * @brief Checks whether or not the object is a 3-band double image
 *
 * @param[in] img Image pointer
 *
 * @return true if object is a 3-band double image;
 *         false otherwise
 *
 * @author M. Emre Celebi
 * @date 07.25.2007
 */

int
is_dbl_3b_img ( const Image * img )
{
 return ( !IS_NULL ( img ) && ( img->type == PIX_DBL_3B ) );
}

int
is_dbl_img ( const Image * img )
{
 return ( !IS_NULL ( img ) &&
	  ( ( img->type == PIX_DBL_1B ) || ( img->type == PIX_DBL_3B ) ) );
}

int
is_int_img ( const Image * img )
{
 return ( !IS_NULL ( img ) &&
	  ( ( img->type == PIX_INT_1B ) || ( img->type == PIX_INT_3B ) ) );
}

/** 
 * @brief Checks whether or not the object is a binary or label image
 *
 * @param[in] img Image pointer
 *
 * @return true if object is a binary or label image;
 *         false otherwise
 *
 * @author M. Emre Celebi
 * @date 11.08.2007
 */

int
is_bin_or_label_img ( const Image * img )
{
 return ( is_bin_img ( img ) || is_label_img ( img ) );
}

/** 
 * @brief Checks whether or not the object is a binary or grayscale image
 *
 * @param[in] img Image pointer
 *
 * @return true if object is a binary or grayscale image;
 *         false otherwise
 *
 * @author M. Emre Celebi
 * @date 11.08.2007
 */

int
is_bin_or_gray_img ( const Image * img )
{
 return ( is_bin_img ( img ) || is_gray_img ( img ) );
}

/** 
 * @brief Returns the pixel type of an image
 *
 * @param[in] img Image pointer
 *
 * @return Pixel type if the image is valid;
 *         PIX_INVALID otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

PixelType
get_pix_type ( const Image * img )
{
 SET_FUNC_NAME ( "get_pix_type" );

 if ( !IS_VALID_OBJ ( img ) )
  {
   /* Return an impossible value */
   ERROR_RET ( "Invalid image object !", PIX_INVALID );
  }

 return img->type;
}

/** 
 * @brief Returns the # bands of an image
 *
 * @param[in] img Image pointer
 *
 * @return # bands if the image is valid;
 *         INT_MIN otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
get_num_bands ( const Image * img )
{
 SET_FUNC_NAME ( "get_num_bands" );

 if ( !IS_VALID_OBJ ( img ) )
  {
   /* Return an impossible value */
   ERROR_RET ( "Invalid image object !", INT_MIN );
  }

 return img->num_bands;
}

static int
set_num_bands ( Image * img )
{
 SET_FUNC_NAME ( "set_num_bands" );

 switch ( get_pix_type ( img ) )
  {
   case PIX_BIN:		/*@fallthrough@ */

   case PIX_GRAY:		/*@fallthrough@ */

   case PIX_INT_1B:		/*@fallthrough@ */

   case PIX_DBL_1B:
    img->num_bands = 1;
    break;

   case PIX_RGB:		/*@fallthrough@ */

   case PIX_INT_3B:		/*@fallthrough@ */

   case PIX_DBL_3B:
    img->num_bands = 3;
    break;

   case PIX_INVALID:		/*@fallthrough@ */

   default:
    FATAL ( "Invalid pixel type !" );
    return INT_MIN;		/* Suppress compiler warning */
  }

 return img->num_bands;
}

/** 
 * @brief Returns the # rows of an image
 *
 * @param[in] img Image pointer
 *
 * @return # rows if image is valid;
 *         INT_MIN otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
get_num_rows ( const Image * img )
{
 SET_FUNC_NAME ( "get_num_rows" );

 if ( !IS_VALID_OBJ ( img ) )
  {
   /* Return an impossible value */
   ERROR_RET ( "Invalid image object !", INT_MIN );
  }

 return img->num_rows;
}

/** 
 * @brief Returns the # columns of an image
 *
 * @param[in] img Image pointer
 *
 * @return # columns if image is valid;
 *         INT_MIN otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
get_num_cols ( const Image * img )
{
 SET_FUNC_NAME ( "get_num_cols" );

 if ( !IS_VALID_OBJ ( img ) )
  {
   /* Return an impossible value */
   ERROR_RET ( "Invalid image object !", INT_MIN );
  }

 return img->num_cols;
}

/** 
 * @brief Returns the # connected components of a label image
 *
 * @param[in] img Image pointer { label }
 *
 * @return # connected components or INT_MIN
 *
 * @author M. Emre Celebi
 * @date 06.19.2007
 */

int
get_num_cc ( const Image * img )
{
 SET_FUNC_NAME ( "get_num_cc" );

 if ( !is_label_img ( img ) )
  {
   ERROR_RET ( "Not a label image !", INT_MIN );
  }

 return img->num_cc;
}

/** 
 * @brief Returns the 1-D pixel array of an image
 *
 * @param[in] img Image pointer
 *
 * @return 1-D pixel array if the image is valid;
 *         NULL otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

void *
get_img_data_1d ( const Image * img )
{
 SET_FUNC_NAME ( "get_img_data_1d" );

 if ( !IS_VALID_OBJ ( img ) )
  {
   ERROR_RET ( "Invalid image object !", NULL );
  }

 switch ( get_pix_type ( img ) )
  {
   case PIX_BIN:		/*@fallthrough@ */

   case PIX_GRAY:		/*@fallthrough@ */

   case PIX_RGB:
    return img->data_1d.byte_data;

   case PIX_INT_1B:		/*@fallthrough@ */

   case PIX_INT_3B:
    return img->data_1d.int_data;

   case PIX_DBL_1B:		/*@fallthrough@ */

   case PIX_DBL_3B:
    return img->data_1d.double_data;

   case PIX_INVALID:		/*@fallthrough@ */

   default:
    FATAL ( "Invalid pixel type !" );
    return NULL;		/* Suppress compiler warning */
  }

 /*@notreached@ */
}

/** 
 * @brief Returns the n-D pixel array of an image
 *
 * @param[in] img Image pointer
 *
 * @return n-D pixel array if image is valid;
 *         NULL otherwise
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

void *
get_img_data_nd ( const Image * img )
{
 SET_FUNC_NAME ( "get_img_data_nd" );

 if ( !IS_VALID_OBJ ( img ) )
  {
   ERROR_RET ( "Invalid image object !", NULL );
  }

 switch ( get_pix_type ( img ) )
  {
   case PIX_BIN:		/*@fallthrough@ */

   case PIX_GRAY:
    return img->data_nd.byte_data_1b;

   case PIX_RGB:
    return img->data_nd.byte_data_3b;

   case PIX_INT_1B:
    return img->data_nd.int_data_1b;

   case PIX_INT_3B:
    return img->data_nd.int_data_3b;

   case PIX_DBL_1B:
    return img->data_nd.double_data_1b;

   case PIX_DBL_3B:
    return img->data_nd.double_data_3b;

   case PIX_INVALID:		/*@fallthrough@ */

   default:
    FATAL ( "Invalid pixel type !" );
    return NULL;		/* Suppress compiler warning */
  }

 /*@notreached@ */
}

static int
alloc_img_data ( Image * img )
{
 SET_FUNC_NAME ( "alloc_img_data" );
 int num_rows, num_cols;
 int num_bands;

 num_rows = get_num_rows ( img );
 num_cols = get_num_cols ( img );
 num_bands = get_num_bands ( img );

 switch ( get_pix_type ( img ) )
  {
   case PIX_BIN:		/*@fallthrough@ */

   case PIX_GRAY:

    img->data_nd.byte_data_1b = (byte**)
     alloc_nd ( sizeof ( byte ), 2, num_rows, num_cols );
    if ( IS_NULL ( img->data_nd.byte_data_1b ) )
     {
      return E_NOMEM;
     }
    img->data_1d.byte_data = *( img->data_nd.byte_data_1b );
    img->max_pix_val = UCHAR_MAX;
    break;

   case PIX_RGB:

    img->data_nd.byte_data_3b = (byte ***)
     alloc_nd ( sizeof ( byte ), 3, num_rows, num_cols, num_bands );
    if ( IS_NULL ( img->data_nd.byte_data_3b ) )
     {
      return E_NOMEM;
     }
    img->data_1d.byte_data = **( img->data_nd.byte_data_3b );
    img->max_pix_val = UCHAR_MAX;
    break;

   case PIX_INT_1B:

    img->data_nd.int_data_1b = (int **)
     alloc_nd ( sizeof ( int ), 2, num_rows, num_cols );
    if ( IS_NULL ( img->data_nd.int_data_1b ) )
     {
      return E_NOMEM;
     }
    img->data_1d.int_data = *( img->data_nd.int_data_1b );
    img->max_pix_val = INT_MAX;
    break;

   case PIX_INT_3B:

    img->data_nd.int_data_3b = (int ***)
     alloc_nd ( sizeof ( int ), 3, num_rows, num_cols, num_bands );
    if ( IS_NULL ( img->data_nd.int_data_3b ) )
     {
      return E_NOMEM;
     }
    img->data_1d.int_data = **( img->data_nd.int_data_3b );
    img->max_pix_val = INT_MAX;
    break;

   case PIX_DBL_1B:

    img->data_nd.double_data_1b = (double **)
     alloc_nd ( sizeof ( double ), 2, num_rows, num_cols );
    if ( IS_NULL ( img->data_nd.double_data_1b ) )
     {
      return E_NOMEM;
     }
    img->data_1d.double_data = *( img->data_nd.double_data_1b );
    img->max_pix_val = INT_MAX;	/* type of MAX_PIX_VAL is int */
    break;

   case PIX_DBL_3B:

    img->data_nd.double_data_3b = (double ***)
     alloc_nd ( sizeof ( double ), 3, num_rows, num_cols, num_bands );
    if ( IS_NULL ( img->data_nd.double_data_3b ) )
     {
      return E_NOMEM;
     }
    img->data_1d.double_data = **( img->data_nd.double_data_3b );
    img->max_pix_val = INT_MAX;	/* type of MAX_PIX_VAL is int */
    break;

   case PIX_INVALID:		/*@fallthrough@ */

   default:
    FATAL ( "Invalid pixel type !" );
    return E_INVOBJ;		/* Suppress compiler warning */
  }

 return E_SUCCESS;
}

/** 
 * @brief Allocates an image object
 *
 * @param[in] pix_type Pixel type
 * @param[in] num_rows # rows { positive }
 * @param[in] num_cols # columns { positive }
 *
 * @return Pointer to the allocated image or NULL
 * 
 * @see #free_img
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

Image *
alloc_img ( const PixelType pix_type, const int num_rows, const int num_cols )
{
 SET_FUNC_NAME ( "alloc_img" );
 int num_bands;
 int ret_code;
 Image *img;

 /* Validate input parameters - BEGIN */
 if ( !pix_type )
  {
   ERROR_RET ( "Invalid pixel type !", NULL );
  }

 if ( num_rows <= 0 )
  {
   ERROR ( "Number of rows ( %d ) must be positive !", num_rows );
   return NULL;
  }

 if ( num_cols <= 0 )
  {
   ERROR ( "Number of columns ( %d ) must be positive !", num_cols );
   return NULL;
  }
 /* Validate input parameters - END */

 img = MALLOC_STRUCT ( Image );
 if ( IS_NULL ( img ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 img->type = pix_type;
 num_bands = set_num_bands ( img );
 img->num_rows = num_rows;
 img->num_cols = num_cols;

 /* The number of connected components will be updated after labeling */
 img->num_cc = INT_MIN;

 ret_code = alloc_img_data ( img );
 if ( ret_code )
  {
   free ( img );
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 return img;
}

/** 
 * @brief Deallocates an image object
 *
 * @param[in,out] img Image pointer
 *
 * @note nothing happens if the image object is invalid
 * @see #alloc_img
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

void
free_img ( Image * img )
{
 SET_FUNC_NAME ( "free_img" );

 if ( IS_VALID_OBJ ( img ) )
  {
   /* Free the pixel data */
   switch ( get_pix_type ( img ) )
    {
     case PIX_BIN:		/*@fallthrough@ */

     case PIX_GRAY:

      free_nd ( img->data_nd.byte_data_1b, 2 );
      img->data_nd.byte_data_1b = NULL;
      img->data_1d.byte_data = NULL;
      break;

     case PIX_RGB:

      free_nd ( img->data_nd.byte_data_3b, 3 );
      img->data_nd.byte_data_3b = NULL;
      img->data_1d.byte_data = NULL;
      break;

     case PIX_INT_1B:

      free_nd ( img->data_nd.int_data_1b, 2 );
      img->data_nd.int_data_1b = NULL;
      img->data_1d.int_data = NULL;
      break;

     case PIX_INT_3B:

      free_nd ( img->data_nd.int_data_3b, 3 );
      img->data_nd.int_data_3b = NULL;
      img->data_1d.int_data = NULL;
      break;

     case PIX_DBL_1B:

      free_nd ( img->data_nd.double_data_1b, 2 );
      img->data_nd.double_data_1b = NULL;
      img->data_1d.double_data = NULL;
      break;

     case PIX_DBL_3B:

      free_nd ( img->data_nd.double_data_3b, 3 );
      img->data_nd.double_data_3b = NULL;
      img->data_1d.double_data = NULL;
      break;

     case PIX_INVALID:		/*@fallthrough@ */

     default:
      FATAL ( "Invalid pixel type !" );
    }

   /* Set each field to an impossible value */
   img->type = PIX_INVALID;
   img->num_bands = INT_MIN;
   img->num_rows = INT_MIN;
   img->num_cols = INT_MIN;
   img->max_pix_val = INT_MIN;
   img->num_cc = INT_MIN;
  }
}

/** 
 * @brief Determines the # unique colors in an RGB image
 *
 * @param[in] img Image pointer { rgb }
 *
 * @return # unique colors or INT_MIN
 * 
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
count_colors ( const Image * img )
{
 SET_FUNC_NAME ( "count_colors" );
 static int bit_masks[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
 byte *data_1d;
 byte *colors;
 byte *byte_ptr;
 int i;
 int num_elems;
 int num_colors;
 int max_entries = 2097153;	/* 2MB memory */
 static int bit_counts[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
 unsigned long rgb_pack;

 if ( !is_rgb_img ( img ) )
  {
   ERROR_RET ( "Not an RGB image !", INT_MIN );
  }

 data_1d = (byte *) get_img_data_1d ( img );

 colors = ( byte * ) calloc ( ( size_t ) max_entries, sizeof ( byte ) );
 if ( IS_NULL ( colors ) )
  {
   ERROR_RET ( "Insufficient memory !", INT_MIN );
  }

 num_elems = 3 * get_num_rows ( img ) * get_num_cols ( img );

 for ( i = 0; i < num_elems; i += 3 )
  {
   rgb_pack = ( ( ( ( data_1d[i] & 0xff ) << 8 ) |
		  ( data_1d[i + 1] & 0xff ) ) << 8 ) |
    ( data_1d[i + 2] & 0xff );
   colors[rgb_pack >> 3] |= bit_masks[rgb_pack & 7];
  }

 num_colors = 0;
 for ( byte_ptr = colors; byte_ptr < colors + max_entries; byte_ptr++ )
  {
   if ( *byte_ptr )
    {
     /* count the number of "on" bits ( 4 bits at a time ) */
     num_colors += bit_counts[*byte_ptr & 0x0f];
     *byte_ptr >>= 4;
     num_colors += bit_counts[*byte_ptr & 0x0f];
    }
  }

 free ( colors );

 return num_colors;
}

/** 
 * @brief Converts an RGB image to a luminance image
 *
 * @param[in] rgb_img Image pointer { rgb }
 *
 * @return Pointer to the luminance image or NULL
 * 
 * @note Uses the Rec. 601 (NTSC 1953) standard.
 * @todo Provide an option to use the Rec. 709 standard.
 *
 * @ref Poynton C. "Frequently-Asked Questions about Color" 
 *      http://www.poynton.com/ColorFAQ.html 
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

Image *
rgb_to_gray ( const Image * rgb_img )
{
 SET_FUNC_NAME ( "rgb_to_gray" );
 byte *gray_data;
 byte *rgb_data;
 int i;
 int pos;
 int num_rows, num_cols;
 int num_elems;
 Image *gray_img;

 if ( !is_rgb_img ( rgb_img ) )
  {
   ERROR_RET ( "Not an RGB image !", NULL );
  }

 rgb_data = (byte *) get_img_data_1d ( rgb_img );

 num_rows = get_num_rows ( rgb_img );
 num_cols = get_num_cols ( rgb_img );

 gray_img = alloc_img ( PIX_GRAY, num_rows, num_cols );
 if ( IS_NULL ( gray_img ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 gray_data = (byte *) get_img_data_1d ( gray_img );

 num_elems = 3 * num_rows * num_cols;
 pos = 0;
 for ( i = 0; i < num_elems; i += 3 )
  {
   gray_data[pos++] = 0.29893602129378 * rgb_data[i] +
    0.58704307445112 * rgb_data[i + 1] + 0.11402090425510 * rgb_data[i + 2];
  }

 return gray_img;
}

/** 
 * @brief Negates an image
 *
 * @param[in] in_img Image pointer { binary, grayscale, rgb }
 *
 * @return Pointer to the negated image or NULL
 * 
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

Image *
negate_img ( const Image * in_img )
{
 SET_FUNC_NAME ( "negate_img" );
 byte max_val;
 byte *in_data;
 byte *out_data;
 int i;
 int num_rows, num_cols;
 int num_elems;
 PixelType pix_type;
 Image *out_img;

 if ( !is_byte_img ( in_img ) )
  {
   ERROR_RET ( "Not a byte image !", NULL );
  }

 in_data = (byte * ) get_img_data_1d ( in_img );

 num_rows = get_num_rows ( in_img );
 num_cols = get_num_cols ( in_img );
 pix_type = get_pix_type ( in_img );

 out_img = alloc_img ( pix_type, num_rows, num_cols );
 if ( IS_NULL ( out_img ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 out_data = (byte *) get_img_data_1d ( out_img );

 /* Default values for PIX_GRAY */
 num_elems = num_rows * num_cols;
 max_val = MAX_GRAY;

 if ( pix_type == PIX_BIN )
  {
   max_val = 1;
  }
 else if ( pix_type == PIX_RGB )
  {
   num_elems *= 3;		/* 3 samples per pixel */
  }

 /* Negate the pixels */
 for ( i = 0; i < num_elems; i++ )
  {
   out_data[i] = max_val - in_data[i];
  }

 return out_img;
}

/** 
 * @brief Checks whether or not two images have the same dimensions 
 *
 * @param[in] img_a Image #1
 * @param[in] img_b Image #2
 *
 * @return true or false
 *
 * @author M. Emre Celebi
 * @date 01.20.2008
 */

int
img_dims_agree ( const Image * img_a, const Image * img_b )
{
 SET_FUNC_NAME ( "img_dims_agree" );

 if ( !IS_VALID_OBJ ( img_a ) || !IS_VALID_OBJ ( img_b ) )
  {
   ERROR_RET ( "Invalid image object(s) !", 0 );
  }

 return ( ( get_num_rows ( img_a ) == get_num_rows ( img_b ) ) &&
	  ( get_num_cols ( img_a ) == get_num_cols ( img_b ) ) );
}

/** 
 * @brief Checks whether or not two images are of the same type
 *
 * @param[in] img_a Image #1
 * @param[in] img_b Image #2
 *
 * @return true or false
 *
 * @author M. Emre Celebi
 * @date 01.20.2008
 */

int
img_types_agree ( const Image * img_a, const Image * img_b )
{
 SET_FUNC_NAME ( "img_types_agree" );

 if ( !IS_VALID_OBJ ( img_a ) || !IS_VALID_OBJ ( img_b ) )
  {
   ERROR_RET ( "Invalid image object(s) !", 0 );
  }

 return ( get_pix_type ( img_a ) == get_pix_type ( img_b ) );
}

/** 
 * @brief Checks whether or not two images have the same data
 *
 * @param[in] in_img_a Image #1 { byte }
 * @param[in] in_img_b Image #2 { byte }
 *
 * @return true or false
 *
 * @author M. Emre Celebi
 * @date 01.20.2008
 */

int
is_equal_img ( const Image * in_img_a, const Image * in_img_b )
{
 SET_FUNC_NAME ( "is_equal_img" );
 byte *in_data_a;
 byte *in_data_b;
 int ik;
 int num_elems;

 if ( !img_dims_agree ( in_img_a, in_img_b ) )
  {
   ERROR_RET ( "Image dimensions must agree !", 0  );
  }

 if ( !img_types_agree ( in_img_a, in_img_b ) )
  {
   ERROR_RET ( "Image types must agree !", 0 );
  }

 if ( !is_byte_img ( in_img_a ) )
  {
   ERROR_RET ( "Unsupported pixel type !", 0 );
  }

 num_elems =
  get_num_bands ( in_img_a ) * get_num_rows ( in_img_a ) *
  get_num_cols ( in_img_a );

 in_data_a = (byte *) get_img_data_1d ( in_img_a );
 in_data_b = (byte *) get_img_data_1d ( in_img_b );

 for ( ik = 0; ik < num_elems; ik++ )
  {
   if ( in_data_a[ik] != in_data_b[ik] )
    {
     return 0;
    }
  }

 return 1;
}

/** 
 * @brief Converts a double image to a byte image
 *
 * @param[in] in_img Image pointer { double }
 *
 * @return Pointer to the byte image or NULL
 * @note If the value of an output pixel is negative or greater 
 *       than MAX_GRAY, it is set to 0 or MAX_GRAY, respectively.
 *
 * @author M. Emre Celebi
 * @date 01.20.2008
 */

Image *
dbl_to_byte_img ( const Image * in_img )
{
 SET_FUNC_NAME ( "dbl_to_byte_img" );
 byte *out_data;
 int ik;
 int value;
 int num_rows, num_cols;
 int num_elems;
 int num_bands;
 double *in_data;
 Image *out_img;

 if ( !is_dbl_img ( in_img ) )
  {
   ERROR_RET ( "Not a double image !", NULL );
  }

 num_rows = get_num_rows ( in_img );
 num_cols = get_num_cols ( in_img );
 num_bands = get_num_bands ( in_img );
 num_elems = num_bands * num_rows * num_cols;

 in_data = (double *) get_img_data_1d ( in_img );

 /* Allocate a grayscale or rgb image depending on the input image type */
 out_img =
  alloc_img ( num_bands == 1 ? PIX_GRAY : PIX_RGB, num_rows, num_cols );
 if ( IS_NULL ( out_img ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 out_data = (byte *) get_img_data_1d ( out_img );

 for ( ik = 0; ik < num_elems; ik++ )
  {
   value = ( int ) ROUND ( in_data[ik] );
   out_data[ik] = CLAMP_BYTE ( value );
  }

 return out_img;
}

Image *
byte_to_dbl_img ( const Image * in_img )
{
 SET_FUNC_NAME ( "byte_to_dbl_img" );
 byte *in_data;
 int ik;
 int num_rows, num_cols;
 int num_elems;
 int num_bands;
 double *out_data;
 Image *out_img;

 if ( !is_byte_img ( in_img ) )
  {
   ERROR_RET ( "Not a byte image !", NULL );
  }

 num_rows = get_num_rows ( in_img );
 num_cols = get_num_cols ( in_img );
 num_bands = get_num_bands ( in_img );
 num_elems = num_bands * num_rows * num_cols;

 in_data = (byte *) get_img_data_1d ( in_img );

 /* Allocate a grayscale or rgb image depending on the input image type */
 out_img =
  alloc_img ( num_bands == 1 ? PIX_DBL_1B : PIX_DBL_3B, num_rows, num_cols );
 if ( IS_NULL ( out_img ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 out_data = (double *) get_img_data_1d ( out_img );

 for ( ik = 0; ik < num_elems; ik++ )
  {
   out_data[ik] = in_data[ik];
  }

 return out_img;
}

/** 
 * @brief Extracts the individual bands of an RGB image
 *
 * @param[in] rgb_img Input image pointer { rgb }
 * @param[in,out] red_img Red band pointer
 * @param[in,out] green_img Green band pointer
 * @param[in,out] blue_img Blue band pointer
 *
 * @return none
 *
 * @author M. Emre Celebi
 * @date 02.10.2008
 */

void
get_rgb_bands ( const Image * rgb_img, Image ** red_img, Image ** green_img,
		Image ** blue_img )
{
 SET_FUNC_NAME ( "get_rgb_bands" );
 byte *rgb_data;
 byte *red_data, *green_data, *blue_data;
 int num_rows, num_cols;
 int num_pixels_t3;
 int index;
 int ik;

 *red_img = *green_img = *blue_img = NULL;

 if ( !is_rgb_img ( rgb_img ) )
  {
   ERROR ( "Not an RGB image !" );
   return;
  }

 num_rows = get_num_rows ( rgb_img );
 num_cols = get_num_cols ( rgb_img );
 num_pixels_t3 = 3 * num_rows * num_cols;

 rgb_data = (byte *) get_img_data_1d ( rgb_img );

 *red_img = alloc_img ( PIX_GRAY, num_rows, num_cols );
 *green_img = alloc_img ( PIX_GRAY, num_rows, num_cols );
 *blue_img = alloc_img ( PIX_GRAY, num_rows, num_cols );

 if ( IS_NULL ( *red_img ) || IS_NULL ( *green_img ) || IS_NULL ( *blue_img ) )
  {
   ERROR ( "Insufficient memory !" );
   return;
  }

 red_data = (byte*) get_img_data_1d ( *red_img );
 green_data = (byte*)get_img_data_1d ( *green_img );
 blue_data = (byte*)get_img_data_1d ( *blue_img );

 index = 0;
 for ( ik = 0; ik < num_pixels_t3; ik += 3 )
  {
   red_data[index] = rgb_data[ik];
   green_data[index] = rgb_data[ik + 1];
   blue_data[index] = rgb_data[ik + 2];
   index++;
  }
}

/** 
 * @brief Combines the individual bands of an RGB image
 *
 * @param[in] red_img Red band pointer { grayscale }
 * @param[in] green_img Green band pointer { grayscale }
 * @param[in] blue_img Blue band pointer { grayscale }
 *
 * @return Pointer to the RGB image or NULL
 *
 * @author M. Emre Celebi
 * @date 02.10.2008
 */

Image *
combine_rgb_bands ( const Image * red_img, const Image * green_img,
		    const Image * blue_img )
{
 SET_FUNC_NAME ( "combine_rgb_bands" );
 byte *rgb_data;
 byte *red_data, *green_data, *blue_data;
 int num_rows, num_cols;
 int num_pixels_t3;
 int index;
 int ik;
 Image *rgb_img;

 if ( !is_gray_img ( red_img ) ||
      !is_gray_img ( green_img ) || !is_gray_img ( blue_img ) )
  {
   ERROR_RET ( "Band images must be grayscale !", NULL );
  }

 if ( !img_dims_agree ( red_img, green_img ) ||
      !img_dims_agree ( green_img, blue_img ) )
  {
   ERROR_RET ( "Dimensions of the band images must agree !", NULL );
  }

 num_rows = get_num_rows ( red_img );
 num_cols = get_num_cols ( red_img );
 num_pixels_t3 = 3 * num_rows * num_cols;

 rgb_img = alloc_img ( PIX_RGB, num_rows, num_cols );
 if ( IS_NULL ( rgb_img ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 rgb_data = (byte *)get_img_data_1d ( rgb_img );

 red_data = (byte *)get_img_data_1d ( red_img );
 green_data = (byte *)get_img_data_1d ( green_img );
 blue_data = (byte *)get_img_data_1d ( blue_img );

 index = 0;
 for ( ik = 0; ik < num_pixels_t3; ik += 3 )
  {
   rgb_data[ik] = red_data[index];
   rgb_data[ik + 1] = green_data[index];
   rgb_data[ik + 2] = blue_data[index];
   index++;
  }

 return rgb_img;
}

/** 
 * @brief Clones an image
 *
 * @param[in] in_img Image pointer
 *
 * @return Pointer to the clone image or NULL
 *
 * @author M. Emre Celebi
 * @date 02.10.2008
 */

Image *
clone_img ( const Image * in_img )
{
 SET_FUNC_NAME ( "clone_img" );
 int num_rows, num_cols;
 size_t num_bytes;
 PixelType pix_type;
 Image *out_img;

 if ( !IS_VALID_OBJ ( in_img ) )
  {
   ERROR_RET ( "Invalid image object !", NULL );
  }

 pix_type = get_pix_type ( in_img );
 num_rows = get_num_rows ( in_img );
 num_cols = get_num_cols ( in_img );

 out_img = alloc_img ( pix_type, num_rows, num_cols );
 if ( IS_NULL ( out_img ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 num_bytes = get_num_bands ( in_img ) * num_rows * num_cols;

 switch ( pix_type )
  {
   case PIX_BIN:		/*@fallthrough@ */

   case PIX_GRAY:		/*@fallthrough@ */

   case PIX_RGB:

    memcpy ( ( byte * ) get_img_data_1d ( out_img ),
	     ( byte * ) get_img_data_1d ( in_img ), num_bytes );
    break;

   case PIX_INT_1B:		/*@fallthrough@ */

   case PIX_INT_3B:

    memcpy ( ( int * ) get_img_data_1d ( out_img ),
	     ( int * ) get_img_data_1d ( in_img ), num_bytes * sizeof ( int ) );
    break;

   case PIX_DBL_1B:		/*@fallthrough@ */

   case PIX_DBL_3B:

    memcpy ( ( double * ) get_img_data_1d ( out_img ),
	     ( double * ) get_img_data_1d ( in_img ),
	     num_bytes * sizeof ( double ) );
    break;

   default:

    ERROR ( "Invalid pixel type ( %d ) !", pix_type );
    return NULL;
  }

 return out_img;
}

/** 
 * @brief Allocates an image with a constant value
 *
 * @param[in] pix_type Pixel type
 * @param[in] num_rows Number of rows { positive }
 * @param[in] num_cols Number of rows { positive }
 * @param[in] value Initial value for the pixels
 *
 * @return Pointer to the resulting image or NULL
 *
 * @author M. Emre Celebi
 * @date 02.10.2008
 */

Image *
alloc_const_img ( const PixelType pix_type, const int num_rows,
		  const int num_cols, const double value )
{
 SET_FUNC_NAME ( "alloc_const_img" );
 size_t num_elems;
 Image *out_img;

 if ( !pix_type )
  {
   ERROR ( "Invalid pixel type ( %d ) !", pix_type );
   return NULL;
  }

 if ( num_rows <= 0 || num_cols <= 0 )
  {
   ERROR ( "Image dimensions ( %d, %d ) must be positive !", num_rows,
	   num_cols );
   return NULL;
  }

 out_img = alloc_img ( pix_type, num_rows, num_cols );
 if ( IS_NULL ( out_img ) )
  {
   ERROR_RET ( "Insufficient memory !", NULL );
  }

 num_elems = num_rows * num_cols;

 switch ( pix_type )
  {
   case PIX_BIN:		/*@fallthrough@ */

   case PIX_GRAY:		/*@fallthrough@ */

   case PIX_RGB:

    if ( !IS_BYTE ( value ) )
     {
      ERROR ( "Pixel value ( %f ) for byte images must be in [0,%d] !", value,
	      MAX_GRAY );
      return NULL;
     }

    num_elems *= ( pix_type == PIX_RGB ? 3 : 1 );
    memset ( ( byte * ) get_img_data_1d ( out_img ), ( int ) value, num_elems );

    break;

   case PIX_INT_1B:		/*@fallthrough@ */

   case PIX_INT_3B:

    if ( value < INT_MIN || value > INT_MAX )
     {
      ERROR ( "Pixel value ( %f ) for int images must be in [%d,%d] !", value,
	      INT_MIN, INT_MAX );
      return NULL;
     }

    num_elems *= ( pix_type == PIX_INT_3B ? 3 : 1 );

    if ( IS_BYTE ( value ) )	/* Quicker way to set the values */
     {
      memset ( ( int * ) get_img_data_1d ( out_img ), ( int ) value,
	       num_elems * sizeof ( int ) );
     }
    else
     {
      int ik;
      int *out_data = (int*) get_img_data_1d ( out_img );

      for ( ik = 0; ik < num_elems; ik++ )
       {
	out_data[ik] = value;
       }
     }

    break;

   case PIX_DBL_1B:		/*@fallthrough@ */

   case PIX_DBL_3B:

    {
     int ik;
     double *out_data = (double *)get_img_data_1d ( out_img );

     num_elems *= ( pix_type == PIX_DBL_3B ? 3 : 1 );

     for ( ik = 0; ik < num_elems; ik++ )
      {
       out_data[ik] = value;
      }
    }

    break;

   default:

    ERROR ( "Invalid pixel type ( %d ) !", pix_type );
    return NULL;
  }

 return out_img;
}

void normalize(float* input_array1d, int length) {

	int max = 0;
	for (int i = 0; i < length; i++)
		if (input_array1d[i] > max)
			max = input_array1d[i];

	for (int i = 0; i < length; i++)
		input_array1d[i] = (input_array1d[i] / max) * 255;
}


/**
 * @brief Crops an image
 *
 * @param[in] in_img Input image
 * @param[in] crop_size Number of pixels to be cropped from the border
 *
 * @return Pointer to the resulting image of size of the same width and height
 *
 * @author Damian Kusnik
 * @date 26.09.2020
 */
Image* crop_img(const Image* in_img, int crop_size)
{
	int num_rows, num_cols, crop_rows_cols;
	size_t num_bands;
	PixelType pix_type;
	Image* out_img;
	byte*** in_img_data, ***out_img_data;

	SET_FUNC_NAME("crop_img");
	if (!IS_VALID_OBJ(in_img))
	{
		ERROR_RET("Invalid image object !", NULL);
	}

	pix_type = get_pix_type(in_img);
	num_rows = get_num_rows(in_img) -2 * crop_size;
	num_cols = get_num_cols(in_img) -2 * crop_size;

	num_bands = get_num_bands(in_img); // ytes = get_num_bands(in_img) * num_rows * num_cols;
	// We need the shorter border
	if (num_rows > num_cols)
		crop_rows_cols = num_cols;
	else
		crop_rows_cols = num_rows;
	out_img = alloc_img(pix_type, crop_rows_cols, crop_rows_cols);
	if (IS_NULL(out_img))
	{
		ERROR_RET("Insufficient memory !", NULL);
	}
	switch (pix_type)
	{
	case PIX_BIN:		/*@fallthrough@ */

	case PIX_GRAY:		/*@fallthrough@ */

	case PIX_RGB:
		in_img_data = (byte***)get_img_data_nd(in_img);
		out_img_data = (byte***)get_img_data_nd(out_img);
		for(int i=0; i< crop_rows_cols; i++)
			for (int j = 0; j < crop_rows_cols; j++)
				for(int d=0; d<num_bands; d++)
				{
					out_img_data[i][j][d] = in_img_data[i + crop_size][j + crop_size][d];
				}
		break;

	case PIX_INT_1B:		/*@fallthrough@ */

	case PIX_INT_3B:

	case PIX_DBL_1B:		/*@fallthrough@ */

	case PIX_DBL_3B:

		ERROR("Not supported pixel type yet( %d ) !", pix_type);
		break;

	default:

		ERROR("Invalid pixel type ( %d ) !", pix_type);
		return NULL;
	}
	return out_img;
}

double* calculate_ssim(const Image* ref_img, const Image* test_img, FILE* fp)
{
	//first crop image 10 px each border (mostly a black window)
	Image* ref_img_crop = crop_img(ref_img, 10);
	Image* test_img_crop = crop_img(test_img, 10);
	Image * ref_red, * ref_blue, * ref_green;
	Image * test_red, * test_blue, * test_green;
	int num_bands = get_num_bands(ref_img); 
	double ms_ssim, ms_ssim_avg=0;
	double* result = (double*)malloc(3 * sizeof(double));


	int height = get_num_rows(ref_img_crop);
	int width = get_num_cols(test_img_crop);
	
	unsigned char* ref_data = (unsigned char*)get_img_data_1d(rgb_to_gray(ref_img_crop));
	unsigned char* test_data = (unsigned char*)get_img_data_1d(rgb_to_gray(test_img_crop));

	ms_ssim = iqa_ssim(ref_data, test_data, width, height, width, 0, NULL);
	result[0] = ms_ssim;

	ms_ssim = iqa_ms_ssim(ref_data, test_data, width, height, width, NULL);
	//printf("\nMSSIM %f\n", ms_ssim);
	result[1] = ms_ssim;

	//printf("SSIM %f\n", ms_ssim);
	//printf("\n\n");

	if (num_bands > 1)
	{

		get_rgb_bands(ref_img_crop, &ref_red, &ref_green, &ref_blue);
		get_rgb_bands(test_img_crop, &test_red, &test_green, &test_blue);
		//write_img(ref_blue, "red.pgm", FMT_PGM);
		
		ref_data = (unsigned char*) get_img_data_1d(ref_red);
		test_data = (unsigned char*) get_img_data_1d(test_red);
		ms_ssim = iqa_ms_ssim(ref_data, test_data, width, height, width, NULL);
		ms_ssim_avg += ms_ssim;

		ms_ssim = iqa_ms_ssim((unsigned char*)get_img_data_1d(ref_green),
			(unsigned char*)get_img_data_1d(test_green), width, height, width, NULL);
		ms_ssim_avg += ms_ssim;

		ms_ssim = iqa_ms_ssim((unsigned char*)get_img_data_1d(ref_blue),
			(unsigned char*)get_img_data_1d(test_blue), width, height, width, NULL);
		ms_ssim_avg += ms_ssim;
		
		free_img(ref_red);
		free_img(ref_green);
		free_img(ref_blue);

		free_img(test_red);
		free_img(test_green);
		free_img(test_blue);

		result[2] = ms_ssim_avg / 3.0;
	}
	else
	{
		result[2] = ms_ssim;
	}
	printf("SSIM: %f, MS_SSIM: %f, MS_SSIM_AVG: %f\n", result[0], result[1], result[2]);
	//printf("SSIM: %f, MS_SSIM: %f, MS_SSIM_AVG: %f\n", 10.0 * log(1-result[0]) / log(10.0), log(1-result[1]), result[2]);
	
	if (fp)
		fprintf(fp, "SSIM: % f, MS_SSIM : % f, MS_SSIM_AVG : % f\n", result[0], result[1], result[2]);

	free_img(ref_img_crop);
	free_img(test_img_crop);
	return result;
}

double*
calculate_snr(const Image* ref_img, const Image* test_img, FILE* fp)
{
	SET_FUNC_NAME("calculate_snr");
	byte*** ref_data;
	byte*** test_data;
	long height, width, data_len;
    double* result;

	if (!is_rgb_img(ref_img) || !is_rgb_img(test_img))
	{
		ERROR_RET("Not a color image !", NULL);
	}
	
	height = get_num_rows(ref_img);
	width = get_num_cols(ref_img);
	data_len = width * height;

	ref_data = (byte***)get_img_data_nd(ref_img);
	test_data = (byte***)get_img_data_nd(test_img);

	double mse = 0, mae = 0, es = 0, ms = 0, iri = 0;
	long s1, t1;
	double snr=0, psnr=0, rmse=0;
	double N = 0;
	//obcinamy krawedzie, zeby ich nei liczyl
	for (int y = 10; y < height-10; y++) {
		for (int x = 10; x < width-10; x++)
		{
			//s1 =  ipRef.getPixelValue(x, y);
			for (int dim = 0; dim <= 2; dim++)
			{
				s1 = (long) ref_data[y][x][dim];
				t1 = (long) test_data[y][x][dim];
				mse += (s1 - t1) * (s1 - t1);
				mae += abs((s1 - t1));
				es += s1 * s1;
				ms += s1;
				N+=1;
			}
		}
	}
	if (N > 0.0) {
		mse /= N;
		mae /= N;
		es /= N;
		ms /= N;
		iri= calculate_iri(ref_img, test_img, fp);

		if (mse != 0.0) {
			snr = 10.0 * log(es / mse) / log(10.0);
			psnr = 10.0 * log(255.0 * 255.0 / mse) / log(10.0);
			rmse = sqrt(mse);
			printf("SNR: %f, PSNR: %f, MSE: %f, MAE: %f, IRI: %f\n", snr, psnr, rmse, mae, iri);
			if(fp)
				fprintf(fp,"SNR: %f, PSNR: %f, MSE: %f, MAE: %f, IRI: %f\n", snr, psnr, rmse, mae, iri);
		}
		else {
			printf("SNR: Invalid\n");

		}
	}
    result = (double*) malloc(5*sizeof(double));
    result[0]=snr;
    result[1]=psnr;
    result[2]=rmse;
    result[3]=mae;
	result[4] = iri;
	return result;
}

double
calculate_iri(const Image* ref_img, const Image* test_img, FILE* fp)
{
	SET_FUNC_NAME("calculate_iri");
	byte*** ref_data;
	byte*** test_data;
	long height, width, data_len;

	if (!is_rgb_img(ref_img) || !is_rgb_img(test_img))
	{
		ERROR_RET("Not a color image !", NULL);
	}

	height = get_num_rows(ref_img);
	width = get_num_cols(ref_img);
	data_len = width * height;

	ref_data = (byte***)get_img_data_nd(ref_img);
	test_data = (byte***)get_img_data_nd(test_img);

	double iri = 0;
	long s1, t1, diff, min;
	double N = 0;
	//obcinamy krawedzie, zeby ich nei liczyl
	for (int y = 10; y < height-10; y++) {
		for (int x = 10; x < width-10; x++)
		{
			//s1 =  ipRef.getPixelValue(x, y);
			min = LONG_MAX;
			for (int i=-1; i<1; i++)
				for (int j =-1; j < 1; j++)
				{
					diff = 0;
					for (int dim = 0; dim < 3; dim++)
					{
						s1 = (long)ref_data[y+i][x+j][dim];
						t1 = (long)test_data[y][x][dim];
						diff+=(s1 - t1)* (s1 - t1);
						
					}
					if (diff < min)
					{
						min = diff;
					}
					
				}
			N += 1;
			iri += min;
		}
	}
	if (N > 0.0) {
		iri /= (double)N;
		iri = 10.0 * log(255.0 * 255.0 / iri) / log(10.0);
		printf("IRI: %f \n", iri);
		if (fp)
			fprintf(fp, "IRI: %f \n", iri);
	}
	else 
	{
			printf("IRI: Invalid \n");
	}
	
	return iri;
}


float square(float var)
{
	return var * var;
}

#define INF 1E20

/* dt of 1d function using squared distance */
static float* dt(float* f, int n) {
	float* d = (float*) malloc(sizeof(float)*n);
	int* v = (int*)malloc(sizeof(int) * n);
	float* z = (float*)malloc(sizeof(float) * (n+1));
	int k = 0;
	v[0] = 0;
	z[0] = -INF;
	z[1] = +INF;
	for (int q = 1; q <= n - 1; q++) {
		float s = ((f[q] + square(q)) - (f[v[k]] + square(v[k]))) / (2 * q - 2 * v[k]);
		while (s <= z[k]) {
			k--;
			s = ((f[q] + square(q)) - (f[v[k]] + square(v[k]))) / (2 * q - 2 * v[k]);
		}
		k++;
		v[k] = q;
		z[k] = s;
		z[k + 1] = +INF;
	}

	k = 0;
	for (int q = 0; q <= n - 1; q++) {
		while (z[k + 1] < q)
			k++;
		d[q] = square(q - v[k]) + f[v[k]];
	}

	free(v);
	free(z);
	return d;
}

/* dt of 2d function using squared distance */
Image* dt(byte** in_img_data, int width, int height, int on) {
	Image* out_img = alloc_img(PIX_DBL_1B, height, width);
	double** out_data = (double**)get_img_data_nd(out_img);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (in_img_data[y][x] == on)
				out_data[y][x] = 0;
			else
				out_data[y][x] = INF;
		}
	}
	float* f;
	if (width > height)
		f = (float*)malloc(sizeof(float) * width);
	else
		f = (float*)malloc(sizeof(float) * height);

	// transform along columns
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			f[y] = out_data[y][x];
		}
		float* d = dt(f, height);
		for (int y = 0; y < height; y++) {
			out_data[y][x] = d[y];
		}
		free(d);
	}
	
	// transform along rows
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			f[x] = out_data[y][x];
		}
		float* d = dt(f, width);
		for (int x = 0; x < width; x++) {
			out_data[y][x] = d[x];
		}
		free(d);
	}
	
	free(f);
	Image* out_img2 = alloc_img(PIX_GRAY, height, width);
	byte** out_data2 = (byte**)get_img_data_nd(out_img2);
	int max = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (out_data[y][x] > max)
				max = out_data[y][x];
		}
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			out_data2[y][x] = (byte)(sqrt(out_data[y][x]));
		}
	}
	return out_img2;
}

/** 
 * @brief Checks whether or not the object is a binary image
 *
 * @param[in] ref_img Reference Image pointer
 * @param[in] test_img Test Image pointer
 *
 * @return prat value
 *
 * @author D. Kusnik
 * @date 10.10.2020
 */


double
calculate_prat(const Image* ref_img, const Image* test_img)
{
	//first crop image 10 px each border (mostly a black window)
	Image* ref_img_crop = crop_img(ref_img, 10);
	Image* test_img_crop = crop_img(test_img, 10);
	double result=0;
	byte** ref_data;
	byte** test_data;

	int height = get_num_rows(ref_img_crop);
	int width = get_num_cols(test_img_crop);
	int count_test = 0, count_ref = 0;

	Image* edge_ref_img = detect_edge_VR(ref_img_crop, 100);
	Image* edge_test_img = detect_edge_VR(test_img_crop, 100);
	
	/*Image* edge_ref_img = detect_edge_canny(rgb_to_gray(ref_img_crop), 1, 0.89, 0.9);
	Image* edge_test_img = detect_edge_canny(rgb_to_gray(test_img_crop), 1, 0.89, 0.9);
	*/
	//write_img(rgb_to_gray(ref_img_crop), "gray_oryginal.pbm", FMT_PGM);


	ref_data = (byte**)get_img_data_nd(edge_ref_img);
	test_data = (byte**)get_img_data_nd(edge_test_img);

	/********************************/
	write_img(edge_ref_img, "gradient_oryginal.pbm", FMT_PGM);
	write_img(edge_test_img, "gradient_test.pbm", FMT_PGM);

	//THRESHOLD 
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			if (ref_data[y][x] < 20)
				ref_data[y][x] = 0;
			else
				ref_data[y][x] = 255;

			if (test_data[y][x] < 20)
				test_data[y][x] = 0;
			else
				test_data[y][x] = 255;
		}
	}

	write_img(edge_ref_img, "edge_oryginal.pbm", FMT_PGM);
	write_img(edge_test_img, "edge_test.pbm", FMT_PGM);


	Image* df_ref_img = dt(ref_data, width, height, 255);
	Image* df_test_img = dt(test_data, width, height, 255);

	write_img(df_ref_img, "edge_oryginal_dt.pgm", FMT_PGM);
	write_img(df_test_img, "edge_test_dt.pgm", FMT_PGM);
	/********************************/
	ref_data = (byte**)get_img_data_nd(df_ref_img);
	test_data = (byte**)get_img_data_nd(df_test_img);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			if (ref_data[y][x] == 0)
			{
				result += 1.0 / (1 + test_data[y][x] / 9);
				count_ref++;
			}
			if (test_data[y][x] == 0)
				count_test++;	
			//if (test_data[y][x] > 0)
		}
	}
	free(edge_ref_img);
	free(edge_test_img);
	if (count_ref > count_test)
		return result / count_ref;
	return result / count_test;
}
