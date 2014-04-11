#include <cassert>
#include <iostream>
#include <cstring>

#include "../include/Parser.h"
#include "../include/util.h"

using namespace std;
using namespace MiniCalc;

enum class PARSER_STATE
{
	STATE_STOP,
	STATE_START,
	STATE_APPED,
	STATE_COUNT
};

//number | op_code | bracket
/*PARSER_STATE g_ParserStateTbl[STATE_COUNT*3] = 
{
	STATE_START,STATE_START,STATE_START,
	STATE_APPED,STATE_STOP,STATE_STOP,
	STATE_STOP, STATE_STOP, STATE_STOP,
};*/

CParser::~CParser()
{
	TOKEN* pToken = m_pTokenList;
	TOKEN* pNextToken = nullptr;
	while (pToken)
	{
		pNextToken = pToken->pNext;
		delete pToken;
		pToken = pNextToken;
	}
}

void CParser::ShowToken(const TOKEN* pToken)
{
	if (!pToken)
	{
		return;
	}
	switch (pToken->Type)
	{
	case TOKEN_TYPE::TYPE_NUMBER:
		cout << "\tType : number \t\tValue : " << pToken->llValue << endl;
		break;
	case TOKEN_TYPE::TYPE_OP_PLUS:
		cout << "\tType : plus \t\tValue : +"<<endl;
		break;
	case TOKEN_TYPE::TYPE_OP_MINUS:
		cout << "\tType : minus \t\tValue : -"<<endl;
		break;
	case TOKEN_TYPE::TYPE_OP_TIME:
		cout << "\tType : time \t\tValue : *" << endl;
		break;
	case TOKEN_TYPE::TYPE_OP_DIVIDE:
		cout << "\tType : divide \t\tValue : /" << endl;
		break;
	case TOKEN_TYPE::TYPE_LEFT_BRACKET:
		cout << "\tType : left bracket \tValue : (" << endl;
		break;
	case TOKEN_TYPE::TYPE_RIGHT_BRACKET:
		cout << "\tType : right bracket \tValue : )" << endl;
		break;
	default:
		assert(false);
		break;
	}
}
void CParser::ShowTokenValue(const TOKEN* pToken)
{
	switch (pToken->Type)
	{
	case TOKEN_TYPE::TYPE_NUMBER:
		cout<<(pToken->llValue)<<endl;
		break;
	case TOKEN_TYPE::TYPE_OP_PLUS:
		cout<<"+"<<endl;
		break;
	case TOKEN_TYPE::TYPE_OP_MINUS:
		cout << "-" << endl;
		break;
	case TOKEN_TYPE::TYPE_OP_TIME:
		cout << "*" << endl;
		break;
	case TOKEN_TYPE::TYPE_OP_DIVIDE:
		cout << "/" << endl;
		break;
	default:
		break;
	}
}
void CParser::ShowTokenList(const TOKEN_LINK_LIST* pTokenList)
{
	cout<<"list tokens begin : "<<endl;
	const TOKEN* pCurrent = pTokenList->pNext;
	while (pCurrent)
	{
		ShowToken(pCurrent);
		pCurrent = pCurrent->pNext;
	}
	cout<<"list tokens end : "<<endl;
}

static inline bool IsVilidateChar(const char instr)
{
	if (
		(instr >= '0' && instr <= '9')
		|| instr == '+'
		|| instr == '-'
		|| instr == '*'
		|| instr == '/'
		|| instr == '('
		|| instr == ')'
	)
	{
		return true;
	}
	return false;
}
TOKEN* CParser::GetNextToken(const char* pStateMent)
{
	TOKEN* pToken = new TOKEN();
	PARSER_STATE state = PARSER_STATE::STATE_START;
	int nNumStartPos = 0;
	int nNumStopPos = 0;
	bool done = false;
	while (!m_bParseError && !done)
	{
		if (pStateMent[m_nParsePos] == '\0')
		{
			nNumStopPos = m_nParsePos - 1;
			done = true;
			break;
		}else if (!IsVilidateChar(pStateMent[m_nParsePos]))
		{
			m_bParseError = true;
			break;
		}
		switch (state)
		{
		case PARSER_STATE::STATE_STOP:
			nNumStopPos = m_nParsePos - 1;
			done = true;
			break;
		case PARSER_STATE::STATE_START:
		{
			char curchar = pStateMent[m_nParsePos++];
			switch (curchar)
			{
			case '+':
				pToken->Type = TOKEN_TYPE::TYPE_OP_PLUS;
				pToken->llValue = curchar;
				state = PARSER_STATE::STATE_STOP;
				break;
			case '-':
				pToken->Type = TOKEN_TYPE::TYPE_OP_MINUS;
				pToken->llValue = curchar;
				state = PARSER_STATE::STATE_STOP;
				break;
			case '*':
				pToken->Type = TOKEN_TYPE::TYPE_OP_TIME;
				pToken->llValue = curchar;
				state = PARSER_STATE::STATE_STOP;
				break;
			case '/':
				pToken->Type = TOKEN_TYPE::TYPE_OP_DIVIDE;
				pToken->llValue = curchar;
				state = PARSER_STATE::STATE_STOP;
				break;
			case '(':
				pToken->Type = TOKEN_TYPE::TYPE_LEFT_BRACKET;
				pToken->llValue = curchar;
				state = PARSER_STATE::STATE_STOP;
				break;
			case ')':
				pToken->Type = TOKEN_TYPE::TYPE_RIGHT_BRACKET;
				pToken->llValue = curchar;
				state = PARSER_STATE::STATE_STOP;
				break;
			default:
				if (curchar >= '0' && curchar <= '9')
				{
					pToken->Type = TOKEN_TYPE::TYPE_NUMBER;
					nNumStartPos = m_nParsePos - 1;
					state = PARSER_STATE::STATE_APPED;
				}
				break;
			}
			break;
		}
		//lookahead
		case PARSER_STATE::STATE_APPED:
		{
			char curchar = pStateMent[m_nParsePos++];
			if (curchar >= '0' && curchar <= '9')
			{
				continue;
			}
			else
			{
				nNumStopPos = m_nParsePos - 1;
				m_nParsePos--;
				done = true;
			}
			break;
		}
		default:
			m_bParseError = true;
			break;
		}
	}
	if (pToken->Type == TOKEN_TYPE::TYPE_NUMBER)
	{
		pToken->llValue = Translate2Num(&(pStateMent[nNumStartPos]),nNumStopPos-nNumStartPos);
		pToken->pStrPos = pStateMent + nNumStartPos;
	}
	else
	{
		pToken->pStrPos = pStateMent + m_nParsePos;
	}
	return pToken;
}
bool CParser::Parse(const char* pStateMent)
{
	m_pTokenList = new TOKEN_LINK_LIST();
	int nLength = strlen(pStateMent);

	TOKEN* pNextToken = GetNextToken(pStateMent);
	m_pTokenList->pNext = pNextToken;
	while (!m_bParseError && m_nParsePos < nLength)
	{
		pNextToken->pNext = GetNextToken(pStateMent);
		pNextToken = pNextToken->pNext;
	}
	if (m_bParseError)
	{
		cout<<"Parse error : "<< &(pStateMent[m_nParsePos]) <<endl;
	}
	return !m_bParseError;
}
const TOKEN_LINK_LIST* CParser::GetResult() const
{
	return m_pTokenList;
}
