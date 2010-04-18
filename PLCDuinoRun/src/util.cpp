#include "util.h"

unsigned char getBit(unsigned char by, int bit) {
	return by >> bit & 0x01;
}

unsigned char setBit(unsigned char *by, int bit) {
	*by = *by | (0x01 << bit);
	return *by;
}

unsigned char resetBit(unsigned char *by, int bit) {

	unsigned char mask;

	mask = ~(0x01 << bit);
	*by = *by & mask;
	return *by;
}

unsigned char modBit(unsigned char *by, int bit, BOOL value) {
	if (value) {
		return setBit(by, bit);
	} else {
		return resetBit(by, bit);
	}
}

unsigned char MSB(WORD val) {
	return ((val & 0xFF00) >> 8);
}

unsigned char LSB(WORD val) {
	return (val & 0x00FF);
}

