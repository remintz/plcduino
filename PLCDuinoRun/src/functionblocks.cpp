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
		newQ = (newQ & ~mask) | (tonQ << i);
		memcpy(p_out + 1 + (i * 2), &tonET, sizeof(int));
		in = in >> 1;
		mask = mask << 1;
	}
	memcpy(p_out, &newQ, 1);
}
