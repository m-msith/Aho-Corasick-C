#ifndef _DEFLIB_
#define _DEFLIB_

/**
* Here we have the default libraries that will be used at pretty much any point. 
*
* DefLib can be included at any point, however, it's only really needed at the lowest point, output.h
*
* DefLib.h - By: Martin Smith
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PRINT /* include print statements */
#define TEST  /* run the tests! */

#define NTS 1 /* null terminator size */

#define TRUE 1
#define FALSE 0

#endif
 