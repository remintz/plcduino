#include <stdio.h>
#include <WProgram.h>
#include "util.h"
#include "memory.h"
#include "errmsg.h"
#include "nvmem.h"

#define	MAX_STACK				10

unsigned char memory[MEM_SIZE];
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

unsigned char *getPMem(int addr) {
	return &memory[addr];
}

void setMemBit(int addr, int bit, WORD val) {
	val = val & 0x0001;
	if (isDebugOn()) {
		Serial.print("setMemBit(");
		Serial.print(addr,DEC);
		Serial.print(",");
		Serial.print(bit,DEC);
		Serial.print(",");
		Serial.print(val,DEC);
		Serial.println(")");
	}
	if ((bit < 0) || (bit > 7))
		doAbort(MSG_ILLEGAL_BIT);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	modBit(&memory[addr], bit, val);
}

unsigned char getMemBit(int addr, int bit) {
	unsigned char result;
	if ((bit < 0) || (bit > 7))
		doAbort(MSG_ILLEGAL_BIT);
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	result = getBit(memory[addr], bit);
	if (isDebugOn()) {
		Serial.print("getMemBit(");
		Serial.print(addr, DEC);
		Serial.print(",");
		Serial.print(bit, DEC);
		Serial.print("):");
		Serial.println(result, DEC);
	}
	return result;
}

void setMemInt(int addr, unsigned char val) {
	if (isDebugOn()) {
		Serial.print("setMemInt(");
		Serial.print(addr,DEC);
		Serial.print(",");
		Serial.print(val, DEC);
		Serial.println(")");
	}
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	memory[addr] = val;
}

unsigned char getMemInt(int addr) {
	if ((addr < 0) || (addr >= MEM_SIZE))
		doAbort(MSG_ILLEGAL_MEMORY_ADDRESS);
	if (isDebugOn()) {
		Serial.print("getMemInt(");
		Serial.print(addr,DEC);
		Serial.print("):");
		Serial.println(memory[addr], DEC);
	}
	return memory[addr];
}

void setMem(unsigned char operand, int addr, int bit, unsigned char val) {
	if (isDebugOn()) {
		Serial.print("setMem: operand=");
		Serial.print(operand);
		Serial.print(" addr=");
		Serial.print(addr,DEC);
		Serial.print(" bit=");
		Serial.print(bit, DEC);
		Serial.print(" val=");
		Serial.println(val, DEC);
	}
	if (operand == 'A') {
		//--- NVRAM
		if (bit == 64)
			setNVMem(addr, val);
		else
			setNVMemBit(addr, bit, val);
	}
	else {
		unsigned char offset;
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
	//	case 'C':
	//		offset = MEM_OFFSET_CTU_IN;
	//		break;
	//	case 'D':
	//		offset = MEM_OFFSET_CTD_IN;
	//		break;
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
}

unsigned char getMem(unsigned char operand, int addr, int bit) {
	if (isDebugOn()) {
		Serial.print("getMem: operand=");
		Serial.print(operand);
		Serial.print(" addr=");
		Serial.print(addr,DEC);
		Serial.print(" bit=");
		Serial.println(bit, DEC);
	}
	if (operand == 'A') {
		//--- NVRAM
		if (bit == 64)
			return getNVMem(addr);
		else
			return getNVMemBit(addr, bit);
	}
	else {
		unsigned char offset;
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
			//	case 'C':
			//		offset = MEM_OFFSET_CTU_OUT;
			//		break;
			//	case 'D':
			//		offset = MEM_OFFSET_CTD_OUT;
			//		break;
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
}

unsigned char getControl() {
	return getMem('X', 0, 64);
}

BOOL isDebugOn() {
	return false;
//	return ((getControl() & 0x02) != 0);
}

BOOL isAbortOn() {
	return ((getControl() & 0x01) != 0);
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
