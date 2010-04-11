#ifndef MEMORY_H_
#define MEMORY_H_

//--- memory map
#define MEM_OFFSET_MEM			0
#define MEM_OFFSET_IN			4
#define MEM_OFFSET_RS_IN		6
#define MEM_OFFSET_SR_IN		7
//#define MEM_OFFSET_CTU_IN		8	// +0=Controle, +1=PV0, +3=PV1
//#define MEM_OFFSET_CTD_IN		13	// +0=Controle, +1=PV0, +3=PV1
#define MEM_OFFSET_CTUD_IN		8	// +0=Ctrl0, +1=Ctrl1, +2=PV0, +4=PV1, +6=PV2, +8=PV3
#define MEM_OFFSET_TP_IN		17	// +0=Ctrl, +1=PT0, +3=PT1
#define MEM_OFFSET_TON_IN		22	// +0=Ctrl, +1=PT0, +3=PT1
#define MEM_OFFSET_TOF_IN		27	// +0=Ctrl, +1=PT0, +3=PT1
#define MEM_OFFSET_RTRIG_IN		32
#define MEM_OFFSET_FTRIG_IN		33
#define MEM_OFFSET_OUT			34
#define MEM_OFFSET_RS_OUT		35
#define MEM_OFFSET_SR_OUT		36
//#define MEM_OFFSET_CTU_OUT		0
//#define MEM_OFFSET_CTD_OUT		0
#define MEM_OFFSET_CTUD_OUT		37	// +0=Ctrl, +1=CV0, +3=CV1, +5=CV2, +7=CV3
#define MEM_OFFSET_TP_OUT		46	// +0=Ctrl, +1=ET0, +3=ET1
#define MEM_OFFSET_TON_OUT		51	// +0=Ctrl, +1=ET0, +3=ET1
#define MEM_OFFSET_TOF_OUT		56	// +0=Ctrl, +1=ET0, +3=ET1
#define MEM_OFFSET_RTRIG_OUT	61	// Bits 0..3
#define MEM_OFFSET_FTRIG_OUT	61	// Bits 4..7
#define MEM_OFFSET_CONTROL		62
#define MEM_SIZE				64

void initMem();
void initStack();
WORD *getPMem(int addr);
void setMemBit(int addr, int bit, WORD val);
WORD getMemBit(int addr, int bit);
void setMemInt(int addr, WORD val);
WORD getMemInt(int addr);
void setMem(unsigned char operand, int addr, int bit, WORD val);
WORD getMem(unsigned char operand, int addr, int bit);
void dumpMem(int begin, int end);
void opPush(WORD op);
WORD opPop();
WORD opTop();
void valPush(WORD val);
WORD valPop();
WORD valTop();

#endif /* MEMORY_H_ */
