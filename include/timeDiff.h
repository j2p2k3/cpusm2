#ifndef TIMEDIFF_H
#define TIMEDIFF_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "handlers.h"

void time_diff(timeD xx, double rStat[], int em[], struct timespec exestart, struct timespec exeend);

#endif
