/*
 * ILRun.c
 *
 *  Created on: 12/10/2009
 *      Author: Renato
 */

#include <stdio.h>
#include <stdlib.h>

#include "util.h"
//#include "ILRun.h"
#include "ilcode.h"

const char ERR_OPSTACK_OVERFLOW[] = "Op Stk Ovflw";
const char ERR_OPSTACK_UNDERFLOW[] = "Op Stk Unflw";
const char ERR_VALSTACK_OVERFLOW[] = "Val Stk Ovflw";
const char ERR_VALSTACK_UNDERFLOW[] = "Val Stk Unflw";
const char ERR_ILLEGAL_INSTRUCTION[] = "Ill Instruct";
const char ERR_ILLEGAL_MEMORY_ADDRESS[] = "Ill Mem Addr";

#define	MAX_STACK	32
#define	MEM_SIZE	10

int memory[MEM_SIZE];
int opStack[MAX_STACK];
int valStack[MAX_STACK];
int opSP = 0;
int valSP = 0;

void initMem() {
	int i;
	for (i = 0; i < MEM_SIZE; i++) {
		memory[i] = 0;
	}
}

void setMemBit(int addr, int bit, int val) {
	if ((bit < 0) || (bit > 7))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	modBit(&memory[addr], bit, val);
}

BOOL getMemBit(int addr, int bit) {
	if ((bit < 0) || (bit > 7))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	return getBit(memory[addr], bit);
}

void setMem(int addr, int val) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	memory[addr] = val;
}

int getMem(int addr) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	return memory[addr];
}

void dumpMem() {
	int i;
	for (i = 0; i < MEM_SIZE; i++) {
		printf("MEM[%d] = 0x%04X\n", i, memory[i]);
	}
}

void opPush(int op) {
	if (opSP >= MAX_STACK)
		doAbort(ERR_OPSTACK_OVERFLOW);
	opStack[opSP] = op;
	opSP++;
}

int opPop() {
	opSP--;
	if (opSP < 0)
		doAbort(ERR_OPSTACK_UNDERFLOW);
	return opStack[opSP];
}

int opTop() {
	if (opSP == 0)
		doAbort(ERR_OPSTACK_UNDERFLOW);
	return opStack[opSP-1];
}

void valPush(int val) {
	if (valSP >= MAX_STACK)
		doAbort(ERR_VALSTACK_OVERFLOW);
	valStack[valSP] = val;
	valSP++;
}

int valPop() {
	valSP--;
	if (valSP < 0)
		doAbort(ERR_VALSTACK_UNDERFLOW);
	return valStack[valSP];
}

int valTop() {
	if (valSP == 0)
		doAbort(ERR_OPSTACK_UNDERFLOW);
	return valStack[valSP-1];
}

void ilRun(Instruction *pInstructions) {
	unsigned int iCurInstruction = 0;
	Instruction curInstruction;
	int i;
	unsigned int operator;
	int val1;
	int val2;

	BOOL theEnd = FALSE;
	BOOL found = FALSE;

	while (!theEnd) {
		curInstruction = pInstructions[iCurInstruction];
		found = TRUE;
		switch (curInstruction.operation) {
		case IL_LD:
			if (curInstruction.bit == 64) {
				valPush(getMem(curInstruction.byte));
			}
			else {
				valPush(getMemBit(curInstruction.byte, curInstruction.bit));
			}
			break;
		case IL_ST:
			if (curInstruction.bit == 64) {
				setMem(curInstruction.byte, valTop());
			} else {
				setMemBit(curInstruction.byte, curInstruction.bit, valTop());
			}
			break;
		case IL_SET:
			if (curInstruction.bit == 64) {
				setMem(curInstruction.byte, 1);
			} else {
				setMemBit(curInstruction.byte, curInstruction.bit, 1);
			}
			break;
		case IL_RESET:
			if (curInstruction.bit == 64) {
				setMem(curInstruction.byte, 0);
			} else {
				setMemBit(curInstruction.byte, curInstruction.bit, 0);
			}
			break;
		case IL_CAL:
			break;
		case IL_RET:
			break;
		case IL_JMP:
			break;
		case IL_END:
			theEnd = TRUE;
			break;
		default:
			found = FALSE;
			break;
		}
		if (!found) {
			if (curInstruction.modifier == IL_PUSH) {
				if (curInstruction.bit == 64) {
					valPush(getMem(curInstruction.byte));
				} else {
					valPush(getMemBit(curInstruction.byte, curInstruction.bit));
				}
				opPush(curInstruction.operation);
			} else {
				if (curInstruction.operation == IL_POP) {
					operator = opPop();
					val1 = valPop();
					val2 = valPop();
				} else {
					operator = curInstruction.operation;
					val1 = valPop();
					if (curInstruction.bit == 64) {
						val2 = getMem(curInstruction.byte);
					} else {
						val2 = getMemBit(curInstruction.byte, curInstruction.bit);
					}
					if (curInstruction.modifier == IL_NEG) {
						val2 = !val2;
					}
				}
				switch (operator) {
				case IL_AND:
					valPush(val1 & val2);
					break;
				case IL_OR:
					valPush(val1 | val2);
					break;
				case IL_XOR:
					valPush(val1 ^ val2);
					break;
				case IL_ADD:
					valPush(val1 + val2);
					break;
				case IL_SUB:
					valPush(val1 - val2);
					break;
				case IL_MUL:
					valPush(val1 * val2);
					break;
				case IL_DIV:
					valPush(val1 / val2);
					break;
				case IL_GT:
					valPush(val1 > val2);
					break;
				case IL_GE:
					valPush(val1 >= val2);
					break;
				case IL_EQ:
					valPush(val1 == val2);
					break;
				case IL_NE:
					valPush(val1 != val2);
					break;
				case IL_LE:
					valPush(val1 <= val2);
					break;
				case IL_LT:
					valPush(val1 < val2);
					break;
				default:
					doAbort(ERR_ILLEGAL_INSTRUCTION);
					break;
				}
			}
		}
		iCurInstruction++;
	}
}
