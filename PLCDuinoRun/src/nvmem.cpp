#include <WProgram.h>
#include "EEPROM.h"
#include "errmsg.h"
#include "util.h"

#define NVMEM_SIZE				512
#define	PROGRAM_SIZE			200
#define GENERAL_SIZE			10

#define	ADDR_GENERAL	0
#define ADDR_IOMASK		10	// ADDR_GENERAL + GENERAL_SIZE
#define ADDR_IOCONFIG	12
#define	ADDR_PROGRAM	20

void setNVMem(WORD addr, unsigned char b) {
	EEPROM.write(addr, b);
}

void setNVMemBit(WORD addr, unsigned char bit, unsigned char b) {
	unsigned char tmp;
	b = b & 0x01;
	if ((bit < 0) || (bit > 7))
		doAbort(MSG_ILLEGAL_BIT);
	if ((addr < 0) || (addr >= NVMEM_SIZE))
		doAbort(MSG_ILLEGAL_NVMEM_ADDRESS);
	tmp = EEPROM.read(addr);
	modBit(&tmp, bit, b);
	EEPROM.write(addr, tmp);
}

unsigned char getNVMem(WORD addr) {
	return EEPROM.read(addr);
}

unsigned char getNVMemBit(WORD addr, unsigned char bit) {
	unsigned char tmp;
	if ((bit < 0) || (bit > 7))
		doAbort(MSG_ILLEGAL_BIT);
	if ((addr < 0) || (addr >= NVMEM_SIZE))
		doAbort(MSG_ILLEGAL_NVMEM_ADDRESS);
	tmp = EEPROM.read(addr);
	return getBit(tmp, bit);
}

void setProgByte(WORD addr, unsigned char b) {
	setNVMem(addr + ADDR_PROGRAM, b);
}

unsigned char getProgByte(WORD addr) {
	return getNVMem(addr + ADDR_PROGRAM);
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
