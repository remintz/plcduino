/*
 * Hw.c
 *
 *  Created on: 13/03/2010
 *      Author: Renato
 */

#include <conio.h>
#include <stdio.h>

#include "util.h"

void readInputs(WORD *p_Memory) {
	WORD c;
	printf("<");
	fflush(stdout);
	scanf("%d", &c);
	printf("Lido: 0x%08X\n",c);
	fflush(stdout);
	p_Memory[0] = c;
}

void writeOutputs(WORD *p_Memory) {
	int i;
	printf("> ");
	for (i = 0; i < 8; i++)
		printf(" 0x%04X", p_Memory[i]);
	printf("\n");
	fflush(stdout);
}
