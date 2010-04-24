#include <WProgram.h>
#include "EEPROM.h"
#include "ilcode.h"
#include "util.h"

#define ADDR_IOMASK		0
#define ADDR_IOCONFIG	2
#define	ADDR_PROGRAM	10

#define	MAX_PROGRAM_SIZE		200

void setNVMem(WORD addr, unsigned char b) {
	EEPROM.write(addr, b);
}

unsigned char getNVMem(WORD addr) {
	return EEPROM.read(addr);
}

void setProgByte(WORD addr, unsigned char b) {
	setNVMem(addr + ADDR_PROGRAM, b);
}

unsigned char getProgByte(WORD addr) {
	return getNVMem(addr + ADDR_PROGRAM);
}

void setInstruction(WORD instructionPtr, Instruction instr) {
	WORD memAddr = (instructionPtr * sizeof(Instruction));
	setProgByte(memAddr, instr.operation);
	setProgByte(memAddr+1, instr.operand);
	setProgByte(memAddr+2, instr.modifier);
	setProgByte(memAddr+3, instr.byte);
	setProgByte(memAddr+4, instr.bit);
}

void getInstruction(WORD instructionPtr, Instruction *instr) {
	WORD memAddr = (instructionPtr * sizeof(Instruction));
	instr->operation = getProgByte(memAddr);
	instr->operand = getProgByte(memAddr+1);
	instr->modifier = getProgByte(memAddr+2);
	instr->byte = getProgByte(memAddr+3);
	instr->bit = getProgByte(memAddr+4);
}

WORD getIOConfig(void) {
	return getNVMem(ADDR_IOCONFIG) + getNVMem(ADDR_IOCONFIG + 1) * 256;
}

WORD getIOMask(void) {
	return getNVMem(ADDR_IOMASK) + getNVMem(ADDR_IOMASK + 1) * 256;
}

void setIOMask(WORD val) {
	setNVMem(ADDR_IOMASK, LSB(val));
	setNVMem(ADDR_IOMASK+1, MSB(val));
}

void setIOConfig(WORD val) {
	setNVMem(ADDR_IOCONFIG, LSB(val));
	setNVMem(ADDR_IOCONFIG+1, MSB(val));
}

void printIOSetup() {
	WORD ioMask;
	WORD ioConfig;
	int i;
	ioConfig = getIOConfig();
	ioMask = getIOMask();
	Serial.println("IO Setup:");
	for (i = 0; i < 16; i++) {
		Serial.print(i, DEC);
		if (ioMask & 0x0001) {
			if (ioConfig & 0x0001) {
				Serial.print("I");
			}
			else {
				Serial.print("O");
			}
		}
		Serial.println();
	}
}
