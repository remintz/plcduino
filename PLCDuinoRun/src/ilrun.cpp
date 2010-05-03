#include "util.h"
#include "ilcode.h"
#ifdef ARDUINO
	#include <Wprogram.h>
	#include "nvmem.h"
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

#include "memory.h"
#include "hw.h"
#include "functionblocks.h"
#include "errmsg.h"
#include "timer.h"

enum _IL_MODIFIERS {
	IL_NEG 		= 1,	// '!'
	IL_PUSH 	= 2,	// '('
	IL_NORM 	= 3,	// ' '
	IL_COND 	= 4,	// '?'
	IL_NOP		= 0
} IL_MODIFIERS;

enum _IL_OPCODES {
	//no operand
	IL_END = 0,		//END
	IL_POP,			//)
	IL_RET,			//RET
	//arithmetic LABEL
	IL_JMP,			//JMP
	//subroutine call (unimplemented)
	IL_CAL,			//CAL
	//--------------- all operators below support arguments
	//boolean, no modifier
	IL_SET,			//S
	IL_RESET,		//R
	//boolean, all modifiers
	IL_AND,		 	//AND
	IL_OR,			//OR
	IL_XOR,			//XOR
	IL_ANDN,
	IL_ORN,
	IL_XORN,
	//any operand, only negation
	IL_LD,			//LD
	IL_ST,			//ST
	IL_LDN,			//LDN
	IL_STN,			//STN
	//any operand, only push
	IL_ADD,
	IL_SUB,
	IL_MUL,
	IL_DIV,
	IL_GT,
	IL_GE,
	IL_EQ,
	IL_NE,
	IL_LT,
	IL_LE,
	IL_LAST			// last OP... flag
} IL_OPCODES;

void getInstruction(WORD instructionPtr, Instruction *instr) {
	WORD memAddr = (instructionPtr * sizeof(Instruction));
	instr->operation = getProgByte(memAddr);
	instr->operand = getProgByte(memAddr+1);
	instr->modifier = getProgByte(memAddr+2);
	instr->byte = getProgByte(memAddr+3);
	instr->bit = getProgByte(memAddr+4);
}

BOOL ilRun() {
	unsigned int iCurInstruction = 0;
	Instruction curInstruction;
	WORD operat;
	WORD curResult = 0;
	WORD val1;
	WORD val2;

	BOOL theEnd = false;
	BOOL abort = false;
	BOOL found = false;

	initStack();

	while (!theEnd) {
		getInstruction(iCurInstruction, &curInstruction);
		found = true;
		switch (curInstruction.operation) {
		case IL_LD:
			curResult = getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
			break;
		case IL_ST:
			setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, curResult);
			break;
		case IL_LDN:
			curResult = ~getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
		case IL_STN:
			setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, ~curResult);
			break;
		case IL_SET:
			if (curResult == 1) {
				setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, 1);
			}
			break;
		case IL_RESET:
			if (curResult == 1) {
				setMem(curInstruction.operand, curInstruction.byte, curInstruction.bit, 0);
			}
			break;
		case IL_CAL:
			break;
		case IL_RET:
			break;
		case IL_JMP:
			break;
		case IL_END:
			theEnd = true;
			break;
		default:
			found = false;
			break;
		}
		if (!found) {
			if (curInstruction.modifier == IL_PUSH) {
				valPush(curResult);
				curResult = getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
				opPush(curInstruction.operation);
			} else {
				if (curInstruction.operation == IL_POP) {
					operat = opPop();
					val1 = valPop();
					val2 = curResult;
				} else {
					operat = curInstruction.operation;
					val1 = curResult;
					val2 = getMem(curInstruction.operand, curInstruction.byte, curInstruction.bit);
					if (curInstruction.modifier == IL_NEG) {
						val2 = ~val2;
					}
				}
				switch (operat) {
				case IL_AND:
					curResult = val1 & val2;
					break;
				case IL_OR:
					curResult = val1 | val2;
					break;
				case IL_XOR:
					curResult = val1 ^ val2;
					break;
				case IL_ANDN:
					curResult = val1 & (~val2);
					break;
				case IL_ORN:
					curResult = val1 | (~val2);
					break;
				case IL_XORN:
					curResult = val1 ^ (~val2);
					break;
				case IL_ADD:
					curResult = val1 + val2;
					break;
				case IL_SUB:
					curResult = val1 - val2;
					break;
				case IL_MUL:
					curResult = val1 * val2;
					break;
				case IL_DIV:
					curResult = val1 / val2;
					break;
				case IL_GT:
					curResult = val1 > val2;
					break;
				case IL_GE:
					curResult = val1 >= val2;
					break;
				case IL_EQ:
					curResult = val1 == val2;
					break;
				case IL_NE:
					curResult = val1 != val2;
					break;
				case IL_LE:
					curResult = val1 <= val2;
					break;
				case IL_LT:
					curResult = val1 < val2;
					break;
				default:
					doAbort(MSG_ILLEGAL_INSTRUCTION);
					break;
				}
			}
		}
		iCurInstruction++;
	}
	return abort;
}

void ilRunForever() {
	int oldTime, newTime, deltaTime;
	initMem();
	oldTime = getTimer();
	while (true) {
		kickWDT();
		readInputs(getPMem(MEM_OFFSET_IN));
		if (ilRun()) {
			Serial.println("Aborted.");
			//--- abort instruction executed
			break;
		}
		newTime = getTimer();
		deltaTime = diffTimer(oldTime);
		oldTime = newTime;
		TONScan((unsigned char *)getPMem(MEM_OFFSET_TON_IN), (unsigned char *)getPMem(MEM_OFFSET_TON_OUT), deltaTime);
		TOFScan((unsigned char *)getPMem(MEM_OFFSET_TOF_IN), (unsigned char *)getPMem(MEM_OFFSET_TOF_OUT), deltaTime);
		TPScan((unsigned char *)getPMem(MEM_OFFSET_TP_IN), (unsigned char *)getPMem(MEM_OFFSET_TP_OUT), deltaTime);
		SRScan((unsigned char *)getPMem(MEM_OFFSET_SR_IN), (unsigned char *)getPMem(MEM_OFFSET_SR_OUT));
		RSScan((unsigned char *)getPMem(MEM_OFFSET_RS_IN), (unsigned char *)getPMem(MEM_OFFSET_RS_OUT));
		CTUDScan((unsigned char *)getPMem(MEM_OFFSET_CTUD_IN), (unsigned char *)getPMem(MEM_OFFSET_CTUD_OUT));

//		int oBit;
//		oBit = getMemBit(MEM_OFFSET_IN, 2);
//		Serial.print("oBit=");
//		Serial.print(oBit);
//		Serial.print("\n\r");
//		setMemBit(MEM_OFFSET_OUT, 13, oBit);
		if (isAbortOn()) {
			Serial.println("Stopped.");
			break;
		}
		writeOutputs(getPMem(MEM_OFFSET_OUT));
	}
}

