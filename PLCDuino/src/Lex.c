/*
 * Lex.c
 *
 *  Created on: 09/03/2010
 *      Author: Renato
 */

#include <string.h>
#include "util.h"

typedef enum {
	ident,
	numeric_literal
} Symbol;

char delim[] = " \t\r\n";

Symbol sym;
char token[255];
char line[255];
int l;
char c;

char getAChar(void) {
	return line[l++];
}

void getNextChar(void) {
	char d;
	c = getAChar();
	if (strchr(delim, c)) {
		// skip adjacent delimiters
		d = c;
		while (strchr(delim, d))
			d = getAChar();
	}
}

void scanIdent() {
	while (isalpha(c) || isdigit(c) || c == '_') {
		token[t++] = c;
		c = getAChar();
	}
	sym = ident;
}

void scanNumber() {
	while (isdigit(c)) {
		token[t++] = c;
	}
	sym = numeric_literal;
}

void getsym(void) {
	int t;
	int operator;
	int i;

	memset(token,0,255);
	t = 0;
	if (isalpha(c)) {
		scanIdent();
		return;
	}
	if (isdigit(c)) {
		scanNumber();
		return;
	}
	switch (c) {
		// delimiters
	case ' ':
	case '\t':
	case '\n':
	case '\r':
	}
}

void error(const char msg);

