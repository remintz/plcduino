/*
 ============================================================================
 Name        : PLCDuino.c
 Author      : Renato Mintz
 Version     :
 Copyright   : Renato Mintz
 Description : Hello World in C, Ansi-style
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
	char in[] = "LD %I000/0\nAND( %I000/1\nOR( %I000/2\nAND %I000/3\n)\n)\nST %O001/0";
	ParseIL(in, &program[0]);
	//initProgram();
	ilRun(program);
	dumpMem();
	return EXIT_SUCCESS;
}
