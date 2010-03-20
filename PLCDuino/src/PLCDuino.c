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
	char in[] = "LD %I000/7\nST %X000/0";
	initMem();
#endif
#if 0
	char in[] = "LD %I000/0\nAND( %I000/1\nOR( %I000/2\nAND! %I000/3\n)\n)\nST %Q001/0\nLD %I000/7\nST %X000/0";
	initMem();
	setMemInt(0,0x03);
#endif
#if 1
	char in[] =
		"LD %I000/0\n" //
		"ST %S000/0\n" // SR.0.S1 = IN.0.0
		"LD %I000/1\n"
		"ST %S000/1\n" // SR.0.R = IN.0.1
		"LD %I000/2\n"
		"ST %O000/0\n" // TON.0.IN = IN.0.2
		"LD %M000\n"
		"ST %O002\n"   // TON.0.PT = MEM.0
		"LD %I000/7\n"
		"ST %X000/0";  // CONTROL.0.0 = IN.0.7
	initMem();
	setMemInt(0,20);
#endif
	ParseIL(in, &program[0]);
	ilRunForever(program);
	return EXIT_SUCCESS;
}
