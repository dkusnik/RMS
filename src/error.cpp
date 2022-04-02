
/** 
 * @file error.c
 * Routines for error handling
 */

#include "image.h"

static int abort_on_err = 1;
static int warn_on = 1;

/** @cond INTERNAL_FUNCTION */

static void
print_var_args ( const char *format, va_list arg_ptr )
{
 vfprintf ( stderr, format, arg_ptr );
 fprintf ( stderr, "\n" );
 fflush ( stdout );
}

/** @endcond INTERNAL_FUNCTION */

/** 
 * @brief Returns the error mode 
 *
 * @return true if programs abort when they encounter an error; 
 *         false otherwise
 *
 * @see #set_err_mode
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
get_err_mode ( void )
{
 return abort_on_err;
}

/** 
 * @brief Sets the error mode 
 *
 * @param[in] err_mode Error mode
 *
 * @return none
 *
 * @see #get_err_mode
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

void
set_err_mode ( const int err_mode )
{
 abort_on_err = err_mode ? 1 : 0;
}

/** 
 * @brief Returns the warning mode 
 *
 * @return true if programs can display warning messages; 
 *         false otherwise
 *
 * @see #set_warn_mode

 * @author M. Emre Celebi
 * @date 10.15.2006
 */

int
get_warn_mode ( void )
{
 return warn_on;
}

/** 
 * @brief Sets the warning mode 
 *
 * @param[in] warn_mode Warning mode
 *
 * @return none
 *
 * @see #get_warn_mode

 * @author M. Emre Celebi
 * @date 10.15.2006
 */

void
set_warn_mode ( const int warn_mode )
{
 warn_on = warn_mode ? 1 : 0;
}

/** @cond INTERNAL_FUNCTION */

/*@printflike@*/
void
do_nothing (  /*@unused@ */ const char *format, ... )
{
 /* do nothing */
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/*@printflike@*/
void
debug ( const char *format, ... )
{
 va_list arg_ptr;

 va_start ( arg_ptr, format );
 print_var_args ( format, arg_ptr );
 va_end ( arg_ptr );
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/*@printflike@*/
void
warning ( const char *format, ... )
{
 if ( get_warn_mode (  ) )
  {
   va_list arg_ptr;

   va_start ( arg_ptr, format );
   print_var_args ( format, arg_ptr );
   va_end ( arg_ptr );
  }
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/*@printflike@*/
void
error ( const char *format, ... )
{
 va_list arg_ptr;

 va_start ( arg_ptr, format );
 print_var_args ( format, arg_ptr );
 va_end ( arg_ptr );

 if ( get_err_mode (  ) )
  {
	 return;
  }
}

/** @endcond INTERNAL_FUNCTION */

/** @cond INTERNAL_FUNCTION */

/*@printflike@*/
void
fatal ( const char *format, ... )
{
 va_list arg_ptr;

 fprintf ( stderr, "FATAL: " );
 va_start ( arg_ptr, format );
 print_var_args ( format, arg_ptr );
 va_end ( arg_ptr );
 /* unconditional abort */
 abort (  );
}

/** @endcond INTERNAL_FUNCTION */

/** 
 * @brief Returns the error message that corresponds to an error code
 *
 * @param[in] err_no Error code
 *
 * @return Error message (string)
 *
 * @author M. Emre Celebi
 * @date 10.15.2006
 */

const char *
error_str ( const int err_no )
{
 switch ( err_no )
  {
   case E_SUCCESS:
    return "success";
   case E_FAILURE:
    return "failure";
   case E_INVARG:
    return "invalid argument";
   case E_INVOBJ:
    return "invalid object";
   case E_INVBPP:
    return "invalid pixel depth";
   case E_NULL:
    return "null pointer";
   case E_NOMEM:
    return "insufficient memory";
   case E_DIVZERO:
    return "divide by zero";
   case E_UNIMPL:
    return "feature not implemented";
   case E_UNFMT:
    return "unknown file format";
   case E_FOPEN:
    return "file open error";
   case E_FREAD:
    return "file read error";
   case E_FEOF:
    return "end of file reached";
   default:
    return "unknown error code";
  }

 /*@notreached@ */
}
