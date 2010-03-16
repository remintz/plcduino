/*
 * Memory.h
 *
 *  Created on: 15/03/2010
 *      Author: Renato
 */

#ifndef MEMORY_H_
#define MEMORY_H_

//--- memory map
#define MEM_OFFSET_MEM			0
#define MEM_OFFSET_IN			32
#define MEM_OFFSET_RS_IN		64
#define MEM_OFFSET_SR_IN		65
#define MEM_OFFSET_TP_IN		66
#define MEM_OFFSET_TON_IN		74
#define MEM_OFFSET_TOF_IN		0
#define MEM_OFFSET_CTU_IN		0
#define MEM_OFFSET_CTD_IN		0
#define MEM_OFFSET_CTUD_IN		0
#define MEM_OFFSET_RTRIG_IN		0
#define MEM_OFFSET_FTRIG_IN		0
#define MEM_OFFSET_OUT			126
#define MEM_OFFSET_RS_OUT		158
#define MEM_OFFSET_SR_OUT		159
#define MEM_OFFSET_TP_OUT		0
#define MEM_OFFSET_TON_OUT		198
#define MEM_OFFSET_TOF_OUT		0
#define MEM_OFFSET_CTU_OUT		0
#define MEM_OFFSET_CTD_OUT		0
#define MEM_OFFSET_CTUD_OUT		0
#define MEM_OFFSET_RTRIG_OUT	0
#define MEM_OFFSET_FTRIG_OUT	0
#define MEM_OFFSET_CONTROL		160

void initMem();
void initStack();
void *getPMem(int addr);
void setMemBit(int addr, int bit, WORD val);
WORD getMemBit(int addr, int bit);
void setMemInt(int addr, WORD val);
WORD getMemInt(int addr);
void setMem(BYTE operand, int addr, int bit, WORD val);
WORD getMem(BYTE operand, int addr, int bit);
void dumpMem(int begin, int end);
void opPush(WORD op);
WORD opPop();
WORD opTop();
void valPush(WORD val);
WORD valPop();
WORD valTop();

#endif /* MEMORY_H_ */
