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
WORD lastInput;
WORD lastOutput;

void initHw() {
	#ifdef ARDUINO
		Serial.print("initHw\r\n");
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
		//Serial.print(".\r");
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
//	Serial.print("readInputs\r\n");
	WORD in;
	WORD ioMask;
	WORD ioConfig;
	WORD onMask;
	int i;

	in = 0;
	onMask = 0x01;
	ioMask = getIOMask();
	ioConfig = getIOConfig();
//	Serial.print("ioConfig: ");
//	Serial.print(ioConfig);
//	Serial.print(" ioMask: ");
//	Serial.print(ioMask);
//	Serial.print("\r\n");
	for (i = 0; i < 16; i++) {
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
	memcpy(p_Memory, &in, sizeof(WORD));
	if (in != lastInput) {
		Serial.print("In:");
		Serial.print(in);
		Serial.print("\r\n");
		lastInput = in;
	}
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

void writeOutputs(WORD *p_Memory) {
#ifdef ARDUINO
//	Serial.print("writeOutputs\r\n");
	WORD out;
	WORD ioMask;
	WORD ioConfig;
	int i;

	memcpy(&out, p_Memory, sizeof(WORD));
	if (out != lastOutput) {
		Serial.print("out=");
		Serial.print(out);
		Serial.print('\r\n');
		lastOutput = out;
	}
	ioMask = getIOMask();
	ioConfig = ~getIOConfig();
	for (i = 0; i < 16; i++) {
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
