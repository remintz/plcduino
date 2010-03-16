/*
 * ErrMsg.c
 *
 *  Created on: 15/03/2010
 *      Author: Renato
 */

#include <stdio.h>
#include <stdlib.h>

void doAbort(char *msg) {
	printf("ABORT:%s/n", msg);
	exit(EXIT_FAILURE);
}
