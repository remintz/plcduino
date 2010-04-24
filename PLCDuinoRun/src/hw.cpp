#ifdef ARDUINO

	#include <WProgram.h>
	#include "config.h"
	#define LEDPIN	13    // LED connected to digital pin 13
#else

	#include <conio.h>
	#include <stdio.h>

#endif
#include "util.h"

#define WDT_LED_CICLE	2000

int wdtLed;

//WORD lastInput;
//WORD lastOutput;

void initHw() {
	#ifdef ARDUINO
		Serial.println("initHw");
		WORD ioMask;
		WORD ioConfig;
		int i;

		ioMask = getIOMask();
		ioConfig = getIOConfig();
		Serial.println("Config:");
		for (i = 0; i < 16; i++) {
			Serial.print(i, DEC);
			if (ioMask & 0x01) {
				if (ioConfig & 0x01) {
					pinMode(i, INPUT);
					//--- ativa resistor pull-up interno
					digitalWrite(i, HIGH);
					Serial.print('I');
				}
				else {
					pinMode(i, OUTPUT);
					Serial.print('O');
				}
			}
			ioMask = ioMask >> 1;
			ioConfig = ioConfig >> 1;
			Serial.println();
		}
//		pinMode(LEDPIN, OUTPUT);
		wdtLed = WDT_LED_CICLE;
	#endif
}
void kickWDT() {
	#ifdef ARDUINO
//		Serial.print(".");
//		if (wdtLed > 0) {
//			digitalWrite(LEDPIN, HIGH);
//		}
//		else {
//			digitalWrite(LEDPIN, LOW);
//		}
//		wdtLed--;
//		if (wdtLed < -WDT_LED_CICLE) {
//			wdtLed = WDT_LED_CICLE;
//		}
	#endif
}

void readInputs(unsigned char *p_Memory) {
#ifdef ARDUINO
//	Serial.print("readInputs\r\n");
	unsigned char in;
	unsigned char ioMask;
	unsigned char ioConfig;
	unsigned char onMask;
	int i;

	//--- LSB
	in = 0;
	onMask = 0x01;
	ioMask = LSB(getIOMask());
	ioConfig = LSB(getIOConfig());
	for (i = 0; i < 8; i++) {
		if ((ioMask & ioConfig & 0x01) == 1) {
//			Serial.print("Leu ");
//			Serial.print(i);
//			Serial.print('\r\n');
			if ((digitalRead(i) & 0x01) == 1) {
				in = in | onMask;
			}
		}
		onMask = onMask << 1;
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
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
	for (i = 8; i < 16; i++) {
		if ((ioMask & ioConfig & 0x01) == 1) {
//			Serial.print("Leu ");
//			Serial.print(i);
//			Serial.print('\r\n');
			if ((digitalRead(i) & 0x01) == 1) {
				in = in | onMask;
			}
		}
		onMask = onMask << 1;
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
	}
	memcpy(p_Memory+1, &in, 1);
//	if (in != lastInput) {
//		Serial.print("In MSB:");
//		Serial.println(in, DEC);
//		lastInput = in;
//	}
#else
	WORD c;
	printf("<");
	fflush(stdout);
	scanf("%d", &c);
	printf("Lido: 0x%08X\r\n",c);
	fflush(stdout);
	p_Memory[0] = c;
#endif
}

void writeOutputs(unsigned char *p_Memory) {
#ifdef ARDUINO
	unsigned char out;
	unsigned char ioMask;
	unsigned char ioConfig;
	int i;

	//--- lsb
	memcpy(&out, p_Memory, 1);
//	if (out != lastOutput) {
//		Serial.print("out lsb:");
//		Serial.println(out,DEC);
//		lastOutput = out;
//	}
	ioMask = LSB(getIOMask());
	ioConfig = ~LSB(getIOConfig());
	for (i = 0; i < 8; i++) {
		if ((ioMask & ioConfig & 0x01) == 1) {
			if ((out & 0x01) == 1) {
				digitalWrite(i, HIGH);
			} else {
				digitalWrite(i, LOW);
			}
		}
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
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
	for (i = 8; i < 16; i++) {
		if ((ioMask & ioConfig & 0x01) == 1) {
			if ((out & 0x01) == 1) {
				digitalWrite(i, HIGH);
			} else {
				digitalWrite(i, LOW);
			}
		}
		ioMask = ioMask >> 1;
		ioConfig = ioConfig >> 1;
		out = out >> 1;
	}

#else
	int i;
	printf("> ");
	for (i = 0; i < 8; i++)
		printf(" 0x%04X", p_Memory[i]);
	printf("\r\n");
	fflush(stdout);
#endif
}
