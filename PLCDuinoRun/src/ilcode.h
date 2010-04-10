#ifndef ILCODE_H_
#define ILCODE_H_

typedef struct _instruction
{
//	char lineLabel[MAXSTR];
//	char jmpLabel[MAXSTR];
	unsigned char operation;
	unsigned char operand;
	unsigned char modifier;
	unsigned char byte;
	unsigned char bit;
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
