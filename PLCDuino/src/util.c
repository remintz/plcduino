/*
 * util.c
 *
 *  Created on: 12/10/2009
 *      Author: Renato
 */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void doAbort(const char *msg) {
	printf("ABORT:%s/n", msg);
	exit(EXIT_FAILURE);
}

BOOL getBit(BYTE by, int bit) {
	return by >> bit & 0x01;
}

BYTE setBit(BYTE *by, int bit) {
	*by = *by | (0x01 << bit);
	return *by;
}

BYTE resetBit(BYTE *by, int bit) {
	*by = *by & !(0x01 << bit);
	return *by;
}

BYTE modBit(BYTE *by, int bit, BOOL value) {
	if (value) {
		return setBit(by, bit);
	} else {
		return resetBit(by, bit);
	}
}
