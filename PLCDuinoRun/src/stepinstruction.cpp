#include <WProgram.h>

#include "hw.h"
#include "util.h"
#include "ilcode.h"
#include "nvmem.h"
#include "memory.h"
#include "Messenger.h"
#include "ilrun.h"

void printStatus() {
	Serial.println();
	printCurInstruction();
	Serial.println();
	Serial.print("RESULT: ");
	printCurResult();
	Serial.println();
}

void stepInstruction() {

	if (!isStep()) return;

	Messenger msg = Messenger();
	BOOL modoStep = true;
	unsigned char c;
	unsigned int i;
	unsigned int addr1 = 0;
	unsigned int addr2 = 0;

	//--- prompt
	printStatus();
	Serial.print("STEP>");
	while (modoStep) {
		//--- aguarda caracteres pela serial
		while (!Serial.available());
		c = Serial.read();
		Serial.print(c);
		msg.process(c);
		if (msg.available()) {
			//--- uma mensagem completa
			switch (msg.readChar()) {
			case 'i':
				//--- le e exibe o valor de um pino de entrada
				//--- parametros: numero do pino
				Serial.println();
				addr1 = msg.readInt();
				for (i = 0; i < 100; i++) {
					addr2 = digitalRead(addr1);
					Serial.print("Pin ");
					Serial.print(addr1, DEC);
					Serial.print("=");
					Serial.println(addr2, DEC);
				}
				break;

			case 'l':
				//--- escreve um valor em uma memoria n�o volatil
				//--- parametros: endereco de memoria, valor decimal
				addr1 = msg.readInt();
				addr2 = msg.readInt();
				setNVMem(addr1, addr2);
				break;

			case 'd':
				//--- dump memoria n�o volatil
				//--- parametros: endereco inicial, endereco final
				addr1 = msg.readInt();
				addr2 = msg.readInt();
				Serial.println();
				Serial.print("DUMP NV ");
				Serial.println(addr1,DEC);
				Serial.println(addr2,DEC);
				for (unsigned int i = addr1; i <= addr2; i++) {
					Serial.print(getNVMem(i),DEC);
					Serial.print(" ");
				}
				break;

			case 'r':
				//--- dump memoria normal
				//--- parametros: endereco inicial, endereco final
				addr1 = msg.readInt();
				addr2 = msg.readInt();
				dumpMem(addr1, addr2);
				break;

			case 's':
				//--- toggle STEP state
				toggleStep();
				if (isStep()) {
					Serial.println("STEP on");
				} else {
					Serial.println("STEP off");
				}
				break;

			case 'x':
				//--- print status
				printStatus();
				break;

			case 't':
				//--- print stack
				printStack();
				break;

			case '.':
				modoStep = false;
				break;

			default:
				break;
			}
			if (modoStep) {
				//--- prompt
				Serial.println();
				Serial.print("STEP>");
			}
		}
	}
}
