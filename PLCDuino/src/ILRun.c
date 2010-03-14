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
#include "FunctionBlocks.h"
#include "Hw.h"

const char ERR_OPSTACK_OVERFLOW[] = "Op Stk Ovflw";
const char ERR_OPSTACK_UNDERFLOW[] = "Op Stk Unflw";
const char ERR_VALSTACK_OVERFLOW[] = "Val Stk Ovflw";
const char ERR_VALSTACK_UNDERFLOW[] = "Val Stk Unflw";
const char ERR_ILLEGAL_INSTRUCTION[] = "Ill Instruct";
const char ERR_ILLEGAL_MEMORY_ADDRESS[] = "Ill Mem Addr";

//--- memory map

#define	MAX_STACK				32
#define	MEM_SIZE				256
#define MEM_OFFSET_MEM			0
#define MEM_OFFSET_IN			32
#define MEM_OFFSET_RS_IN		64
#define MEM_OFFSET_SR_IN		65
#define MEM_OFFSET_TP_IN		66
#define MEM_OFFSET_TON_IN		0
#define MEM_OFFSET_TOF_IN		0
#define MEM_OFFSET_CTU_IN		0
#define MEM_OFFSET_CTD_IN		0
#define MEM_OFFSET_CTUD_IN		0
#define MEM_OFFSET_RTRIG_IN		0
#define MEM_OFFSET_FTRIG_IN		0
#define MEM_OFFSET_OUT			126
#define MEM_OFFSET_RS_OUT		158
#define MEM_OFFSET_SR_OUT		159
#define MEM_OFFSET_TP_OUT		0
#define MEM_OFFSET_TON_OUT		0
#define MEM_OFFSET_TOF_OUT		0
#define MEM_OFFSET_CTU_OUT		0
#define MEM_OFFSET_CTD_OUT		0
#define MEM_OFFSET_CTUD_OUT		0
#define MEM_OFFSET_RTRIG_OUT	0
#define MEM_OFFSET_FTRIG_OUT	0
#define MEM_OFFSET_CONTROL		160

WORD memory[MEM_SIZE];
WORD opStack[MAX_STACK];
WORD valStack[MAX_STACK];
int opSP;
int valSP;

void initMem() {
	int i;
	for (i = 0; i < MEM_SIZE; i++) {
		memory[i] = 0;
	}
}

void setMemBit(int addr, int bit, WORD val) {
	if ((bit < 0) || (bit > 7))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	modBit(&memory[addr], bit, val);
}

WORD getMemBit(int addr, int bit) {
	if ((bit < 0) || (bit > 7))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	return getBit(memory[addr], bit);
}

void setMemInt(int addr, WORD val) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	memory[addr] = val;
}

WORD getMemInt(int addr) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(ERR_ILLEGAL_MEMORY_ADDRESS);
	return memory[addr];
}

int offsetOperand(BYTE operand) {
	int offset;
	return offset;
}

void setMem(BYTE operand, int addr, int bit, WORD val) {
	int offset;
	offset = 0;
	switch (operand) {
	case 'Q':
		offset = MEM_OFFSET_OUT;
		break;
	case 'M':
		offset = MEM_OFFSET_MEM;
		break;
	case 'N':
		offset = MEM_OFFSET_TP_IN;
		break;
	case 'O':
		offset = MEM_OFFSET_TON_IN;
		break;
	case 'P':
		offset = MEM_OFFSET_TOF_IN;
		break;
	case 'C':
		offset = MEM_OFFSET_CTU_IN;
		break;
	case 'D':
		offset = MEM_OFFSET_CTD_IN;
		break;
	case 'E':
		offset = MEM_OFFSET_CTUD_IN;
		break;
	case 'T':
		offset = MEM_OFFSET_RTRIG_IN;
		break;
	case 'U':
		offset = MEM_OFFSET_FTRIG_IN;
		break;
	case 'S':
		offset = MEM_OFFSET_SR_IN;
		break;
	case 'R':
		offset = MEM_OFFSET_RS_IN;
		break;
	case 'X':
		offset = MEM_OFFSET_CONTROL;
		break;
	}
	if (bit == 64) {
		setMemInt(addr + offset, val);
	}
	else {
		setMemBit(addr + offset, bit, val);
	}
}

WORD getMem(BYTE operand, int addr, int bit) {
	int offset;
	offset = 0;
	switch (operand) {
	case 'I':
		offset = MEM_OFFSET_IN;
		break;
	case 'M':
		offset = MEM_OFFSET_MEM;
		break;
	case 'N':
		offset = MEM_OFFSET_TP_OUT;
		break;
	case 'O':
		offset = MEM_OFFSET_TON_OUT;
		break;
	case 'P':
		offset = MEM_OFFSET_TOF_OUT;
		break;
	case 'C':
		offset = MEM_OFFSET_CTU_OUT;
		break;
	case 'D':
		offset = MEM_OFFSET_CTD_OUT;
		break;
	case 'E':
		offset = MEM_OFFSET_CTUD_OUT;
		break;
	case 'T':
		offset = MEM_OFFSET_RTRIG_OUT;
		break;
	case 'U':
		offset = MEM_OFFSET_FTRIG_OUT;
		break;
	case 'S':
		offset = MEM_OFFSET_SR_OUT;
		break;
	case 'R':
		offset = MEM_OFFSET_RS_OUT;
		break;
	case 'X':
		offset = MEM_OFFSET_CONTROL;
		break;
	}
	if (bit == 64)
		return getMemInt(addr + offset);
	else
		return getMemBit(addr + offset, bit);
}

void dumpMem(int begin, int end) {
	int i;
	if (end >= MEM_SIZE)
		end = MEM_SIZE - 1;
	for (i = begin; i <= end; i++) {
		printf("MEM[%d] = 0x%04X\n", i, memory[i]);
	}
}

void opPush(WORD op) {
	if (opSP >= MAX_STACK)
		doAbort(ERR_OPSTACK_OVERFLOW);
	opStack[opSP] = op;
	opSP++;
}

WORD opPop() {
	opSP--;
	if (opSP < 0)
		doAbort(ERR_OPSTACK_UNDERFLOW);
	return opStack[opSP];
}

WORD opTop() {
	if (opSP == 0)
		doAbort(ERR_OPSTACK_UNDERFLOW);
	return opStack[opSP-1];
}

void valPush(WORD val) {
	if (valSP >= MAX_STACK)
		doAbort(ERR_VALSTACK_OVERFLOW);
	valStack[valSP] = val;
	valSP++;
}

WORD valPop() {
	valSP--;
	if (valSP < 0)
		doAbort(ERR_VALSTACK_UNDERFLOW);
	return valStack[valSP];
}

WORD valTop() {
	if (valSP == 0)
		doAbort(ERR_OPSTACK_UNDERFLOW);
	return valStack[valSP-1];
}

BOOL ctrlAbort() {
	return (memory[MEM_OFFSET_CONTROL] & 0x01);
}

BOOL ilRun(Instruction *pInstructions) {
	unsigned int iCurInstruction = 0;
	Instruction curInstruction;
	WORD operator;
	WORD curResult = 0;
	WORD val1;
	WORD val2;

	BOOL theEnd = FALSE;
	BOOL abort = FALSE;
	BOOL found = FALSE;

	opSP = 0;
	valSP = 0;

	while (!theEnd) {
		curInstruction = pInstructions[iCurInstruction];
		found = TRUE;
		switch (curInstruction.operation) {
		case IL_LD:
			curResult = getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
			break;
		case IL_ST:
			setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, curResult);
			break;
		case IL_LDN:
			curResult = ~getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
		case IL_STN:
			setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, ~curResult);
			break;
		case IL_SET:
			if (curResult == 1) {
				setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, 1);
			}
			break;
		case IL_RESET:
			if (curResult == 1) {
				setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, 0);
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
				valPush(curResult);
				curResult = getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
				opPush(curInstruction.operation);
			} else {
				if (curInstruction.operation == IL_POP) {
					operator = opPop();
					val1 = valPop();
					val2 = curResult;
				} else {
					operator = curInstruction.operation;
					val1 = curResult;
					val2 = getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
					if (curInstruction.modifier == IL_NEG) {
						val2 = ~val2;
					}
				}
				switch (operator) {
				case IL_AND:
					curResult = val1 & val2;
					break;
				case IL_OR:
					curResult = val1 | val2;
					break;
				case IL_XOR:
					curResult = val1 ^ val2;
					break;
				case IL_ANDN:
					curResult = val1 & (~val2);
					break;
				case IL_ORN:
					curResult = val1 | (~val2);
					break;
				case IL_XORN:
					curResult = val1 ^ (~val2);
					break;
				case IL_ADD:
					curResult = val1 + val2;
					break;
				case IL_SUB:
					curResult = val1 - val2;
					break;
				case IL_MUL:
					curResult = val1 * val2;
					break;
				case IL_DIV:
					curResult = val1 / val2;
					break;
				case IL_GT:
					curResult = val1 > val2;
					break;
				case IL_GE:
					curResult = val1 >= val2;
					break;
				case IL_EQ:
					curResult = val1 == val2;
					break;
				case IL_NE:
					curResult = val1 != val2;
					break;
				case IL_LE:
					curResult = val1 <= val2;
					break;
				case IL_LT:
					curResult = val1 < val2;
					break;
				default:
					doAbort(ERR_ILLEGAL_INSTRUCTION);
					break;
				}
			}
		}
		iCurInstruction++;
	}
	return abort;
}
void ilRunForever(Instruction *pInstructions) {
	BOOL theEnd = FALSE;
	while (!theEnd) {
		readInputs(&memory[MEM_OFFSET_IN]);
		if (ilRun(pInstructions)) {
			//--- abort instruction executed
			break;
		}
		//dumpMem(0,8);
		//dumpMem(126,132);
		if (ctrlAbort()) {
			break;
		}
		SRTick((WORD *)&memory[MEM_OFFSET_SR_IN], (WORD *)&memory[MEM_OFFSET_SR_OUT]);
		RSTick((WORD *)&memory[MEM_OFFSET_RS_IN], (WORD *)&memory[MEM_OFFSET_RS_OUT]);
		writeOutputs(&memory[MEM_OFFSET_OUT]);
	}
}

