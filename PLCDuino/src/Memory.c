/*
 * Memory.c
 *
 *  Created on: 15/03/2010
 *      Author: Renato
 */

//--- memory map

#include <stdio.h>
#include "util.h"
#include "Memory.h"
#include "ErrMsg.h"

#define	MAX_STACK				32
#define	MEM_SIZE				256

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

void initStack() {
	opSP = 0;
	valSP = 0;

}
void setMemBit(int addr, int bit, WORD val) {
	if ((bit < 0) || (bit > 7))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	modBit(&memory[addr], bit, val);
}

void *getPMem(int addr) {
	return &memory[addr];
}

WORD getMemBit(int addr, int bit) {
	if ((bit < 0) || (bit > 7))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	return getBit(memory[addr], bit);
}

void setMemInt(int addr, WORD val) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	memory[addr] = val;
}

WORD getMemInt(int addr) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	return memory[addr];
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
		doAbort(MSG_OPSTACK_OVERFLOW);
	opStack[opSP] = op;
	opSP++;
}

WORD opPop() {
	opSP--;
	if (opSP < 0)
		doAbort(MSG_OPSTACK_UNDERFLOW);
	return opStack[opSP];
}

WORD opTop() {
	if (opSP == 0)
		doAbort(MSG_OPSTACK_UNDERFLOW);
	return opStack[opSP-1];
}

void valPush(WORD val) {
	if (valSP >= MAX_STACK)
		doAbort(MSG_VALSTACK_OVERFLOW);
	valStack[valSP] = val;
	valSP++;
}

WORD valPop() {
	valSP--;
	if (valSP < 0)
		doAbort(MSG_VALSTACK_UNDERFLOW);
	return valStack[valSP];
}

WORD valTop() {
	if (valSP == 0)
		doAbort(MSG_OPSTACK_UNDERFLOW);
	return valStack[valSP-1];
}
