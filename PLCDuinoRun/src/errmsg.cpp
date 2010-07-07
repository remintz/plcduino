#include <WProgram.h>

void doAbort(const char *msg) {
	Serial.print("ABORT ");
	Serial.print(msg);
	Serial.print("\n\r");
	exit(-1);
}
