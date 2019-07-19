/*
 * 	seqerr.h
 * 		Central module to manage the
 * 		driver errors
 */

#include "mytypes.h"
#include "rkhassert.h"

/*
 * 	Error codes
 */


enum
{
	ERR_OK,
	BAD_MAJOR, BAD_MINORS, BAD_CODE,
	BAD_CHANNEL,
	NUM_FATAL_ERRORS
};

#define fatal_driver(x)	rkh_assert(__FILE__, __LINE__)

