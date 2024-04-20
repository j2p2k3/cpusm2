#include "roundtriptimeThread.h"
void *rttstatsT(void * g)
{
	cpuZ *h;
	h=(cpuZ*)g;
	srand(time(NULL));
	int cr = 0, tt = 0;
	int rollsize = rint(h->rollintval/h->zecdecz)/h->slip;
	//~ int rollsize = rint(h->rollintval/h->zecdecz);
	double rttMeanSpot = 0;
	double rollingMean =0;
	char destIP[16] = "";
	memcpy(destIP, h->ipa, 16);
	struct hostent *host = {0};
	struct protoent *protocol = {0};
	struct timespec pseudoSleeps = {0,0};
	pseudoSleeps.tv_sec = h->dekvar/1;
	pseudoSleeps.tv_nsec = fmod(h->dekvar, 1) * NSECS;
	struct timespec packetWaits = {0,0};
	packetWaits.tv_sec = 1;
	packetWaits.tv_nsec = 0;
	struct timespec icmpstart = {0,0};
	struct timespec exe = {0,0};
	unsigned long inaddr = 0l;
	int size = 50 * 1024;
	int sockfd = 0;
	uint16_t icmpid[2] = {0};
	uint16_t lsbmsb = 0;
	char sendpacket[PACKET_SIZE] = "";
	char recvpacket[PACKET_SIZE] = "";
	if ((protocol = getprotobyname("icmp")) == NULL)
	{
		perror("getprotobyname");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto);
	setuid(u);
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &packetWaits, sizeof(struct timespec));
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	if ( (inaddr = inet_addr(destIP)) == INADDR_NONE ) {
		if ((host = gethostbyname(destIP)) == NULL) {
			perror("gethostbyname error");
			exit(1);
		}
		memcpy((char*) &dest_addr.sin_addr, host->h_addr, host->h_length);
	}else{
		dest_addr.sin_addr.s_addr = inet_addr(destIP);
	}
	h->cndcnt2 = 0;
	catch = 0;
	signal(SIGALRM, rtttimeout);
	h->rStat[6] = h->rStat[7] = 0;
	int datalength = h->rStat[9];
	while (1) {
	    if ( catch == 0 ){
		icmpid[0] = rng(1, 65535, 1);
		if ( h->rStat[9] == 0 ) {
			datalength = rng(1, 1472, 1);
		}
		lsbmsb++;
		lsbmsb=lsbmsb==65535?0:lsbmsb;
		icmpid[1] = lsbmsb;
		for (int i = 0; i < 8; i++) {
			uint16_t keep = icmpid[1] >> 15;
			icmpid[1] <<= 1; 
			icmpid[1] |= keep;
		}
		send_packet(sendpacket, sockfd, datalength, icmpid, h->rStat, &icmpstart);
		h->rStat[6]++;
		h->rStat[10] = mean_calc(h->rStat[10], datalength+8, h->rStat[6]);
		catch = 1;
	}
	int packetreceive = -1;
	while ( catch == 1 ) {
		alarm(1);
		packetreceive = recv_packet(recvpacket, sockfd, icmpid, h->rStat, icmpstart);
		if ( packetreceive >= 0 ) {
			alarm(0);
			h->rStat[7]++;
			h->rStat[5] = ((double)1 - h->rStat[7] / h->rStat[6]) * (double)100;
			tt=h->cndcnt2<rollsize?cr:rollsize;
			rollingMean = mean_calc(rollingMean, h->rStat[0], tt+1);
			//~ h->rStat[2] = mean_calc(h->rStat[2], h->rStat[0], tt+1);
			if (++cr == rollsize) cr = 0;
			rttMeanSpot = mean_calc(rttMeanSpot, h->rStat[0], h->cndcnt2+1);
			//~ h->rStat[1] = mean_calc(h->rStat[1], h->rStat[0], h->cndcnt2+1);
			h->rStat[1] = rttMeanSpot;
			h->rStat[2] = rollingMean;
			h->rStat[3] = (h->rStat[6] / (double)(h->cndcnt)) * (1L/h->zecdecz);
			h->rStat[4] = (h->rStat[7] / (double)(h->cndcnt)) * (1L/h->zecdecz);
			catch = 4;
			break;
		}
		}
		if ( catch == 2 ) {
		    	h->rStat[5] = ((double)1 - h->rStat[7] / h->rStat[6]) * (double)100;
			h->rStat[3] = (h->rStat[6] / (double)(h->cndcnt)) * (1L/h->zecdecz);
			h->rStat[4] = (h->rStat[7] / (double)(h->cndcnt)) * (1L/h->zecdecz);
    			catch = 4;
		}
		h->cndcnt2++;
		if ( h->flop != 0 && catch == 4 ) break;
		catch=catch==4?0:catch;
		for ( int i=0;i<h->slip;i++ ) {
			pselect(0, NULL, NULL, NULL, &pseudoSleeps, NULL);
		}
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &exe);
		h->cpuspin2 = ((double)exe.tv_sec +  NSECS2SECS*(double)exe.tv_nsec);
	}
	pthread_exit(NULL);
	return 0;
}

