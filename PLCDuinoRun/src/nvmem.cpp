#include "EEPROM.h"
#include "ilcode.h"
#include "util.h"

#define	INITIAL_PROGRAM_ADDRESS	0
#define	MAX_PROGRAM_SIZE		100

void setInstruction(WORD addr, Instruction instr) {
	WORD memAddr = (addr * sizeof(Instruction)) + INITIAL_PROGRAM_ADDRESS;
	EEPROM.write(memAddr, instr.operation);
	EEPROM.write(memAddr+1, instr.operand);
	EEPROM.write(memAddr+2, instr.modifier);
	EEPROM.write(memAddr+3, instr.byte);
	EEPROM.write(memAddr+4, instr.bit);
}

void getInstruction(WORD addr, Instruction *instr) {
	WORD memAddr = (addr * sizeof(Instruction)) + INITIAL_PROGRAM_ADDRESS;
	instr->operation = EEPROM.read(memAddr);
	instr->operand = EEPROM.read(memAddr+1);
	instr->modifier = EEPROM.read(memAddr+2);
	instr->byte = EEPROM.read(memAddr+3);
	instr->bit = EEPROM.read(memAddr+4);
}

void setNVMem(WORD addr, unsigned char b) {
	EEPROM.write(addr, b);
}

unsigned char getNVMem(WORD addr) {
	return EEPROM.read(addr);
}
