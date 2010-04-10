#ifndef UTIL_H_
#define UTIL_H_

//#ifndef BYTE
//	#define BYTE	unsigned char
//#endif

#define BOOL 	unsigned char
#define WORD	unsigned int
#define TRUE	1
#define FALSE	0
#define ERROR	-1

#define MAXSTR	255
#define MAXBUF	1000

WORD getBit(int by, int bit);
WORD setBit(WORD *by, int bit);
WORD resetBit(WORD *by, int bit);
WORD modBit(WORD *by, int bit, BOOL value);


#endif /* UTIL_H_ */
