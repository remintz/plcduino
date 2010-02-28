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
	//any operand, only negation
	IL_LD,			//LD
	IL_ST,			//ST
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
	char lineLabel[MAXSTR];
	char jmpLabel[MAXSTR];
	BYTE operation;
	BYTE operand;
	BYTE modifier;
	BYTE byte;
	BYTE bit;
} Instruction;


#endif /* ILCODE_H_ */
