#ifndef ONEKEYACTION_H
#define ONEKEYACTION_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define MEM_STRING "%*s %lf %*s"
#define LDAVG "/proc/loadavg"
#define TEMPQUERY "cat /sys/class/hwmon/hwmon*/temp*_input"
#define LOAD_STRING "%lf %lf %lf %s"

#include "handlers.h"

void *one_key_actionT(void * q);

#endif
