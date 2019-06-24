/*
 * gsmlog.h
 */

#ifndef __GSMLOG_H__
#define __GSMLOG_H_

void gsmlog_open(char *fname);
void gsmlog_close(void);
void gsmlog_write(char c);

#endif
