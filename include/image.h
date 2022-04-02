#ifndef IMAGE_H
#define IMAGE_H
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* DEFINE CONSTANTS */

#define MAX_LINE_LEN 80	    /**< Max. line length */

#define NEW_LINE '\n'	    /**< New line character */

#define NUM_GRAY 256	    /**< Number of gray levels in an 8-bit gray-scale image */

#define MAX_GRAY 255	    /**< Max. gray level in an 8-bit gray-scale image */

#define OBJECT 1

#define BACKGROUND 0

#define PI ( 3.1415926535897932384626433832795 )

#define TWO_PI ( 6.283185307179586476925286766559 )

#define PI_OVER_3 ( 1.0471975511965977461542144610932 )

#define FIVE_PI_OVER_3 ( 5.2359877559829887307710723054658 )

#define SQRT3 ( 1.7320508075688772935274463415059 )

/* MACROS */

/**
 * @brief Sets the function name
 */

#define SET_FUNC_NAME( f_name ) static const char func_name[] = f_name

/**
 * @brief Determines whether or not a given pointer is NULL
 */

#define IS_NULL( x ) ( ( x ) == NULL )

/**
 * @brief Determines whether or not a given number is in the range of [0,255]
 * @warning This is not a safe macro
 */

#define IS_BYTE( x ) ( ( 0 <= ( x ) ) && ( ( x ) < 256 ) )

/**
 * @brief Determines whether or not a given floating-point number is close to 0.0
 */

#define IS_ZERO( x ) ( fabs ( ( x ) ) < DBL_EPSILON )

/**
 * @brief Determines whether or not a given floating-point number is positive
 */

#define IS_POS( x ) ( ( x ) > DBL_EPSILON )

/**
 * @brief Determines whether or not a given floating-point number is negative
 */

#define IS_NEG( x ) ( ( x ) < -DBL_EPSILON )

#define IS_ODD( x ) ( ( x ) % 2 )
#define IS_EVEN( x ) ( !IS_ODD ( ( x ) ) )
#define IS_POS_ODD( x ) ( ( ( x ) > 0 ) && IS_ODD ( x ) )

/**
 * @brief Determines whether or not a given number is in the range of [0,1]
 * @warning This is not a safe macro
 */
#define IS_IN_0_1( x ) ( ! ( IS_NEG ( ( x )  ) || IS_POS ( ( x ) - 1.0 ) ) )

#define SAFE_LOG( x ) ( ( ( x ) > 0.0 ) ? log ( ( x ) ) : ( 0.0 ) )

#define IS_VALID_OBJ( x ) ( !IS_NULL ( ( x ) ) && ( x )->type )

#define GET_NUM_ROWS( x ) ( ( x )->num_rows )

#define GET_NUM_COLS( x ) ( ( x )->num_cols )

#define L1_DIST_3D( x1, y1, z1, x2, y2, z2 )\
 ( abs ( ( x1 ) - ( x2 ) ) + abs ( ( y1 ) - ( y2 ) )  + abs ( ( z1 ) - ( z2 ) ) )

#define L2_NORM_3D_SQR( x, y, z )\
 ( ( x ) * ( x ) + ( y ) * ( y ) + ( z ) * ( z ) )

#define L2_NORM_3D( x, y, z )\
 sqrt ( L2_NORM_3D_SQR ( ( x ), ( y ), ( z ) ) )

#define L2_NORM_2D_SQR( x, y )\
 ( ( x ) * ( x ) + ( y ) * ( y ) )

#define L2_NORM_2D( x, y )\
 sqrt ( L2_NORM_2D_SQR ( ( x ), ( y ) ) )

#define L2_DIST_3D_SQR( x1, y1, z1, x2, y2, z2 )\
 ( ( ( x1 ) - ( x2 ) ) * ( ( x1 ) - ( x2 ) )\
 + ( ( y1 ) - ( y2 ) ) * ( ( y1 ) - ( y2 ) )\
 + ( ( z1 ) - ( z2 ) ) * ( ( z1 ) - ( z2 ) ) )

#define L2_DIST_3D( x1, y1, z1, x2, y2, z2 )\
 sqrt ( L2_DIST_3D_SQR ( ( x1 ), ( y1 ), ( z1 ), ( x2 ), ( y2 ), ( z2 ) ) )

#define L2_DIST_2D_SQR( x1, y1, x2, y2 )\
 ( ( ( x1 ) - ( x2 ) ) * ( ( x1 ) - ( x2 ) )\
 + ( ( y1 ) - ( y2 ) ) * ( ( y1 ) - ( y2 ) ) )\

#define L2_DIST_2D( x1, y1, x2, y2 )\
 sqrt ( L2_DIST_2D_SQR( ( x1 ), ( y1 ), ( x2 ), ( y2 ) ) )\

#define ROUND( X )      ( ( X ) < 0.0 ? ( X ) - 0.5 : ( X ) + 0.5 )

#define DEG_TO_RAD( D ) ( 0.017453292519943295769236907684886 * ( D ) )

#define CLAMP_BYTE( X ) ( ( X ) < 0 ? 0 : ( ( X ) > MAX_GRAY ) ? MAX_GRAY : ( X ) )

#define MIN_2( A, B ) ( ( A ) < ( B ) ? ( A ) : ( B ) )

#define MAX_2( A, B ) ( ( A ) < ( B ) ? ( B ) : ( A ) )

#define MIN_3( A, B, C ) MIN_2 ( ( A ), MIN_2 ( ( B ), ( C ) ) )

#define MAX_3( A, B, C ) MAX_2 ( ( A ), MAX_2 ( ( B ), ( C ) ) )

/** @cond INTERNAL_MACRO */
#define SWAP_INT( a, b ) { tmp_var = ( a ); ( a ) = ( b ); ( b ) = tmp_var; }

/** @endcond INTERNAL_MACRO */

/** @cond INTERNAL_MACRO */
#define SORT_INT( a, b ) { if ( ( a ) > ( b ) ) SWAP_INT ( a, b ); }

/** @endcond INTERNAL_MACRO */

#define DIST_FUNC L2_DIST_3D
#define DIST_FUNC_SQR L2_DIST_3D_SQR
#define ACOS acos

/**
 * @brief If DEBUG_ON is defined, prints file name and line number and then
 *        calls the variable-argument debug function; otherwise does nothing
 */

#ifdef DEBUG_ON
# define DEBUG fprintf ( stderr, "Debug in %s: ", func_name ), debug
#else
# define DEBUG do_nothing
#endif

/**
 * @brief Prints file name and line number and then 
 *        calls the variable-argument warning function
 */

#define WARNING fprintf ( stderr, "Warning in %s: ", func_name ), warning

/**
 * @brief Prints file name and line number and then 
 *        calls the variable-argument error function
 */

#define ERROR fprintf ( stderr, "Error in %s: ", func_name ), error

/**
 * @brief Prints file name and line number and then 
 *        calls the variable-argument fatal function
 */

#define FATAL fprintf ( stderr, "Fatal error in %s: ", func_name ), fatal

/**
 * @brief Prints file name, line number, and the given error message; 
 *        conditionally aborts the program and then returns the value of 
 *        the given error number
 */

#define ERROR_RET( err_msg, err_no ) \
         do \
          { \
           fprintf ( stderr, "Error in %s: %s\n", func_name, ( err_msg ) ); \
           fflush ( stderr ); \
           if ( get_err_mode ( ) ) abort ( ); \
           return ( err_no ) ; \
          } while ( 0 )

#define MALLOC_STRUCT( x ) ( ( x * ) malloc ( sizeof ( x ) ) )

#define CALLOC_STRUCT( x ) ( ( x * ) calloc ( 1, sizeof ( x ) ) )

#define NEG_ONE_POW( x ) ( IS_EVEN ( ( x ) ) ? ( 1 ) : ( -1 ) )

/* ENUMERATIONS/TYPEDEFS */

enum
{

 E_SUCCESS = 0,	 /**< success */

 E_FAILURE,	 /**< failure */

 E_INVARG,	 /**< invalid argument */

 E_INVOBJ,	 /**< invalid object */

 E_INVBPP,	 /**< invalid pixel depth */

 E_NULL,	 /**< null pointer */

 E_NOMEM,	 /**< insufficient memory */

 E_DIVZERO,	 /**< divide by zero */

 E_UNIMPL,	 /**< feature not implemented */

 E_UNFMT,	 /**< unknown file format */

 E_FOPEN,	 /**< file open error */

 E_FREAD,	 /**< file read error */

 E_FEOF		 /**< end of file reached */

}; /**< Error Codes */

typedef unsigned char byte; /**< Byte Type */

#define IS_BOOL( x ) ( ( ( x ) == 0 ) || ( ( x ) == 1 ) )

typedef enum
{

 PIX_INVALID = 0,  /**< invalid */

 PIX_BIN,	   /**< binary */

 PIX_GRAY,	   /**< gray-scale */

 PIX_RGB,	   /**< RGB color */

 PIX_INT_1B,	   /**< single-band integer */

 PIX_INT_3B,	   /**< 3-band integer */

 PIX_DBL_1B,	   /**< single-band double */

 PIX_DBL_3B	   /**< 3-band double */

} PixelType; /**< Pixel Type Enumeration */

typedef enum
{

 FMT_UNKNOWN = 0, /**< Unknown */

 FMT_BMP,	     /**< BMP */

 FMT_GIF,	     /**< GIF */

 FMT_JPG,	     /**< JPG */

 FMT_PBMA,	     /**< Plain/ASCII PBM */

 FMT_PBM,	     /**< Raw/Binary PBM */

 FMT_PCX,	     /**< PCX */

 FMT_PGMA,	     /**< Plain/ASCII PGM */

 FMT_PGM,	     /**< Raw/Binary PGM */

 FMT_PNG,	     /**< PNG */

 FMT_PPMA,	     /**< Plain/ASCII PPM */

 FMT_PPM,	     /**< Raw/Binary PPM */

 FMT_PSD,	     /**< PSD */

 FMT_RAS,	     /**< RAS */

 FMT_TGA,	     /**< TGA */

 FMT_TIFF	     /**< TIFF */

} ImageFormat; /**< Image Format Enumeration */

typedef struct
{

 PixelType type;     /**< Pixel Type */

 int num_bands;	     /**< Number of Bands */

 int num_rows;	     /**< Number of Rows */

 int num_cols;	     /**< Number of Columns */

 int max_pix_val;    /**< Max. Pixel Value (Defined only for PIX_BIN, PIX_GRAY, and PIX_RGB) */

 int num_cc;	     /**< Number of Connected Components (Defined only for PIX_INT_1B) */

 union
 {

  byte *byte_data;	 /**< For PIX_BIN, PIX_GRAY, PIX_RGB */

  int *int_data;	 /**< For PIX_INT_1B, PIX_INT_3B */

  double *double_data;	 /**< For PIX_DBL_1B, PIX_DBL_3B */

 } data_1d;  /**< 1-dimensional contiguous pixel array */

 union
 {

  byte **byte_data_1b;	      /**< For PIX_BIN, PIX_GRAY */

  byte ***byte_data_3b;	      /**< For PIX_RGB */

  int **int_data_1b;	      /**< For PIX_INT_1B */

  int ***int_data_3b;	      /**< For PIX_INT_3B */

  double **double_data_1b;    /**< For PIX_DBL_1B */

  double ***double_data_3b;   /**< For PIX_DBL_3B */

 } data_nd;  /**< 2 or 3-dimensional pixel array */

} Image; /**< Image Structure */

typedef enum
{

 HISTO_INVALID = 0,	/**< invalid */

 HISTO_INT,	/**< unnormalized */

 HISTO_DBL	/**< normalized */

} HistoType; /**< Histogram Type Enumeration */

typedef struct
{

 HistoType type;

 int num_bins;	      /**< Number of Bins */

 int num_pixels;      /**< Number of Pixels in the Source Image */

 union
 {

  int *int_data;	 /**< For HISTO_INT */

  double *double_data;	 /**< For HISTO_DBL */

 } data;   /**< 1-dimensional contiguous histogram data array */

} Histo; /**< Histogram Structure */

typedef enum
{

 EEM_INVALID = 0, /**< invalid */

 EM_MAE,     /**< Mean Absolute Error */

 EM_MSE,     /**< Mean Squared Error */

 EM_RMSE,    /**< Root Mean Squared Error */

 EM_PSNR,    /**< Peak Signal-to-Noise Ratio */

 EM_NMSE,    /**< Normalized Mean Squared Error */

 EM_NCD	     /**< Normalized Color Distance */

} ErrorMeasure;	/**< Error Measure Enumeration */

typedef enum
{

 GEN_INVALID = 0,    /**< invalid */

 GEN_VALID	     /**< valid */

} GenericType; /**< Generic Type Enumeration */

typedef struct
{

 GenericType type;

 int length;			/* length (# points) of the chain */
 int *row;			/* row coordinates */
 int *col;			/* column coordinates */
 int *dir;			/* Freeman chaincode directions */

} Chain; /**< Chain Code Structure */

typedef struct
{

 GenericType type;

 int num_chains;		/* number of chains (objects) in the list */
 int max_length;		/* max. length of an object chain */
 int num_rows;			/* # rows in the parent image */
 int num_cols;			/* # columns in the parent image */
 Chain **chain;			/* chain code pointers */

} ChainList; /**< Chain Code List Structure */

typedef struct
{

 int min_row;			/* minimum row coordinate */
 int max_row;			/* maximum row coordinate */
 int min_col;			/* minimum column coordinate */
 int max_col;			/* maximum column coordinate */

} Box; /**< Bounding-Box Structure */

typedef enum
{

 CMAP_INVALID = 0,

 CMAP_HSV,

 CMAP_INFRARED,

 CMAP_JET

} ColorMap; /**< Color Map Enumeration */

typedef struct
{
 double row;
 double col;
} Point;

typedef struct
{

 GenericType type;

 int num_pts;			/* number of points in the list */
 Point **point;			/* Point pointers */

} PointList; /**< 2-Dimensional Point List Structure */

typedef struct
{
 double maj_axis_len;
 double min_axis_len;
 double aspect_ratio;
 double eccentricity;
 double orientation;
} EllipseFeatures;

typedef struct
{
 double mean;
 double stdev;
 double entropy;
 double roughness;
} RadialDistFeatures;

typedef struct
{
 double F1;
 double F2;
 double F3;
 double F4;
} CSMoments;

typedef struct
{
 double mean;
 double variance;
 double skewness;
 double kurtosis;
} ChordLenStats;

typedef struct
{
 double m00;
 double m01;
 double m10;
 double m11;
 double m02;
 double m20;
 double m12;
 double m21;
 double m03;
 double m30;
} GeoMoments;

typedef struct
{
 double phi1;
 double phi2;
 double phi3;
 double phi4;
 double phi5;
 double phi6;
 double phi7;
} HuMoments;

typedef struct
{
 double I1;
 double I2;
 double I3;
 double I4;
} AffineMoments;

typedef struct
{
 double real;
 double imag;
} Complex;

typedef struct
{
 GenericType type;

 int max_order;
 int radius;
 int num_terms;
 Complex ***data;

} ZernikeBasis;

typedef enum
{

 STRL_INVALID = 0,  /**< invalid */

 STRL_LINE,	   /**< flat line */

 STRL_RECT,	   /**< flat rectangle */

 STRL_DISK,	   /**< flat disk */

 STRL_FLAT,	   /**< arbitrary flat */

 STRL_NONFLAT	   /**< arbitrary non-flat */

} StrelType; /**< Structuring Element Type Enumeration */

typedef struct
{

 StrelType type;

 int num_rows;

 int num_cols;

 int origin_row;

 int origin_col;

 byte *data_1d;

 byte **data_2d;

} Strel;

typedef enum
{

 SEQ_INVALID = 0,  /**< invalid */

 SEQ_OC,	   /**< */

 SEQ_CO,	   /**< */

 SEQ_OCO,	   /**< */

 SEQ_COC	   /**< */

} ASFSeq; /**< Alternating Sequential Filter Sequence */

typedef enum
{
 TYPE_UNKNOWN /* unknown type */ ,
 UCHAR /* unsigned char */ ,
 SCHAR /* signed char */ ,
 USHORT /* unsigned short int */ ,
 SSHORT /* signed short int */ ,
 INT /* signed int */ ,
 ULINT /* unsigned long int */ ,
 FLOAT /* float */ ,
 DOUBLE				/* double */
} bufferType;

typedef enum
{
 UNKNOWN_FILTER = 0 /* unknown filter type */ ,
 ALPHA_DERICHE = 1 /* Deriche's filter (exponential (- alpha |X|)) */ ,
 GAUSSIAN_DERICHE = 2		/* gaussian approximation (Deriche's coefficients) */
} recursiveFilterType;

/* FUNCTION PROTOTYPES */

/* add_noise.c */
Image *add_gaussian_noise ( const Image * in_img, const double mean,
			    const double stdev );
Image *add_saltpepper_noise ( const Image * in_img, const double prob );
Image *add_speckle_noise ( const Image * in_img, const double stdev );
double calc_error ( const ErrorMeasure err_meas, const Image * orig_img,
		    const Image * est_img );

double calc_error_win ( const ErrorMeasure err_meas, const Image * orig_img,
		    const Image * est_img, int win_size );

/* alloc_nd.c */
void *alloc_nd ( const size_t elem_size, const int dim_count, ... );
void free_nd ( void *base_ptr, const int dim_count );

/* arith.c */
Image *add_img ( const Image * in_img_a, const Image * in_img_b );
Image *sub_img ( const Image * in_img_a, const Image * in_img_b );
Image *mult_img ( const Image * in_img_a, const Image * in_img_b );
Image *abs_diff_img ( const Image * in_img_a, const Image * in_img_b );
Image *union_img ( const Image * in_img_a, const Image * in_img_b );
Image *intersect_img ( const Image * in_img_a, const Image * in_img_b );
Image *add_img_scalar ( const Image * in_img, const double scalar );
Image *sub_img_scalar ( const Image * in_img, const double scalar );
Image *mult_img_scalar ( const Image * in_img, const double scalar );
Image *div_img_scalar ( const Image * in_img, const double scalar );

/* basic_shape_desc.c */
int count_obj_pixels ( const Image * img, const int label );
double calc_obj_area ( const Image * img, const int label );
double calc_aspect_ratio ( const Image * img, const int label );
Point *calc_obj_centroid ( const Image * img, const int label );
Image *crop_object ( const Image * in_img, const int label );
Image *circular_scale ( const Image * in_img, const int label,
			const int radius );
double calc_triangularity ( const Image * img, const int label );
double calc_ellipticity ( const Image * img, const int label );
EllipseFeatures *calc_ellipse_feats ( const Image * img, const int label );
double calc_equi_diameter ( const Image * img, const int label );
double calc_elliptic_var ( const Image * img, const int label,
			   const PointList * cont );
double calc_circularity ( const Image * img, const int label,
			  const PointList * cont );
RadialDistFeatures *calc_radial_dist_feats ( const Image * img, const int label,
					     const PointList * cont );
ChordLenStats *calc_chord_len_stats ( const PointList * cont );


/* ccv_enhance.c */
Image *ccv_enhance ( const Image * in_img, const double sharp_factor );

/* colorspaces.c */
Image *rgb_to_nrgb ( const Image * in_img, double undefined );
Image *rgb_to_ohta ( const Image * in_img );
Image *ohta_to_rgb ( const Image * in_img );
Image *rgb_to_hsi ( const Image * in_img, double undefined );
Image *rgb_to_hsi_kender ( const Image * in_img, double undefined );
Image *hsi_to_rgb ( const Image * in_img );
Image *rgb_to_hsv ( const Image * in_img, double undefined );
Image *hsv_to_rgb ( const Image * in_img );
Image *rgb_to_hsl ( const Image * in_img, double undefined );
Image *hsl_to_rgb ( const Image * in_img );
Image *rgb_to_lab ( const Image * in_img );
Image *lab_to_rgb ( const Image * in_img );
Image *rgb_to_luv ( const Image * in_img );
Image *luv_to_rgb ( const Image * in_img );
Image *rgb_to_sct ( const Image * in_img, double undefined );
Image *sct_to_rgb ( const Image * in_img );
Image *lab_to_lch ( const Image * in_img, double undefined );
Image *lch_to_lab ( const Image * in_img );
Image *rgb_to_l1l2l3 ( const Image * in_img, double undefined );

/* convex_hull.c */
PointList *calc_2d_convex_hull ( const PointList * inp_point_list );
double calc_max_diameter ( const PointList * hull );
double calc_compactness ( const Image * img, const int label,
			  const PointList * hull );
double calc_solidity ( const Image * in_img, const int label,
		       const PointList * hull );

/* cs_moments.c */
CSMoments *calc_cs_moments ( const Image * img, const int label,
			     const PointList * cont );

/* draw_line.c */
void draw_line ( const Point * point1, const Point * point2,
		 const int pix_value, Image * img );

/* edge_advanced.c */
Image *detect_edge_canny ( const Image * in_img, const double sigma,
			   const double low, const double high );
Image *detect_edge_deriche ( const Image * in_img, const double alpha,
			     const double low, const double high );

/* edge_basic.c */
Image *detect_edge_roberts ( const Image * in_img );
Image *detect_edge_prewitt ( const Image * in_img );
Image *detect_edge_sobel ( const Image * in_img );
Image *detect_edge_log ( const Image * in_img, const double sigma );

/* ellipticity.c */
double calc_elliptic_var ( const Image * img, const int label,
			   const PointList * cont );

/* error.c */
int get_err_mode ( void );
void set_err_mode ( const int err_mode );
int get_warn_mode ( void );
void set_warn_mode ( const int warn_mode );
void do_nothing ( const char *format, ... );
void debug ( const char *format, ... );
void warning ( const char *format, ... );
void error ( const char *format, ... );
void fatal ( const char *format, ... );
const char *error_str ( const int err_no );

/* expand_histo.c */
Image *expand_histo ( const Image * in_img, const double percent_clip );

/* fill_region.c */
int calc_box_area ( const Box * box );
int get_box_height ( const Box * box );
int get_box_width ( const Box * box );
Box *alloc_box ( void );
void free_box ( Box * box );
Box *calc_box ( const Image * img, const int label );
void fill_region ( const int seed_row, const int seed_col, const int label,
		   Image * img );

/* filter_abvdf.c */
Image *filter_abvdf ( const Image * in_img, const int win_size,
		      const int k_value, const double tolerance );

/* filter_acwddf.c */
Image *filter_acwddf ( const Image * in_img, const int win_size,
		       const int lambda, const double tolerance );

/* filter_acwvdf.c */
Image *filter_acwvdf ( const Image * in_img, const int win_size,
		       const int lambda, const double tolerance );

/* filter_acwvmf.c */
Image *filter_acwvmf ( const Image * in_img, const int win_size,
		       const int lambda, const double tolerance );

/* filter_adap_smooth.c */
Image *filter_adap_smooth ( const Image * in_img, const double k_value,
			    const int num_iters );

/* filter_ahdf.c */
Image *filter_ahdf ( const Image * in_img, const int win_size );

/* filter_amnfe.c */
Image *filter_amnfe ( const Image * in_img, const int win_size,
		      const double k_value );

/* filter_amnfg.c */
Image *filter_amnfg ( const Image * in_img, const int win_size,
		      const double k_value );

/* filter_annf.c */
Image *filter_annf ( const Image * in_img, const int win_size );

/* filter_annmf.c */
Image *filter_annmf ( const Image * in_img, const int win_size );

/* filter_asbvdf_mean.c */
Image *filter_asbvdf_mean ( const Image * in_img, const int win_size );

/* filter_asbvdf_rank.c */
Image *filter_asbvdf_rank ( const Image * in_img, const int win_size );

/* filter_asddf_mean.c */
Image *filter_asddf_mean ( const Image * in_img, const int win_size );

/* filter_asddf_rank.c */
Image *filter_asddf_rank ( const Image * in_img, const int win_size );

/* filter_asvmf_mean.c */
Image *filter_asvmf_mean ( const Image * in_img, const int win_size );

/* filter_asvmf_rank.c */
Image *filter_asvmf_rank ( const Image * in_img, const int win_size );

/* filter_atvmf.c */
Image *filter_atvmf ( const Image * in_img, const int win_size,
		      const int k_value );

/* filter_avmf.c */
Image *filter_avmf ( const Image * in_img, const int win_size,
		     const int k_value, const double tolerance );

/* filter_bilateral.c */
Image *filter_bilateral ( const Image * in_img, const double range_sigma,
			  const double spatial_sigma );

/* filter_bvdf.c */
Image *filter_bvdf ( const Image * in_img, const int win_size );

/* filter_cbrf.c */
Image *filter_cbrf ( const Image * in_img, const int win_size );

/* filter_cfmf.c */
Image *filter_cfmf ( const Image * in_img, const int win_size,
		    const double C_value, const double t_value );

/* filter_ddf.c */
Image *filter_ddf ( const Image * in_img, const int win_size );

/* filter_ebvdf.c */
Image *filter_ebvdf ( const Image * in_img, const int win_size );

/* filter_eddf.c */
Image *filter_eddf ( const Image * in_img, const int win_size );

/* filter_evmf.c */
Image *filter_evmf ( const Image * in_img, const int win_size );

/* filter_exvmf.c */
Image *filter_exvmf ( const Image * in_img, const int win_size );

/* filter_fddrhf.c */
Image *filter_fddrhf ( const Image * in_img, const double weight,
		       const double h_value, const double k_value,
		       const double beta, const double gamma );

/* filter_ffnrf.c */
Image *filter_ffnrf ( const Image * in_img, const int win_size,
		      const int K_value, const double alpha );

/* filter_fmvmf.c */
Image *filter_fmvmf ( const Image * in_img, const int win_size,
		      const double beta );

/* filter_fmvdf.c */
Image *filter_fmvdf ( const Image * in_img, const int win_size,
		      const double K_value, const double alpha );

/* filter_fmddf.c */
Image *filter_fmddf ( const Image * in_img, const int win_size,
		      const double K1_value, const double K2_value,
		      const double alpha );

/* filter_fovdf.c */
Image *filter_fovdf ( const Image * in_img, const int win_size,
		      const double beta, const double gamma );

/* filter_fovmf.c */
Image *filter_fovmf ( const Image * in_img, const int win_size,
		      const double beta, const double gamma );

/* filter_fpgf.c */
Image *filter_fpgf ( const Image * in_img, const int win_size,
		     const int pg_size, const double dist_thresh );

/* filter_fvdf.c */
Image *filter_fvdf ( const Image * in_img, const int win_size,
		     const double beta, const double gamma );

/* filter_fvdrhf.c */
Image *filter_fvdrhf ( const Image * in_img, const double weight,
		       const double h_value, const double k_value,
		       const double beta, const double gamma );

/* filter_fvmf.c */
Image *filter_fvmf ( const Image * in_img, const int win_size,
		     const double beta, const double gamma );

/* filter_fvmrhf.c */
Image *filter_fvmrhf ( const Image * in_img, const double weight,
		       const double h_value, const double k_value,
		       const double beta, const double gamma );

/* filter_gaussian.c */
Image *filter_gaussian ( const Image * in_img, const double stdev );
Image *filter_ani_gaussian ( const Image * in_img, const double sigma_v,
			     const double sigma_u, const double phi,
			     const int order_v, const int order_u );

/* filter_giws.c */
Image *filter_giws ( const Image * in_img, const int win_size );

/* filter_gvdf.c */
Image *filter_gvdf ( const Image * in_img, const int win_size );

/* filter_hdf.c */
Image *filter_hdf ( const Image * in_img, const int win_size );

/* filter_kuwahara.c */
Image *filter_kuwahara ( const Image * in_img, const int k_value );

/* filter_kvmf.c */
Image *filter_kvmf ( const Image * in_img, const int win_size );

/* filter_lce.c */
Image *filter_lce ( const Image * in_img, const int win_size );

/* filter_lee.c */
Image *filter_lee ( const Image * in_img, const int win_size,
		    const double stdev );

/* filter_lum.c */
Image *filter_lum ( const Image * in_img, const int win_size,
		    const int k_value );

/* filter_mad.c */
Image *filter_mad ( const Image * in_img, const int win_size, const int a_value,
		    const int b_value );

/* filter_majority.c */
Image *filter_majority ( const Image * in_img, const int win_size );

/* filter_mcv.c */
Image *filter_mcv ( const Image * in_img, const int win_size );

/* filter_mcwvmf.c */
Image *filter_mcwvmf ( const Image * in_img, const int win_size,
		       const double weight );

/* filter_mean.c */
Image *filter_mean ( const Image * in_img, const int win_size );

/* filter_median.c */
Image *filter_classical_median ( const Image * in_img, const int win_size );
Image *filter_running_median ( const Image * in_img, const int win_size );
Image *filter_3x3_median ( const Image * in_img );
Image *filter_const_time_median ( const Image * in_img, const int win_size );

/* filter_mlv.c */
Image *filter_mlv ( const Image * in_img, const int win_size );

/* filter_mmf.c */
Image *filter_mmf ( const Image * in_img, const int win_size );

/* filter_mpv.c */
Image *filter_mpv ( const Image * in_img, const int win_size );

/* filter_perona_malik.c */
Image *filter_perona_malik ( const Image * in_img, const double k_value,
			     const double lambda, const int num_iters );

/* filter_pgf.c */
Image *filter_pgf ( const Image * in_img, const int win_size,
		    const double alpha );

/* filter_pvof.c */
Image *filter_pvof ( const Image * in_img, const int win_size );

/* filter_robust_aniso.c */
Image *filter_robust_aniso ( const Image * in_img, const int num_iters );

/* filter_rodsvmf.c */
Image *filter_rodsvmf ( const Image * in_img, const int win_size,
			const double dist_thresh );

/* filter_sbvdf_mean.c */
Image *filter_sbvdf_mean ( const Image * in_img, const int win_size,
			   const double lambda );

/* filter_sbvdf_rank.c */
Image *filter_sbvdf_rank ( const Image * in_img, const int win_size,
			   const double lambda );

/* filter_sddf_mean.c */
Image *filter_sddf_mean ( const Image * in_img, const int win_size,
			  const double lambda );

/* filter_sddf_rank.c */
Image *filter_sddf_rank ( const Image * in_img, const int win_size,
			  const double lambda );

/* filter_sdrom.c */
Image *filter_sdrom ( const Image * in_img, const int t0, const int t1,
		      const int t2, const int t3 );

/* filter_sep_median */
Image *filter_sep_median ( const Image * in_img, const int win_size );

/* filter_sigma.c */
Image *filter_sigma ( const Image * in_img, const int win_size,
		      const int k_value );

/* filter_snn.c */
Image *filter_snn ( const Image * in_img );

/* filter_susan.c */
Image *filter_susan ( const Image * in_img, const int bright_thresh,
		      const double dist_thresh );

/* filter_svmf_mean.c */
Image *filter_svmf_mean ( const Image * in_img, const int win_size,
			  const double lambda );

/* filter_svmf_rank.c */
Image *filter_svmf_rank ( const Image * in_img, const int win_size,
			  const double lambda );

/* filter_vmf.c */
Image *filter_vmf ( const Image * in_img, const int win_size );

/* filter_vmrhf.c */
Image *filter_vmrhf ( const Image * in_img, const double weight,
		      const double h_value, const double k_value );

/* filter_vsdromf.c */
Image *filter_vsdromf ( const Image * in_img, const double t0, const double t1,
			const double t2, const double t3 );

/* gen_fourier_desc.c */
double *calc_gen_fourier_desc ( const Image * in_img, const int label,
				const int rad_res, const int ang_res,
				const int radius );


/* gray_morpho.c */
Image *morpho_gradient ( const Image * in_img, const Strel * se );
Image *h_minima_transform ( const Image * in_img, const Strel * se,
			    const int contrast );
Image *h_maxima_transform ( const Image * in_img, const Strel * se,
			    const int contrast );
Image *regional_minima ( const Image * in_img, const Strel * se );
Image *regional_maxima ( const Image * in_img, const Strel * se );
Image *extended_minima_transform ( const Image * in_img, const Strel * se,
				   const int contrast );
Image *extended_maxima_transform ( const Image * in_img, const Strel * se,
				   const int contrast );

/* image.c */
int is_bin_img ( const Image * img );
int is_gray_img ( const Image * img );
int is_rgb_img ( const Image * img );
int is_byte_img ( const Image * img );
int is_label_img ( const Image * img );
int is_bin_or_label_img ( const Image * img );
int is_bin_or_gray_img ( const Image * img );
int is_dbl_3b_img ( const Image * img );
int is_dbl_img ( const Image * img );
int is_int_img ( const Image * img );
PixelType get_pix_type ( const Image * img );
int get_num_bands ( const Image * img );
int get_num_rows ( const Image * img );
int get_num_cols ( const Image * img );
int get_num_cc ( const Image * img );
void *get_img_data_1d ( const Image * img );
void *get_img_data_nd ( const Image * img );
Image *alloc_img ( const PixelType pix_type, const int num_rows,
		   const int num_cols );
void free_img ( Image * img );
int count_colors ( const Image * img );
Image *rgb_to_gray ( const Image * rgb_img );
Image *negate_img ( const Image * in_img );
int img_dims_agree ( const Image * img_a, const Image * img_b );
int img_types_agree ( const Image * img_a, const Image * img_b );
int is_equal_img ( const Image * in_img_a, const Image * in_img_b );
Image *dbl_to_byte_img ( const Image * in_img );
Image *byte_to_dbl_img ( const Image * in_img );
void get_rgb_bands ( const Image * rgb_img, Image ** red_img,
		     Image ** green_img, Image ** blue_img );
Image *combine_rgb_bands ( const Image * red_img, const Image * green_img,
			   const Image * blue_img );
Image *clone_img ( const Image * in_img );
Image *alloc_const_img ( const PixelType pix_type, const int num_rows,
			 const int num_cols, const double value );

/* image_format.c */
int get_img_format ( FILE * file_ptr, ImageFormat * img_format );
const char *img_format_str ( const ImageFormat img_format );

/* image_io.c */
Image *read_img ( const char *file_name );
int write_img ( const Image * img, const char *file_name,
		const ImageFormat img_format );

/* invariant_moments.c */
GeoMoments *calc_geo_moments ( const Image * img, const int label );
GeoMoments *calc_central_moments ( const Image * img, const int label );
GeoMoments *calc_norm_central_moments ( const Image * img, const int label );
HuMoments *calc_hu_moments ( const Image * img, const int label );
AffineMoments *calc_affine_moments ( const Image * img, const int label );


/* label_cc_hybrid.c */
Image *label_cc_hybrid ( const Image * in_img, const int connectivity );

/* label_cc_trace.c */
Image *label_cc_trace ( const Image * in_img, const int connectivity );

/* match_histo.c */
Image *match_histo ( const Image * in_img, const Histo * target_histo );

/* morpho.c */
Image *close_img ( const Image * in_img, const Strel * se );
Image *dilate_img ( const Image * in_img, const Strel * se );
Image *erode_img ( const Image * in_img, const Strel * se );
Image *open_img ( const Image * in_img, const Strel * se );
Image *tophat_img ( const Image * in_img, const Strel * se );
Image *bothat_img ( const Image * in_img, const Strel * se );
Image *cond_dilate_img ( const Image * in_img, const Image * cond_img,
			 const Strel * se, const int num_iters );
Image *cond_erode_img ( const Image * in_img, const Image * cond_img,
			const Strel * se, const int num_iters );
Image *inf_rec_img ( const Image * marker_img, const Image * cond_img,
		     const Strel * se );
Image *sup_rec_img ( const Image * marker_img, const Image * cond_img,
		     const Strel * se );
Image *open_rec_img ( const Image * in_img, const Strel * se_ero,
		      const Strel * se_conn );
Image *close_rec_img ( const Image * in_img, const Strel * se_dil,
		       const Strel * se_conn );
Image *open_rec_tophat_img ( const Image * in_img, const Strel * se_ero,
			     const Strel * se_conn );
Image *close_rec_tophat_img ( const Image * in_img, const Strel * se_dil,
			      const Strel * se_conn );
Image *filter_asf ( const Image * in_img, const Strel * se, const ASFSeq seq,
		    const int num_iters );
Image *fill_holes ( const Image * in_img, const Strel * se );
Image *clear_border ( const Image * in_img, const Strel * se );

/* pnm_header.c */
int read_pbmb_header ( FILE * file_ptr, int *num_rows, int *num_cols );
void write_pbmb_header ( const int num_rows, const int num_cols,
			 FILE * file_ptr );
int read_pgmb_header ( FILE * file_ptr, int *num_rows, int *num_cols,
		       int *max_gray );
void write_pgmb_header ( const int num_rows, const int num_cols,
			 const int max_gray, FILE * file_ptr );
int read_ppmb_header ( FILE * file_ptr, int *num_rows, int *num_cols,
		       int *max_rgb );
void write_ppmb_header ( const int num_rows, const int num_cols,
			 const int max_rgb, FILE * file_ptr );

/* point.c */
Point *alloc_point ( const double row_val, const double col_val );
void free_point ( Point * point );
PointList *alloc_point_list ( const int num_pts );
void free_point_list ( PointList * point_list );
int get_num_pts ( const PointList * point_list );
/*cant find definition of this bellow. The library when importing, makes errors */
/*Point *calc_cont_centroid ( const PointList * cont ); */
Image *point_list_to_img ( const PointList * point_list, const int num_rows,
			   const int num_cols );
PointList *chain_to_point_list ( const Chain * chain );

/* pnm_io.c */
Image *read_pbmb_data ( const int num_rows, const int num_cols,
			FILE * file_ptr );
int write_pbmb ( const Image * img, FILE * file_ptr );
Image *read_pgmb_data ( const int num_rows, const int num_cols,
			FILE * file_ptr );
int write_pgmb ( const Image * img, FILE * file_ptr );
Image *read_ppmb_data ( const int num_rows, const int num_cols,
			FILE * file_ptr );
int write_ppmb ( const Image * img, FILE * file_ptr );

/* pseudo_color.c */
Image *pseudo_color ( const Image * in_img, const ColorMap color_map );

/* quant_neural.c */
Image *quant_neural ( const Image * in_img, const int num_colors,
		      const int sampling_factor );

/* quant_wan.c */
Image *quant_wan ( const Image * in_img, const int num_colors,
		   const int num_bits );

/* quant_wu.c */
Image *quant_wu ( const Image * in_img, const int num_colors );

/* rectangularity.c */
PointList *calc_min_area_rect ( const Image * img, const int label,
				const PointList * cont, double *area );
double calc_rectangularity ( const Image * img, const int label,
			     const PointList * cont );

/* rotate.c */
Image *rotate_img ( const Image * in_img, const double angle );

/* scale.c */
Image *scale_img ( const Image * in_img, const int num_rows,
		   const int num_cols );

/* srm.c */
Image *srm ( const Image * in_img, const double Q_value,
	     const double size_factor );

/* threshold.c */
Image *threshold_img ( const Image * gray_img, const int threshold );

/* threshold_bernsen.c */
Image *threshold_bernsen ( const Image * in_img, const int win_size,
			   const int contrast_threshold );

/* threshold_huang.c */
int threshold_huang ( const Image * img );

/* threshold_iter.c */
int threshold_iter ( const Image * img );

/* threshold_kapur.c */
int threshold_kapur ( const Image * img );

/* threshold_li.c */
int threshold_li ( const Image * img );

/* threshold_moment.c */
int threshold_moment ( const Image * img );

/* threshold_niblack.c */
Image *threshold_niblack ( const Image * in_img, const int win_size,
			   const double k_value );

/* threshold_otsu.c */
int threshold_otsu ( const Image * img );

/* threshold_renyi.c */
int threshold_renyi ( const Image * img );

/* threshold_sauvola.c */
Image *threshold_sauvola ( const Image * in_img, const int win_size,
			   const double k_value, const double R_value );

/* threshold_savakis.c */
Image *threshold_savakis ( const Image * in_img, const int win_size,
			   int ( *global_thresholding_method ) ( const Image
								 * ) );

/* threshold_savakis_opt.c */
Image *threshold_savakis_opt ( const Image * in_img, const int win_size,
			       int ( *global_thresholding_method ) ( const Image
								     * ) );

/* threshold_shanbhag.c */
int threshold_shanbhag ( const Image * img );

/* threshold_yen.c */
int threshold_yen ( const Image * img );

/* trace_contour.c */
Chain *alloc_chain ( const int max_length );
void free_chain ( Chain * chain );
int get_chain_len ( const Chain * chain );
int *get_chain_rows ( const Chain * chain );
int *get_chain_cols ( const Chain * chain );
int *get_chain_dir ( const Chain * chain );
ChainList *alloc_chain_list ( const int num_chains, const int max_length,
			      const int num_rows, const int num_cols );
void free_chain_list ( ChainList * chain_list );
Chain *get_chain ( const ChainList * chain_list, const int chain_index );
int get_num_chains ( const ChainList * chain_list );
Image *chain_list_to_img ( const ChainList * chain_list );
Chain *trace_contour ( const Image * lab_img, const int label,
		       const int max_length );
ChainList *trace_contours ( const Image * lab_img, const int max_length );

/* util.c */
clock_t start_timer ( void );
double stop_timer ( const clock_t start_time );
double *gauss_1d ( const double sigma, int *mask_size );
int select_kth_smallest ( const int num_elems, const int k, int *data );
int find_median ( const int num_elems, int *data );
void sort_int ( const int num_elems, int *data );

/* zernike_moments.c */
void free_zernike_basis ( ZernikeBasis * basis );
ZernikeBasis *calc_zernike_basis ( const int max_order, const int radius );
double *calc_zernike_moments ( const Image * in_img, const int label,
			       const ZernikeBasis * basis );

/*this is not needed, because we include math.h at the top */
/* math.h */
/*double cbrt ( double ); */

#ifdef CUDA
	Image* CUDA_filter_ms_rlsf(const Image* in_img, const int r, int alpha, const float sigma, const int iter);
#else
	Image* filter_ms_rlsf(const Image* in_img, const int r, int alpha, const float sigma, const int iter);
#endif

double* calculate_snr(const Image* ref_img, const Image* test_img, FILE* fp);
double calculate_iri(const Image* ref_img, const Image* test_img, FILE* fp);
double* calculate_ssim(const Image* ref_img, const Image* test_img, FILE* fp);
Image* crop_img(const Image* in_img, int crop_size);

void normalize(float* input_array1d, int length);

Image* read_png_file(FILE* fp);
int write_png_file(const Image* _img, FILE* fp);

float filter_road(const Image* in_img, const int alpha);
Image* detect_edge_VR(const Image* in_img, const int threshold);
double calculate_prat(const Image* ref_img, const Image* test_img);

#endif
