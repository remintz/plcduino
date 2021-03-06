#include <WProgram.h>

#include "hw.h"
#include "util.h"
#include "ilcode.h"
#include "nvmem.h"
#include "Messenger.h"
#include "timer.h"

#define	TIMEOUT_PROGRAM_MODE	10 // seconds

void programMode() {
	Messenger msg = Messenger();
	BOOL modoProgramacao = true;
	BOOL carregando = false;
	BOOL comingFromBoot = true;
	unsigned char c;
	unsigned int i;
	unsigned int addr1 = 0;
	unsigned int addr2 = 0;
	int oldTime, newTime, deltaTime;

	//--- prompt
	Serial.print(">");
	oldTime = getTimer();
	while (modoProgramacao) {
		//--- aguarda caracteres pela serial
		while (!Serial.available()) {
			if (comingFromBoot) {
				newTime = getTimer();
				deltaTime = diffTimer(oldTime);
				if (deltaTime > TIMEOUT_PROGRAM_MODE)
					return;
			}
		}
		comingFromBoot = false;
		c = Serial.read();
		Serial.print(c);
		msg.process(c);
		if (msg.available()) {
			//--- uma mensagem completa
			if (carregando) {
				i = msg.readInt();
				if (i > 255) {
					carregando = false;
				}
				else {
					setNVMem(addr1, i);
					addr1++;
				}
				Serial.println();
			}
			else {
				Serial.println();
				switch (msg.readChar()) {
				case 'i':
					//--- le e exibe o valor de um pino de entrada
					//--- parametros: numero do pino
					addr1 = msg.readInt();
					for (i = 0; i < 100; i++) {
						addr2 = digitalRead(addr1);
						Serial.print("Pin ");
						Serial.print(addr1, DEC);
						Serial.print("=");
						Serial.println(addr2, DEC);
					}
					break;

				case 'o':
					//--- escreve um valor em um pino de sa�da
					//--- parametros: numero do pino, valor (0 ou 1)
					addr1 = msg.readInt();
					addr2 = msg.readInt();
					if (addr2 & 0x01)
						digitalWrite(addr1,HIGH);
					else
						digitalWrite(addr1,LOW);
					break;

				case 'l':
					//--- entra em modo de carga de bytes
					//--- parametros: endereco
					addr1 = msg.readInt();
					carregando = true;
					Serial.print("Loading @");
					Serial.print(addr1, DEC);
					Serial.println(". Enter 256 to stop.");
					break;

				case 'd':
					//--- dump
					//--- parametros: endereco inicial, endereco final
					addr1 = msg.readInt();
					addr2 = msg.readInt();
					Serial.print("DUMP ");
					Serial.println(addr1,DEC);
					for (unsigned int i = addr1; i <= addr2; i++) {
						Serial.print(getNVMem(i),DEC);
						Serial.print(" ");
					}
					break;

				case 's':
					//--- toggle STEP Instruction state
					toggleStepInstruction();
					if (isStepInstruction()) {
						Serial.println("STEP INSTR on");
					} else {
						Serial.println("STEP INSTR off");
					}
					break;

				case 'S':
					//--- toggle STEP cycle state
					toggleStepCycle();
					if (isStepCycle()) {
						Serial.println("STEP CYCLE on");
					} else {
						Serial.println("STEP CYCLE off");
					}
					break;

				case 'c':
					//--- config
					//--- parametros: tipo, valor
					//--- tipo 0: exibe config
					//--- tipo 1: iomask
					//--- tipo 2: ioconfig
					//--- tipo 3: ioneg
					//--- tipo 4: initHw
					addr1 = msg.readInt();
					addr2 = msg.readInt();
					switch (addr1) {
					case 0:
						Serial.println("Config");
						Serial.print("1 - ioMask  : 0b");
						Serial.println(getIOMask(),BIN);
						Serial.print("2 - ioConfig: 0b");
						Serial.println(getIOConfig(),BIN);
						Serial.print("3 - ioNeg   : 0b");
						Serial.println(getIONeg(),BIN);
						break;
					case 1:
						setIOMask(addr2);
						break;
					case 2:
						setIOConfig(addr2);
						break;
					case 3:
						setIONeg(addr2);
						break;
					case 4:
						initHw();
						displayHwConfig();
						break;
					}
					break;

				case '.':
					modoProgramacao = false;
					break;

				default:
					break;
				}
			}
			if (!carregando) {
				//--- prompt
				Serial.println();
				Serial.print(">");
			}
		}
	}
}
