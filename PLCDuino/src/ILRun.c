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

#define	MAX_STACK			32
#define	MEM_SIZE			256
#define MEM_OFFSET_MEM		0
#define MEM_OFFSET_IN		32
#define MEM_OFFSET_OUT		32
#define MEM_OFFSET_RS		64
#define MEM_OFFSET_SR		64
#define MEM_OFFSET_TP		64
#define MEM_OFFSET_TON		64
#define MEM_OFFSET_TOF		64
#define MEM_OFFSET_CTU		64
#define MEM_OFFSET_CTD		64
#define MEM_OFFSET_CTUD		64
#define MEM_OFFSET_RTRIG	64
#define MEM_OFFSET_FTRIG	64

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

int getMemBit(int addr, int bit) {
	if ((bit < 0) || (bit > 7))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	return getBit(memory[addr], bit);
}

void setMemInt(int addr, int val) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	memory[addr] = val;
}

int getMemInt(int addr) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	return memory[addr];
}

void setMem(BYTE operand, int addr, int bit, int val) {
	int offset;
	offset = 0;
	switch (operand) {
	case 'I':
		offset = MEM_OFFSET_IN;
		break;
	case 'Q':
		offset = MEM_OFFSET_OUT;
		break;
	case 'M':
		offset = MEM_OFFSET_MEM;
		break;
	case 'N':
		offset = MEM_OFFSET_TP;
		break;
	case 'O':
		offset = MEM_OFFSET_TON;
		break;
	case 'P':
		offset = MEM_OFFSET_TOF;
		break;
	case 'C':
		offset = MEM_OFFSET_CTU;
		break;
	case 'D':
		offset = MEM_OFFSET_CTD;
		break;
	case 'E':
		offset = MEM_OFFSET_CTUD;
		break;
	case 'T':
		offset = MEM_OFFSET_RTRIG;
		break;
	case 'U':
		offset = MEM_OFFSET_FTRIG;
		break;
	case 'S':
		offset = MEM_OFFSET_SR;
		break;
	case 'R':
		offset = MEM_OFFSET_RS;
		break;
	}
	if (bit == 64) {
		setMemInt(addr + offset, val);
	}
	else {
		setMemBit(addr + offset, bit, val);
	}
}

int getMem(BYTE operand, int addr, int bit) {
	if (bit == 64)
		return getMemInt(addr);
	else
		return getMemBit(addr, bit);
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
			valPush(getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit));
			break;
		case IL_ST:
			setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, valTop());
			break;
		case IL_LDN:
			valPush(!getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit));
		case IL_STN:
			setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, !valTop());
			break;
		case IL_SET:
			setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, 1);
			break;
		case IL_RESET:
			setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, 0);
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
				valPush(getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit));
				opPush(curInstruction.operation);
			} else {
				if (curInstruction.operation == IL_POP) {
					operator = opPop();
					val1 = valPop();
					val2 = valPop();
				} else {
					operator = curInstruction.operation;
					val1 = valPop();
					val2 = getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
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
				case IL_ANDN:
					valPush(val1 & (!val2));
					break;
				case IL_ORN:
					valPush(val1 | (!val2));
					break;
				case IL_XORN:
					valPush(val1 ^ (!val2));
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
