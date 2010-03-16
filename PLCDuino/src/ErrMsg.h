/*
 * ErrMsg.h
 *
 *  Created on: 15/03/2010
 *      Author: Renato
 */

#ifndef ERRMSG_H_
#define ERRMSG_H_

#define MSG_OPSTACK_OVERFLOW "Op Stk Ovflw"
#define MSG_OPSTACK_UNDERFLOW "Op Stk Unflw"
#define MSG_VALSTACK_OVERFLOW "Val Stk Ovflw"
#define MSG_VALSTACK_UNDERFLOW "Val Stk Unflw"
#define MSG_ILLEGAL_INSTRUCTION "Ill Instruct"
#define MSG_ILLEGAL_MEMORY_ADDRESS "Ill Mem Addr"
#define	MSG_PARSE_ERROR "Parse Err"
#define MSG_PARSE_ERR_BADCHAR "Parse Bad Chr"
#define MSG_PARSE_ERR_BADFILE	"Parse Bad File"
#define	MSG_PARSE_ERR_BADOPERAND "Parse Bad Oprnd"
#define	MSG_PARSE_ERR_BADINDEX "Parse Bad Idx"
#define	MSG_PARSE_ERR_BADCOIL	"Parse Bad Coil"
#define	MSG_PARSE_ERR_BADOPERATOR	"Parse Bad Oprtr"

void doAbort(char *msg);

#endif /* ERRMSG_H_ */
