/*----------------------------------------------------------------------------
 *      R T L  -  T C P N E T
 *----------------------------------------------------------------------------
 *      Name:    HTTP_ERR.C
 *      Purpose: HTTP Server Error Page Definitions
 *      Rev.:    V3.50
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2009 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>

/* Note: Keep HTTP Error pages small sized. */

/* HTTP Error 401 - Unauthorized access. */
const U8 http_err_401[] = {
        "<h2>HTTP 1.0 401 Error. Unauthorized Access</h2>"
        "You are not authorized to access this server."};

/* HTTP Error 404 - File not Found */
const U8 http_err_404[] = {
        "<h2>HTTP 1.0 404 Error. File Not Found</h2>"
        "The requested URL was not found on this server."};

/* HTTP Error 501 - Not Implemented */
const U8 http_err_501[] = {
        "<h2>HTTP 1.0 501 Error. Not Implemented</h2>"
        "The requested Method is not supported."};

/* Common HTTP Error Page Header */
const U8 http_err_head[] = {
        "<head><title>STM32 Embeded Web Server</title></head>"
        "<body>"};

/* Common HTTP Error Page Footer */
const U8 http_err_foot[] = {
        "<hr><br>"
        "<i>STM32 Embeded Web Server<br>"
        " - Robotell</i>"
        "</body>"};




/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
