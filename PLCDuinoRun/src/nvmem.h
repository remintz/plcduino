#ifndef NVMEM_H_
#define NVMEM_H_

void setNVMem(WORD addr, unsigned char b);
unsigned char getNVMem(WORD addr);
void setProgByte(WORD addr, unsigned char b);
unsigned char getProgByte(WORD addr);
void setInstruction(WORD instructionPtr, Instruction instr);
void getInstruction(WORD instructionPtr, Instruction *instr);
WORD getIOConfig(void);
WORD getIOMask(void);
void setIOMask(WORD val);
void setIOConfig(WORD val);
void printIOSetup();

#endif /* NVMEM_H_ */
