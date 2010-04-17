/*
 * main.cpp
 *
 *  Created on: Mar 5, 2010
 *      Author: ssmethurst
 */
#include <WProgram.h>
#include "util.h"
#include "ilcode.h"
#include "ilrun.h"
#include "hw.h"
#include "pgmmode.h"

//--- resolver referencia indefinida da biblioteca do Arduino
extern "C" void __cxa_pure_virtual(void) {
    while(1);
}

//--------------------------------------------------------------------
int main(void) {

	//--- inicializando biblioteca do Arduino
	init();
	Serial.begin(9600);

	//--- modo de programação
	Serial.println();
	Serial.println("PLCDuino PGM Mode");
	programMode();
	//--- modo execucao
	Serial.println();
	Serial.println("EXEC Mode");
	initHw();
	ilRunForever();
}
