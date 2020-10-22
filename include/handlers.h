#ifndef HANDLERS_H
#define HANDLERS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define APPNAME "CPU STATISTICS MONITOR"
#define AFFCPU "/sys/devices/system/cpu/online"
#define CPUI "/proc/cpuinfo"
#define VER "/proc/version"
#define HN "/etc/hostname"
#define GOV "/sys/devices/system/cpu/cpufreq/policy0/scaling_governor"
#define TEMPNUM "cat /sys/class/hwmon/hwmon*/temp*_input | grep -c ."
#define MSTAT "/proc/meminfo"
#define FREQ "lscpu --parse=MINMHZ,MAXMHZ | egrep -v '#'"
#define ID "uname -s"
#define DEFDEV "ip route list 0.0.0.0/0 | egrep -o 'dev [[:graph:]]*' | sed -e 's/dev //'"
#define DEFDSK "mount | egrep -o '^/dev/[[:graph:]]*' | sed -e 's/.dev.//'"
#define VALMAX "\033[1;37m"
#define HSLEEP 11
#define NSECS 1000000000L
#define USECS 1000000L
#define ILOADLOW "\033[0;34m"
#define CLOADLOW "\033[0;31m"
#define CLOADHI "\033[1;31m"
#define ILOADHI "\033[1;34m"
#define DEFCOLOR "\033[0;37m"
#define HEADER "\033[1;7m"
#define VCPUTH 0.75
#define NSECS2SECS 1.0e-9
#define DESTIP  "208.67.220.220"
#define PSTAT "/proc/stat"
#define NETSTAT "/proc/net/dev"
#define STAT_STRING "%*s %lf %lf %lf %lf %lf %lf %lf"
#define NET_STRING "%*s %lf %lf %*i %*i %*i %*i %*i %*i %lf %lf"
#define PACKET_SIZE     4096
#define DATEALLEN     64
#define SWCOMP     52289

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>
#include <pthread.h>
#include <inttypes.h>
#include <termios.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sched.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "arrayCalc.h"

FILE *fo;
int u, catch;
enum { NS_PER_SECOND = 1000000000 };
typedef unsigned long long cnt_nsec_t;
cnt_nsec_t exptime, cnttime;
pthread_mutex_t lock;
struct sockaddr_in dest_addr;
struct sockaddr_in from;
struct termios g_old_kbd_mode;
typedef struct cpuZ
{
	char ch;
	char disk[8][255];
	char *ndev;
	char *ipa;
	double cStat[32];
	double vStat[65];
	double fStat[65];
	double tStat[65];
	double rStat[9];
	double loadavg[3];
	int cndcnt, cndcnt2; 
	int cpucorecnt;
	double zecdecz;
	double freqs[9];
	double ctemp;
	double dekvar;
	int rollintval;
	int flop;
	int diskcnt;
	int netdiz;
	int verb;
	int multistat;
	int slip;
	int optalg;
	int core;
	int thermalzones;
	double memstat[5];
	int scroll;
	char procsS[9];
	double freqmax;
	double cpuspin0, cpuspin1, cpuspin2;
} cpuZ;

typedef struct timeD
{
    char appstart[255], dev[255];
	double cpuspin;
	int samples, sig, scroll, netdis, colour, ppid, multistat, cpun, dsize;
	double ttstart;
	int slat, step;
	int diskcnt;
	int corecnt;
	double idur, idurv;
	double minfreq, maxfreq;
	char *sysid;
	char *disk[8];
	char *cpuol;
	char *model;
	char *version;
	char *uname;
	char *freqgov;
	int rcnt;
	int verb;
	int mmstat, rate, actlog;
	double secdecs, cntsum;
} timeD;

typedef struct prntsctrl
{
	int scnt, sthis, sthat;
	int colour;
	int idurh;
	int idurm;
	int focus;
	int stat_count;
	double idurs;
	double zecdecz;
	double gen_stat[4][16];
	char runtime[255];
} prntsctrl;

typedef struct psv0
{
	int verb, sthis, sthat;
	int type;
	int colour;
	int core;
	int corecnt;
	int stat_count;
	int counter;
	int focus;
	int multistat;
	int thermalzones;
	double stat[192];
	double max[5][64];
	double min[5][64];
	double mean[5][64];
	double threshold;
	
} psv0;

typedef struct psheader
{
	int roll;
	int xcnt;
	int netdis;
	int colour;
	int focus;
	int pri;
	int ppid;
	int nice;
	int sched;
	int cpun;
	int verb;
	int corecnt;
	int stat_count;
	int scroll;
	int thermalzones;
	int multistat;
	int pcputype;
	char *dev;
	char *ipa;
	double rtt;
} psheader;

// startup information set and acqusition functions
void old_attr(void);
char *getNetDev(void);
int getmem(int *mmstat);
void keyboard_attr(void);
int set_ppid(int colour);
struct winsize termsize (void);
int getsystype(timeD *xx);
int getsysname(timeD *xx);
void print_app_usage(void);
int getcpuonline(timeD *xx);
int getsysmodel(timeD *xx);
int getsysversion(timeD *xx);
int getsysfreqgov(timeD *xx);
int set_priority(int sched[]);
int gethdd(timeD *xx, cpuZ *g);
int set_cpuAffinity(int cpuN, int rcnt);
int getthermalzones(int *gtz, int *vtz, int *ptz);
int getfreqlimits( double *freqmin, double *freqmax, double *maxfreq);

//utility functionss
int whicher(psheader psh);
int rng(const int nMin, const int nMax, const int  nN);

//print functions
char *commaprint(unsigned long long n);
void printci(int term, int colour, int basecolour);
void printcc(char* term, int colour, int basecolour);
void printcl(double term, int colour, int basecolour);
void printcs(char* term, int termsize, int colour, int basecolour);
void printccc(const char* term, int termsize, int colour, int basecolour);

//structure initialization functions
void vv0init(psv0 *vv0, int em[]);
void ginit(cpuZ *g, int em[], double dur[], char *dev, char *ipa);
void xxinit(timeD *xx, int em[], double dur[], char *dev, char *appstart);
void pscshinit(prntsctrl *psc, psheader *psh, int em[], double dur[], char *dev, char *ipa);

//rtt packet handling functions
void rtttimeout(int sig);
unsigned short cal_chksum(unsigned short *addr, int len);
int packuppacket(int datalength, char sendpacket[], uint16_t icmpid[], double rStat[]);
int recv_packet(char recvpacket[], int sockfd, uint16_t icmpid[], double rStat[], struct timespec icmpstart);
int packetunpack(char buf[], int len, uint16_t icmpid[], double rStat[], struct timespec icmpstart, struct timespec icmpend);
void send_packet(char sendpacket[], int sockfd, int datalength, uint16_t icmpid[], double rStat[], struct timespec *icmpstart);

//time-handling functions
double seconds_handler(int dhm[], double seconds);
cnt_nsec_t get_time_nsec(struct timespec exestart);
double check_60(int dhm[], double secondx, double secdecs);
double getproctime(double *cpuspin, double cntsum, int pdiv);
void setruntime(int em[], double dur[], int runtimetype, prntsctrl *psc);
void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td);
void timeFormat(int em13, char runtime[], double dursofar, double secdecs, int sign);
void print_current_time_with_ms(char x_string[], char y_string[], struct timespec exetime);
double cowhunter(int slot, int em15, double dur2, double *cnt1, double *xxcntsum);
double derive_interval(int qual, int rate, double secdecs, int *slot, int *scroll, struct timespec execnt);


#endif
