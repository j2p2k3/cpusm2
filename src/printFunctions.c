#include "./printFunctions.h"

void scanner(int *y, int em[], psv0 *vv0, prntsctrl *psc)
{
	if ( em[27] == 0 ) {
		int lim = 0;
		if ( em[10] == 2 || em[10] == 4 ) {
			lim = 16 + em[11];
		}else if ( em[10] == 3 ) {
			lim = 16 + vv0->thermalzones;
		}else if ( em[10] == 1 ) {
			if ( u == 0 ) lim = 16 + 14;
			if ( u != 0 ) lim = 16 + 8;
		}else{
			lim = 24;
		}
		if ( em[3] !=6 ) {
			if ( *y == 0 ) {
				if ( em[23] >= lim ) {
					*y = 1;
					em[23] = lim-1;
				}else{
					psc->focus = em[23];
					vv0->focus = em[23]+1;
				}
				em[23]++;
			}
			if ( *y == 1 ) {
				if ( em[23] == 3 ) {
					*y = 0;
					em[23] = 5;
				}else if ( em[23] >= 15 ) {
						vv0->focus = em[23]-1;
				}else{
					vv0->focus = em[23];
					psc->focus = em[23];
				}
				for ( int i=lim;i>2;i-- ) {
					if ( em[23] == i ) psc->focus = i-2;
				}
				em[23]--;
			}
		}
		if ( em[3] ==6 ) {
			if ( *y == 0 ) {
				if ( em[23] >= lim ) {
					*y = 1;
					em[23] = lim-1;
				}else{
					psc->focus = em[23];
					vv0->focus = em[23]+1;
				}
				em[23]++;
			}
			if ( *y == 1 ) {
				if ( em[23] == 3 ) {
					*y = 0;
					em[23] = 3;
					vv0->focus = 0;
				}else{
					vv0->focus = em[23]-1;
					psc->focus = em[23]-1;
				}
				for ( int i=lim;i>2;i-- ) {
					if ( em[23] == i ) psc->focus = i-2;
				}
				em[23]--;
			}
		}
	}
	if ( em[27] == 1 ) vv0->focus = psc->focus = 0;
	
	return;
}

void print_div(double stat, int verb)
{
	if ( verb == 4 ) {
		printf ("%-7.1lf", stat);
	}else if ( verb == 3 ) {
		printf ("%-7.2lf", stat);
	}else if ( verb == 5 ) {
		printf ("%05.0lf  ", stat);
	}else if ( verb == 7 ) {
		printf ("%05.0lf  ", stat);
	}else if ( verb == 9 ) {
		printf ("       ");
	}else if ( verb == 0 || verb == 2 ) {
		if ( stat >= 1L ) {
			printf ("%-7.1lf", stat*100L);
		}else if ( stat*100L < 10L && stat*100L > 9.999) {
			printf ("%-7.2f", stat*100L);
		}else if ( stat < 0.1L ) {
			printf ("%-7.3f", stat*100L);
		}else{
			printf ("%-7.2f", stat*100L);
		}
	}else{
		if ( stat < 10) printf ("%-7.3lf", stat);
		if ( stat < 100 && stat >= 10) printf ("%-7.2lf", stat);
		if ( stat < 1000 && stat >= 100 ) printf ("%-7.1lf", stat);
		if ( stat < 10000 && stat >= 1000 ) printf ("%05.0lf  ", stat);
		if ( stat >= 10000 ) printf ("%-7.0lf", stat);
	}
	return;
}
void print_div_fo(double stat, int verb)
{
	if ( verb == 4 ) {
		fprintf(fo, "%-4.2lf", stat);
	}else if ( verb == 3 ) {
		fprintf(fo, "%-4.2lf", stat);
	}else if ( verb == 5 ) {
		fprintf(fo, "%05.0lf  ", stat);
	}else if ( verb == 7 ) {
		fprintf(fo, "%05.0lf  ", stat);
	}else if ( verb == 9 ) {
		fprintf(fo, "       ");
	}else if ( verb == 0 || verb == 2 ) {
		if ( stat >= 1L ) {
			fprintf(fo, "%-4.1lf", stat*100L);
		}else if ( stat*100L < 10L && stat*100L > 9.999) {
			fprintf(fo, "%-4.2f", stat*100L);
		}else if ( stat < 0.1L ) {
			fprintf(fo, "%-4.3f", stat*100L);
		}else{
			fprintf(fo, "%-4.2f", stat*100L);
		}
	}else{
		if ( stat < 10) fprintf(fo, "%-5.3lf", stat);
		if ( stat < 100 && stat >= 10) fprintf(fo, "%-5.2lf", stat);
		if ( stat < 1000 && stat >= 100 ) fprintf(fo, "%-6.2lf", stat);
		if ( stat < 10000 && stat >= 1000 ) fprintf(fo, "%-7.1lf  ", stat);
		if ( stat >= 10000 ) fprintf(fo, "%-8.0lf", stat);
	}
	return;
}
void print_colour(int colour, double stat, double val, int focus)
{
	if ( colour != 0 ) {
		if ( focus == 1 ) {
			if ( colour == 1 ) printf(CLOADLOW);
			if ( colour == 2 ) printf(ILOADLOW);
			//~ if ( colour == 3 ) printf("\033[0;38m");
			if ( colour == 3 ) printf(ILOADLOW);
			if ( colour == 4 ) printf(ILOADLOW);
			if ( colour == 7 ) printf(ILOADLOW);
			if ( colour == 6 ) printf(CLOADLOW);
			if ( colour == 5 ) printf(ILOADLOW);
		}else if ( focus == 2 ) {
			if ( colour == 1 ) {
				printf(ILOADHI);
			}else if ( colour == 2 ){
				printf (CLOADHI);
			}else if ( colour == 3 ){
				printf (CLOADHI);
			}else if ( colour == 7 ){
				printf (CLOADHI);
			}else{
				printf (CLOADHI);
			}
		}else{
			if ( colour == 1 ) {
				printf (ILOADLOW);
			}
			if ( colour == 2 ) {
				printf (CLOADLOW);
			}
			if ( colour == 5 ) {
				printf ("\033[1;%im", rng(MINCOLOUR,MAXCOLOUR,1));
			}
			if ( colour == 6 ) {
				printf (ILOADLOW);
			}
			if ( colour == 4 ) {
				printf ("\033[1;%im", rng(MINCOLOUR3,MAXCOLOUR3,1));
			}
			if ( colour == 3 ) {
				if ( val != 0 ) printf (ILOADLOW);
				if ( val == 0 ) printf (CLOADLOW);
			}
		}
	}else{
		printf(DEFCOLOR);
	}
	return;
}
void print_max_colour(double stat, double val, int colour)
{
	if ( colour != 0 ) {
		if ( stat > val && stat != 0 ) printf (VALMAX);
	}
	return;
}
void print_stats_c(prntsctrl psc)
{
	if ( psc.colour == 1 ) printcc("", 2, psc.colour);
	if ( psc.colour >= 2 ) printcc("", 1, psc.colour);
	printf ("%-10i%-7.2lf ", psc.scnt,psc.gen_stat[0][0]);
	printf ("%-6.3lf%s", psc.gen_stat[0][1], psc.runtime);
	if ( psc.colour == 3 ) {
		for ( int i=2;i<16;i++ ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( psc.focus == i ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
			}else if ( psc.focus == i+1 && psc.colour != 0 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( i % 2 == 0 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthis, 0);
			}else{
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthat, 0);
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
			if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[0][i], 0);
			if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[0][i], 1);
			if ( i == 11 ) print_div(psc.gen_stat[0][i], 5);
			if ( i == 12 ) print_div(psc.gen_stat[0][i], 7);
		}
	}else if ( psc.colour == 6 ) {
		for ( int i=2;i<16;i++ ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( psc.focus == (16+psc.stat_count+1)-i ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else{
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 0);
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
			if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[0][i], 0);
			if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[0][i], 1);
			if ( i == 11 ) print_div(psc.gen_stat[0][i], 5);
			if ( i == 12 ) print_div(psc.gen_stat[0][i], 7);
		}
	}else{
		for ( int i=2;i<16;i++ ) {
			if ( psc.colour != 0 ) {
				if ( psc.focus == i-1 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else if ( psc.focus == i && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
				}else if ( psc.focus == i+1 && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else{
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 0);
				}
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
			if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[0][i], 0);
			if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[0][i], 1);
			if ( i == 11 ) print_div(psc.gen_stat[0][i], 5);
			if ( i == 12 ) print_div(psc.gen_stat[0][i], 7);
		}
	}
	return;
}
void print_stats_cs(prntsctrl psc)
{
	if ( psc.colour == 1 ) printcc("", 2, psc.colour);
	if ( psc.colour >= 2 ) printcc("", 1, psc.colour);
	printf ("%-9i", psc.scnt);
	printf ("%s", psc.runtime);
	if ( psc.colour == 3 ) {
		for ( int i=2;i<4;i++ ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( psc.focus == i ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
			}else if ( i % 2 == 0 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthis, 0);
			}else{
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthat, 0);
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
		}
		for ( int i=13;i<16;i++ ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( psc.focus == i ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
			}else if ( i % 2 == 0 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthis, 0);
			}else{
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthat, 0);
			}
			print_div(psc.gen_stat[0][i], 0);
		}
	}else{
		for ( int i=2;i<4;i++ ) {
			if ( psc.colour != 0 ) {
				if ( psc.focus == i-1 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else if ( psc.focus == i && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
				}else if ( psc.focus == i+1 && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else{
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 0);
				}
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
		}
		for ( int i=13;i<16;i++ ) {
			if ( psc.colour != 0 ) {
				if ( psc.focus == i-1 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else if ( psc.focus == i && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
				}else if ( psc.focus == i+1 && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else{
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 0);
				}
			}
			print_div(psc.gen_stat[0][i], 0);
		}
	}
	return;
}
void print_stats(psv0 vv0)
{
	int whichone = 0;
	int onewhich = 0;
	if ( vv0.multistat == 1 ) {
		whichone = vv0.corecnt>vv0.thermalzones?vv0.corecnt:vv0.thermalzones;
		if ( u == 0 ) onewhich = whichone>14?whichone:14;
		if ( u != 0 ) onewhich = whichone>8?whichone:8;	
	}else{
		if ( vv0.verb == 0 ) onewhich = 8;
		if ( vv0.verb == 1 ) {
			if ( u == 0 ) onewhich = 14;
			if ( u != 0 ) onewhich = 8;
		}
		if ( vv0.verb == 2 || vv0.verb == 4 ) onewhich = vv0.corecnt;
		if ( vv0.verb == 3 ) onewhich = vv0.thermalzones;
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		if ( vv0.colour == 3 ) {
			if ( vv0.focus == i+16 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else if ( vv0.focus == i+17 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 2);
			}else if ( vv0.focus == i+18 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else if ( i % 2 == 0 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.sthis, 0);
			}else{
				print_colour(vv0.colour, vv0.stat[i], vv0.sthat, 0);
			}
			print_max_colour(vv0.stat[i], vv0.max[vv0.verb][i], vv0.colour);
			if ( (vv0.verb == 2 || vv0.verb ==4) && vv0.core == i ) printf(ACTIVECPU);
			print_div(vv0.stat[i], vv0.verb);
				print_colour(vv0.colour, vv0.stat[i], vv0.sthat, 0);
		}else if ( vv0.colour == 6 ) {
			if ( vv0.focus == (vv0.stat_count+2)-i ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else if ( vv0.focus == i+16 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else{
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 0);
			}
			print_max_colour(vv0.stat[i], vv0.max[vv0.verb][i], vv0.colour);
			if ( (vv0.verb == 2 || vv0.verb ==4) && vv0.core == i ) printf(ACTIVECPU);
			print_div(vv0.stat[i], vv0.verb);
				print_colour(vv0.colour, vv0.stat[i], vv0.sthat, 0);
		}else{
			if ( vv0.focus == i+16 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else if ( vv0.focus == i+17 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 2);
			}else if ( vv0.focus == i+18 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else{
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 0);
			}
			if ( vv0.verb == 1 && u == 0 && ( i == 11 || i == 12 ) ) {
				print_max_colour(vv0.stat[i], vv0.max[vv0.verb][i], 0);
			}else{
				if ( vv0.colour != 7 ) print_max_colour(vv0.stat[i], vv0.max[vv0.verb][i], vv0.colour);
			}
			if ( (vv0.verb == 2 || vv0.verb ==4) && vv0.core == i ) printf(ACTIVECPU);
			print_div(vv0.stat[i], vv0.verb);
		}
	}
	if ( vv0.multistat == 1 ) {
		if ( vv0.stat_count < onewhich ) {
			for ( int i=vv0.stat_count;i<onewhich;i++ ) {
				print_div(0, 9);
			}
		}
		if ( onewhich < 8 ) {
			for ( int i=onewhich;i<8;i++ ) {
				print_div(0, 9);
			}
		}
	}

	printf(DEFCOLOR);
	printf("\n");
	return;
}
void print_stats_F(psv0 vv0, prntsctrl psc, psheader psh)
{
	int onewhich = whicher(psh);
	for ( int i=0;i<(40+(14+onewhich)*7);i++) {
		printcc("\u2500", 1, psc.colour);
	}
	printcc("\u2510", 1, psc.colour);
	for ( int j=1;j<4;j++ ) {
		printcc("\n", 0, 0);
		if ( j == 1 ) printcs("minimum   ", -10, 2, psh.colour);
		if ( j == 2 ) printcs("maximum   ", -10, 2, psh.colour);
		if ( j == 3 ) printcs("average   ", -10, 2, psh.colour);
		printcc("", 1, psh.colour);
		printf ("%-7.2lf ", psc.gen_stat[j][0]);
		printf("%-6.3lf     ",psc.gen_stat[j][1]);
		if ( j == 1 ) printcs("min.stat", -8, 2, psh.colour);
		if ( j == 2 ) printcs("max.stat", -8, 2, psh.colour);
		if ( j == 3 ) printcs("avg.stat", -8, 2, psh.colour);
		printcc("", 1, psh.colour);
		for ( int i=2;i<16;i++ ) {
			print_colour(psc.colour, psc.gen_stat[j][i], 1, 0);
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[j][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[j][3]);
			if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[j][i], 0);
			if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[j][i], 1);
			if ( i == 11 ) print_div(psc.gen_stat[j][i], 5);
			if ( i == 12 ) print_div(psc.gen_stat[j][i], 7);
		}
		for ( int i=0;i<vv0.stat_count;i++ ) {
			if ( j == 1 ) {
				print_colour(vv0.colour, vv0.min[vv0.verb][i], 0, 0);
				print_div(vv0.min[vv0.verb][i], vv0.verb);
			}else if ( j == 2 ) {
				print_colour(vv0.colour, vv0.max[vv0.verb][i], 0, 0);
				print_div(vv0.max[vv0.verb][i], vv0.verb);
			}else{
				print_colour(vv0.colour, vv0.mean[vv0.verb][i], 0, 0);
				print_div(vv0.mean[vv0.verb][i], vv0.verb);
			}								
		}
	}
	return;
}
void print_stats_Fs(psv0 vv0, prntsctrl psc, psheader psh)
{
	if ( psc.colour != 0 ) printf (ILOADLOW);
	int onewhich = whicher(psh);
	for ( int i=0;i<(25+(5+onewhich)*7);i++) {
		printf("%s", "\u2500");
	}
	printcc("\u2510", 1, psc.colour);
	for ( int j=1;j<4;j++ ) {
		printcc("\n", 0, 0);
		if ( psc.colour == 1 ) printcc("", 2, psc.colour);
		if ( psc.colour >= 2 ) printcc("", 1, psc.colour);
		if ( j == 1 ) printcs("minimum    ", -10, 1, psh.colour);
		if ( j == 2 ) printcs("maximum    ", -10, 1, psh.colour);
		if ( j == 3 ) printcs("average    ", -10, 1, psh.colour);
		if ( j == 1 ) printcs("min.stat  ", 8, 1, psh.colour);
		if ( j == 2 ) printcs("max.stat  ", 8, 1, psh.colour);
		if ( j == 3 ) printcs("avg.stat  ", 8, 1, psh.colour);
		for ( int i=2;i<4;i++ ) {
			if ( psc.colour != 0 ) {
				if ( psc.focus == i-1 ) {
					print_colour(psc.colour, psc.gen_stat[j][i], 0, 1);
				}else if ( psc.focus == i && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[j][i], 0, 2);
				}else{
					print_colour(psc.colour, psc.gen_stat[j][i], 0, 0);
				}
			}
			if ( i == 2 ) printf (" %6.1lf  ", psc.gen_stat[j][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[j][3]);
		}
		for ( int i=13;i<16;i++ ) {
			if ( psc.colour != 0 ) {
				if ( psc.focus == i-1 ) {
					print_colour(psc.colour, psc.gen_stat[j][i], 0, 1);
				}else if ( psc.focus == i && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[j][i], 0, 2);
				}else{
					print_colour(psc.colour, psc.gen_stat[j][i], 0, 0);
				}
			}
			print_div(psc.gen_stat[j][i], 0);
		}
		for ( int i=0;i<vv0.stat_count;i++ ) {
			if ( j == 1 ) {
				print_colour(vv0.colour, vv0.min[vv0.verb][i], 0, 0);
				print_div(vv0.min[vv0.verb][i], vv0.verb);
			}else if ( j == 2 ) {
				print_colour(vv0.colour, vv0.max[vv0.verb][i], 0, 0);
				print_div(vv0.max[vv0.verb][i], vv0.verb);
			}else{
				print_colour(vv0.colour, vv0.mean[vv0.verb][i], 0, 0);
				print_div(vv0.mean[vv0.verb][i], vv0.verb);
			}								
		}
	}
	return;
}
void print_header(psheader psh)
{
	int vs[3] = {0};
	const char* verb0[3];
	const char* schd0;
	if ( psh.verb == 0 ) {
		verb0[0] = "cpu.statistics";
	}else if ( psh.verb == 1 ) {
		if ( u == 0 ) {
			verb0[0] = "network.statistics [ icmp.rtt -> ";
			verb0[1] = psh.ipa;
			vs[1] = strlen(verb0[1]);
			verb0[2] = " ]";
			vs[2] = strlen(verb0[2]);
		}
		if ( u != 0 ) verb0[0] = "network.statistics";
	}else if ( psh.verb == 2 ) {
		verb0[0] = "core.statistics";
	}else if ( psh.verb == 3 ) {
		verb0[0] = "thermal.statsistics";
	}else{
		verb0[0] = "frequency.statistics";
	}
	if ( psh.sched == 0 ) {
		schd0 = "nrml";
	}else if ( psh.sched == 1 ) {
		schd0 = "fifo";
	}else if ( psh.sched == 2 ) {
		schd0 = "rdrn";
	}else if ( psh.sched == 3 ) {
		schd0 = "btch";
	}else if ( psh.sched == 5 ) {
		schd0 = "idle";
	}else if ( psh.sched == 6 ) {
		schd0 = "ddln";
	}else{
		schd0 = "n/a ";
	}
	vs[0] = strlen(verb0[0]);
	printf(DEFCOLOR);
	char sppid[1234567];
	sprintf(sppid, "%d", psh.ppid);
	printci(psh.ppid, 2, psh.colour);
	printcc(".metrics", 2, psh.colour);
	int strlensppid = 29 - strlen(sppid);
	for (int i=0;i<strlensppid;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510", 1, psh.colour);
	printcc("\u2503", 1, psh.colour);
	printcc("\u250C", 1, psh.colour);
	printccc(verb0[0], vs[0], 2, psh.colour);
	if ( psh.verb == 1 ) {
		if ( u == 0 ) {
			printccc(verb0[1], vs[1], 2, psh.colour);
			printccc(verb0[2], vs[2], 2, psh.colour);
			vs[0] += vs[1];
			vs[0] += vs[2];
		}
	}
	int onewhich = whicher(psh);
	for ( int i=0;i<(14+onewhich)*7-vs[0];i++) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510\n", 1, psh.colour);
	verb0[0]=NULL;
	printcc("                                     ", 4, psh.colour);
	printcc(" \u2503 ", 1, psh.colour);
	for ( int i=0;i<(14+onewhich)*7;i++) {
		printcc(" ", 4, psh.colour);
	}
	printf(DEFCOLOR);
	printf("\n");
	printcc("", 4, psh.colour);
	printcc(" schd:", 4, psh.colour);
	printccc(schd0, 4, 4, psh.colour);
	printcs(" pri:", 4, 4, psh.colour);
	printf("%02i  ", psh.pri);
	printcs(" nce:", 4, 4, psh.colour);
	printf("%03i  ", psh.nice);
	if ( psh.cpun == -9 ) {
		printcc(" core:k ", 4, psh.colour);
	}else{
		printcc(" core:", 4, psh.colour);
		printf("%02i", psh.cpun);
	}
	printcc(" ┃ ", 1, psh.colour);
	for ( int i=0;i<(14+onewhich)*7;i++) {
		printcc(" ", 4, psh.colour);
	}
	printf(DEFCOLOR);
	printf("\n");
	for (int i=0;i<37;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510", 1, psh.colour);
	printcc("┃", 1, psh.colour);
	printcc("\u250C", 1, psh.colour);
	for (int i=0;i<12;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510\u250C", 1, psh.colour);
	for (int i=0;i<9;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("memory", 2, psh.colour);
	for (int i=0;i<10;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510\u250C", 1, psh.colour);
	for (int i=0;i<6;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("load.avg", 2, psh.colour);
	for (int i=0;i<6;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510\u250C", 1, psh.colour);
	for (int i=0;i<4;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("procs", 2, psh.colour);
	for (int i=0;i<3;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510\u250C", 1, psh.colour);
	for (int i=0;i<6;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("cpu.avg", 2, psh.colour);
	for (int i=0;i<6;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510", 1, psh.colour);
	const char* title0;
	int sizeTitle = 0, sizeTitlePrint = 0;
	if ( psh.verb == 0 ) {
		title0 = "kernel.performance";
	}else if ( psh.verb == 1 ) {
		title0 = psh.dev;
	}else if ( psh.verb == 2 ) {
		title0 = "core.performance";
	}else if ( psh.verb == 3 ) {
		title0 = "sys.temp";
	}else{
		title0 = "core.frequency";
	}
	sizeTitle = strlen(title0);
	if ( psh.verb == 1 ) {
		sizeTitlePrint = (4*7-(sizeTitle+3))/2;
		printcc("\u250C", 1, psh.colour);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printccc(title0, sizeTitle, 2, psh.colour);
		printcc(".rx", 2, psh.colour);
		for ( int q=0;q<sizeTitlePrint-1;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printcc("\u2510\u250C", 1, psh.colour);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printccc(title0, sizeTitle, 2, psh.colour);
		printcc(".tx", 2, psh.colour);
		if ( u == 0 ) {
			for ( int q=0;q<sizeTitlePrint-1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
		}else{
			for ( int q=0;q<sizeTitlePrint+1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
		}
		if ( u == 0 ) {
			sizeTitlePrint = (3*7-strlen("rtt.avg"))/2;
			printcc("\u2510\u250C", 1, psh.colour);
			for ( int q=0;q<sizeTitlePrint-1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
			printcc("rtt.avg", 2, psh.colour);
			for ( int q=0;q<sizeTitlePrint-1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
			printcc("\u2510\u250C", 1, psh.colour);
			for ( int q=0;q<sizeTitlePrint-1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
			printcc("rtt.trk", 2, psh.colour);
			for ( int q=0;q<sizeTitlePrint+1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
			if ( psh.multistat == 1 && onewhich > 14 ) {
				printcc("\u2510\u250C", 1, psh.colour);
				for ( int q=2;q<(onewhich-14)*7;q++ ) {
					printcc("\u2500", 1, psh.colour);
				}
			}
		}else if ( psh.multistat == 1 && onewhich > 8 ) {
			printcc("\u2510\u250C", 1, psh.colour);
			for ( int q=2;q<(onewhich-8)*7;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
		}
		printcc("\u2510\n", 1, psh.colour);
	}else{
		if ( sizeTitle % 2 == 1 || onewhich % 2 == 1 ) sizeTitle += 1;
		sizeTitlePrint = (onewhich*7-sizeTitle)/2;
		printcc("\u250C", 1, psh.colour);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printccc(title0, sizeTitle, 2, psh.colour);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printcc("\u2510\n", 1, psh.colour);
	}
	title0=NULL;
	printcs("count", -10, 1, psh.colour);
	printcs("algn", -8, 1, psh.colour);
	if ( psh.pcputype == 0 ) printcs("pcpu", 12, 1, psh.colour);
	if ( psh.pcputype != 0 ) {
		printcs("pcpu.", 5, 1, psh.colour);
		printci(psh.pcputype-1, 1, psh.colour);
		printcs("", 6, 1, psh.colour);
	}
	printcs("runtime", 7, 1, psh.colour);
	printcc(" ┃ ", 1, psh.colour);
	printcs("a.freq", -6, 1, psh.colour);
	printcs("  a.\u00B0C", -7, 1, psh.colour);
	printcs("  ", 2, 1, psh.colour);
	printcs("free", -7, 1, psh.colour);
	printcs("avai", -7, 1, psh.colour);
	printcs("buff", -7, 1, psh.colour);
	printcs("cach", -7, 1, psh.colour);
	printcs("01mn", -7, 1, psh.colour);
	printcs("05mn", -7, 1, psh.colour);
	printcs("15mn", -7, 1, psh.colour);
	printcs("rnng", -7, 1, psh.colour);
	printcs("actv", -7, 1, psh.colour);
	printcs("spot", -7, 1, psh.colour);
	printcs("runn", -7, 1, psh.colour);
	printcs("r.", -2, 1, psh.colour);
	printf("%03i  ", psh.roll);
	if ( psh.verb == 0 ) {
		printcs("user", -7, 1, psh.colour);
		printcs("nice", -7, 1, psh.colour);
		printcs("syst", -7, 1, psh.colour);
		printcs("idle", -7, 1, psh.colour);
		printcs("iowa", -7, 1, psh.colour);
		printcs("irqs", -7, 1, psh.colour);
		printcs("sirq", -7, 1, psh.colour);
		printcs("maxi", -7, 1, psh.colour);
		if ( psh.multistat == 1 && onewhich > psh.stat_count ) {
			for ( int f=8; f<onewhich; f++ ) {
				printcs("  --", -7, 1, psh.colour);
			}
		}
		printcc("\n\n", 1, psh.colour);
	}
	if ( psh.verb == 1 ) {
		printcs("spot", -7, 1, psh.colour);
		printcs("runn", -7, 1, psh.colour);
		printcs("r.", -2, 1, psh.colour);
		printf("%03i  ", psh.roll);
		printcs("pp/s", -7, 1, psh.colour);
		printcs("spot", -7, 1, psh.colour);
		printcs("runn", -7, 1, psh.colour);
		printcs("r.", -2, 1, psh.colour);
		printf("%03i  ", psh.roll);
		printcs("pp/s", -7, 1, psh.colour);
		if ( u == 0 ) {
			printcs("spot", -7, 1, psh.colour);
			printcs("runn", -7, 1, psh.colour);
			printcs("r.", -2, 1, psh.colour);
			printf("%03i  ", psh.roll);
			printcs("tx/s", -7, 1, psh.colour);
			printcs("rx/s", -7, 1, psh.colour);
			printcs("\%loss", -7, 1, psh.colour);
			if ( psh.multistat == 1 && onewhich > 14 ) {
				for ( int f=14;f<onewhich;f++ ) {
				printcs("  --", -7, 1, psh.colour);
				}
			}
		}else if ( psh.multistat == 1 && onewhich > 8 ) {
			for ( int f=8; f<onewhich; f++ ) {
				printcs("  --", -7, 1, psh.colour);
			}
		}
		printcc("\n\n", 1, psh.colour);
	}
	if (psh.verb == 2 || psh.verb == 4 ) {
	   for ( int f=0; f<psh.corecnt; f++ ) {
			printcs("cpu.", -4, 1, psh.colour);
			printf("%-3i", f);
		}
		if ( psh.multistat == 1 && onewhich > psh.corecnt ) {
			for ( int f=psh.corecnt ;f<onewhich;f++ ) {
				printcs("  --", -7, 1, psh.colour);
			}
		}
		printcc("\n\n", 1, psh.colour);
	}
	if (psh.verb == 3) {
		for ( int k=0;k<psh.thermalzones;k++ ) {
			printcs("\u00B0C", 2, 1, psh.colour);
			printf("%02i   ", k);
		}
		if ( psh.multistat == 1 && psh.corecnt > psh.thermalzones ) {
			for ( int f=psh.thermalzones; f<psh.corecnt; f++ ) {
				printcs("  --", -7, 1, psh.colour);
			}
		}
		printcc("\n\n", 1, psh.colour);
	}
	if (psh.scroll <= 0) {
		printcc("\n", 1, psh.colour);
	}
	return;
}
void print_header_s(psheader psh)
{
	int vs[3] = {0};
	const char* verb0[3];
	if ( psh.verb == 0 ) {
		verb0[0] = "cpu.statistics";
	}else if ( psh.verb == 1 ) {
		if ( u == 0 ) {
			verb0[0] = "network.statistics [ icmp.rtt -> ";
			verb0[1] = psh.ipa;
			vs[1] = strlen(verb0[1]);
			verb0[2] = " ]";
			vs[2] = strlen(verb0[2]);
		}
		if ( u != 0 ) verb0[0] = "network.statistics";
	}else if ( psh.verb == 2 ) {
		verb0[0] = "core.statistics";
	}else if ( psh.verb == 3 ) {
		verb0[0] = "thermal.statsistics";
	}else{
		verb0[0] = "frequency.statistics";
	}
	vs[0] = strlen(verb0[0]);
	printf(DEFCOLOR);
	char sppid[1234567];
	sprintf(sppid, "%d", psh.ppid);
	printci(psh.ppid, 2, psh.colour);
	printcc(".metrics", 2, psh.colour);
	int strlensppid = 14 - strlen(sppid);
	for (int i=0;i<strlensppid;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510", 1, psh.colour);
	printcc("\u2503", 1, psh.colour);
	printcc("\u250C", 1, psh.colour);
	printccc(verb0[0], vs[0], 2, psh.colour);
	if ( psh.verb == 1 ) {
		if ( u == 0 ) {
			printccc(verb0[1], vs[1], 2, psh.colour);
			printccc(verb0[2], vs[2], 2, psh.colour);
			vs[0] += vs[1];
			vs[0] += vs[2];
		}
	}
	int onewhich = whicher(psh);
	for ( int i=0;i<(5+onewhich)*7-vs[0];i++) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510\n", 1, psh.colour);
	verb0[0]=NULL;
	printcc("                      ", 4, psh.colour);
	printcc(" \u2503 ", 1, psh.colour);
	for ( int i=0;i<(5+onewhich)*7;i++) {
		printcc(" ", 4, psh.colour);
	}
	printf("\n");
	printcc("", 4, psh.colour);
	printcc(" s:", 4, psh.colour);
	printci(psh.sched, 4, psh.colour);
	printcc(" p:", 4, psh.colour);
	printf("%02i ", psh.pri);
	printcc(" n:", 4, psh.colour);
	printf("%03i ", psh.nice);
	if ( psh.cpun == -9 ) {
		printcc(" c:k ", 4, psh.colour);
	}else{
		printcc(" c:", 4, psh.colour);
		printci(psh.cpun, 4, psh.colour);
	}
	printcc(" ┃ ", 1, psh.colour);
	for ( int i=0;i<(5+onewhich)*7;i++) {
		printcc(" ", 4, psh.colour);
	}
	printf("\n");
	printcc("\u2500", 1, psh.colour);
	for (int i=0;i<21;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510", 1, psh.colour);
	printcc("┃", 1, psh.colour);
	printcc("\u250C", 1, psh.colour);
	for (int i=0;i<12;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510\u250C", 1, psh.colour);
	for (int i=0;i<6;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("cpu.avg", 2, psh.colour);
	for (int i=0;i<6;i++ ) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510", 1, psh.colour);	
	const char* title0;
	int sizeTitle = 0, sizeTitlePrint = 0;
	if ( psh.verb == 0 ) {
		title0 = "kernel.performance";
	}else if ( psh.verb == 1 ) {
		title0 = psh.dev;
	}else if ( psh.verb == 2 ) {
		title0 = "core.performance";
	}else if ( psh.verb == 3 ) {
		title0 = "sys.temp";
	}else{
		title0 = "core.frequency";
	}
	sizeTitle = strlen(title0);
	if ( psh.verb == 1 ) {
		sizeTitlePrint = (4*7-(sizeTitle+3))/2;
		printcc("\u250C", 1, psh.colour);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printccc(title0, sizeTitle, 2, psh.colour);
		printcc(".rx", 2, psh.colour);
		for ( int q=0;q<sizeTitlePrint-1;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printcc("\u2510\u250C", 1, psh.colour);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printccc(title0, sizeTitle, 2, psh.colour);
		printcc(".tx", 2, psh.colour);
		if ( u == 0 ) {
			for ( int q=0;q<sizeTitlePrint-1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
		}else{
			for ( int q=0;q<sizeTitlePrint+1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
		}
		if ( u == 0 ) {
			sizeTitlePrint = (3*7-strlen("rtt.avg"))/2;
			printcc("\u2510\u250C", 1, psh.colour);
			for ( int q=0;q<sizeTitlePrint-1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
			printcc("rtt.avg", 2, psh.colour);
			for ( int q=0;q<sizeTitlePrint-1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
			printcc("\u2510\u250C", 1, psh.colour);
			for ( int q=0;q<sizeTitlePrint-1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
			printcc("rtt.trk", 2, psh.colour);
			for ( int q=0;q<sizeTitlePrint+1;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
			if ( psh.multistat == 1 && onewhich > 14 ) {
				printcc("\u2510\u250C", 1, psh.colour);
				for ( int q=2;q<(onewhich-14)*7;q++ ) {
					printcc("\u2500", 1, psh.colour);
				}
			}
		}else if ( psh.multistat == 1 && onewhich > 8 ) {
			printcc("\u2510\u250C", 1, psh.colour);
			for ( int q=2;q<(onewhich-8)*7;q++ ) {
				printcc("\u2500", 1, psh.colour);
			}
		}
		printcc("\u2510\n", 1, psh.colour);
	}else{
		if ( sizeTitle % 2 == 1 || onewhich % 2 == 1 ) sizeTitle += 1;
		sizeTitlePrint = (onewhich*7-sizeTitle)/2;
		printcc("\u250C", 1, psh.colour);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printccc(title0, sizeTitle, 2, psh.colour);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printcc("\u2500", 1, psh.colour);
		}
		printcc("\u2510\n", 1, psh.colour);
	}
	title0=NULL;
	printcs("count", -10, 1, psh.colour);
	printcs(" ", 5, 1, psh.colour);
	printcs("runtime", 7, 1, psh.colour);
	printcc(" ┃ ", 1, psh.colour);
	printcs("a.freq", -6, 1, psh.colour);
	printcs("  a.\u00B0C", -7, 1, psh.colour);
	printcs("  ", 2, 1, psh.colour);
	printcs("spot", -7, 1, psh.colour);
	printcs("runn", -7, 1, psh.colour);
	printcs("r.", -2, 1, psh.colour);
	printf("%03i  ", psh.roll);
	if ( psh.verb == 0 ) {
		printcs("user", -7, 1, psh.colour);
		printcs("nice", -7, 1, psh.colour);
		printcs("syst", -7, 1, psh.colour);
		printcs("idle", -7, 1, psh.colour);
		printcs("iowa", -7, 1, psh.colour);
		printcs("irqs", -7, 1, psh.colour);
		printcs("sirq", -7, 1, psh.colour);
		printcs("maxi", -7, 1, psh.colour);
		if ( psh.multistat == 1 && onewhich > psh.stat_count ) {
			for ( int f=8; f<onewhich; f++ ) {
				printcs("  --", -7, 1, psh.colour);
			}
		}
		printcc("\n\n", 1, psh.colour);
	}
	if ( psh.verb == 1 ) {
		printcs("spot", -7, 1, psh.colour);
		printcs("runn", -7, 1, psh.colour);
		printcs("r.", -2, 1, psh.colour);
		printf("%03i  ", psh.roll);
		printcs("pp/s", -7, 1, psh.colour);
		printcs("spot", -7, 1, psh.colour);
		printcs("runn", -7, 1, psh.colour);
		printcs("r.", -2, 1, psh.colour);
		printf("%03i  ", psh.roll);
		printcs("pp/s", -7, 1, psh.colour);
		if ( u == 0 ) {
			printcs("spot", -7, 1, psh.colour);
			printcs("runn", -7, 1, psh.colour);
			printcs("r.", -2, 1, psh.colour);
			printf("%03i  ", psh.roll);
			printcs("tx/s", -7, 1, psh.colour);
			printcs("rx/s", -7, 1, psh.colour);
			printcs("\%loss", -7, 1, psh.colour);
			if ( psh.multistat == 1 && onewhich > 14 ) {
				for ( int f=14;f<onewhich;f++ ) {
				printcs("  --", -7, 1, psh.colour);
				}
			}
		}else if ( psh.multistat == 1 && onewhich > 8 ) {
			for ( int f=8; f<onewhich; f++ ) {
				printcs("  --", -7, 1, psh.colour);
			}
		}
		printcc("\n\n", 1, psh.colour);
	}
	if (psh.verb == 2 || psh.verb == 4 ) {
	   for ( int f=0; f<psh.corecnt; f++ ) {
			printcs("cpu.", -4, 1, psh.colour);
			printf("%-3i", f);
		}
		if ( psh.multistat == 1 && onewhich > psh.corecnt ) {
			for ( int f=psh.corecnt ;f<onewhich;f++ ) {
				printcs("  --", -7, 1, psh.colour);
			}
		}
		printcc("\n\n", 1, psh.colour);
	}
	if (psh.verb == 3) {
		for ( int k=0;k<psh.thermalzones;k++ ) {
			printcs("\u00B0C", 2, 1, psh.colour);
			printf("%02i   ", k);
		}
		if ( psh.multistat == 1 && psh.corecnt > psh.thermalzones ) {
			for ( int f=psh.thermalzones; f<psh.corecnt; f++ ) {
				printcs("  --", -7, 1, psh.colour);
			}
		}
		printcc("\n\n", 1, psh.colour);
	}
	if (psh.scroll <= 0) {
		printcc("\n", 1, psh.colour);
	}
	return;
}
void print_log_stats(prntsctrl psc, psv0 vv0)
{
	fprintf(fo, "%i,%.1lf,%.1lf,%s,", psc.scnt,psc.gen_stat[0][0],psc.gen_stat[0][1],psc.runtime);
	for ( int i=2;i<16;i++ ) {
		print_div_fo(psc.gen_stat[0][i], 4); 
		fprintf(fo, ",");
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		print_div_fo(vv0.stat[i], 4);
		if ( i < (vv0.stat_count-1) ) {
			fprintf(fo, ",");
		}else{
			fprintf(fo, "\n");
		}
	}
}
void print_logheader(int netdis, int corecnt)
{
	fprintf (fo,"%-4s %-4s %-4s %-4s ","cntt","ctme","%cpu","time");
		fprintf (fo,"%-4s %-4s %-4s ","freq","temp","load");
		fprintf (fo,"%-4s ", "idle");
		fprintf (fo,"%-4s ", "vidl");
		fprintf (fo,"%-4s ", "mcpu");
		fprintf (fo,"%-4s ", "vcpu");
		fprintf (fo,"%-4s ", "cavg");
		fprintf (fo,"%-4s ", "ca1s");
		fprintf (fo,"%-4s ", "syss");
		fprintf (fo,"%-4s ", "usrr");
		fprintf (fo,"%-4s ", "nice");
		fprintf (fo,"%-4s ", "ioio");
		fprintf (fo,"%-4s ", "irqq");
		for ( int i=0; i<corecnt; i++ ) {
			fprintf (fo,"%-3s%i,", "cpu",i);
		}
		if ( netdis != 0 ) {
			fprintf (fo, "%-4s ", "rxMs");
			fprintf (fo, "%-4s ", "rxMa");
			fprintf (fo, "%-4s ", "rxM1");
			fprintf (fo, "%-4s ", "rxps");
			fprintf (fo, "%-4s ", "txMs");
			fprintf (fo, "%-4s ", "txMa");
			fprintf (fo, "%-4s ", "txM1");
			fprintf (fo, "%-4s ", "txps");
		}
		fprintf (fo,"%-4s %-4s ", "prcs","nlwp");
		fprintf (fo,"%-4s ", "avai");
		fprintf (fo,"%-4s ", "free");
		fprintf (fo,"%-4s\n", "swap");
	return;
}
