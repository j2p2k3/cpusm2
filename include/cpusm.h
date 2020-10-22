#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define DISLINEWRAP "setterm -linewrap off"
#define ENALINEWRAP "setterm -linewrap on"
#define RATEMAX 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <getopt.h>

#include "handlers.h"
#include "dmtp.h"

//~ em[0]: return value for the dmtp function
//~ em[1]: cli option 'z' - total samples
//~ em[2]: cli option 'n' - network device
//~ em[3]: cli option 'c' - terminal output colour
//~ em[4]: cli option 'q' - time tracking precision [0 == off, 1* == on]
//~ em[5]: cli option 'y' - execution nice value [-20 to 20]
//~ em[6]: cpusm process identification number [pid]
//~ em[7]: cli option 'a' - cpusm core affinity
//~ em[8]: cli option 'b' - core optimization algorithm [<0*> kernel, <1> min.cpu, <2> max.cpu, <3> random]
//~ em[9]: cli option 'p' - time tracking alogrithm [0* - dynamic, 1 - static]
//~ em[10]: cli option 'v' - cpusm statistics output type [0 - cpu kernel, 1 - network, 2 - core utilization, 3 - temperature, 4* core frequency]
//~ em[11]: system processor count per sysconf(_SC_NPROCESSORS_CONF)
//~ em[12]: running sample count
//~ em[13]: cli option 's' - cpusm output mode [0* fixed-line, 1 scrolling]
//~ em[14]: cli option 'k' - cpusm statistics output mode [0 - uni/em[10]-fixed, 1* multi]
//~ em[15]: cli option 'r' - sample rate per second [1 - 1000, 10*]
//~ em[16]: cli option 'l' - cpusm output mode [0* - stdout/terminal, 1 - stdout/terminal+file, 2 - file]
//~ em[17]: cli option 'x' - cpusm periodic header display interval [40* - runtime switch default]
//~ em[18]: copy of em[17] initial value
//~ em[19]: cli option 'u' - cpusm statistics rolling average interval [1* - 999] seconds
//~ em[20]: cli option 'e' - cpusm 'slip' interval [4*]
//~ em[21]: cli option 'j' - cpusm statistics output step interval [1/j * r * u]
//~ em[22]: copy of em[21] initial value
//~ em[23]: cpusm statistics focus value
//~ em[24]: cli option 'o' - cpusm application execution priority [0* - 99]
//~ em[25]: cli option 'w' - cpusm process scheduling algorithm [0* - 6]
//~ em[26]: cli option 'i' - cpusm start information mode [0* - normal, 1 - abbreviated]
//~ em[27]: cli option 'm' - cpusm statistics display mode [0* - normal, 1 - short]
//~ em[28]: cli option '#' - cpusm rtt destination.ip specified
//~ dur[0]: cli option 'd' - cpusm execution duration [x.y seconds]
//~ dur[1]: copy of dur[0] initial value
//~ dur[2]: cli option 't' - cpusm sample interval [conflicts with em[15]]
//~ dur[3]: cli option 'g' - coefficient of statistics sampling interval [g * t]
//0	int scanning = 0;
//1	int x = 0;
//2	int y = 0;
//3	int xcnt = 0;
//4	int scsw = 0;
//5	int pdiv = em[11];
//6	int slot = 0;
//7	int step = 1;
//8	int stepper = em[21];
//9	int jay = stepper==0?0:2;
//10	int jaystep = stepper!=0?1:0;
//11	int stephold = 1;
//12	int runtimetype = 0;
//13	int runtimecounttype = 0;
//14	int threadaligntrack = 0;
