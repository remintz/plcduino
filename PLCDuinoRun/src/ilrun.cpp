#ifdef ARDUINO
	#include <Wprogram.h>
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

#include "util.h"
#include "memory.h"
#include "ilcode.h"
#include "hw.h"
//#include "FunctionBlocks.h"
#include "errmsg.h"

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

#if 0
BOOL ilRun(Instruction *pInstructions) {
	unsigned int iCurInstruction = 0;
	Instruction curInstruction;
	WORD operat;
	WORD curResult = 0;
	WORD val1;
	WORD val2;

	BOOL theEnd = FALSE;
	BOOL abort = FALSE;
	BOOL found = FALSE;

	initStack();

	while (!theEnd) {
		curInstruction = pInstructions[iCurInstruction];
		found = TRUE;
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
			theEnd = TRUE;
			break;
		default:
			found = FALSE;
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

BOOL ctrlAbort() {
	return (getMemBit(MEM_OFFSET_CONTROL, 0));
}
#endif

void ilRunForever(Instruction *pInstructions) {
	BOOL theEnd = FALSE;
//	time_t newTime, lastTime;
//	int deltaTime;
//	time(&lastTime);
	initMem();
	while (!theEnd) {
		//kickWDT();
		readInputs(getPMem(MEM_OFFSET_IN));
#if 0
		if (ilRun(pInstructions)) {
			//--- abort instruction executed
			break;
		}
		dumpMem(0,1);
		dumpMem(74,83);
		dumpMem(198,207);
		if (ctrlAbort()) {
			break;
		}
		time(&newTime);
		deltaTime = (int)difftime(newTime, lastTime);
		lastTime = newTime;
		TONScan((WORD *)getPMem(MEM_OFFSET_TON_IN), (WORD *)getPMem(MEM_OFFSET_TON_OUT), deltaTime);
		SRScan((WORD *)getPMem(MEM_OFFSET_SR_IN), (WORD *)getPMem(MEM_OFFSET_SR_OUT));
		RSScan((WORD *)getPMem(MEM_OFFSET_RS_IN), (WORD *)getPMem(MEM_OFFSET_RS_OUT));
#endif
		int oBit;
		oBit = getMemBit(MEM_OFFSET_IN, 2);
//		Serial.print("oBit=");
//		Serial.print(oBit);
//		Serial.print("\n\r");
		setMemBit(MEM_OFFSET_OUT, 13, oBit);
		writeOutputs(getPMem(MEM_OFFSET_OUT));
	}
}

