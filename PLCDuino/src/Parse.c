/*
 * Parse.c
 *
 *  Created on: 18/10/2009
 *      Author: Renato
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "util.h"
#include "ilcode.h"
//#include "ILRun.h"

#define ERROR 		-1
#define ERR_BADCHAR	-2
#define ERR_BADFILE	-3
#define ERR_BADOPERAND	-4
#define ERR_BADINDEX	-5
#define ERR_BADCOIL	-6
#define ERR_BADOPERATOR	-7
#define ERR_BADOPERATOR	-7

//--- Ladder character codes
enum {
	CH_BLANK	= 10,	//blank character
	CH_COIL	 	= 11,	//(
	CH_AND 		= 12,	//-
	CH_OR 		= 13,	//|
	CH_NOT 		= 14,	//!
	CH_NODE 	= 15,	//+
	CH_SET 		= 16,	//[ NEW! set
	CH_RESET 	= 17,	//] NEW! reset
	CH_DOWN 	= 18,	//) NEW! negate coil

	//operands
	CH_INPUT 	= 19,	//i
	CH_FALLING 	= 20,	//f
	CH_MEMORY 	= 21,	//m NEW!
	CH_COMMAND 	= 22,	//c
	CH_BLINKOUT = 23,	//b
	CH_RISING 	= 24,	//r
	CH_TIMEOUT 	= 25,	//t
	CH_OUTPUT 	= 26,  	//q

	//coils (work for IL too)
	CH_CONTACT 	= 27,	//Q
	CH_START 	= 28,	//T
	CH_PULSEIN 	= 29,	//M NEW!
	CH_WRITE 	= 30,	//W
	CH_END 		= 31,	//0

	CH_ERR_BADCHAR = 255
} CH_CODES;

const char IL_COMMANDS[IL_LAST][5]=
{
	//--- same order as IL_OPCODES enum
	"",
	")",
	"RET",
	"JMP",
	"CAL",
	"S",
	"R",
	"AND",
	"OR",
	"XOR",
	"ANDN",
	"ORN",
	"XORN",
	"LD",
	"ST",
	"LDN",
	"STN",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"GT",
	"GE",
	"EQ",
	"NE",
	"LT",
	"LE"
};

int extract_number(char *line, int start)
{
	//read characters from string line
	int i, n;
	//starting @ position start
	if (start >= strlen(line))
		return ERROR;
	n = 0;
	for (i = start; isdigit(line[i]); i++)
		n = 10 * n + (line[i] - '0');
	if (i == start)
		//no digits read
		return ERROR;
	return n;
	//return number read or error
}

#if 0
// este read_char ta mais preparado para ladder e esta desativado veja o outro
int read_char(char * line, int idx)
{
	//read ONE character from line[idx] parse grammatically
	int r = 0;
	if (idx > strlen(line))
		return ERROR;
	if (line[idx]==0
			|| line[idx]==';'
				|| line[idx]=='\n'
					|| line[idx]=='\r')
		return CH_END;
	if (line[idx]==' '
		|| line[idx]=='.'
			|| line[idx]=='#'
				|| line[idx]=='\t' )
		return CH_BLANK;
	if (isdigit(line[idx]))
		return line[idx] - '0';
	switch (line[idx])
	{
	case '('://Coil
		r = CH_COIL;
		break;
	case '-'://horizontal line
		r = CH_AND;
		break;
	case '|'://vertical line
		r = CH_OR;
		break;
	case '!'://normally closed
		r = CH_NOT;
		break;
	case '+'://
		r = CH_NODE;
		break;
	case 'Q'://dry contact output
		r = CH_CONTACT;
		break;
		//	case 'S'://set output
	case '[': //NU!
		r = CH_SET;
		break;
		//	case 'R'://reset output
	case ']'://NU!
		r = CH_RESET;
		break;
	case 'T'://start timer
		r = CH_START;
		break;
		//	case 'D'://down timer DEPRECATED. USED FOR COIL NEGATION
	case ')':
		r = CH_DOWN;
		break;
	case 'M'://pulse to counter
		r = CH_PULSEIN;
		break;
	case 'W'://write response
		r = CH_WRITE;
		break;
	case 'i'://input
		r = CH_INPUT;
		break;
	case 'q'://output value
		r = CH_OUTPUT;
		break;
	case 'f'://falling edge
		r = CH_FALLING;
		break;
	case 'r'://rising Edge
		r = CH_RISING;
		break;
	case 'm'://pulse of counter
		r = CH_MEMORY;
		break;
	case 't'://timer.q
		r = CH_TIMEOUT;
		break;
	case 'c'://read command
		r = CH_COMMAND;
		break;
	case 'b'://blinker
		r = CH_BLINKOUT;
		break;
	default:
		r = CH_ERR_BADCHAR;//error
	}
	//return value or error
	return r;
}
#endif

int read_char(char * line, int idx)
{
	//read ONE character from line[idx] parse grammatically
	int r = 0;
	if (idx > strlen(line))
		return ERROR;
	if (line[idx]==0
			|| line[idx]==';'
				|| line[idx]=='\n'
					|| line[idx]=='\r')
		return CH_END;
	if (line[idx]==' '
		|| line[idx]=='.'
			|| line[idx]=='#'
				|| line[idx]=='\t' )
		return CH_BLANK;
	if (isdigit(line[idx]))
		return line[idx] - '0';
	switch (line[idx])
	{
	case 'I':
	case 'Q':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'C':
	case 'D':
	case 'E':
	case 'T':
	case 'U':
	case 'S':
	case 'R':
	case 'X':
		r = line[idx];
		break;
	default:
		r = CH_ERR_BADCHAR;//error
		break;
	}
	//return value or error
	return r;
}

int parse_il_line(char *line, Instruction *op)
{//    line format:[label:]<operator>[<modifier>[%<operand><byte>[/<bit>]]|<label>][;comment]
	char buf[MAXSTR];
	int i, j, byte, bit;
	char * str;
	char * cursor;
	char op_buf[4];
	char label_buf[MAXSTR];
	BYTE modifier, operand,operator;

	printf("Instr: %s\n",line);

	memset(label_buf, 0, MAXSTR);
	memset(buf, 0, MAXSTR);

	//--- 1. read up to ';' or \n and truncate comments
	i = 0;
	while (line[i] != 0 && line[i] != '\n' && line[i] != ';')
		i++;
	for (j = i; j < strlen(line); j++)
		line[j] = 0;

	//--- 2. find last ':', trunc up to there, store label.
	str = strrchr(line, ':');
	if (str)
	{
		i = 0;
		while (line+i != str) {
			label_buf[i] = line[i];
			i++;
		}
		strcpy(buf, str+1);
		printf("label:%s\n",label_buf);
	}
	else
		strcpy(buf, line);

	// skip blanks and tabs
	i = 0;
	while (buf[i] == ' ' || buf[i] == '\t')
		i++;
	strcpy(buf, buf+i);

	//---3. find first ' ','!','('. store modifier (0 if not found)
	modifier = IL_NOP;
	str = strchr(buf, '(');
	if (str)
		modifier = IL_PUSH;	// push stack
	else
	{
		str = strchr(buf, '!');
		if (str)
			modifier = IL_NEG;	// negate
		else
		{
			str = strchr(buf, ' ');
			if (str)
				modifier = IL_NORM;	// normal
			else
			{
				str = strchr(buf, '?');
				if (str)
					modifier = IL_COND;	// conditional branch
			}
		}
	}
	printf("modifier:%d\n",modifier);

	//---4. read operator from beginning to modifier. check if invalid, (return error)
	if (str)
	{
		cursor = buf;
		i = 0;
		memset(op_buf, 0, 4);
		while (cursor+i != str && i < 4) {
			op_buf[i] = cursor[i];
			i++;
		}
	}
	else
	{
		if (strlen(buf) < 4)
			strcpy(op_buf, buf);
		else
			return ERR_BADOPERATOR;
	}
	printf("operator:%s ",op_buf);

	operator = IL_LAST;
	for (i = 0; i < IL_LAST; ++i)
	{
		if (!strcmp(op_buf, IL_COMMANDS[i])) {
			operator = i;
			break;
		}
	}
	printf("(%d)\n",operator);

	if (operator==IL_LAST)
		return ERR_BADOPERATOR;

	if (operator > IL_CAL)
		{//5. if operator supports arguments, find first '%'. (special case: JMP (string). if not found return error
		str = strchr(buf, '%');
		if (!str)
			return ERR_BADCHAR;

		//6.  read first non-numeric char after '%'. if not found return error. store operand. check if invalid (return error).
		operand = 0;
		if (isalpha(str[1]))
			operand = read_char(str,1);
		else
			return ERR_BADOPERAND;
		printf("operand:%d\n",operand);

		//7.  read first numeric chars after operand. store byte. if not found or out of range, return error.
		byte = extract_number(str,2);
		if (byte<0)
			return ERR_BADINDEX;
		printf("byte:%d\n",byte);

		//8.  find '/'. if not found truncate, return.
		cursor = strchr(str,'/');
		bit = 64;
		if (cursor)
		{
			if (!isdigit(cursor[1]) || cursor[1] > '7')
				return ERR_BADINDEX;
			else
				bit = cursor[1] - '0';
			printf ("bit:%d\n",bit);
		}

		//9.  if found, read next char. if not found, or not numeric, or >7, return error. store bit.
	}
	else
	{
		if (operator==IL_JMP)
		{
			str = strchr(buf, ' ');
			if (!str)
				return ERR_BADOPERAND;
			strcpy(op->jmpLabel, str+1);
		}
	}
	strcpy(op->lineLabel, label_buf);
	op->operation = operator;
	op->modifier = modifier;
	op->operand = operand;
	op->byte = byte;
	op->bit = bit;
	return 0;
}

void ParseIL(char *in, Instruction *program_ptr) {
	char line[80];
	int i;
	int pc = 0;
	i = strcspn(in, "\n");
	while (i > 0)
	{
		strncpy(line, in, i);
		line[i] = 0;
		parse_il_line(line, (program_ptr + pc));
		pc++;
		in = in + i;
		if (in[0] == '\n')
			in = in + 1;
		i = strcspn(in, "\n");
	}
}
