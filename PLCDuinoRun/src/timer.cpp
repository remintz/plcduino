#include <WProgram.h>
#include "MsTimer2.h"

#define TIMER_CICLE		30000

int timerCount;

void tickTimer() {
//	Serial.print("T");
	timerCount++;
	if (timerCount >= TIMER_CICLE) {
		timerCount = 0;
	}
}

int getTimer() {
	return timerCount;
}

int diffTimer(int since) {
//	Serial.print("diffTimer: since=");
//	Serial.print(since, DEC);
//	Serial.print("Cnt=");
//	Serial.println(timerCount,DEC);
	int t;
	t = timerCount;
	if (t >= since)
		return (t - since);
	else
		return (t + TIMER_CICLE - since);
}

void iniTimer() {
	Serial.println("iniTimer");
	timerCount = 0;
	MsTimer2::set(1000, tickTimer);
	MsTimer2::start();
}
