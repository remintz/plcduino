/*
 * ErrMsg.c
 *
 *  Created on: 15/03/2010
 *      Author: Renato
 */

#include <stdio.h>
#include <stdlib.h>

void doAbort(const char *msg) {
	printf("ABORT:%s/n", msg);
	exit(-1);
}
