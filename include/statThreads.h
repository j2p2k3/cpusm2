#ifndef STATTHREADS_H
#define STATTHREADS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif





#include "handlers.h"
#include "arrayCalc.h"




void *cpustatsT(void * g);
void *justcpustatsT(void * g);
void *justvpustatsT(void * g);
void *justnetstatsT(void * g);
void *simplecpustatsT(void * s);

#endif
