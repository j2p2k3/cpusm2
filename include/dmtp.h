#ifndef DMTP_H
#define DMTP_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "handlers.h"
#include "timeDiff.h"
#include "arrayCalc.h"
#include "printFunctions.h"
#include "statThreads.h"
#include "oneKeyAction.h"
#include "roundtriptimeThread.h"

int dmtp(char *outputfile, char *appstart, char *dev, int em[], double dur[], char *ipa);

#endif
