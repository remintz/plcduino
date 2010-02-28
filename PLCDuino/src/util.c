/*
 * util.c
 *
 *  Created on: 12/10/2009
 *      Author: Renato
 */

#include <stdio.h>
#include <stdlib.h>

void doAbort(const char *msg) {
	printf("ABORT:%s/n", msg);
	exit(EXIT_FAILURE);
}
