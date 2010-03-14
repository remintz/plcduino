/*
 * util.h
 *
 *  Created on: 12/10/2009
 *      Author: Renato
 */

#ifndef UTIL_H_
#define UTIL_H_

#define BOOL 	unsigned char
#define BYTE	unsigned char
#define WORD	unsigned int
#define TRUE	1
#define FALSE	0
#define ERROR	-1
#define MAXSTR	255
#define MAXBUF	1000

void doAbort(const char *msg);
WORD getBit(int by, int bit);
WORD setBit(WORD *by, int bit);
WORD resetBit(WORD *by, int bit);
WORD modBit(WORD *by, int bit, BOOL value);


#endif /* UTIL_H_ */
