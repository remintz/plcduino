/*
 * ErrMsg.h
 *
 *  Created on: 15/03/2010
 *      Author: Renato
 */

#ifndef ERRMSG_H_
#define ERRMSG_H_

const char MSG_OPSTACK_OVERFLOW[] = "Op Stk Ovflw";
const char MSG_OPSTACK_UNDERFLOW[] = "Op Stk Unflw";
const char MSG_VALSTACK_OVERFLOW[] = "Val Stk Ovflw";
const char MSG_VALSTACK_UNDERFLOW[] = "Val Stk Unflw";
const char MSG_ILLEGAL_INSTRUCTION[] = "Ill Instruct";
const char MSG_ILLEGAL_MEMORY_ADDRESS[] = "Ill Mem Addr";
const char MSG_PARSE_ERROR[] = "Parse Err";
const char MSG_PARSE_ERR_BADCHAR[] = "Parse Bad Chr";
const char MSG_PARSE_ERR_BADFILE[] = "Parse Bad File";
const char MSG_PARSE_ERR_BADOPERAND[] = "Parse Bad Oprnd";
const char MSG_PARSE_ERR_BADINDEX[] = "Parse Bad Idx";
const char MSG_PARSE_ERR_BADCOIL[] = "Parse Bad Coil";
const char MSG_PARSE_ERR_BADOPERATOR[] = "Parse Bad Oprtr";

void doAbort(const char *msg);

#endif /* ERRMSG_H_ */
