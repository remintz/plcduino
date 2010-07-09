#ifndef UTIL_H_
#define UTIL_H_

//#ifndef BYTE
//	#define BYTE	unsigned char
//#endif

#define BOOL 	unsigned char
#define WORD	unsigned int
#define ERROR	-1

#define MAXSTR	255
#define MAXBUF	1000

unsigned char getBit(unsigned char by, int bit);
unsigned char setBit(unsigned char *by, int bit);
unsigned char resetBit(unsigned char *by, int bit);
unsigned char  modBit(unsigned char *by, int bit, int value);
unsigned char MSB(WORD val);
unsigned char LSB(WORD val);

#endif /* UTIL_H_ */
