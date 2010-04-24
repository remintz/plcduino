#include <WProgram.h>

#include "util.h"
#include "ilcode.h"
#include "nvmem.h"
#include "Messenger.h"

void programMode() {
	Messenger msg = Messenger();
	BOOL modoProgramacao = true;
	BOOL carregando = false;
	unsigned char c;
	unsigned int i;
	unsigned int addr1 = 0;
	unsigned int addr2 = 0;

	//--- prompt
	Serial.print(">");
	while (modoProgramacao) {
		//--- aguarda caracteres pela serial
		while (!Serial.available());
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
				switch (msg.readChar()) {
				case 'l':
					//--- entra em modo de carga de bytes
					//--- parametros: endereco
					addr1 = msg.readInt();
					carregando = true;
					Serial.println();
					Serial.print("Loading @");
					Serial.print(addr1, DEC);
					Serial.println(". Enter 256 to stop.");
					break;
				case 'd':
					//--- dump
					//--- parametros: endereco inicial, endereco final
					addr1 = msg.readInt();
					addr2 = msg.readInt();
					Serial.println();
					Serial.print("DUMP ");
					Serial.println(addr1,DEC);
					for (unsigned int i = addr1; i <= addr2; i++) {
						Serial.print(getNVMem(i),DEC);
						Serial.print(" ");
					}
					break;
				case 'c':
					//--- config
					//--- parametros: tipo, valor
					//--- tipo 0: exibe config
					//--- tipo 1: iomask
					//--- tipo 2: ioconfig
					addr1 = msg.readInt();
					addr2 = msg.readInt();
					switch (addr1) {
					case 0:
						Serial.println("Config");
						Serial.print("ioMask: 0b");
						Serial.println(getIOMask(),BIN);
						Serial.print("ioConfig: 0b");
						Serial.println(getIOConfig(),BIN);
						break;
					case 1:
						setIOMask(addr2);
						break;
					case 2:
						setIOConfig(addr2);
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
