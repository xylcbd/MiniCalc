#include "../include/Parser.h"
#include "../include/Syntax.h"
#include "../include/Walker.h"
#include <iostream>
#include <queue>
#include <stack>

using namespace std;
using namespace MiniCalc;

long long CWalker::GetResult(const SYNTAXNODE* pSyntaxNode)
{
	if (!pSyntaxNode)
	{
		return 0;
	}
	long long llResult = 0;
	const SYNTAXNODE* pCurrent = pSyntaxNode;

	if (pCurrent->pToken->Type == TOKEN_TYPE::TYPE_NUMBER)
	{
		llResult = pCurrent->pToken->llValue;
	}
	else
	{
		long long llLValue = GetResult(pCurrent->lChild);
		long long llRValue = GetResult(pCurrent->rChild);
		switch (pCurrent->pToken->Type)
		{
		case TOKEN_TYPE::TYPE_OP_PLUS:
			llResult = llLValue + llRValue;
			break;
		case TOKEN_TYPE::TYPE_OP_MINUS:
			llResult = llLValue - llRValue;
			break;
		case TOKEN_TYPE::TYPE_OP_TIME:
			llResult = llLValue * llRValue;
			break;
		case TOKEN_TYPE::TYPE_OP_DIVIDE:
			if (llRValue != 0)
			{
				llResult = llLValue / llRValue;
			}
			break;
		default:
			break;
		}
	}
	return llResult;
}