/**
 *  \file       IpValid.c
 *  \brief      Ip format validation
 *              see: geeksforgeeks.org/program-to-validate-an-ip-address
 *              Driver program to test above functions:
 *
 *              int main() 
 *              { 
 *                  char ip1[] = "128.0.0.1"; 
 *                  char ip2[] = "125.16.100.1"; 
 *                  char ip3[] = "125.512.100.1"; 
 *                  char ip4[] = "125.512.100.abc"; 
 *                  is_valid_ip(ip1)? printf("Valid\n"): printf("Not valid\n"); 
 *                  is_valid_ip(ip2)? printf("Valid\n"): printf("Not valid\n"); 
 *                  is_valid_ip(ip3)? printf("Valid\n"): printf("Not valid\n"); 
 *                  is_valid_ip(ip4)? printf("Valid\n"): printf("Not valid\n"); 
 *                  return 0; 
 *              } 
 *
 *                Output:
 *                   Valid
 *                   Valid
 *                   Not valid
 *                   Not valid
 *
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.04.17  Daba  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <stdlib.h>
#include <string.h>
#include "IpValid.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define IP_DELIMITER "."

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/*
 *  return 1 if string contain only digits, else return 0 
 */
static
int validDigit(char *ip) 
{ 
    while (*ip) { 
        if (*ip >= '0' && *ip <= '9') 
            ++ip; 
        else
            return 0; 
    } 
    return 1; 
} 

/* ---------------------------- Global functions --------------------------- */
/*
 *  return 1 if IP string is valid, else return 0 
 */
int
IpValid_check(char *ip) 
{ 
    int num, dots = 0; 
    char *ptr; 
  
    if (ip == NULL) 
        return 0; 
  
    // See following link for strtok() 
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/strtok_r.html 
    ptr = strtok(ip, IP_DELIMITER); 
  
    if (ptr == NULL) 
        return 0; 
  
    while (ptr) { 
  
        /* after parsing string, it must contain only digits */
        if (!validDigit(ptr)) 
            return 0; 
  
        num = atoi(ptr); 
  
        /* check for valid IP */
        if (num >= 0 && num <= 255) { 
            /* parse remaining string */
            ptr = strtok(NULL, IP_DELIMITER); 
            if (ptr != NULL) 
                ++dots; 
        } else
            return 0; 
    } 
  
    /* valid IP string must contain 3 dots */
    if (dots != 3) 
        return 0; 
    return 1; 
} 

/* ------------------------------ End of file ------------------------------ */
