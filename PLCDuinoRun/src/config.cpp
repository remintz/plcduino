#include "util.h"

//--- bit 1 - input
//--- bit 0 - output

//const WORD ioMask =   0b0011111111111100;
const WORD ioMask =   0b0010000000000000;
const WORD ioConfig = 0b0000000001111100;

const WORD getIOConfig(void) {
	return ioConfig;
}

const WORD getIOMask(void) {
	return ioMask;
}
