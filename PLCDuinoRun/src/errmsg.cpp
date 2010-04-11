#ifdef ARDUINO
	#include <WProgram.h>
#else
	#include <stdio.h>
	#include <stdlib.h>
#endif

void doAbort(const char *msg) {
#ifdef ARDUINO
	Serial.print("ABORT ");
	Serial.print(msg);
	Serial.print("\n\r");
#else
	printf("ABORT:%s/n", msg);
#endif
	exit(-1);
}
