
/*
 * gsmlog.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "gsmlog.h"

static FILE *ftbin = NULL;

void
gsmlog_open(char *fname)
{
    if ((ftbin = fopen(fname, "w+b")) == NULL)
    {
        printf("Can't open trace file %s\n", fname);
        exit(EXIT_FAILURE);
    }    
}

void
gsmlog_close(void)
{
    if (ftbin != NULL)
    {
        fflush(ftbin);
        fclose(ftbin);
    }
}

void
gsmlog_write(char c)
{
    if (ftbin != NULL)
    {
        fwrite(&c, 1, 1, ftbin);
        fflush(ftbin);
    }
}

