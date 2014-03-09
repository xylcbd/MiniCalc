#include <stdio.h>

#include "../include/Syntax.h"

// 文法 : 
/*
exp		->	exp op1 exp | term op1 term
term	->	exp op2 exp
exp		->	(exp)
exp		->	number
number	->	digtial+
op1		->	+|-|
op2		->	*|/
digtial	->	0|1|2|3|4|5|6|7|8|9
*/
//number和op1 op2是终结符
//exp 为非终结符

bool g_bSyntaxError = false;
TOKEN* g_pCurToken = NULL;

void ShowSyntaxError(VALUE_TYPE type)
{
	if (g_bSyntaxError)
	{
		if (g_pCurToken)
		{
			switch (type)
			{
			case TYPE_NUMBER:
				printf("%s is not a number!\n", g_pCurToken->pStrPos);
				break;
			case TYPE_OP_PLUS:
				printf("%s is not a plus opcode!\n", g_pCurToken->pStrPos);
				break;
			case TYPE_OP_MINUS:
				printf("%s is not a minus opcode!\n", g_pCurToken->pStrPos);
				break;
			case TYPE_OP_TIME:
				printf("%s is not a time opcode!\n", g_pCurToken->pStrPos);
				break;
			case TYPE_OP_DIVIDE:
				printf("%s is not a divide opcode!\n", g_pCurToken->pStrPos);
				break;
			case TYPE_LEFT_BRACKET:
				printf("%s is not a left bracket!\n", g_pCurToken->pStrPos);
				break;
			case TYPE_RIGHT_BRACKET:
				printf("%s is not a right bracket!\n", g_pCurToken->pStrPos);
				break;
			}
		}
		else
		{
			switch (type)
			{
			case TYPE_NUMBER:
				printf("request a number!\n");
				break;
			case TYPE_OP_PLUS:
				printf("request a plus opcode!\n");
				break;
			case TYPE_OP_MINUS:
				printf("request a minus opcode!\n");
				break;
			case TYPE_OP_TIME:
				printf("request a time opcode!\n");
				break;
			case TYPE_OP_DIVIDE:
				printf("request a divide opcode!\n");
				break;
			case TYPE_LEFT_BRACKET:
				printf("request a left bracket!\n");
				break;
			case TYPE_RIGHT_BRACKET:
				printf("request a right bracket!\n");
				break;
			}
		}
		//g_bSyntaxError = false;
	}
}
inline bool Match(VALUE_TYPE type)
{
	if (g_pCurToken)
	{
		bool result = type == g_pCurToken->type;
		g_bSyntaxError = !result;
		ShowSyntaxError(type);
		g_pCurToken = g_pCurToken->pNext;
		return result;
	}
	else
	{
		g_bSyntaxError = true;
		ShowSyntaxError(type);
		return false;
	}
}

SYNTAXNODE * term(void);
SYNTAXNODE * factor(void);

SYNTAXNODE * exp(void)
{
	SYNTAXNODE * t = term();
	while (g_pCurToken && ((g_pCurToken->type == TYPE_OP_PLUS) || (g_pCurToken->type == TYPE_OP_MINUS)))
	{
		SYNTAXNODE * p = new SYNTAXNODE();
		p->lChild = t;
		p->pToken = g_pCurToken;
		t = p;
		Match(g_pCurToken->type);
		t->rChild = term();
	}
	return t;
}

SYNTAXNODE * term(void)
{
	SYNTAXNODE * t = factor();
	while (g_pCurToken && ((g_pCurToken->type == TYPE_OP_TIME) || (g_pCurToken->type == TYPE_OP_DIVIDE)))
	{
		SYNTAXNODE * p = new SYNTAXNODE();
		p->lChild = t;
		p->pToken = g_pCurToken;
		t = p;
		Match(g_pCurToken->type);
		p->rChild = factor();
	}
	return t;
}

SYNTAXNODE * factor(void)
{
	if (!g_pCurToken)
	{
		return NULL;
	}
	SYNTAXNODE * t = NULL;
	switch (g_pCurToken->type) {
	case TYPE_NUMBER:
		t = new SYNTAXNODE();
		t->pToken = g_pCurToken;
		t->lChild = NULL;
		t->rChild = NULL;
		Match(TYPE_NUMBER);
		break;
	case TYPE_LEFT_BRACKET:
		Match(TYPE_LEFT_BRACKET);
		t = exp();
		Match(TYPE_RIGHT_BRACKET);
		break;
	default:
		break;
	}
	return t;
}
///*
//exp		->	exp op1 exp
//exp		->	term
//term	->	exp op2 exp
//exp		->	(exp)
//exp		->	number
//number	->	digtial+
//op1		->	+|-|
//op2		->	*|/
//digtial	->	0|1|2|3|4|5|6|7|8|9
//*/
//SYNTAXNODE* exp()
//{
//	SYNTAXNODE* pSyntaxNode = new SYNTAXNODE();
//	VALUE_TYPE curType = g_pCurToken->type;
//	switch (curType)
//	{
//	//exp		->	number
//	case TYPE_NUMBER:
//		pSyntaxNode->pToken = g_pCurToken;
//		pSyntaxNode->lChild = NULL;
//		pSyntaxNode->rChild = NULL;
//		break;
//	//exp		->	exp op1 exp
//	case TYPE_OP_PLUS:
//	case TYPE_OP_MINUS:
//		pSyntaxNode->lChild = exp();
//		Match(curType);
//		pSyntaxNode->rChild = exp();
//		break;
//	case TYPE_OP_TIME:
//	case TYPE_OP_DIVIDE:
//		break;
//	case TYPE_LEFT_BRACKET:
//		break;
//	case TYPE_RIGHT_BRACKET:
//		break;
//	default:
//		g_bSyntaxError = true;
//		break;
//	}
//	return pSyntaxNode;
//}
///*
//exp		->	exp op1 exp | term op1 term
//term	->	exp op2 exp
//exp		->	(exp)
//exp		->	number
//number	->	digtial+
//op1		->	+|-|
//op2		->	*|/
//digtial	->	0|1|2|3|4|5|6|7|8|9
//*/
//SYNTAXNODE* term()
//{
//	SYNTAXNODE* pSyntaxNode = new SYNTAXNODE();
//	TOKEN* pNext = g_pCurToken+1;
//	//term	->	exp op2 exp
//	if (pNext->type == TYPE_OP_TIME || pNext->type == TYPE_OP_DIVIDE)
//	{
//		pSyntaxNode->pToken = g_pCurToken;
//
//	}
//	return pSyntaxNode;
//}

SYNTAXNODE* Analyse()
{
	g_pCurToken = g_pCurToken->pNext;
	return exp();
}
SYNTAXTREE* SyntaxAnalyse(TOKEN_LINK_LIST* pTokenList)
{
	g_pCurToken = pTokenList;
	SYNTAXTREE* pSyntaxTree = Analyse();
	if (g_bSyntaxError)
	{
		printf("Syntax analyse error \n");
	}
	return pSyntaxTree;
}