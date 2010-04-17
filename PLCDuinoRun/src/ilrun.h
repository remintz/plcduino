/*
 * ILRun.h
 *
 *  Created on: 12/10/2009
 *      Author: Renato
 */

#ifndef ILRUN_H_
#define ILRUN_H_

#include "ilcode.h"

void initMem();
void setMemInt(int addr, int val);
void dumpMem();
void ilRunForever();

#endif /* ILRUN_H_ */
