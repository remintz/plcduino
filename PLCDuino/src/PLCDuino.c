/*
 ============================================================================
 Name        : PLCDuino.c
 Author      : Renato Mintz
 Version     :
 Copyright   : Renato Mintz
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "ilcode.h"
#include "ILRun.h"
#include "Parse.h"

#define PROG_SIZE	100
static Instruction program[PROG_SIZE];

void initProgram() {
}

int main(void) {

#if 0
	char in[] =
		"LD %I000/2\n"
		"STN %Q001/5\n"
		"LD %I000/4\n"
		"STN %X000/0";  // CONTROL.0.0 = IN.0.4
;
	initMem();
#endif

#if 0
	//------- teste TON
	char in[] =
		"LD %I000/2\n"
		"STN %O000/0\n"	// TON.0.IN = ~IN.0.2
		"LD %A000\n"
		"ST %O0001\n"	// TON.0.PT = NVRAM[0]
		"LD %O000/0\n"
		"ST %Q001/5\n"	// OUT.1.5 = TON.0.Q
		"LD %I000/4\n"
		"STN %X000/0";  // CONTROL.0.0 = IN.0.4
	initMem();
#endif

#if 0
	//------- teste TP
	char in[] =
		"LD %I000/2\n"
		"STN %N000/0\n"	// TP.0.IN = ~IN.0.2
		"LD %A000\n"
		"ST %N0001\n"	// TP.0.PT = NVRAM[0]
		"LD %N000/0\n"
		"ST %Q001/5\n"	// OUT.1.5 = TP.0.Q
		"LD %I000/4\n"
		"STN %X000/0";  // CONTROL.0.0 = IN.0.4
	initMem();
#endif

#if 0
	//------- teste TOF
	char in[] =
		"LD %I000/2\n"
		"STN %P000/0\n"	// TOF.0.IN = ~IN.0.2
		"LD %A000\n"
		"ST %P0001\n"	// TOF.0.PT = NVRAM[0]
		"LD %P000/0\n"
		"ST %Q001/5\n"	// OUT.1.5 = TOF.0.Q
		"LD %I000/4\n"
		"STN %X000/0";  // CONTROL.0.0 = IN.0.4
	initMem();
#endif

#if 1
	//------- teste CTUD Up
	char in[] =
		"LD %I000/3\n"
		"STN %E001/0\n"	// CTUD.0.R = ~IN.0.3
		"LD %I000/2\n"
		"STN %E000/0\n"	// CTUD.0.CU = ~IN.0.2
		"LD %A000\n"
		"ST %E002\n"	// CTUD.0.PV = NVRAM[0]
		"LD %E000/1\n"
		"ST %Q001/1\n"	// OUT.1.1 (9) = CTUD.0.QD
		"LD %E000/0\n"
		"ST %Q001/2\n"	// OUT.1.2 (10) = CTUD.0.QU
		"LD %I000/4\n"
		"STN %X000/0";  // CONTROL.0.0 = IN.0.4
	initMem();
#endif

#if 0
	char in[] = "LD %I000/0\nAND( %I000/1\nOR( %I000/2\nAND! %I000/3\n)\n)\nST %Q001/0\nLD %I000/7\nST %X000/0";
	initMem();
	setMemInt(0,0x03);
#endif
#if 0
	char in[] =
		"LD %I000/2\n" //
		"ST %R000/0\n" // RS.0.S = IN.0.2
		"LD %I000/4\n"
		"ST %R000/1\n" // RS.0.R1 = IN.0.4
		"LD %R000/0\n"
		"ST %Q001/5\n"; // OUT.1.5 = RS.0.Q1
	initMem();
#endif
#if 0
	char in[] =
		"LD %I000/2\n" //
		"ST %S000/0\n" // SR.0.S1 = IN.0.2
		"LD %I000/4\n"
		"ST %S000/1\n" // SR.0.R = IN.0.4
		"LD %S000/0\n"
		"ST %Q001/5\n"; // OUT.1.5 = SR.0.Q1
	initMem();
#endif
	ParseIL(in, &program[0]);
	unsigned char dump[255];
	memcpy(&dump[0], &program[0], 64);
	int i;
	for (i = 0; i < 64; i++) {
		printf("%d\n", dump[i]);
	}
//	ilRunForever(program);
	return EXIT_SUCCESS;
}
