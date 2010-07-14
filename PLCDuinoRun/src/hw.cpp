#include <WProgram.h>
#include "util.h"
#include "nvmem.h"

void initHw() {
	WORD ioMask;
	WORD ioConfig;
	WORD ioNeg;
	int i;

	ioMask = getIOMask();
	ioConfig = getIOConfig();
	ioNeg = getIONeg();
	for (i = 0; i < 16; i++) {
		Serial.print(i, DEC);
		if (ioMask & 0x01) {
			if (ioConfig & 0x01) {
				pinMode(i, INPUT);
				//--- ativa resistor pull-up interno
				digitalWrite(i, HIGH);
			}
			else {
				pinMode(i, OUTPUT);
			}
			if (ioNeg & 0x01) {
				Serial.print('N');
			}
		}
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
		ioNeg = ioNeg >> 1;
	}
}

void readInputs(unsigned char *p_Memory) {
//	Serial.print("readInputs\r\n");
	unsigned char in;
	unsigned char ioMask;
	unsigned char ioConfig;
	unsigned char ioNeg;
	unsigned char onMask;
	int tmpBit;
	int i;

	//--- LSB
	in = 0;
	onMask = 0x01;
	ioMask = LSB(getIOMask());
	ioConfig = LSB(getIOConfig());
	ioNeg = LSB(getIONeg());
	for (i = 0; i < 8; i++) {
		if ((ioMask & ioConfig & 0x01) == 1) {
//			Serial.print("Leu ");
//			Serial.print(i);
//			Serial.print('\r\n');
			tmpBit = digitalRead(i);
			if (ioNeg & 0x01)
				tmpBit = (~tmpBit) & 0x01;
			if ((tmpBit & 0x01) == 1) {
				in = in | onMask;
			}
		}
		onMask = onMask << 1;
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
		ioNeg = ioNeg >> 1;
	}
	memcpy(p_Memory, &in, 1);
//	if (in != lastInput) {
//		Serial.print("In LSB:");
//		Serial.println(in, DEC);
//		lastInput = in;
//	}
	//--- MSB
	in = 0;
	onMask = 0x01;
	ioMask = MSB(getIOMask());
	ioConfig = MSB(getIOConfig());
	ioNeg = MSB(getIONeg());
	for (i = 8; i < 16; i++) {
		if ((ioMask & ioConfig & 0x01) == 1) {
//			Serial.print("Leu ");
//			Serial.print(i);
//			Serial.print('\r\n');
			tmpBit = digitalRead(i);
			if (ioNeg & 0x01)
				tmpBit = (~tmpBit) & 0x01;
			if ((tmpBit & 0x01) == 1) {
				in = in | onMask;
			}
		}
		onMask = onMask << 1;
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
		ioNeg = ioNeg >> 1;
	}
	memcpy(p_Memory+1, &in, 1);
//	if (in != lastInput) {
//		Serial.print("In MSB:");
//		Serial.println(in, DEC);
//		lastInput = in;
//	}
}

void writeOutputs(unsigned char *p_Memory) {
	unsigned char out;
	unsigned char ioMask;
	unsigned char ioConfig;
	unsigned char ioNeg;
	int i;
	int bitRef;

	//--- lsb
	memcpy(&out, p_Memory, 1);
//	if (out != lastOutput) {
//		Serial.print("out lsb:");
//		Serial.println(out,DEC);
//		lastOutput = out;
//	}
	ioMask = LSB(getIOMask());
	ioConfig = ~LSB(getIOConfig());
	ioNeg = LSB(getIONeg());
	for (i = 0; i < 8; i++) {
		if ((ioMask & ioConfig & 0x01) == 1) {
			bitRef = 1;
			if (ioNeg & 0x01)
				bitRef = 0;
			if ((out & 0x01) == bitRef) {
				digitalWrite(i, HIGH);
			} else {
				digitalWrite(i, LOW);
			}
		}
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
		ioNeg = ioNeg >> 1;
		out = out >> 1;
	}
	//--- MSB
	memcpy(&out, p_Memory+1, 1);
//	if (out != lastOutput) {
//		Serial.print("out msb:");
//		Serial.println(out,DEC);
//		lastOutput = out;
//	}
	ioMask = MSB(getIOMask());
	ioConfig = ~MSB(getIOConfig());
	ioNeg = MSB(getIONeg());
	for (i = 8; i < 16; i++) {
		if ((ioMask & ioConfig & 0x01) == 1) {
			bitRef = 1;
			if (ioNeg & 0x01)
				bitRef = 0;
			if ((out & 0x01) == bitRef) {
				digitalWrite(i, HIGH);
			} else {
				digitalWrite(i, LOW);
			}
		}
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
		ioNeg = ioNeg >> 1;
		out = out >> 1;
	}
}
