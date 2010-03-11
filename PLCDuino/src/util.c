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

int getBit(int by, int bit) {
	return by >> bit & 0x01;
}

int setBit(int *by, int bit) {
	*by = *by | (0x01 << bit);
	return *by;
}

int resetBit(int *by, int bit) {
	*by = *by & !(0x01 << bit);
	return *by;
}

int modBit(int *by, int bit, BOOL value) {
	if (value) {
		return setBit(by, bit);
	} else {
		return resetBit(by, bit);
	}
}
