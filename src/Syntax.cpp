#include <iostream>
#include <cassert>
#include <queue>

#include "../include/Parser.h"
#include "../include/Syntax.h"

using namespace std;
using namespace MiniCalc;

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
CSyntax::~CSyntax()
{
	if (m_pSyntaxTree)
	{
		queue<SYNTAXNODE*> nodeque;
		//root node
		nodeque.push(m_pSyntaxTree);
		while (!nodeque.empty())
		{
			SYNTAXNODE* pCurrent = nodeque.front();
			SYNTAXNODE* pCurLChild = pCurrent->lChild;
			SYNTAXNODE* pCurRChild = pCurrent->rChild;
			delete pCurrent;
			nodeque.pop();

			if (pCurLChild)
			{
				nodeque.push(pCurLChild);
			}
			if (pCurRChild)
			{
				nodeque.push(pCurRChild);
			}
		}
	}
}
void CSyntax::ShowSyntaxTree(const SYNTAXTREE* pSyntaxTree)
{
	if (!pSyntaxTree)
	{
		return;
	}
	
	queue<const SYNTAXNODE*> nodeque;
	//root node
	nodeque.push(pSyntaxTree);
	while (!nodeque.empty())
	{
		const SYNTAXNODE* pCurrent = nodeque.front();
		CParser::ShowTokenValue(pCurrent->pToken);
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

void CSyntax::ShowSyntaxError(const TOKEN_TYPE Type) const
{
	if (m_pCurToken)
	{
		cout << m_pCurToken->pStrPos;
		switch (Type)
		{
		case TOKEN_TYPE::TYPE_NUMBER:
			cout << " is not a number!" << endl;
			break;
		case TOKEN_TYPE::TYPE_OP_PLUS:
			cout << " is not a plus opcode!" << endl;
			break;
		case TOKEN_TYPE::TYPE_OP_MINUS:
			cout << " is not a minus opcode!" << endl;
			break;
		case TOKEN_TYPE::TYPE_OP_TIME:
			cout << " is not a time opcode!" << endl;
			break;
		case TOKEN_TYPE::TYPE_OP_DIVIDE:
			cout << " is not a divide opcode!" << endl;
			break;
		case TOKEN_TYPE::TYPE_LEFT_BRACKET:
			cout << " is not a left bracket!" << endl;
			break;
		case TOKEN_TYPE::TYPE_RIGHT_BRACKET:
			cout << " is not a right bracket!" << endl;
			break;
		default:
			assert(false);
			break;
		}
	}
	else
	{
		switch (Type)
		{
		case TOKEN_TYPE::TYPE_NUMBER:
			cout << "request a number!" << endl;
			break;
		case TOKEN_TYPE::TYPE_OP_PLUS:
			cout << "request a plus opcode!" << endl;
			break;
		case TOKEN_TYPE::TYPE_OP_MINUS:
			cout << "request a minus opcode!" << endl;
			break;
		case TOKEN_TYPE::TYPE_OP_TIME:
			cout << "request a time opcode!" << endl;
			break;
		case TOKEN_TYPE::TYPE_OP_DIVIDE:
			cout << "request a divide opcode!" << endl;
			break;
		case TOKEN_TYPE::TYPE_LEFT_BRACKET:
			cout << "request a left bracket!" << endl;
			break;
		case TOKEN_TYPE::TYPE_RIGHT_BRACKET:
			cout << "request a right bracket!" << endl;
			break;
		default:
			assert(false);
			break;
		}
	}
}
inline bool CSyntax::Match(const TOKEN_TYPE Type)
{
	if (m_pCurToken)
	{
		m_bSyntaxError = (Type != m_pCurToken->Type);
		if (m_bSyntaxError)
		{
			ShowSyntaxError(Type);
		}
		m_pCurToken = m_pCurToken->pNext;
		return m_bSyntaxError;
	}
	else
	{
		m_bSyntaxError = true;
		ShowSyntaxError(Type);
		return false;
	}
}

SYNTAXNODE * CSyntax::AnalyseExp()
{
	SYNTAXNODE * t = AnalyseTerm();
	while (m_pCurToken && ((m_pCurToken->Type == TOKEN_TYPE::TYPE_OP_PLUS) || (m_pCurToken->Type == TOKEN_TYPE::TYPE_OP_MINUS)))
	{
		SYNTAXNODE * p = new SYNTAXNODE();
		assert(p);
		p->lChild = t;
		p->pToken = m_pCurToken;
		t = p;
		Match(m_pCurToken->Type);
		t->rChild = AnalyseTerm();
	}
	return t;
}

SYNTAXNODE * CSyntax::AnalyseTerm(void)
{
	SYNTAXNODE * t = AnalyseFactor();
	while (m_pCurToken && ((m_pCurToken->Type == TOKEN_TYPE::TYPE_OP_TIME) || (m_pCurToken->Type == TOKEN_TYPE::TYPE_OP_DIVIDE)))
	{
		SYNTAXNODE * p = new SYNTAXNODE();
		assert(p);
		p->lChild = t;
		p->pToken = m_pCurToken;
		t = p;
		Match(m_pCurToken->Type);
		p->rChild = AnalyseFactor();
	}
	return t;
}

SYNTAXNODE * CSyntax::AnalyseFactor(void)
{
	if (!m_pCurToken)
	{
		return nullptr;
	}
	SYNTAXNODE * p = nullptr;
	switch (m_pCurToken->Type) {
	case TOKEN_TYPE::TYPE_NUMBER:
		p = new SYNTAXNODE();
		assert(p);
		p->pToken = m_pCurToken;
		p->lChild = nullptr;
		p->rChild = nullptr;
		Match(TOKEN_TYPE::TYPE_NUMBER);
		break;
	case TOKEN_TYPE::TYPE_LEFT_BRACKET:
		Match(TOKEN_TYPE::TYPE_LEFT_BRACKET);
		p = AnalyseExp();
		Match(TOKEN_TYPE::TYPE_RIGHT_BRACKET);
		break;
	default:
		break;
	}
	return p;
}
SYNTAXNODE* CSyntax::Analyse()
{
	m_pCurToken = m_pCurToken->pNext;
	return AnalyseExp();
}
bool CSyntax::SyntaxAnalyse(const TOKEN_LINK_LIST* pTokenList)
{
	m_pCurToken = pTokenList;
	m_pSyntaxTree = Analyse();
	if (m_bSyntaxError)
	{
		cout<<"Syntax analyze error \n";
	}
	return !m_bSyntaxError;
}
const SYNTAXTREE* CSyntax::GetResult() const
{
	return m_pSyntaxTree;
}