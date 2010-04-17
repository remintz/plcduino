#ifndef NVMEM_H_
#define NVMEM_H_

void setInstruction(WORD addr, Instruction instr);
void getInstruction(WORD addr, Instruction *instr);
void setNVMem(WORD addr, unsigned char b);
unsigned char getNVMem(WORD addr);

#endif /* NVMEM_H_ */
