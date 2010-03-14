/*
 * FunctionBlocks.c
 *
 *  As funcoes *tick são chamadas a cada loop
 *  Os parametros correspondem a pointers para o endereço inicial dos blocos de memória
 *  de entrada e saída.
 *
 *  Created on: 13/03/2010
 *      Author: Renato
 */

#include <string.h>
#include "util.h"

void SRTick(WORD *p_in, WORD *p_out) {
	WORD in, out, newOut;
	BOOL s1, r, q1;
	int i;

	memcpy(&in, p_in, sizeof(WORD));
	memcpy(&out, p_out, sizeof(WORD));
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
	memcpy(p_out, &newOut, sizeof(WORD));
}

void RSTick(WORD *p_in, WORD *p_out) {
	WORD in, out, newOut;
	BOOL s, r1, q1;
	int i;

	memcpy(&in, p_in, sizeof(WORD));
	memcpy(&out, p_out, sizeof(WORD));
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
	memcpy(p_out, &newOut, sizeof(WORD));
}
