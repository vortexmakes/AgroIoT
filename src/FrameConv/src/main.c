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
static char finName[96], foutName[96];
static uint8_t buf[128], frame[128];

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
    int index, c, result, res;
    FILE *fin, *fout;
    size_t finSize, foutSize, nElem, frameSize;
    GStatus status;
    int nRead;

    opterr = 0;
    result = 0;
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
        result = 1;
    }
    else
    {
        strcpy(finName, argv[optind]);
        fin = fopen(finName, "r");
        if (fin != (FILE *)0)
        {
            fseek(fin, 0, SEEK_END);
            finSize = ftell(fin);
            rewind(fin);

            sprintf(foutName, "%s.str", finName);
            fout = fopen(foutName, "w+");
            if (fout != (FILE *)0)
            {
                nRead = 0;
                while (feof(fin) == 0)
                {
                    nElem = fread(buf, SIZEOF_GSTATUS_STM32, 1, fin);
                    if (nElem == 1)
                    {
                        ++nRead;
                        res = FrameConv_STM32ToX86(&status, 
                                                   buf, 
                                                   SIZEOF_GSTATUS_STM32);
                        if (res == 1)
                        {
                            result = 1;
                            break;
                        }
                        else
                        {
                            res = FrameConv_GStatusToFrame(frame, &status, 
                                                           &frameSize);
                            if (res == 0)
                            {
                                frame[frameSize] = '\n';
                                fwrite(frame, frameSize + 1, 1, fout);
                            }
                        }
                    }
                }
                printf("Read file %s of %d [bytes], which is equivalent to ", 
                       finName, finSize);
                printf("%d [registers]\n", finSize/SIZEOF_GSTATUS_STM32);
                printf("Process %d registers from %s ", nRead, finName);
                printf("into %s\n", foutName);
                fclose(fout);
            }
            else
            {
                fprintf(stderr, "Cannot open file %s\n", foutName);
                result = 1;
            }
            /* store converted data into ivalue+.str file */
            /* print imei, number of converted registers */
            fclose(fin);
        }
        else
        {
            fprintf(stderr, "Cannot open file %s\n", finName);
            result = 1;
        }
    }
    return result;
}

/* ------------------------------ End of file ------------------------------ */
