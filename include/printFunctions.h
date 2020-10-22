#ifndef PRINTFUNCTIONS_H
#define PRINTFUNCTIONS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define ACTIVECPU "\033[0;36m"
#define MINCOLOUR 31
#define MAXCOLOUR 33
#define MINCOLOUR2 34
#define MAXCOLOUR2 36
#define MINCOLOUR3 34
#define MAXCOLOUR3 36

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "handlers.h"

void scanner(int *y, int em[],  psv0 *vv0,  prntsctrl *psc);
void print_stats_c(prntsctrl psc);
void print_stats_cs(prntsctrl psc);
void print_stats(psv0 vv0);
void print_stats_F(psv0 vv0,  prntsctrl psc,  psheader psh);
void print_stats_Fs(psv0 vv0,  prntsctrl psc,  psheader psh);
void print_stats_v0(psv0 vv0);
void print_header(psheader psh);
void print_header_s(psheader psh);
void print_logheader(int netdis, int corecnt);
void print_log_stats(prntsctrl psc,  psv0 vv0);
void print_div(double stat, int verb);
void print_div_fo(double stat, int verb);
void print_colour(int colour, double stat, double val, int focus);

#endif
