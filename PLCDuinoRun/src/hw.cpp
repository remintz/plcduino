#ifdef ARDUINO

	#include <WProgram.h>
	#include "config.h"
	#define LEDPIN	13    // LED connected to digital pin 13

#else
	#include <conio.h>
	#include <stdio.h>
#endif
#include "util.h"

#define WDT_LED_CICLE	20
int wdtLed;
WORD lastInput;

void initHw() {
	#ifdef ARDUINO
		Serial.print("initHw\n");
		WORD ioMask;
		WORD ioConfig;
		ioMask = getIOMask();
		ioConfig = getIOConfig();
		int i;

		for (i = 0; i < 16; i++) {
			if (ioMask & 0x01) {
				if (ioConfig & 0x01) {
					pinMode(i, INPUT);
				}
				else {
					pinMode(i, OUTPUT);
				}
			}
			ioMask = ioMask >> 1;
			ioConfig = ioConfig >> 1;
		}
		pinMode(LEDPIN, OUTPUT);
		wdtLed = WDT_LED_CICLE;
	#endif
}
void kickWDT() {
	#ifdef ARDUINO
		Serial.print(".\b");
		if (wdtLed > 0) {
			digitalWrite(LEDPIN, HIGH);
		}
		else {
			digitalWrite(LEDPIN, LOW);
		}
		wdtLed--;
		if (wdtLed < -WDT_LED_CICLE) {
			wdtLed = WDT_LED_CICLE;
		}
	#endif
}

void readInputs(WORD *p_Memory) {
#ifdef ARDUINO
	WORD in;
	WORD ioMask;
	WORD ioConfig;
	int i;

	in = 0;
	ioMask = getIOMask();
	ioConfig = getIOConfig();
//	for (i = 15; i >= 0; i--) {
//		if ((ioMask & 0x80) && (ioConfig & 0x80)) {
//			in = (in << 1) | (digitalRead(i) & 0x01);
//		}
//		ioMask = ioMask << 1;
//		ioConfig = ioConfig << 1;
//	}
	memcpy(p_Memory, &in, sizeof(WORD));
	if (in != lastInput) {
		Serial.print(in);
		Serial.print('\n');
		lastInput = in;
	}
#else
	WORD c;
	printf("<");
	fflush(stdout);
	scanf("%d", &c);
	printf("Lido: 0x%08X\n",c);
	fflush(stdout);
	p_Memory[0] = c;
#endif
}

void writeOutputs(WORD *p_Memory) {
#ifndef ARDUINO
	int i;
	printf("> ");
	for (i = 0; i < 8; i++)
		printf(" 0x%04X", p_Memory[i]);
	printf("\n");
	fflush(stdout);
#endif
}
