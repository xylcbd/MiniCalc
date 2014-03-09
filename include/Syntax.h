#ifndef _SYNTAX_H_
#define _SYNTAX_H_

#include "../include/Parser.h"

typedef struct _SYNTAXNODE_
{
	TOKEN* pToken;
	_SYNTAXNODE_* lChild;
	_SYNTAXNODE_* rChild;
}SYNTAXNODE;
typedef SYNTAXNODE SYNTAXTREE;

SYNTAXTREE* SyntaxAnalyse(TOKEN_LINK_LIST* pTokenList);

extern bool g_bSyntaxError;

#endif //_SYNTAX_H_