/*
 * ilcode.h
 *
 *  Created on: 07/11/2009
 *      Author: Renato
 */
#ifndef ILCODE_H_
#define ILCODE_H_

enum {
	IL_NEG 		= 1,	// '!'
	IL_PUSH 	= 2,	// '('
	IL_NORM 	= 3,	// ' '
	IL_COND 	= 4,	// '?'
	IL_NOP		= 0
} IL_MODIFIERS;

enum {
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


typedef struct _instruction
{
//	char lineLabel[MAXSTR];
//	char jmpLabel[MAXSTR];
	BYTE operation;
	BYTE operand;
	BYTE modifier;
	BYTE byte;
	BYTE bit;
} Instruction;

#if 0
const char OPERATORS[70][7] = {
	// expression operators
	"AND",
	"&",
	"OR",
	"XOR",
	"ANDN",
	"&N",
	"ORN",
	"XORN",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"GT",
	"GE",
	"EQ",
	"LT",
	"NE",
	"LE",
	// simple operators
	"LD",
	"LDN",
	"ST",
	"STN",
	"NOT",
	"S",
	"R",
	"S1",
	"R1",
	"CLK",
	"CU",
	"CD",
	"PV",
	"IN",
	"PT",
	// call operators
	"CAL",
	"CALC",
	"CALCN",
	// return operators
	"RET",
	"RETC",
	"RETCN",
	// jump operators
	"JMP",
	"JMPC",
	"JMPCN",
	// not operator
	"NOT"
};
#endif

#endif /* ILCODE_H_ */
