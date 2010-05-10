/*
 * FunctionBlocks.c
 *
 *  As funcoes *Scan são chamadas a cada loop
 *  Os parametros correspondem a pointers para o endereço inicial dos blocos de memória
 *  de entrada e saída.
 *
 *  Created on: 13/03/2010
 *      Author: Renato
 */

#include <Wprogram.h>
#include <string.h>
//#include <time.h>
#include "util.h"

void SRScan(unsigned char *p_in, unsigned char *p_out) {
	unsigned char in, out, newOut;
	BOOL s1, r, q1;
	int i;

	memcpy(&in, p_in, 1);
	memcpy(&out, p_out, 1);
	newOut = 0;
	for (i = 0; i < 4; i++) {
		//--- input
		s1 = in & 0x01;
		r = (in >> 1) & 0x01;
		q1 = out & 0x01;
		//--- action
		//--- on SR, the priority is for the S1 input
		if (r) {
			q1 = 0;
		}
		if (s1) {
			q1 = 1;
		}
		newOut = newOut | ((q1 & 0x01) << i);
		in = in >> 2;
		out = out >> 1;
	}
	memcpy(p_out, &newOut, 1);
}

void RSScan(unsigned char *p_in, unsigned char *p_out) {
	WORD in, out, newOut;
	BOOL s, r1, q1;
	int i;

	memcpy(&in, p_in, 1);
	memcpy(&out, p_out, 1);
	newOut = 0;
	for (i = 0; i < 4; i++) {
		//--- input
		s = in & 0x01;
		r1 = (in >> 1) & 0x01;
		q1 = out & 0x01;
		//--- action
		//--- on RS, the priority is for the R1 input
		if (s) {
			q1 = 1;
		}
		if (r1) {
			q1 = 0;
		}
		newOut = newOut | ((q1 & 0x01) << i);
		in = in >> 2;
		out = out >> 1;
	}
	memcpy(p_out, &newOut, 1);
}

void TONScan(unsigned char *p_in, unsigned char *p_out, int p_DeltaTime) {
	unsigned char in, newQ;
	unsigned char mask;
	unsigned char tonQ;
	int tonPT, tonET;
	int i;

	memcpy(&in, p_in, 1);
	memcpy(&newQ, p_out, 1);
	mask = 0x01;
	for (i = 0; i < 2; i++) {
		memcpy(&tonET, p_out + 1 + (i * 2), sizeof(int));
		memcpy(&tonPT, p_in + 1 + (i * 2), sizeof(int));
		tonQ = 0;
		if (in & 0x01) {
			Serial.print("in=");
			Serial.print(in,DEC);
			Serial.print(" ET=");
			Serial.print(tonET,DEC);
			Serial.print(" PT=");
			Serial.print(tonPT,DEC);
			Serial.print(" Delta=");
			Serial.println(p_DeltaTime, DEC);
			if (tonET < tonPT) {
				tonET += p_DeltaTime;
				if (tonET > tonPT) {
					tonET = tonPT;
				}
			}
			if (tonET >= tonPT) {
				tonQ = 1;
			}
		}
		else {
			tonET = 0;
			tonQ = 0;
		}
		newQ = (newQ & ~mask) | (tonQ << i);
		memcpy(p_out + 1 + (i * 2), &tonET, sizeof(int));
		in = in >> 1;
		mask = mask << 1;
	}
	memcpy(p_out, &newQ, 1);
}

void TPScan(unsigned char *p_in, unsigned char *p_out, int p_DeltaTime) {
	unsigned char in, newQ;
	unsigned char mask;
	unsigned char tpQ;
	int tpPT, tpET;
	int i;

	memcpy(&in, p_in, 1);
	memcpy(&newQ, p_out, 1);
	mask = 0x01;
	for (i = 0; i < 2; i++) {
		memcpy(&tpET, p_out + 1 + (i * 2), sizeof(int));
		memcpy(&tpPT, p_in + 1 + (i * 2), sizeof(int));
		tpQ = newQ & mask;
		if (tpET >= tpPT) {
			tpQ = 0;
		} else {
			if (tpQ == 1) {
				tpET += p_DeltaTime;
				if (tpET >= tpPT) {
					tpET = tpPT;
					tpQ = 0;
				}
			}
		}
		if (((in & 0x01) == 1) && (tpET == 0)) {
			tpQ = 1;
		}
		if (((in & 0x01) == 0) && (tpET == tpPT)) {
			tpET = 0;
		}
		newQ = (newQ & ~mask) | (tpQ << i);
		memcpy(p_out + 1 + (i * 2), &tpET, sizeof(int));
		in = in >> 1;
		mask = mask << 1;
	}
	memcpy(p_out, &newQ, 1);
}

void TOFScan(unsigned char *p_in, unsigned char *p_out, int p_DeltaTime) {
	unsigned char in, newQ;
	unsigned char mask;
	unsigned char tofQ;
	int tofPT, tofET;
	int i;

	memcpy(&in, p_in, 1);
	memcpy(&newQ, p_out, 1);
	mask = 0x01;
	for (i = 0; i < 2; i++) {
		memcpy(&tofET, p_out + 1 + (i * 2), sizeof(int));
		memcpy(&tofPT, p_in + 1 + (i * 2), sizeof(int));
		tofQ = newQ & mask;
		//		if (tofQ & 0x01) {
		//			Serial.print("in=");
		//			Serial.print(in,DEC);
		//			Serial.print(" Q=");
		//			Serial.print(tofQ,DEC);
		//			Serial.print(" ET=");
		//			Serial.print(tofET,DEC);
		//			Serial.print(" PT=");
		//			Serial.print(tofPT,DEC);
		//			Serial.print(" Delta=");
		//			Serial.println(p_DeltaTime, DEC);
		//		}
		if ((tofQ == 0x01) && ((in & 0x01) == 0)) {
			tofET += p_DeltaTime;
			if (tofET >= tofPT) {
				tofET = tofPT;
				tofQ = 0;
			}
		}
		if ((in & 0x01) == 1) {
			tofQ = 1;
			tofET = 0;
		}
		newQ = (newQ & ~mask) | (tofQ << i);
		memcpy(p_out + 1 + (i * 2), &tofET, sizeof(int));
		in = in >> 1;
		mask = mask << 1;
	}
	memcpy(p_out, &newQ, 1);
}

void CTUDScan(unsigned char *p_in, unsigned char *p_out) {
	unsigned char cu, cd, r, ld, mcu, mcd, m, newM, qu, qd, q, newQ;
	unsigned char in1, in2;
	unsigned char mask;
	int pv, cv;
	int i;

	memcpy(&in1, p_in, 1); // cu e cd
	memcpy(&in2, p_in+1, 1); // r e ld
	memcpy(&m, p_out+1, 1); // status anterior de cu e cd
	memcpy(&q, p_out, 1); // qu e qd
	newQ = q;
	newM = in1;
	mask = 0x03;
	for (i = 0; i < 4; i++) {
		r = in2 & 0x01;
		ld = (in2 >> 1) & 0x01;
		cu = in1 & 0x01;
		cd = (in1 >> 1) & 0x01;
		mcu = m & 0x01;
		mcd = (m >> 1) & 0x01;
		memcpy(&pv, p_in + 2 + (i * 2), sizeof(int));
		memcpy(&cv, p_out + 2 + (i * 2), sizeof(int));
		qu = q & 0x01;
		qd = (q >> 1) & 0x01;
		if (i == 0) {
			Serial.print("r=");
			Serial.print(r,DEC);
			Serial.print(" ld=");
			Serial.print(ld,DEC);
			Serial.print(" cu=");
			Serial.print(cu,DEC);
			Serial.print(" cd=");
			Serial.print(cd,DEC);
			Serial.print(" mcu=");
			Serial.print(mcu,DEC);
			Serial.print(" mcd=");
			Serial.print(mcd,DEC);
			Serial.print(" pv=");
			Serial.print(pv,DEC);
			Serial.print(" cv=");
			Serial.print(cv,DEC);
			Serial.print(" qu=");
			Serial.print(qu,DEC);
			Serial.print(" qd=");
			Serial.println(qd,DEC);
		}
		if (r == 1) {
			cv = 0;
		} else if (ld == 1) {
			cv = pv;
		} else {
			if (!((cu == 1) && (cd == 1))) {
				if ((mcu == 0) && (cu == 1) && (cv < pv)) {
					cv = cv + 1;
				}
				else if ((mcd == 0) && (cd == 1) && (cv > 0)) {
					cv = cv - 1;
				}
			}
		}
		qu = (cv >= pv);
		qd = (cv <= 0);

		newQ = (newQ & ~mask);
		newQ = newQ | (qu << (i * 2)) | (qd << (i * 2 + 1));
		memcpy(p_out + 2 + (i * 2), &cv, sizeof(int));
		in1 = in1 >> 2;
		in2 = in2 >> 2;
		m = m >> 2;
		q = q >> 2;
		mask = mask << 2;
	}
	memcpy(p_out, &newQ, 1);
	memcpy(p_out + 1, &newM, 1);
}
