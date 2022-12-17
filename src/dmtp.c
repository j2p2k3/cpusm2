#include "./dmtp.h"

int dmtp(char *outputfile, char *appstart, char *dev, int em[], double dur[], char *ipa)
{
	int allint[18] = {0,0,0,0,0,em[11],0,1,em[21],0,0,1,0,0,0,0,0,4}; 
	allint[9]=allint[8]==0?0:2;
	allint[10]=allint[8]!=0?1:0;
	double runtime = 0L;
	//~ double cntsum = 0L;
	struct timespec exestart, exeend;
	struct timespec exe0 = {0,0};
	pthread_t ssid[6];
	timeD *xx = (timeD*)malloc( sizeof(timeD));
	prntsctrl *psc = (prntsctrl*) malloc( sizeof(prntsctrl));
	psheader *psh = (psheader*) malloc( sizeof(psheader));
	psv0 *vv0 = (psv0*)malloc( sizeof(psv0));
	cpuZ *g = (cpuZ*)malloc( sizeof(cpuZ));
	dur[0] = 0L;
	ginit(g, em, dur, dev, ipa);
	xxinit(xx, em, dur, dev, appstart);
	vv0init(vv0, em);
	pscshinit(psc, psh, em, dur, dev, ipa);
	int fresult = 0;
	fresult = getfreqlimits( &xx->minfreq, &xx->maxfreq, &g->freqmax);
		if ( fresult == 0 ) printf("failed to query cpu frequency limits\n");
	fresult = getthermalzones( &g->thermalzones, &vv0->thermalzones, &psh->thermalzones);
		if ( fresult == 0 ) printf("failed to query thermal monitoring framework\n");
	fresult = getmem( &xx->mmstat);
		if ( fresult == 0 ) printf("failed to query system memory\n");
	fresult = getsystype(xx);
		if ( fresult == 0 ) printf("failed to query system type\n");
	fresult = getcpuonline(xx);
		if ( fresult == 0 ) printf("failed to query active cpu status\n");
	fresult = getsysmodel(xx);
		if ( fresult == 0 ) printf("failed to query cpu model\n");
	fresult = getsysversion(xx);
		if ( fresult == 0 ) printf("failed to query o/s version\n");
	fresult = getsysname(xx);
		if ( fresult == 0 ) printf("failed to query system hostname\n");
	fresult = getsysfreqgov(xx);
		if ( fresult == 0 ) printf("failed to query cpu frequency governor\n");
	fresult = gethdd(xx, g);
		if ( fresult == 0 ) printf("failed to query active hdd status\n");
	keyboard_attr();
	clock_gettime(CLOCK_MONOTONIC, &exestart);
	exptime = get_time_nsec(exestart);
	double cnt1 = ((double)exestart.tv_sec + NSECS2SECS*exestart.tv_nsec);
	struct timespec pseudoSleep = {0,0};
	struct timespec execnt = {0,0};
	clock_gettime(CLOCK_MONOTONIC, &exeend);
	time_diff(*xx, g->rStat, em, exestart, exeend);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &exe0);
	pthread_create(&ssid[0], NULL, &one_key_actionT, (void *) g);
	if ( em[14] == 1 ) {
		pthread_create(&ssid[1], NULL, &cpustatsT, (void *) g);
		if ( u == 0 ) pthread_create(&ssid[2], NULL, &rttstatsT, (void *) g);
	}
	if ( em[14] == 0 ) {
		if ( em[10] == 0 ) pthread_create(&ssid[1], NULL, &justcpustatsT, (void *) g);
		if ( em[10] == 1 ) {
			pthread_create(&ssid[1], NULL, &justnetstatsT, (void *) g);
			if ( u == 0 ) pthread_create(&ssid[2], NULL, &rttstatsT, (void *) g);
		}
		if ( em[10] == 2 ) pthread_create(&ssid[1], NULL, &justvpustatsT, (void *) g);
		if ( em[10] == 3 ) pthread_create(&ssid[1], NULL, &simplecpustatsT, (void *) g);
		if ( em[10] == 4 ) pthread_create(&ssid[1], NULL, &simplecpustatsT, (void *) g);
	}
	while ((em[1]!=0)?(em[12]<em[1]):((dur[1]!=0)?(em[12]<dur[1]/(dur[2])):(1))) {
		clock_gettime(CLOCK_MONOTONIC, &execnt);
		double decvar = derive_interval(em[4], em[15], dur[2], &allint[6], &em[13], execnt);
		g->dekvar=dur[3]==0?decvar:decvar*dur[3];
		if ( allint[6] == 19 ) allint[6] = 0;
		if ( decvar != 0 ) {
			pseudoSleep.tv_sec = decvar/1;
			pseudoSleep.tv_nsec = fmod(decvar, 1) * NSECS;
			pselect(0, NULL, NULL, NULL, &pseudoSleep, NULL);
			if ( em[12] >= 1 ) {
				if ( g->ch != 0 ) {
					if ( g->ch == '/' ) {
						em[13] = (em[13]==0)?(1):(0);
						if ( em[13] == 1 ) {
							if ( em[21] != 0 ) {
								allint[11] = em[21];
							}
						}else {
							if ( allint[9] >= 2 ) {
								if ( allint[8] != 0 ) {
									if ( allint[10] !=0 ) em[21] = allint[11];
									allint[7] = em[12]%(em[21]*em[15]);
								}
							}
						}
						if ( em[13] == 1 && em[17] == 0 ) allint[1]=10;
						if ( em[13] == 1 && em[17] != 0 ) allint[3] = 0;
						if ( em[13] == 0 && em[17] == 0 ) allint[4]=1;
						if ( em[13] == 0 && em[17] != 0 ) {
							psh->colour = em[3];
							psh->cpun = em[7];
							psh->verb = em[10];
							psh->scroll = em[13];
							if ( em[27] == 0 ) print_header(*psh);
							if ( em[27] == 1 ) print_header_s(*psh);
							if ( em[21] != 0 && allint[3] % (em[17]*em[15]) == 0 ) printf("\x1b[1A");
						}
						if ( em[21] != 0 ) allint[7] = em[12]%(em[19]*em[15]);
					}
					if ( g->ch == 'j' ) {
						if ( allint[9] == 0 ) {
							allint[9] = 2;
						}else if ( allint[9] >= 2 && allint[10] == 1 ) {
							allint[9] = 1;
						}else{
							allint[9] = 1;
						}
						em[21] = em[21]==0?em[22]!=0?em[22]:1:0;
						allint[7] = em[12]%(em[19]*em[15]);
					}
					if ( g->ch == ',' && em[16] >=1 ) {
						em[16] = (em[16]==1)?(2):(1);
						if ( em[16] == 1 ) {
							psh->colour = em[3];
							psh->cpun = em[7];
							psh->verb = em[10];
							psh->scroll = em[13];
							if ( em[27] == 0 ) print_header(*psh);
							if ( em[27] == 1 ) print_header_s(*psh);
						}
					}
					if ( g->ch == 'v' ) {
						if ( em[27] == 1 ) print_header(*psh);
						if ( em[27] == 0 ) print_header_s(*psh);
						em[27] = em[27]==0?1:0;
					}
					if ( g->ch == '.' ) {
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						if ( allint[4] == 1 ) allint[4] = 0;
					}
					if ( g->ch == ' ' ) {
						if ( em[14] == 1 ) {
							if ( em[2] != 0 ) em[10] = em[10]==0?2:em[10]==2?4:em[10]==4?3:em[10]==3?1:0;
							if ( em[2] == 0 ) em[10] = em[10]==0?2:em[10]==2?3:em[10]==3?4:0;
							if ( em[13] != 0 ) {
								vv0->counter = 1;
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
								if ( allint[4] == 1 ) allint[4] = 0;
							}else if ( allint[9] == 0 ) {
								if ( em[13] == 0 && allint[4] == 0 && em[21] == 0 ) {
									printf("\x1b[7A");
								}
								vv0->counter = 1;
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
								if ( allint[4] == 1 ) allint[4] = 0;
							}else if ( allint[9] == 1 ) {
								vv0->counter = 1;
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
								if ( allint[4] == 1 ) allint[4] = 0;
								allint[9] = 0;
							}else if ( allint[9] >= 2 ) {
								vv0->counter = 1;
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
								if ( allint[4] == 1 ) allint[4] = 0;
							}
						}
					}
					if ( g->ch == 'q' ) {
						g->flop = 9;
					}
					if ( g->ch == 'x' ) {
						em[17]=em[17]==0?em[18]!=0?em[18]:screenHeight():0;
						if ( em[21] != 0 ) {
							if ( em[17] != 0 ) {
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
							}
						} else if ( em[17] != 0 && em[13] != 0 ) {
							psh->colour = em[3];
							psh->cpun = em[7];
							psh->verb = em[10];
							psh->scroll = em[13];
							if ( em[27] == 0 ) print_header(*psh);
							if ( em[27] == 1 ) print_header_s(*psh);
						} else if ( em[17] != 0 && em[13] == 0 && allint[4] == 1 ) {
							psh->colour = em[3];
							psh->cpun = em[7];
							psh->verb = em[10];
							psh->scroll = em[13];
							if ( em[27] == 0 ) print_header(*psh);
							if ( em[27] == 1 ) print_header_s(*psh);
							allint[4] = 0;
						}
						allint[3] = 0;
					}
					if ( g->ch == '=' ) {
						if ( em[27] == 0 ) print_stats_F(*vv0, *psc, *psh);
						if ( em[27] == 1 ) print_stats_Fs(*vv0, *psc, *psh);
						xx->sig = SIGINT;
						clock_gettime(CLOCK_MONOTONIC, &exeend);
						time_diff(*xx, g->rStat, em, exestart, exeend);
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
					}
					if ( g->ch == 's' ) {
						xx->sig = SIGINT;
						clock_gettime(CLOCK_MONOTONIC, &exeend);
						time_diff(*xx, g->rStat, em, exestart, exeend);
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
					}
					if ( g->ch == 'm' ) {
						int lim = 0;
						if ( em[10] == 2 || em[10] == 4 ) {
							lim = 17 + em[11];
						}else if ( em[10] == 1 ) {
							if ( u == 0 ) lim = 17 + 14;
							if ( u != 0 ) lim = 17 + 8;
						}else{
							lim = 25;
						}
						em[23]++;
						if ( em[23] >= lim ) {
							em[23] = psh->focus = psc->focus = vv0->focus = 0;
						}else if ( em[23] < 15 ) {
							if ( em[23] == 14 ) {
								psc->focus = em[23];
								vv0->focus = em[23]+1;
							}else{
								psh->focus = psc->focus = em[23];
								vv0->focus = 0;
							}
						}else {
							if ( em[23] == 15 ) {
								psc->focus = em[23];
								vv0->focus = em[23]+1;
							}else if ( em[23] == 16 ) {
								psc->focus = em[23];
								vv0->focus = em[23]+1;
							}else{
								psh->focus = psc->focus = 0;
								vv0->focus = em[23]+1;
							}
						}
					}
					if ( g->ch == 'n' ) {
						if ( em[1] != 0 || dur[1] != 0 ) allint[12]=allint[12]==0?1:0;
					}
					if ( g->ch == 'b' ) {
						if ( em[1] != 0 || dur[1] != 0 ) allint[13]=allint[13]==0?1:0;
					}
					if ( g->ch == '2' ) {
						allint[14]=allint[14]==0?1:0;
					}
					if ( g->ch == '!' ) {
						em[13]=em[13]==13?1:13;
					}
					if ( g->ch == 'k' ) {
						if ( em[27] == 0 ) {
							if ( u!= 0 ) allint[15]=allint[15]==0?1:allint[15]==1?2:allint[15]==2?3:0;
							if ( u== 0 ) allint[15]=allint[15]==0?1:allint[15]==1?2:allint[15]==2?3:allint[15]==3?4:0;
							psh->pcputype = allint[15];
							if ( em[13] == 0 ) printf("\x1b[7A");
							print_header(*psh);
						}
					}
					if ( g->ch == 'l' ) {
						allint[0]=allint[0]==0?1:0;
						if ( allint[0] == 0 ) vv0->focus = psc->focus = 0;
					}
					if ( g->ch == 'p' ) {
						allint[16]=allint[16]==0?1:0;
						if ( allint[16] == 0 ) vv0->focus = psc->focus = 0;
					}
					if ( g->ch == '+' ) {
						if ( ++allint[17] == 100 ) allint[17] = 1;
					}
					if ( g->ch == '-' ) {
						if ( --allint[17] == 0 ) allint[17] = 1;
					}
					if ( g->ch == '`' ) {
						allint[5]=allint[5]==1?em[11]:1;
					}
					if ( g->ch == '1' ) {
						em[4]=em[4]==1?0:1;
					}
					if ( g->ch == 'c' ) {
						em[3]=em[3]==0?1:em[3]==1?2:em[3]==2?3:em[3]==3?4:em[3]==4?5:em[3]==5?6:em[3]==6?7:0;
					}
					if ( g->ch == 'z' ) {
						em[0] = 11;
						g->flop = 9;
					}
					if ( g->ch == 'a' && em[7] != -9 && em[8] == 0 ) {
						for ( int i=0;i<em[11];i++ ) {
							if ( em[7] == i ) {
								if ( i == em[11]-1 ) {
									em[7] = 0;
								}else{
									em[7] = i+1;
								}
								break;
							}
						}
						set_cpuAffinity(em[7], em[12]);
						if ( em[13] == 0 && em[21] == 0 ) printf("\x1b[7A");
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
					}
					if ( g->ch == 'h' ) {
						xx->sig = SIGTRAP;
						xx->scroll = em[13];
						clock_gettime(CLOCK_MONOTONIC, &exeend);
						time_diff(*xx, g->rStat, em, exestart, exeend);
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						if ( em[13] != 0 ) {
							allint[6] = 19;
							xx->slat++;
							allint[3] = 0;
						}
					}
					g->ch = 0;
				}
			}
			if ( u == 0 && em[10] == 1 ) {
				while( g->cndcnt2 == 0 ) {
					continue;
				}
			}
			while( g->cndcnt == 0 ) {
				continue;
			}
			if (em[13] == 0 || ( em[13] == 1 && allint[1] == 10) || em[13] == 4 ) {
				if(em[16] <= 1) {
					if ( em[21] == 0 ) {
						printf ("\x1b[1A");
					}else{
						if ( allint[7] != 0 ) {
							printf ("\x1b[1A");
						}
						allint[7]++;
						if ( em[12] == 0 ) allint[7]--;
						if (allint[7] >= (em[19]*1/dur[2])/em[21]) allint[7] = 0;
					}
				}
				allint[1]=0;
			}
			double cnt = cowhunter(allint[6], em[15], dur[2], &cnt1, &xx->cntsum);
			runtime=em[9]==0?cnt:dur[2];
			dur[0] += runtime;
			double pcpu = getproctime(&xx->cpuspin, xx->cntsum, allint[5]);
			struct timespec exe = {0,0};
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exe);
			double cpuspin = ((double)exe.tv_sec +  NSECS2SECS*(double)exe.tv_nsec);
			double tcpu0 = (cpuspin/xx->cntsum*100L)/allint[5];
			double tcpu1 = (g->cpuspin0/xx->cntsum*100L)/allint[5];
			double tcpu2 = (g->cpuspin1/xx->cntsum*100L)/allint[5];
			double tcpu3 = (g->cpuspin2/xx->cntsum*100L)/allint[5];
			if ( em[8] != 0 ) {
				em[7] = g->core;
				if ( em[12] != 0 ) set_cpuAffinity(em[7], em[12]);
			}
			if ( em[17] != 0 && ( em[13] != 0 || em[21] != 0 ) ) {
				if ( em[13] != 0 ) {
					if ( allint[3] != 0 && allint[3] % em[17] == 0 ) {
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						allint[3] = 0;
					}
				}else{
					
					if ( allint[3] != 0 && allint[3] % ((em[17]*(em[15]/em[21]))*em[19]) == 0 ) {
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						printf ("\x1b[1A");
						allint[3] = 0;
					}
				}
			}
			if ( em[12] < rint((double)1/dur[2]) ) {
				psc->gen_stat[0][0] = allint[14]==0?100:(em[12]+1)-g->cndcnt;
				psc->gen_stat[0][1] = 0;
			}else{
				double alignment = (double)g->cndcnt/(double)em[12]*100L;
				psc->gen_stat[0][0] = allint[14]==0?(alignment>=100?100:alignment):(em[12]+1)-g->cndcnt;			
				if ( allint[15] == 0 )psc->gen_stat[0][1] = pcpu;
				if ( allint[15] == 1 )psc->gen_stat[0][1] = tcpu0;
				if ( allint[15] == 2 )psc->gen_stat[0][1] = tcpu1;
				if ( allint[15] == 3 )psc->gen_stat[0][1] = tcpu2;
				if ( allint[15] == 4 )psc->gen_stat[0][1] = tcpu3;
			}
			setruntime( em, dur, allint[12], psc);
			psc->zecdecz = dur[2];
			psc->colour = em[3];
			if ( em[14] == 1 ) {
				psc->gen_stat[0][2] = g->fStat[em[11]];
			} else if ( em[10] == 4 ) {
				psc->gen_stat[0][2] = g->fStat[em[11]];
			} else {
				psc->gen_stat[0][2] = g->fStat[em[11]];
			}
			psc->gen_stat[0][3] = g->ctemp;
			psc->gen_stat[0][4] = g->memstat[1]/g->memstat[0];
			psc->gen_stat[0][5] = g->memstat[2]/g->memstat[0];
			psc->gen_stat[0][6] = g->memstat[3]/g->memstat[0];
			psc->gen_stat[0][7] = g->memstat[4]/g->memstat[0];
			for ( int i=0;i<3;i++ ) {
				psc->gen_stat[0][i+8] = g->loadavg[i];
			}
			sscanf( g->procsS, "%lf/%lf", &psc->gen_stat[0][11], &psc->gen_stat[0][12] );
			if ( em[10] == 2 && em[14] == 0 ) {
				psc->gen_stat[0][13] = g->cStat[0];
				psc->gen_stat[0][14] = g->cStat[1];
				psc->gen_stat[0][15] = g->cStat[2];
			}else{
				psc->gen_stat[0][13] = g->cStat[10];
				psc->gen_stat[0][14] = g->cStat[8];
				psc->gen_stat[0][15] = g->cStat[9];
			}
			if ( em[1] != 0 ) {
				if ( allint[13] != 0 ) {
					psc->scnt = em[1]- ( em[12] + 1 );
				}else{
					psc->scnt = em[12]+1;
				}
			}else if ( dur[1] != 0L ) {
				if ( allint[13] != 0 ) {
					if ( dur[2] != 0L ) psc->scnt = rint(dur[1]/dur[2]) - (em[12] + 1);
					if ( dur[2] == 0L ) psc->scnt = rint(dur[1]*(double)1/em[15]) - (em[12] + 1);
				}else{
					psc->scnt = em[12]+1;
				}
			}else{
				psc->scnt = em[12]+1;
				//~ psc->scnt = em[23];
			}
			if ( psc->scnt % (em[20]) == 0 ) {
				psc->sthis=psc->sthis==0?1:0;
				psc->sthat=psc->sthis==0?1:0;
			}
			arrayX(psc->gen_stat[0], psc->gen_stat[1], psc->gen_stat[2], psc->gen_stat[3], 16, em[12]+1);
				vv0->corecnt = em[11];
				if ( em[10] == 0 ) {
					vv0->stat_count = 8;
					arraySet(g->cStat, vv0->stat, vv0->stat_count, 0);
					vv0->type = 0;
					vv0->threshold = 0;
				}
				if ( em[10] == 1 ) {
					vv0->stat_count = 8;
					if ( em[14] == 0 ) arraySet(g->cStat, vv0->stat, vv0->stat_count, 0);
					if ( em[14] == 1 ) arraySet(g->cStat, vv0->stat, vv0->stat_count, 11);
					if ( u == 0 ) {
						arrayTes(g->rStat, vv0->stat, 6, vv0->stat_count);
						vv0->stat_count = 14;
					}
					vv0->type = 0;
					vv0->threshold = 0;
				}
				if ( em[10] == 2 ) {
					vv0->stat_count = em[11];
					arraySet(g->vStat, vv0->stat, vv0->stat_count, 0);
					vv0->core = em[7];
					vv0->type = 0;
					vv0->threshold = VCPUTH;
				}
				if ( em[10] == 3 ) {
					vv0->stat_count = g->thermalzones;
					if ( em[14] == 0 ) arraySet(g->tStat, vv0->stat, vv0->stat_count, 0);
					if ( em[14] == 1 ) arraySet(g->tStat, vv0->stat, vv0->stat_count, 0);
					vv0->type = 0;
					vv0->threshold = 0;
				}
				if ( em[10] == 4 ) {
					vv0->stat_count = em[11];
					arraySet(g->fStat, vv0->stat, vv0->stat_count, 0);
					vv0->core = em[7];
					vv0->type = 0;
					vv0->threshold = 0;
				}
				vv0->colour = em[3];
				vv0->verb = em[10];
				if ( allint[0] == 1 ) scanner(&allint[2], em, vv0, psc);
				if ( psc->scnt % (em[20]) == 0 ) {
					vv0->sthis=vv0->sthis==0?1:0;
					vv0->sthat=vv0->sthis==0?1:0;
				}
				psh->stat_count = psc->stat_count = vv0->stat_count;
				if ( em[12] == 0 ) {
					if ( em[16] <= 1 ) {
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];				
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						if ( em[13] == 0) printf ("\x1b[1A");
					}
				}
				if ( em[16] <= 1 ) {
					if ( em[27] == 0 ) print_stats_c(*psc);
					if ( em[27] == 1 ) print_stats_cs(*psc);
				}
			if ( em[16] <= 1 ) {
				print_stats(*vv0);
			}
				arrayX(vv0->stat, vv0->min[em[10]], vv0->max[em[10]], vv0->mean[em[10]], vv0->stat_count, vv0->counter);
				vv0->counter++;
			if ( em[16] >= 1 ) {
				print_log_stats(*psc, *vv0);
			}
			if ( allint[16] == 1 ) {
				if ( em[12] % em[20] == 0 ) allint[0] = allint[0]==0?1:0;
				if ( em[12] % em[20] == allint[17] ) allint[0] = allint[0]==0?1:0;
			}
			em[12]++;
			xx->rcnt++;
			allint[3]++;
			if ( g->flop == 9 ) break;
		}
	}
	setruntime(em, dur, 0, psc);
	clock_gettime(CLOCK_MONOTONIC, &exeend);
	xx->sig = SIGQUIT;
	psc->scnt = em[12];
	printf ("\x1b[1A");
	if ( em[13] != 0 ) {
		if ( em[27] == 0 ) print_header(*psh);
		if ( em[27] == 1 ) print_header_s(*psh);
	}
	if ( em[27] == 0 ) print_stats_c(*psc);
	if ( em[27] == 1 ) print_stats_cs(*psc);
	print_stats(*vv0);
	if ( em[27] == 0 ) print_stats_F(*vv0, *psc, *psh);
	if ( em[27] == 1 ) print_stats_Fs(*vv0, *psc, *psh);
	g->flop = 1;
	pthread_join(ssid[0], NULL);
	pthread_join(ssid[1], NULL);
	if ( em[10] == 1 || em[14] == 1 ) {
		if ( u == 0 ) pthread_join(ssid[2], NULL);
	}
	time_diff(*xx, g->rStat, em, exestart, exeend);
	free(xx->cpuol);
	free(xx->sysid);
	free(xx->freqgov);
	free(xx->model);
	free(xx->version);
	free(xx->uname);
	free(g->ndev);
	free(g->ipa);
	free(psh->dev);
	free(psh->ipa);
	for ( int i=0;i<g->diskcnt;i++ ) {
		free(xx->disk[i]);
	}
	free(xx);
	free(g);
	free(psc);
	free(psh);
	free(vv0);
	return em[0];
}
