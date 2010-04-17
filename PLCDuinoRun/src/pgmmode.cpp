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
					//--- parametros: endereco inicial, endereco final
					addr1 = msg.readInt();
					addr2 = msg.readInt();
					Serial.println();
					Serial.print("DUMP ");
					Serial.println(addr1,DEC);
					for (unsigned int i = addr1; i <= addr2; i++) {
						Serial.print(getNVMem(i),HEX);
						Serial.print(" ");
					}
					break;
				case '.':
					modoProgramacao = FALSE;
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
