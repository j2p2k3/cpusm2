#include "./statThreads.h"

void *cpustatsT(void * g)
{
	cpuZ *h;
	h=(cpuZ*)g;
	int cr = 0, tt = 0;
	int rollsize = rint(h->rollintval/h->zecdecz);
	double cpuTotlStat = 0, cpuMeanSpot = 0;
	double netA[16] = {};
	double vCoreTotlStat[h->cpucorecnt];
	memset( vCoreTotlStat, 0, h->cpucorecnt*sizeof(double) );
	double vCoreSpotStat[h->cpucorecnt];
	memset( vCoreSpotStat, 0, h->cpucorecnt*sizeof(double) );
	double cpuV[3][7] = {{}};
	double rollingMean[3] = {};
	double vCore[h->cpucorecnt][3][7];
	memset( vCore, 0, h->cpucorecnt*3*7*sizeof(double) );
	struct timespec pseudoSleeps = {0,0};
	h->cndcnt = 0;
	while (1) {
		pseudoSleeps.tv_sec = h->dekvar/1;
		pseudoSleeps.tv_nsec = fmod(h->dekvar, 1) * NSECS;
		if( access(PSTAT, F_OK ) != -1 ) {
			FILE *cpustats;
			cpustats = fopen(PSTAT, "r");
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( ( linelen=getline(&linebuf, &linesiz, cpustats) )>0) {
				if ( (strstr(linebuf, "cpu")) != NULL ) {
					sscanf( linebuf, STAT_STRING, &cpuV[0][0], &cpuV[0][1], &cpuV[0][2], &cpuV[0][3], &cpuV[0][4], &cpuV[0][5], &cpuV[0][6] ); 
					if ( h->cndcnt!=0 ) {
						cpuTotlStat = arraySub(cpuV[0], cpuV[1], cpuV[2], 7);
						if ( cpuTotlStat > cpuV[2][3] ) {
							h->cStat[10] = ( cpuTotlStat - cpuV[2][3] ) / cpuTotlStat;
							arrayDiv(cpuV[2], cpuTotlStat, 7);
							arraySet(cpuV[2], h->cStat, 7, 0);
							arraySet(cpuV[0], cpuV[1], 7, 0);
						}else{
							h->cStat[10] = 0;
							arrayInit(h->cStat, 7);
							h->cStat[3] = 1;
							cpuV[1][3] += cpuV[2][3];
						}
					}else{
						arraySet(cpuV[0], cpuV[1], 7, 0);
					}
				}
			}
			for (int a=0;a<h->cpucorecnt;a++) {
				if ( (linelen=getline(&linebuf, &linesiz, cpustats)) > 0 ) {
					if ( (strstr(linebuf, "cpu")) != NULL ) {
						sscanf( linebuf, STAT_STRING, &vCore[a][0][0], &vCore[a][0][1], &vCore[a][0][2], &vCore[a][0][3], &vCore[a][0][4], &vCore[a][0][5], &vCore[a][0][6] ); 
						if ( h->cndcnt!=0 ) {
							vCoreTotlStat[a] = arraySub(vCore[a][0], vCore[a][1], vCore[a][2], 7);
							vCoreSpotStat[a] = vCoreTotlStat[a]>0?(vCoreTotlStat[a] - vCore[a][2][3]) / vCoreTotlStat[a]:0L;
						}
						arraySet(vCore[a][0], vCore[a][1], 7, 0);
					}else{
						break;
					}
				}
			}
			arraySet(vCoreSpotStat, h->vStat, h->cpucorecnt, 0);
			h->cStat[7] = h->cStat[10] > h->cStat[7] ? h->cStat[10] : h->cStat[7];
			free(linebuf);
			linebuf=NULL;
			fclose(cpustats);
		}
		if ( h->netdiz != 0 ) {
			if( access(NETSTAT, F_OK ) != -1 ) {
				FILE *np = fopen(NETSTAT, "r");
				size_t linesiz=0;
				char* linebuf;
				linebuf = NULL;
				ssize_t linelen=0;
				while ( (linelen=getline(&linebuf, &linesiz, np)) > 0 ) {
					if (strstr(linebuf, h->ndev) != NULL) {
						sscanf( linebuf, NET_STRING, &netA[0], &netA[3], &netA[6], &netA[9] );
						netA[0] *= 8; //rxbits
						netA[1] = (h->cndcnt!=0?(netA[0] - netA[2])/h->zecdecz/1000000L:0L); //rxMbps
						netA[2] = netA[0]; //rxbits.prev
						netA[4] = (h->cndcnt!=0?(netA[3] - netA[5])/h->zecdecz:0L); //rxpps
						netA[5] = netA[3]; //rxpp.prev
						netA[6] *= 8; //txbits
						netA[7] = (h->cndcnt!=0?(netA[6] - netA[8])/h->zecdecz/1000000L:0L); //txMbps
						netA[8] = netA[6]; //txbits.prev
						netA[10] = (h->cndcnt!=0?(netA[9] - netA[11])/h->zecdecz:0L); //txpps
						netA[11] = netA[9]; //txpp.prev
						break;
					}
				}
				fclose(np);
				free(linebuf);
				linebuf=NULL;
			}
			netA[12] += netA[1];
			netA[13] += netA[7];
			netA[14] = h->cndcnt!=0?netA[12] / (double)h->cndcnt:0L;
			netA[15] = h->cndcnt!=0?netA[13] / (double)h->cndcnt:0L;
		}
		tt=h->cndcnt<rollsize?cr+1:rollsize;
		rollingMean[0] = mean_calc(rollingMean[0], netA[1], tt);
		rollingMean[1] = mean_calc(rollingMean[1], netA[7], tt);
		rollingMean[2] = mean_calc(rollingMean[2], h->cStat[10], tt);
		if (++cr == rollsize) cr = 0;
		cpuMeanSpot = mean_calc(cpuMeanSpot, h->cStat[10], h->cndcnt+1);
		h->cStat[8] = cpuMeanSpot;
		h->cStat[9] = rollingMean[2];
		h->cStat[11] = netA[1];
		h->cStat[12] = netA[14];
		h->cStat[13] = rollingMean[0];
		h->cStat[14] = netA[4];
		h->cStat[15] = netA[7];
		h->cStat[16] = netA[15];
		h->cStat[17] = rollingMean[1];
		h->cStat[18] = netA[10];

		if ( h->optalg != 0 ) {
			double cvmin = 1;
			double cvmax = 0;
			int mincpu = 0;
			int maxcpu = 0;
			for (int a=0;a<h->cpucorecnt;a++) {
				if ( h->vStat[a] < cvmin ) {
					cvmin = h->vStat[a];
					mincpu = a;
				}else if ( h->vStat[a] > cvmax ) {
					cvmax = h->vStat[a];
					maxcpu = a;
				}
			}
			h->core=h->optalg==1?mincpu:h->optalg==2?maxcpu:rng(0,h->cpucorecnt,1);
		}
		h->cndcnt++;
		if ( h->flop != 0 ) break;
		pselect(0, NULL, NULL, NULL, &pseudoSleeps, NULL);
		struct timespec exe = {0,0};
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exe);
		h->cpuspin1 = ((double)exe.tv_sec +  NSECS2SECS*(double)exe.tv_nsec);
	}
	pthread_exit(NULL);
	return 0;
}
void *justcpustatsT(void * g)
{
	cpuZ *h;
	h=(cpuZ*)g;
	int cr = 0, tt = 0;
	int rollsize = rint(h->rollintval/h->zecdecz);
	double cpuMeanSpot = 0, cpuTotlStat = 0;
	double rollingMean = 0;
	double cpuV[3][7] = {{}};
	struct timespec pseudoSleeps = {0,0};
	h->cndcnt = 1;
	while (1) {
		pseudoSleeps.tv_sec = h->dekvar/1;
		pseudoSleeps.tv_nsec = fmod(h->dekvar, 1) * NSECS;
		if ( h->cndcnt!=1 ) pselect(0, NULL, NULL, NULL, &pseudoSleeps, NULL);
		if( access(PSTAT, F_OK ) != -1 ) {
			FILE *cpustats;
			cpustats = fopen(PSTAT, "r");
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( ( linelen=getline(&linebuf, &linesiz, cpustats) ) >0) {
				if ( (strstr(linebuf, "cpu")) != NULL ) {
					sscanf( linebuf, STAT_STRING, &cpuV[0][0], &cpuV[0][1], &cpuV[0][2], &cpuV[0][3], &cpuV[0][4], &cpuV[0][5], &cpuV[0][6] ); 
					if ( h->cndcnt!=1 ) {
						cpuTotlStat = arraySub(cpuV[0], cpuV[1], cpuV[2], 7);
						if ( cpuTotlStat > cpuV[2][3] ) {
							h->cStat[10] = ( cpuTotlStat - cpuV[2][3] ) / cpuTotlStat;
							arrayDiv(cpuV[2], cpuTotlStat, 7);
							arraySet(cpuV[2], h->cStat, 7, 0);
							arraySet(cpuV[0], cpuV[1], 7, 0);
						}else{
							h->cStat[10] = 0;
							arrayInit(h->cStat, 7);
							h->cStat[3] = 1;
							cpuV[1][3] += cpuV[2][3];
						}
					}else{
						arraySet(cpuV[0], cpuV[1], 7, 0);
						h->cStat[10] = 0;
						arrayInit(h->cStat, 7);
						h->cStat[3] = 1;
					}
				}
			}
			free(linebuf);
			linebuf=NULL;
			fclose(cpustats);
		}
		h->cStat[7] = h->cStat[10] > h->cStat[7] ? h->cStat[10] : h->cStat[7];
		tt=h->cndcnt<rollsize?cr+1:rollsize;
		rollingMean = mean_calc(rollingMean, h->cStat[10], tt);
		if (++cr == rollsize) cr = 0;
		cpuMeanSpot = mean_calc(cpuMeanSpot, h->cStat[10], h->cndcnt);
		h->cStat[8] = cpuMeanSpot;
		h->cStat[9] = rollingMean;
		h->cndcnt++;
		if ( h->flop != 0 ) break;
		struct timespec exe = {0,0};
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exe);
		h->cpuspin1 = ((double)exe.tv_sec +  NSECS2SECS*(double)exe.tv_nsec);
	}
	pthread_exit(NULL);
	return 0;
}
void *justnetstatsT(void * g)
{
	cpuZ *h;
	h=(cpuZ*)g;
	int cr = 1, tt = 0;
	int rollsize = rint(h->rollintval/h->zecdecz);
	double cpuMeanSpot = 0, cpuTotlStat = 0; 
	double netA[16] = {};
	double rollingMean[3] = {};
	double cpuV[3][7] = {{}};
	struct timespec pseudoSleeps = {0,0};
	h->cndcnt = 1;
	while (1) {
		pseudoSleeps.tv_sec = h->dekvar/1;
		pseudoSleeps.tv_nsec = fmod(h->dekvar, 1) * NSECS;
		if( access(PSTAT, F_OK ) != -1 ) {
			FILE *cpustats;
			cpustats = fopen(PSTAT, "r");
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, cpustats)) > 0 ) {
				if ( (strstr(linebuf, "cpu")) != NULL ) {
					sscanf( linebuf, STAT_STRING, &cpuV[0][0], &cpuV[0][1], &cpuV[0][2], &cpuV[0][3], &cpuV[0][4], &cpuV[0][5], &cpuV[0][6] ); 
					if ( h->cndcnt!=0 ) {
						cpuTotlStat = arraySub(cpuV[0], cpuV[1], cpuV[2], 7);
						if ( cpuTotlStat > cpuV[2][3] ) {
							h->cStat[10] = ( cpuTotlStat - cpuV[2][3] ) / cpuTotlStat;
							arraySet(cpuV[0], cpuV[1], 7, 0);
						}else{
							h->cStat[10] = 0;
							cpuV[1][3] += cpuV[2][3];
						}
					}
					arraySet(cpuV[0], cpuV[1], 7, 0);
				}
			}
			free(linebuf);
			linebuf=NULL;
			fclose(cpustats);
		}
		if ( h->netdiz != 0 ) {
			if( access(NETSTAT, F_OK ) != -1 ) {
				FILE *np = fopen(NETSTAT, "r");
				size_t linesiz=0;
				char* linebuf;
				linebuf = NULL;
				ssize_t linelen=0;
				while ( (linelen=getline(&linebuf, &linesiz, np)) > 0 ) {
					if (strstr(linebuf, h->ndev) != NULL) {
						sscanf( linebuf, NET_STRING, &netA[0], &netA[3], &netA[6], &netA[9] );
						netA[0] *= 8; //rxbits
						netA[1] = (h->cndcnt!=1?(netA[0] - netA[2])/h->zecdecz/1000000L:0L); //rxMbps
						netA[2] = netA[0]; //rxbits.prev
						netA[4] = (h->cndcnt!=1?(netA[3] - netA[5])/h->zecdecz:0L); //rxpps
						netA[5] = netA[3]; //rxpp.prev
						netA[6] *= 8; //txbits
						netA[7] = (h->cndcnt!=1?(netA[6] - netA[8])/h->zecdecz/1000000L:0L); //txMbps
						netA[8] = netA[6]; //txbits.prev
						netA[10] = (h->cndcnt!=1?(netA[9] - netA[11])/h->zecdecz:0L); //txpps
						netA[11] = netA[9]; //txpp.prev
						break;
					}
				}
				fclose(np);
				free(linebuf);
				linebuf=NULL;
			}
			netA[12] += netA[1];
			netA[13] += netA[7];
			netA[14] = h->cndcnt!=0?netA[12]/((double)h->cndcnt+1):0L;
			netA[15] = h->cndcnt!=0?netA[13]/((double)h->cndcnt+1):0L;
		}
		tt=h->cndcnt<rollsize?cr:rollsize;
		if ( h->cndcnt >=2 ) {
			rollingMean[0] = mean_calc(rollingMean[0], netA[1], tt+1);
			rollingMean[1] = mean_calc(rollingMean[1], netA[7], tt+1);
		}
			rollingMean[2] = mean_calc(rollingMean[2], h->cStat[10], tt);
			cpuMeanSpot = mean_calc(cpuMeanSpot, h->cStat[10], h->cndcnt);
		//~ }
		if (++cr == rollsize) cr = 0;
		h->cStat[0] = netA[1];
		h->cStat[1] = netA[14];
		h->cStat[2] = rollingMean[0];
		h->cStat[3] = netA[4];
		h->cStat[4] = netA[7];
		h->cStat[5] = netA[15];
		h->cStat[6] = rollingMean[1];
		h->cStat[7] = netA[10];
		h->cStat[8] = cpuMeanSpot;
		h->cStat[9] = rollingMean[2];
		h->cndcnt++;
		if ( h->flop != 0 ) break;
		pselect(0, NULL, NULL, NULL, &pseudoSleeps, NULL);
		struct timespec exe = {0,0};
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exe);
		h->cpuspin1 = ((double)exe.tv_sec +  NSECS2SECS*(double)exe.tv_nsec);
	}
	pthread_exit(NULL);
	return 0;
}
void *justvpustatsT(void * g)
{
	cpuZ *h;
	h=(cpuZ*)g;
	int cr = 0, tt = 0;
	int rollsize = rint(h->rollintval/h->zecdecz);
	double coreMeanSpot = 0, coreMeanSpotA = 0;
	double rollingMean = 0;
	double vCoreTotlStat[h->cpucorecnt];
	memset( vCoreTotlStat, 0, h->cpucorecnt*sizeof(double) );
	double vCoreSpotStat[h->cpucorecnt];
	memset( vCoreSpotStat, 0, h->cpucorecnt*sizeof(double) );
	double vCore[h->cpucorecnt][3][7];
	memset( vCore, 0, h->cpucorecnt*3*7*sizeof(double) );
	struct timespec pseudoSleeps = {0,0};
	h->cndcnt = 0;
	while (1) {
		pseudoSleeps.tv_sec = h->dekvar/1;
		pseudoSleeps.tv_nsec = fmod(h->dekvar, 1) * NSECS;
		if( access(PSTAT, F_OK ) != -1 ) {
			FILE *cpustats;
			cpustats = fopen(PSTAT, "r");
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			linelen=getline(&linebuf, &linesiz, cpustats);
			coreMeanSpot = 0L;
			for (int a=0;a<h->cpucorecnt;a++) {
				if ( (linelen=getline(&linebuf, &linesiz, cpustats)) > 0 ) {
					if ( (strstr(linebuf, "cpu")) != NULL ) {
						sscanf( linebuf, STAT_STRING, &vCore[a][0][0], &vCore[a][0][1], &vCore[a][0][2], &vCore[a][0][3], &vCore[a][0][4], &vCore[a][0][5], &vCore[a][0][6] ); 
						if ( h->cndcnt!=0 ) {
							vCoreTotlStat[a] = arraySub(vCore[a][0], vCore[a][1], vCore[a][2], 7);
							vCoreSpotStat[a] = vCoreTotlStat[a]>0?(vCoreTotlStat[a] - vCore[a][2][3]) / vCoreTotlStat[a]:0L;
							coreMeanSpot += vCoreSpotStat[a];
						}
						arraySet(vCore[a][0], vCore[a][1], 7, 0);
					}else{
						break;
					}
				}
			}
			free(linebuf);
			linebuf=NULL;
			fclose(cpustats);
		}
		coreMeanSpot /= (double)h->cpucorecnt;
		tt=h->cndcnt<rollsize?cr+1:rollsize;
		rollingMean = mean_calc(rollingMean, coreMeanSpot, tt);
		if (++cr == rollsize) cr = 0;
		coreMeanSpotA = mean_calc(coreMeanSpotA, coreMeanSpot, h->cndcnt+1);
		h->cStat[0] = coreMeanSpot;
		h->cStat[1] = coreMeanSpotA;
		h->cStat[2] = rollingMean;
		for ( int a=0;a<h->cpucorecnt;a++ ) {
			h->vStat[a] = vCoreSpotStat[a];
		}
		if ( h->optalg != 0 ) {
			double cvmin = 1;
			double cvmax = 0;
			int mincpu = 0;
			int maxcpu = 0;
			for (int a=0;a<h->cpucorecnt;a++) {
				if ( h->vStat[a] < cvmin ) {
					cvmin = h->vStat[a];
					mincpu = a;
				}else if ( h->vStat[a] > cvmax ) {
					cvmax = h->vStat[a];
					maxcpu = a;
				}
			}
			h->core=h->optalg==1?mincpu:h->optalg==2?maxcpu:rng(0,h->cpucorecnt,1);
		}
		h->cndcnt++;
		if ( h->flop != 0 ) break;
		pselect(0, NULL, NULL, NULL, &pseudoSleeps, NULL);
		struct timespec exe = {0,0};
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exe);
		h->cpuspin1 = ((double)exe.tv_sec +  NSECS2SECS*(double)exe.tv_nsec);
	}

	pthread_exit(NULL);
	return 0;
}
void *simplecpustatsT(void * g)
{
	cpuZ *h;
	h=(cpuZ*)g;
	int cr = 0, tt = 0;
	int rollsize = rint(h->rollintval/h->zecdecz);
	double cpuMeanSpot = 0, cpuTotlStat = 0;
	double rollingMean = 0;
	double cpuV[3][7] = {{}};
	struct timespec pseudoSleeps = {0,0};
	h->cndcnt = 0;
	while (1) {
		pseudoSleeps.tv_sec = h->dekvar/1;
		pseudoSleeps.tv_nsec = fmod(h->dekvar, 1) * NSECS;
		if( access(PSTAT, F_OK ) != -1 ) {
			FILE *cpustats;
			cpustats = fopen(PSTAT, "r");
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, cpustats)) > 0 ) {
				if ( (strstr(linebuf, "cpu")) != NULL ) {
					sscanf( linebuf, STAT_STRING, &cpuV[0][0], &cpuV[0][1], &cpuV[0][2], &cpuV[0][3], &cpuV[0][4], &cpuV[0][5], &cpuV[0][6] ); 
					if ( h->cndcnt!=0 ) {
						cpuTotlStat = arraySub(cpuV[0], cpuV[1], cpuV[2], 7);
						if ( cpuTotlStat > cpuV[2][3] ) {
							h->cStat[10] = ( cpuTotlStat - cpuV[2][3] ) / cpuTotlStat;
							arraySet(cpuV[0], cpuV[1], 7, 0);
						}else{
							h->cStat[10] = 0;
							cpuV[1][3] += cpuV[2][3];
						}
					}
					arraySet(cpuV[0], cpuV[1], 7, 0);
				}
			}
			free(linebuf);
			linebuf=NULL;
			fclose(cpustats);
		}
		tt=h->cndcnt<rollsize?cr+1:rollsize;
		rollingMean = mean_calc(rollingMean, h->cStat[10], tt);
		if (++cr == rollsize) cr = 0;
		cpuMeanSpot = mean_calc(cpuMeanSpot, h->cStat[10], h->cndcnt+1);
		h->cStat[8] = cpuMeanSpot;
		h->cStat[9] = rollingMean;
		h->cndcnt++;
		if ( h->flop != 0 ) break;
		pselect(0, NULL, NULL, NULL, &pseudoSleeps, NULL);
		struct timespec exe = {0,0};
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exe);
		h->cpuspin1 = ((double)exe.tv_sec +  NSECS2SECS*(double)exe.tv_nsec);
	}
	pthread_exit(NULL);
	return 0;
}

