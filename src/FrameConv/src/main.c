/**
 *  \file       main.c
 *  \brief      Read a backup file and covert its data to frames.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.07.05  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "FrameConv.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
usage(char *argv[])
{
    fprintf(stderr, "Usage: \n");
    fprintf(stderr, "\tframeconv -i <imei> <backup-file>\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "\t-i: IMEI used to generate frames\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "\tIt generates a frame file from '00000.frm' by ");
    fprintf(stderr, "using the '355826018345180' IMEI.\n");
    fprintf(stderr, "\tThe generated file will be named as the input ");
    fprintf(stderr, "file name plus '.str' suffix\n");
    fprintf(stderr, "\t$ frameconv 00000.frm -i 355826018345180\n");
}

/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char *argv[])
{
    char *ivalue = (char *)0;
    int index;
    int c;

    opterr = 0;
    while ((c = getopt (argc, argv, "i:")) != -1)
    {
        switch (c)
        {
            case 'i':
                ivalue = optarg;
                break;
            default: /* '?' */
                usage(argv);
                exit(EXIT_FAILURE);
        }
    }

    if (optind == 1)
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("ivalue=%s; optind=%d\n", ivalue, optind);
        printf("name argument = %s\n", argv[optind]);
        /* Open backup file as r */
        /* Open converted file as w+ */
        /* read GStatus bytes according to STM32 and convert it to GStatus */
        /* store converted data into ivalue+.str file */
        /* print imei, number of converted registers */
        /* close both files */
    }
    exit(EXIT_SUCCESS);
}

/* ------------------------------ End of file ------------------------------ */
