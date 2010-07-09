#ifndef MEMORY_H_
#define MEMORY_H_

//--- memory map
#define MEM_OFFSET_MEM			0
#define MEM_OFFSET_IN			4
#define MEM_OFFSET_RS_IN		6
#define MEM_OFFSET_SR_IN		7
#define MEM_OFFSET_CTUD_IN		8	// +0=Ctrl0, +1=Ctrl1, +2=PV0, +4=PV1, +6=PV2, +8=PV3
#define MEM_OFFSET_TP_IN		18	// +0=Ctrl, +1=PT0, +3=PT1
#define MEM_OFFSET_TON_IN		23	// +0=Ctrl, +1=PT0, +3=PT1
#define MEM_OFFSET_TOF_IN		28	// +0=Ctrl, +1=PT0, +3=PT1
#define MEM_OFFSET_RTRIG_IN		33
#define MEM_OFFSET_FTRIG_IN		34
#define MEM_OFFSET_OUT			35
#define MEM_OFFSET_RS_OUT		37
#define MEM_OFFSET_SR_OUT		38
#define MEM_OFFSET_CTUD_OUT		39	// +0=Ctrl, +1=Mem, +2=CV0, +4=CV1, +6=CV2, +8=CV3
#define MEM_OFFSET_TP_OUT		49	// +0=Ctrl, +1=ET0, +3=ET1
#define MEM_OFFSET_TON_OUT		54	// +0=Ctrl, +1=ET0, +3=ET1
#define MEM_OFFSET_TOF_OUT		59	// +0=Ctrl, +1=ET0, +3=ET1
#define MEM_OFFSET_RTRIG_OUT	64	// Bits 0..3
#define MEM_OFFSET_FTRIG_OUT	64	// Bits 4..7
#define MEM_OFFSET_CONTROL		65
#define MEM_SIZE				66

void initMem();
void initStack();
unsigned char *getPMem(int addr);
void setMemBit(int addr, int bit, WORD val);
int getMemBit(int addr, int bit);
void setMemInt(int addr, WORD val);
unsigned char getMemByte(int addr);
void setMem(unsigned char operand, int addr, int bit, unsigned char val);
unsigned char getMem(unsigned char operand, int addr, int bit);
void dumpMem();
void printStack();
void opPush(WORD op);
WORD opPop();
WORD opTop();
void valPush(WORD val);
WORD valPop();
WORD valTop();
BOOL isDebugOn();
BOOL isAbortOn();

#endif /* MEMORY_H_ */
