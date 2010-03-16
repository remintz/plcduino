/*
 * util.c
 *
 *  Created on: 12/10/2009
 *      Author: Renato
 */

#include "util.h"

WORD getBit(int by, int bit) {
	return by >> bit & 0x01;
}

WORD setBit(WORD *by, int bit) {
	*by = *by | (0x01 << bit);
	return *by;
}

WORD resetBit(WORD *by, int bit) {

	WORD mask;

	mask = ~(0x01 << bit);
	*by = *by & mask;
	return *by;
}

WORD modBit(WORD *by, int bit, BOOL value) {
	if (value) {
		return setBit(by, bit);
	} else {
		return resetBit(by, bit);
	}
}
