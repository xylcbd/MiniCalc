#include <stdio.h>
#include <string.h>
#include <queue>
#include <stack>
using namespace std;

#include "../include/version.h"
#include "../include/parser.h"
#include "../include/Syntax.h"

#define SHOW_TOKENS 0
#define SHOW_SYNTAX 0

//version 2014/03/08
//only support : 
//输入   ： 数字全部为整数
//运算符 ： ()+-*/
//输出   ： 小数
//注意   ： 输入式子中不含空格 ，不支持负号， 每个数字不超过sizeof(long long)
//sample (1+2)*5+2/3
void ShowToken(const TOKEN* pToken)
{
	switch (pToken->type)
	{
	case TYPE_NUMBER:
		printf("\tType : number \t\tValue : %I64d\n", pToken->value);
		break;
	case TYPE_OP_PLUS:
		printf("\tType : plus \t\tValue : +\n");
		break;
	case TYPE_OP_MINUS:
		printf("\tType : minus \t\tValue : -\n");
		break;
	case TYPE_OP_TIME:
		printf("\tType : time \t\tValue : *\n");
		break;
	case TYPE_OP_DIVIDE:
		printf("\tType : divide \t\tValue : /\n");
		break;
	case TYPE_LEFT_BRACKET:
		printf("\tType : left bracket \tValue : (\n");
		break;
	case TYPE_RIGHT_BRACKET:
		printf("\tType : right bracket \tValue : )\n");
		break;
	default: 
		break;
	}
}
void ShowToken_value(const TOKEN* pToken)
{
	switch (pToken->type)
	{
	case TYPE_NUMBER:
		printf("%I64d\n", pToken->value);
		break;
	case TYPE_OP_PLUS:
		printf("+\n");
		break;
	case TYPE_OP_MINUS:
		printf("-\n");
		break;
	case TYPE_OP_TIME:
		printf("*\n");
		break;
	case TYPE_OP_DIVIDE:
		printf("/\n");
		break;
	default:
		break;
	}
}
void ShowTokens(const TOKEN_LINK_LIST* pTokens)
{
	printf("list tokens begin : \n");
	const TOKEN* pCurrent = pTokens->pNext;
	while (pCurrent)
	{
		ShowToken(pCurrent);
		pCurrent = pCurrent->pNext;
	}
	printf("list tokens end : \n");
}
void ShowSyntaxTree(const SYNTAXNODE* pSyntaxNode)
{
	if (pSyntaxNode)
	{
		queue<const SYNTAXNODE*> nodeque;
		//root node
		nodeque.push(pSyntaxNode);
		while (!nodeque.empty())
		{
			const SYNTAXNODE* pCurrent = nodeque.front();
			ShowToken_value(pCurrent->pToken);
			nodeque.pop();

			if (pCurrent->lChild)
			{
				nodeque.push(pCurrent->lChild);
			}
			if (pCurrent->rChild)
			{
				nodeque.push(pCurrent->rChild);
			}
		}
	}
}
double GetResult(const SYNTAXNODE* pSyntaxNode)
{
	if (!pSyntaxNode)
	{
		return 0;
	}
	double result = 0;
	const SYNTAXNODE* pCurrent = pSyntaxNode;

	if (pCurrent->pToken->type == TYPE_NUMBER)
	{
		result = pCurrent->pToken->value;
	}else
	{
		double lValue = GetResult(pCurrent->lChild);
		double rValue = GetResult(pCurrent->rChild);
		switch (pCurrent->pToken->type)
		{
		case TYPE_OP_PLUS:
			result = lValue + rValue;
			break;
		case TYPE_OP_MINUS:
			result = lValue - rValue;
			break;
		case TYPE_OP_TIME:
			result = lValue * rValue;
			break;
		case TYPE_OP_DIVIDE:
			if (rValue != 0)
			{
				result = lValue / rValue;
			}
			break;
		default:
			break;
		}
	}
	return result;
}
int CalcMain(const char* pStateMent)
{
	printf("input statement is : %s \n\n", pStateMent);

	printf("parse statement begin ...\n");
	TOKEN_LINK_LIST* pTokenList = Parse(pStateMent);
	if (g_bParseError)
	{
		return -1;
	}
#if SHOW_TOKENS
	if (pTokenList)
	{
		ShowTokens(pTokenList);
	}
#endif
	printf("parse statement end ...\n\n");

	printf("analyse syntax begin ...\n");
	SYNTAXTREE* pSynTaxTree = SyntaxAnalyse(pTokenList);
	if (g_bSyntaxError)
	{
		return -1;
	}
#if SHOW_SYNTAX
	if (pSynTaxTree)
	{
		ShowSyntaxTree(pSynTaxTree);
	}
#endif
	printf("analyse syntax end ...\n\n");

	double result = GetResult(pSynTaxTree);
	printf("%s=%lf\n\n", pStateMent, result);
	return 0;
}

void help(const char* pModuleFile)
{
	const char* pPorcessName = strrchr(pModuleFile, '\\');
	pPorcessName++;
	printf("Usage:\n\t%s [statement]\n", pPorcessName);
}
int main(int argc, char* argv[])
{
	printf("MiniCalc version %s\n", MINICALC_VERSION);
	if (argc != 2)
	{
		help(argv[0]);
		return -1;
	}

	return CalcMain(argv[1]);
}