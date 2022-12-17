#include "./handlers.h"

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
td->tv_nsec = t2.tv_nsec - t1.tv_nsec - SWCOMP;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}
int whicher(psheader psh)
{
	int onewhich = 0;
	int whichone = 0;
	if ( psh.multistat == 1 ) {
		whichone = psh.corecnt>psh.thermalzones?psh.corecnt:psh.thermalzones;
		if ( u == 0 ) onewhich = whichone>14?whichone:14;
		if ( u != 0 ) onewhich = whichone>8?whichone:8;
	}else{
		if ( psh.verb == 0 ) onewhich = 8;
		if ( psh.verb == 1 ) {
			if ( u == 0 ) onewhich = 14;
			if ( u != 0 ) onewhich = 8;
		}
		if ( psh.verb == 2 || psh.verb == 4 ) onewhich = psh.corecnt;
		if ( psh.verb == 3 ) onewhich = psh.thermalzones;
	}
	return onewhich;
}
void printcc(char* term, int colour, int basecolour)
{
	if ( basecolour != 0 ) {
		if ( colour == 1 ) printf(ILOADLOW);
		if ( colour == 2 ) printf(CLOADLOW);
		if ( colour == 3 ) printf(VALMAX);
		if ( colour == 4 ) printf(HEADER);
	}
	printf("%s", term);
	return;
}
void printcs(char* term, int termsize, int colour, int basecolour)
{
	if ( basecolour != 0 ) {
		if ( colour == 1 ) printf(ILOADLOW);
		if ( colour == 2 ) printf(CLOADLOW);
		if ( colour == 3 ) printf(VALMAX);
		if ( colour == 4 ) printf(HEADER);
	}
	printf("%-*s", termsize, term);
	return;
}
void printccc(const char* term, int termsize, int colour, int basecolour)
{
	if ( basecolour != 0 ) {
		if ( colour == 1 ) printf(ILOADLOW);
		if ( colour == 2 ) printf(CLOADLOW);
		if ( colour == 3 ) printf(VALMAX);
		if ( colour == 4 ) printf(HEADER);
	}
	printf("%-*s", termsize, term);
	return;
}
void printci(int term, int colour, int basecolour)
{
	if ( basecolour != 0 ) {
		if ( colour == 1 ) printf(ILOADLOW);
		if ( colour == 2 ) printf(CLOADLOW);
		if ( colour == 3 ) printf(VALMAX);
		if ( colour == 4 ) printf(HEADER);
	}
	printf("%i", term);
	return;
}
void printcl(double term, int colour, int basecolour)
{
	if ( basecolour != 0 ) {
		if ( colour == 1 ) printf(ILOADLOW);
		if ( colour == 2 ) printf(CLOADLOW);
		if ( colour == 3 ) printf(VALMAX);
		if ( colour == 4 ) printf(HEADER);
	}
	printf("%.2lf", term);
	return;
}
int rng(const int nMin, const int nMax, const int  nN)
{
	int nR = 0;
	for (int i = 0; i < nN; i++) {
		nR = rand()%(nMax-nMin) + nMin;
	}
	return nR;
}
int rnn(const int  nN)
{
	int nR = 0;
	nR = nN+2;
	return nR;
}
double derive_interval(int qual, int rate, double secdecs, int *slot, int *scroll, struct timespec execnt)
{
	double decvar = 0;
	if ( qual == 1 && *slot != 11) {
		cnttime = get_time_nsec(execnt);
		if ( *slot == 19 && *scroll != 0 ) {
			exptime += (rate*secdecs*NSECS*HSLEEP);
			exptime += NSECS * secdecs;
			*slot = 1;
		} else {
			exptime += NSECS * secdecs;
		}
		if (cnttime < exptime && *slot != 2) {
			decvar = (exptime - cnttime);
			decvar /= NSECS;
			*slot=*slot==4?5:4;
		} else if (cnttime > exptime && *slot != 4) {
			if ( cnttime - exptime <= secdecs  || *slot ==1 ) {
				decvar = cnttime - exptime;
				decvar /= NSECS;
				*slot=*slot==2?3:2;
			}
		} else {
			decvar = secdecs;
			*slot=7;
		}
	} else {
		decvar = secdecs;
		*slot = *slot==11?0:*slot;
	}
	return decvar;
}
char *getNetDev(void)
{
	char netdevcmd[75];
	char *devdev;
	devdev = NULL;
	strcpy(netdevcmd, DEFDEV);
	FILE *gndp = popen(netdevcmd, "r");
	size_t linesiz=0;
	getline(&devdev, &linesiz, gndp);
	pclose(gndp);
	return strtok(devdev, "\n");
}
struct winsize termsize (void)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}
void old_attr(void)
{
	tcsetattr(0, TCSANOW, &g_old_kbd_mode);
	return;
}
void keyboard_attr(void)
{
	struct termios new_kbd_mode;
	tcgetattr(0, &g_old_kbd_mode);
	memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
	new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
	new_kbd_mode.c_cc[VTIME] = 0;
	new_kbd_mode.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_kbd_mode);
	atexit(old_attr);
	return;
}
cnt_nsec_t get_time_nsec(struct timespec exestart)
{
	clock_gettime(CLOCK_MONOTONIC, &exestart);
	return NSECS *(double)exestart.tv_sec + exestart.tv_nsec;
}
int getfreqlimits( double *freqmin, double *freqmax, double *maxfreq)
{
	int erresult = 0;
	char freqcmd[128];
	*freqmin = *freqmax = *maxfreq = 0;
	strcpy(freqcmd, FREQ);
	FILE *fp;
	fp = popen(freqcmd, "r");
	if ( fp != NULL ) {
		erresult = 1;
		size_t linesiz=0;
		char* linebuf;
		linebuf=NULL;
		ssize_t linelen=0;
		if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
			sscanf(linebuf, "%lf,%lf", &*freqmin, &*freqmax);
			*maxfreq = *freqmax;
		}
		free(linebuf);
		linebuf=NULL;
	}
	pclose(fp);
	return erresult;
}
int getsystype(timeD *xx)
{
	int erresult = 0;
	char sysidcmd[128];
	char sysid[128];
	char na[4] = "n/a";
	xx->sysid = strdup(na);
	free(xx->sysid);
	strcpy(sysidcmd, ID);
	FILE *fp;
	fp = popen(sysidcmd, "r");
	if ( fp != NULL ) {
		erresult = 1;
		size_t linesiz=0;
		char* linebuf;
		linebuf=NULL;
		ssize_t linelen=0;
		if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
			if (strcmp(strtok(linebuf, " \n"), "Linux") == 0) {
				strcpy(sysid, "linux");
			}
			if (strcmp(strtok(linebuf, " \n"), "Darwin") == 0) {
				strcpy(sysid, "mac-os");
			}
			xx->sysid = strdup(sysid);
		}
		free(linebuf);
		linebuf=NULL;
	}else{
		xx->sysid = strdup(na);
	}
	pclose(fp);
	return erresult;
}
int getmem(int *mmstat)
{
	int erresult = 0;
	if( access(MSTAT, F_OK ) != -1 ) {
		FILE *fp;
		fp = fopen(MSTAT, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				strtok(linebuf," ");
				*mmstat = atoi(strtok(NULL, " "));
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			*mmstat = -1;
		}
		fclose(fp);
	}else{
		*mmstat = -1;
	}
	return erresult;
}
int gethdd(timeD *xx, cpuZ *g)
{
	int erresult = 0;
	char hddcmd[61];
	char na[4] = "n/a";
	xx->diskcnt = g->diskcnt = 0;
	xx->disk[0] = strdup(na);
	free(xx->disk[0]);
	strcpy(hddcmd, DEFDSK);
	FILE *fp;
	fp = popen(hddcmd, "r");
	if ( fp != NULL ) {
		erresult = 1;
		int az = 0;
		size_t linesiz=0;
		char* linebuf;
		linebuf=NULL;
		ssize_t linelen=0;
		while ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
			sprintf(g->disk[az], "%s", strtok(linebuf, " \n"));
			xx->disk[az] = strdup(g->disk[az]);
			az++;
		}
		xx->diskcnt = g->diskcnt = az;
		free(linebuf);
		linebuf=NULL;
	}else{
		xx->diskcnt = g->diskcnt = 0;
		xx->disk[0] = strdup(na);
	}
	pclose(fp);
	return erresult;
}
int getcpuonline(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char cpuol[32];
	char na[4] = "n/a";
	if( access(AFFCPU, F_OK ) != -1 ) {
		fp = fopen(AFFCPU, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sprintf(cpuol, "%s", (strtok(linebuf, "\n")));
				xx->cpuol = strdup(cpuol);
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->cpuol = strdup(na);
		}
		fclose(fp);
	}else{
		xx->cpuol = strdup(na);
	}
	return erresult;
}
int getsysversion(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char version[128];
	char na[4] = "n/a";
	if( access(VER, F_OK ) != -1 ) {
		fp = fopen(VER, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sprintf(version, "%s", (strtok(linebuf, " ")));
				for (int i=0; i<2; i++) {
					sprintf(version, "%s", (strtok(NULL, " ")));
				}
				xx->version = strdup(version);
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->version = strdup(na);
		}
		fclose(fp);
	}else{
		xx->version = strdup(na);
	}
	return erresult;
}
int getsysname(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char uname[255];
	char na[4] = "n/a";
	if( access(HN, F_OK ) != -1 ) {
		fp = fopen(HN, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sprintf(uname, "%s", (strtok(linebuf, "\n")));
				xx->uname = strdup(uname);
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->uname = strdup(na);
		}
		fclose(fp);
	}else{
		xx->uname = strdup(na);
	}
	return erresult;
}
int getsysfreqgov(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char freqgov[255];
	char na[4] = "n/a";
	if( access(GOV, F_OK ) != -1 ) {
		fp = fopen(GOV, "r");
		if ( fp != NULL ) {
			erresult = 1;	
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sscanf(linebuf, "%s", freqgov);
				xx->freqgov = strdup(freqgov);
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->freqgov = strdup(na);
		}
		fclose(fp);
	}else{
		xx->freqgov = strdup(na);
	}
	return erresult;
}
int getsysmodel(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char model[2][255];
	char na[4] = "n/a";
	if( access(CPUI, F_OK ) != -1 ) {
		fp = fopen(CPUI, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			while ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sprintf(model[0], "%s", (strtok(linebuf, ":")));
				sprintf(model[1], "%s", (strtok(NULL, ":\n")));
				if (strcmp(model[0], "model name\t") == 0) {
					xx->model = strdup(model[1]);
					break;
				}
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->model = strdup(na);
		}
		fclose(fp);
	}else{
		xx->model = strdup(na);
	}
	return erresult;
}
int getthermalzones(int *gtz, int *vtz, int *ptz)
{
	int erresult = 0;
	FILE *fp;
	char thermalcmd[64];
	strcpy(thermalcmd, TEMPNUM);
	*gtz = *vtz = *ptz = 0;
	fp = popen(thermalcmd, "r");
	if ( fp != NULL ) {
		erresult = 1;
		size_t linesiz=0;
		char* linebuf;
		linebuf=NULL;
		getline(&linebuf, &linesiz, fp);
		*gtz = *vtz = *ptz = atoi(linebuf);
		free(linebuf);
		linebuf=NULL;
	}else{
		*gtz = *vtz = *ptz = -1;
	}
	pclose(fp);
	return erresult;
}
double check_60(int dhm[], double secondx, double secdecs)
{
	int zeczint;
	char secheck[7];
	sprintf(secheck, "%.0lf", secondx);
	zeczint = atoi(secheck);
	if ( zeczint == 60 ) {
		dhm[2]++;
		if ( dhm[2] == 60 ) {
			dhm[1]++;
			dhm[2] = 0;
		}
		secondx = 0;
	}
	return secondx;
}
int set_ppid(int colour)
{
	pid_t ppid;
	ppid = getpid();
	return ppid;
}
int set_priority(int sched[])
{
	int setret[3] = {};
	int getret[3] = {};
	int process = PRIO_PROCESS;
	struct sched_param param ={0};
	param.sched_priority = sched[2];
	setret[0] = setpriority(process, sched[0], sched[1]);
	if ( setret[0] == -1 ) printf("\nfailed to set process priority %i\n", sched[2]);
	setret[1] = sched_setscheduler( 0, sched[3], &param );
	setret[2] = sched_setparam( 0, &param );
	getret[1] = sched_getscheduler(sched[0]);
	getret[2] = sched_getparam(sched[0], &param);
	sched[3] = getret[1];
	sched[2] = param.sched_priority;
	printf("scheduler: %i priority: %i\n", getret[1], param.sched_priority);
	return getret[0];
}
int set_cpuAffinity(int cpuN, int rcnt)
{
	uint32_t i = 0;
	uint8_t flag;
	cpu_set_t mask;
	CPU_ZERO( &mask );
	CPU_SET( cpuN, &mask );
	if( sched_setaffinity( 0, sizeof(mask), &mask ) == 0 ) {
		cpu_set_t mycpuid;
		sched_getaffinity(0, sizeof(mycpuid), &mycpuid);
	    if ( rcnt == 0 ) printf("core affinity configured - application pinned to core ");
	    flag = 0;
	    for (i = 0; i < sizeof(cpu_set_t); i ++) {
	        if (CPU_ISSET(i, &mycpuid)) {
	            flag=flag==0?1:0;
	                if ( flag == 1 && rcnt == 0 ) printf("%ui", i);
	                if ( flag == 0 && rcnt == 0 ) printf(",%ui", i);
	        }
	    }
	}
    if ( rcnt == 0 ) printf("\n");
    return i;
}
char *commaprint(unsigned long long n)
{
	char *p;
	static int comma = '\0';
	static char retbuf[30];
	p = &retbuf[sizeof(retbuf)-1];
	int i = 0;
	if ( comma == '\0' ) {
		struct lconv *lcp = localeconv();
		if (lcp != NULL) {
			if ( lcp->thousands_sep != NULL && *lcp->thousands_sep != '\0' ) {
				comma = *lcp->thousands_sep;
			}else{
				comma = ',';
			}
		}
	}
	*p = '\0';
	while ( n != 0 ) {
		if( i % 3 == 0 && i != 0 ) {
			*--p = comma;
		}
		*--p = '0' + n % 10;
		n /= 10;
		i++;
	}
	return p;
}
double seconds_handler(int dhm[], double seconds)
{
	dhm[0] = (int)seconds/86400;
	seconds = fmod(seconds, 86400);
	dhm[1] = (int)seconds/3600;
	seconds = fmod(seconds, 3600);
	dhm[2] = (int)seconds/60;
	return fmod(seconds, 60);
}
void timeFormat(int em13, char runtime[], double dursofar, double secdecs, int sign)
{
	int dhm[3] = {};
	double secondx = seconds_handler(dhm, dursofar);
	if ( sign == SIGSTOP || sign == SIGINT ) secondx -= secdecs;
	if ( em13 != 2 ) {
		if ( secondx >= 60L - secdecs ) {
			secondx = check_60(dhm, secondx, secdecs);
		}
	}
	if ( dhm[0] == 0 ) {
		if ( dhm[1] == 0 ) {
			if ( dhm[2] == 0 ) {
				if ( secondx < 60L ) {
					sprintf(runtime, "       %06.3lf", secondx);
				}else{
					sprintf(runtime, "        %05.2lf", secondx);
				}
			}else{
				sprintf(runtime, "     %02i:%05.2lf", dhm[2], secondx);
			}
		}else{
			sprintf(runtime, "%4i:%02i:%05.2lf", dhm[1], dhm[2], secondx);
		}
	}else{
		dhm[1] += dhm[0]*24;
		sprintf(runtime, "%4i:%02i:%05.2lf", dhm[1], dhm[2], secondx);
	}
	return;
}
void print_current_time_with_ms(char x_string[], char y_string[], struct timespec exetime)
{
	long  ms;
	time_t ss;
	struct tm* myty;
	ss  = exetime.tv_sec;
	ms = (double)exetime.tv_nsec / (double)1.0e7;
	if (ms > 99) {
		ss++;
		ms = 0;
	}
	myty = localtime (&ss);
	strftime(x_string, 40, "%H:%M:%S", myty);
	sprintf(y_string, ".%02ld", ms);
	return;
}
void setruntime(int em[], double dur[], int runtimetype, prntsctrl *psc)
{
	double secondssofar = dur[0];
	double secondstogo = 0L;
	if ( dur[1] != 0L ) {
		secondstogo = dur[1] - dur[0];
	}else if ( em[1] != 0 ) {
		if ( dur[2] != 0 ) {
			secondstogo = (double)em[1]*dur[2] - dur[0];
		}else{
			secondstogo = (double)(em[1]*1/em[15]) - dur[0];
		}
	}else{
		secondstogo = 0L;
	}
	double whichseconds = runtimetype==0?secondssofar:secondstogo;
	double switch60 = runtimetype==0?2:em[13];
	timeFormat(switch60, psc->runtime, whichseconds, dur[2], 0);
	return;
}
double cowhunter(int slot, int em15, double dur2, double *cnt1, double *xxcntsum)
{
	struct timespec execnt;
	clock_gettime(CLOCK_MONOTONIC, &execnt);
	double cnt0 = ((double)execnt.tv_sec + NSECS2SECS*execnt.tv_nsec);
	double cnt = cnt0 - *cnt1;
	if ( slot == 19 ) cnt -= em15*dur2*HSLEEP;
	//~ *cntsum += cnt;
	*xxcntsum += cnt;
	*cnt1 = cnt0;
	return cnt;
}
double getproctime(double *cpuspin, double cntsum, int pdiv)
{
	struct timespec exe = {0,0};
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &exe);																							
	*cpuspin = ((double)exe.tv_sec +  NSECS2SECS*(double)exe.tv_nsec);
	return (*cpuspin/cntsum*100L)/pdiv;
}
void print_app_usage(void)
{
	const char *appusage[66];
	appusage[0] = "cpusm [option]<value>...[option]<value>      \n";
	appusage[1] = "_______________________________________________________________________________________________________________\n";
	appusage[2] = "command-line options [* default]             \n\n";
	appusage[3] = "-a || --core: cpusm core affinity            \n";
	appusage[4] = "-b || --core-opt: cpusm core optimization [<*0> kernel, <1> min.cpu, <2> max.cpu, <3> random] \n";
	appusage[5] = "-c || --colour: stdout/terminal display colour [*0 - 7]\n";
	appusage[6] = "-d || --duration: cpusm execution duration [x.y seconds] \n";
	appusage[7] = "-e || --slip: deferal interval for process intensive statistics acquisition [frequency, temperature, rtt] [slip>=1, *4]\n";
	appusage[8] = "-f || --file: <filename>: log destination    \n";
	appusage[9] = "-g || --coefficient: statistics sampling interval coefficient [g * t, *1]\n";
	appusage[10] = "-h || --help: application usage/execution assistance          \n";
	appusage[11] = "-i || --shortmode: cpusm start information mode [*0 - normal, 1 - abbreviated] \n";
	appusage[12] = "-j || --step: cpusm statistics output step interval [1/j * r * u] \n";
	appusage[13] = "-k || --mode: cpusm statistics output mode [*0 - uni, 1 - multi] \n";
	appusage[14] = "-l || --log cpusm output mode [*0 - stdout/terminal, 1 - stdout/terminal+file, 2 - file] \n";
	appusage[15] = "-m || --display-size: [cpusm statistics mode [*0 - normal, 1 - abbreviated] \n";
	appusage[16] = "-n || --network: network interface to monitor [-k1 || -k0 -v1] [*interface bound to 0.0.0.0/0]\n";
	appusage[17] = "-o || --priority: cpusm application execution priority [*0 - 99]\n";
	appusage[18] = "-p || --timing: time tracking alogrithm [0* - dynamic, 1 - static]\n";
	appusage[19] = "-q || --precision: time tracking precision [0 - off, *1 - on]\n";
	appusage[20] = "-r || --rate: sample rate per second [1 - 1000, *10]\n";
	appusage[21] = "-s || --scrolling: output display mode [*0 - fixed-line, 1 - scrolling]\n";
	appusage[22] = "-t || --interval: sample interval [s.n seconds, *0.1, minimum == 0.001] \n";
	appusage[23] = "-u || --rolling: cpusm statistics rolling average interval [*1 - 999] seconds \n";
	appusage[24] = "-v || --category: cpusm statistics category \n";
	appusage[25] = "        0: cpu.kernel mode                  \n";
	appusage[26] = "        1: network mode [use in conjunction with -n]\n";
	appusage[27] = "        2: cpu.core mode                    \n";
	appusage[28] = "        3: thermal mode                     \n";
	appusage[29] = "        *4: core.frequency mode             \n";
	appusage[30] = "-w || --scheduler: cpusm process scheduling algorithm [*0 - 6]\n";
	appusage[31] = "-x || --header-interval: periodic column heading display [*0 == off, *40 == runtime switch default]\n";
	appusage[32] = "-y || --nice: cpusm execution nice value [-20 to 20, *0] \n";
	appusage[33] = "-z || --samples: total samples [>=0, *0 - no limit] \n";
	appusage[34] = "-# || --ipaddr: destination ip.address for round.trip.time [rtt] network testing [*208.67.222.222]\n";
	appusage[35] = "_______________________________________________________________________________________________________________\n";
	appusage[36] = "runtime options                             \n\n";
	appusage[37] = "<h>: show runtime help                      \n";
	appusage[38] = "<a>: switch core affinity [base process only - established threads will not switch\n";
	appusage[39] = "<b>: switch 'count' statistic [running-samples*, remaining-samples]\n";
	appusage[40] = "<c>: switch stdout colour                   \n";
	appusage[41] = "<q>: quit cpusm                             \n";
	appusage[42] = "<j>: enable/disable statistics step display \n";
	appusage[43] = "<k>: enable/disable variable colour-mode focus scanning \n";
	appusage[44] = "     - <+>: increase focus scanning interval \n";
	appusage[45] = "     - <->: descrease focus scanning interval\n";
	appusage[46] = "<l>: enable/disable fixed colour-mode focus scanning \n";
	appusage[47] = "<m>: enable/switch colour-mode focus        \n";
	appusage[48] = "<n>: switch 'runtime' statistic [running-time*, remaining-time]\n";
	appusage[49] = "<p>: switch cpu utilization calculation     \n";
	appusage[50] = "     - pcpu: full process including all threads\n";
	appusage[51] = "     - pcpu.0: parent process               \n";
	appusage[52] = "     - pcpu.1: comand and control thread    \n";
	appusage[53] = "     - pcpu.2: data acquisition thread      \n";
	appusage[54] = "     - pcpu.3: network rtt thread           \n";
	appusage[55] = "<s>: display cpusm interim summary statistics \n";
	appusage[56] = "<v>: switch terminal display width          \n";
	appusage[57] = "<x>: enable/disable periodic header printing \n";
	appusage[58] = "<z>: restart cpusm                          \n";
	appusage[59] = "<=>: display interim cpusm min/max/avg statistics\n";
	appusage[60] = "</>: switch terminal scrolling mode [fixed-line/scrolling]\n";
	appusage[61] = "<.>: display column headings                \n";
	appusage[62] = "<`>: switch pcpu accounting [per-core/aggregate]\n";
	appusage[63] = "<,>: enable/diable stdout [file logging-mode]]\n";
	appusage[64] = "<1>: enable/disable precise time-tracking   \n";
	appusage[65] = "<SPACE>: switch display mode [cpu.kernel/cpu.core/core.frequency/thermal/network]\n\n";
	
	keyboard_attr();
	struct winsize w;
	w = termsize();
	fd_set readfds;
	int pagecount = 0;
	int fd_stdin = (fileno(stdin));
	FD_ZERO(&readfds);
	FD_SET(fileno(stdin), &readfds);
	for ( int i=0;i<66;i++ ) {
		if ( i != 0 && i % (w.ws_row-4) == 0 ) {
			pagecount++;
			FD_SET(fileno(stdin), &readfds);
			if ( pagecount == 1 ) printf("\npage:%i <space>-continue, <q>-quit\n", pagecount);			
			if ( pagecount != 1 ) printf("\npage:%i <space>-continue, <b>-back, <q>-quit\n", pagecount);			
			int srett = pselect(fd_stdin + 1, &readfds, NULL, NULL, NULL, NULL);
			while ( srett > 0 ) {
				char cch = getchar();
				if ( cch == ' ') {
					printf("\x1b[2A");
					break;
				}else if ( cch == 'b') {
					if ( pagecount >= 2 ) {
						printf("\x1b[1A");
						i -= 2*(w.ws_row-4);
						pagecount-=2;
						break;
					}
				}else if ( cch == 'q') {
					exit(0);
				}else{
					continue;
				}
			}
		}
		printccc(appusage[i], strlen(appusage[i]), 0, 0);
	}
	return;
}
void ginit(cpuZ *g, int em[], double dur[], char *dev, char *ipa)
{
	g->cpucorecnt = (double)em[11];
	g->rollintval = (double)em[19];
	if ( dur[3] == 0 ) g->zecdecz = dur[2];
	if ( dur[3] != 0 ) g->zecdecz = dur[2]*dur[3];
	g->netdiz = em[2];
	g->ndev = strdup(dev);
	g->ipa = strdup(ipa);
	g->verb = em[10];
	g->multistat = em[14];
	g->slip = em[20];
	g->optalg = em[8];
	g->scroll = em[13];
	g->ctemp = 0;
	g->flop  = 0;
	g->cpuspin0 = g->cpuspin1 = g->cpuspin2 = 0;
	arrayInit(g->cStat, 32);
	arrayInit(g->vStat, 64);
	arrayInit(g->fStat, 65);
	arrayInit(g->rStat, 9);
	arrayInit(g->freqs, 9);
	arrayInit(g->memstat, 5);
	arrayInit(g->loadavg, 3);
	char procsS[9] = "";
	memcpy(g->procsS, procsS, sizeof(procsS));
	g->ch = 0;
	if ( em[8] != 0 ) {
		g->cpucorecnt = em[11];
		g->optalg = em[8];
	}
	return;
}
void xxinit(timeD *xx, int em[], double dur[], char *dev, char *appstart)
{
	xx->colour = em[3];
	xx->cpun = em[7];
	xx->samples = em[1];
	xx->sig = 0;
	xx->netdis = em[2];
	xx->corecnt = em[11];
	xx->idurv = xx->idur = dur[1];
	xx->rate = em[15];
	xx->actlog = em[16];
	xx->secdecs = dur[2];
	xx->rcnt = 0;
	xx->slat = 0;
	xx->ppid = em[6];
	xx->step = em[22];
	xx->dsize = em[27];
	sprintf(xx->appstart, "%s", appstart);
	sprintf(xx->dev, "%s", dev);
	xx->cntsum = 0;
	xx->multistat = em[14];
	xx->scroll = em[13];
	xx->verb = em[10];
	return;
}
void vv0init(psv0 *vv0, int em[])
{
	vv0->multistat = em[14];
	vv0->sthis = 0;
	vv0->sthat = 1;
	vv0->focus = em[23];
	arrayInit(vv0->stat, 192);
	for ( int i=0;i<5;i++ ) {
		arrayInit(vv0->min[i], 64);
		arrayInit(vv0->max[i], 64);
		arrayInit(vv0->mean[i], 64);
	}
	vv0->counter = 1;
	return;
}
void pscshinit(prntsctrl *psc, psheader *psh, int em[], double dur[], char *dev, char *ipa)
{
	psh->verb = em[10];
	psh->multistat = em[14];
	psc->scnt = 0;
	psc->sthis = 0;
	psc->sthat = 1;
	psc->colour = 0;
	psc->focus = psh->focus = em[23];
	for ( int i=0;i<4;i++ ) {
		arrayInit(psc->gen_stat[i], 16);
	}
	psh->roll = em[19];
	psh->netdis = em[2];
	psh->nice = em[5];
	psh->ppid = em[6];
	psh->pri = em[24];
	psh->sched = em[25];
	psh->corecnt = em[11];
	psh->pcputype = 0;
	psh->dev = strdup(dev);
	psh->ipa = strdup(ipa);
	if ( dur[2] > 0.25 ) {
		psh->rtt = 1L;
	}else{
		if ( dur[3] == 0 ) psh->rtt = dur[2];
		if ( dur[3] != 0 ) psh->rtt = dur[2]*dur[3];
		psh->rtt *= em[20];
	}
	return;
}
void rtttimeout(int sig)
{
	catch = 2;
	signal(SIGALRM, rtttimeout);
}
unsigned short chkcalsum(unsigned short *hadher, int langth)
{
	int bunches = langth;
	int sum = 0;
	unsigned short *w = hadher;
	unsigned short razehalt = 0;
	while ( bunches > 1 ) {
		sum += *w++;
		bunches -= 2;
	}
	if (bunches == 1) {
		*(unsigned char*)(&razehalt) = *(unsigned char*)w;
		sum += razehalt;
	}
	sum = (sum >> 16) + (sum &0xffff);
	sum += (sum >> 16);
	razehalt = ~sum;
	return razehalt;
}
int packuppacket(int datalength, char sendpacket[], uint16_t icmpid[], double rStat[])
{
    int capsize = 0;
	char pinnafore[10] = "";
	char damyanks[20] = "";
	char vsthem[10] = "";
	long  timenosee = 0;
	time_t secondes = 0;
	struct tm* zuhreel;
	struct timespec icmpstart = {0,0};
	clock_gettime(CLOCK_REALTIME, &icmpstart);
	secondes  = icmpstart.tv_sec;
	timenosee = round((double)icmpstart.tv_nsec / 1.0e3L);
	if (timenosee > 999999L) {
		secondes++;
		timenosee = 0L;
	}
	zuhreel = localtime (&secondes);
	strftime(pinnafore, 10, "%H:%M:%S", zuhreel);
	strftime(damyanks, 20, " [%Z] %d.%m.%Y", zuhreel);
	sprintf(vsthem, ".%06ld", timenosee);
	struct icmp *icmp;
	icmp = (struct icmp*)sendpacket;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0;
	icmp->icmp_seq = icmpid[1];
	icmp->icmp_id = icmpid[0];
	capsize = 8+datalength;
	char icmpdata[DATEALLEN] = "";
	sprintf(icmpdata, "current time: %s%s%s", pinnafore, vsthem, damyanks);
	memcpy(icmp->icmp_data, icmpdata, sizeof(icmpdata));
	icmp->icmp_cksum = chkcalsum((unsigned short*)icmp, capsize); 
	return capsize;
}
void send_packet(char sendpacket[], int sockfd, int datalength, uint16_t icmpid[], double rStat[], struct timespec *icmpstart)
{
	int packetize = 0;
	packetize = packuppacket(datalength, sendpacket, icmpid, rStat);
	if (sendto(sockfd, sendpacket, packetize, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
		perror("major packetsend minor sendpacket");
	}
	clock_gettime(CLOCK_MONOTONIC, icmpstart);
	return;
}
int recv_packet(char recvpacket[], int sockfd, uint16_t icmpid[], double rStat[], struct timespec icmpstart)
{
	int n = 0;
	uint fromlen = 0;
	extern int errno;
	char recvtekcap[PACKET_SIZE] = "";
	struct timespec icmpend = {0,0};
	fromlen = sizeof(from);
	if ((n = recvfrom(sockfd, recvtekcap, sizeof(recvtekcap), 0, (struct sockaddr*) &from, &fromlen)) < 0) {
		if (errno == EINTR) perror("packet receiving call failure");
	}
	clock_gettime(CLOCK_MONOTONIC, &icmpend);
	int ret = packetunpack(recvtekcap, n, icmpid, rStat, icmpstart, icmpend);
	return ret;
}
int packetunpack(char *recvtekcap, int len, uint16_t icmpid[], double rStat[], struct timespec icmpstart, struct timespec icmpend)
{
	int iphdrlen;
	struct ip *ip;
	struct icmp *icmp;
	struct timespec delta;
	ip = (struct ip*)recvtekcap;
	iphdrlen = ip->ip_hl << 2; 
	icmp = (struct icmp*)(recvtekcap + iphdrlen);
	len -= iphdrlen; 
	if (len < 8) {
		return  -1;
	} 
	if (icmp->icmp_type == ICMP_ECHOREPLY) {
		uint16_t icmpid0 = icmp->icmp_id;
		if (icmpid0 == icmpid[0]) {
			sub_timespec(icmpstart, icmpend, &delta);
			rStat[0] = ((double)delta.tv_sec +  NSECS2SECS*(double)delta.tv_nsec)*1000L;
			return 0;
		}else{
			return -2;
		}
	}else{
		return -1;
	}
}
int screenHeight(void)
{
	struct winsize w;
	w = termsize();
	if (w.ws_row > 9) {
	    return w.ws_row - 4;
	}else{
	    return w.ws_row;
	}
}
