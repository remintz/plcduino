#ifndef NVMEM_H_
#define NVMEM_H_

void setNVMem(WORD addr, unsigned char b);
void setNVMemBit(WORD addr, unsigned char bit, unsigned char b);
unsigned char getNVMem(WORD addr);
unsigned char getNVMemBit(WORD addr, unsigned char bit);
void setProgByte(WORD addr, unsigned char b);
unsigned char getProgByte(WORD addr);
WORD getIOConfig(void);
WORD getIOMask(void);
WORD getIONeg(void);
void setIOMask(WORD val);
void setIOConfig(WORD val);
void setIONeg(WORD val);

void toggleStep();
BOOL isStep();
BOOL isTrace();

//void printIOSetup();

#endif /* NVMEM_H_ */
