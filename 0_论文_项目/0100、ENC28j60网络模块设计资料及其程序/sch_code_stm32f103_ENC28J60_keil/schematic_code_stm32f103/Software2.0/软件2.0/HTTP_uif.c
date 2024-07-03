/*----------------------------------------------------------------------------
 *      R T L  -  T C P N E T
 *----------------------------------------------------------------------------
 *      Name:    HTTP_UIF.C
 *      Purpose: HTTP Server User Interface Module
 *      Rev.:    V3.50
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2009 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "Net_Config.h"


/*----------------------------------------------------------------------------
 * HTTP Server File Access CallBack Functions
 *---------------------------------------------------------------------------*/

static char filepath[128] = "\\web\\";

/*--------------------------- http_fopen ------------------------------------*/

void *http_fopen (U8 *fname) {
   /* Open file 'fname' for reading. Return file handle. */
   FILE *f;
   //filepath
   memcpy(&filepath[5],(const char *)fname,strlen((const char *)fname)+1); 
  // printf("reading file %s \n", fname);
   f = fopen ((const char *)filepath, "r");
  // printf("read file %s OK!\n", fname);

   return (f);
}


/*--------------------------- http_fclose -----------------------------------*/

void http_fclose (void *f) {
   /* Close the file opened for reading. */
   fclose (f);
}


/*--------------------------- http_fread ------------------------------------*/

U16 http_fread (void *f, U8 *buf, U16 len) {
   /* Read 'len' bytes from file to buffer 'buf'. The file will be closed, */
   /* when the number of read bytes is less than 'len'. */
   return (fread (buf, 1, len, f));
}


/*--------------------------- http_fgets ------------------------------------*/

BOOL http_fgets (void *f, U8 *buf, U16 size) {
   /* Read a string from file to buffer 'buf'. The file will be closed, */
   /* when this function returns __FALSE. */
   if (fgets ((char *)buf, size, f) == NULL) {
      return (__FALSE);
   }
   return (__TRUE);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
