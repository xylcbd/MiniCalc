#include <string.h>
#include <stdio.h>

#include "../include/Parser.h"
#include "../include/util.h"

//number | op_code | bracket
PARSER_STATE g_ParserStateTbl[STATE_COUNT*3] = 
{
	STATE_START,STATE_START,STATE_START,
	STATE_APPED,STATE_STOP,STATE_STOP,
	STATE_STOP, STATE_STOP, STATE_STOP,
};

//global varlues
int g_nPos = 0;
bool g_bParseError = false;

inline bool IsVilidateChar(const char instr)
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
TOKEN* GetNextToken(const char* pStateMent)
{
	TOKEN* pToken = new TOKEN();
	PARSER_STATE state = STATE_START;
	int nNumStartPos = 0;
	int nNumStopPos = 0;
	bool done = false;
	while (!g_bParseError && !done)
	{
		if (pStateMent[g_nPos] == '\0')
		{
			nNumStopPos = g_nPos - 1;
			done = true;
			break;
		}else if (!IsVilidateChar(pStateMent[g_nPos]))
		{
			g_bParseError = true;
			break;
		}
		switch (state)
		{
		case STATE_STOP:
			nNumStopPos = g_nPos - 1;
			done = true;
			break;
		case STATE_START:
		{
			char curchar = pStateMent[g_nPos++];
			switch (curchar)
			{
			case '+':
				pToken->type = TYPE_OP_PLUS;
				pToken->value = curchar;
				state = STATE_STOP;
				break;
			case '-':
				pToken->type = TYPE_OP_MINUS;
				pToken->value = curchar;
				state = STATE_STOP;
				break;
			case '*':
				pToken->type = TYPE_OP_TIME;
				pToken->value = curchar;
				state = STATE_STOP;
				break;
			case '/':
				pToken->type = TYPE_OP_DIVIDE;
				pToken->value = curchar;
				state = STATE_STOP;
				break;
			case '(':
				pToken->type = TYPE_LEFT_BRACKET;
				pToken->value = curchar;
				state = STATE_STOP;
				break;
			case ')':
				pToken->type = TYPE_RIGHT_BRACKET;
				pToken->value = curchar;
				state = STATE_STOP;
				break;
			default:
				if (curchar >= '0' && curchar <= '9')
				{
					pToken->type = TYPE_NUMBER;
					nNumStartPos = g_nPos - 1;
					state = STATE_APPED;
				}
				break;
			}
			break;
		}
		//lookahead
		case STATE_APPED:
		{
			char curchar = pStateMent[g_nPos++];
			if (curchar >= '0' && curchar <= '9')
			{
				continue;
			}
			else
			{
				nNumStopPos = g_nPos - 1;
				g_nPos--;
				done = true;
			}
			break;
		}
		default:
			g_bParseError = true;
			break;
		}
	}
	if (pToken->type == TYPE_NUMBER)
	{
		pToken->value = Translate2Num(&(pStateMent[nNumStartPos]),nNumStopPos-nNumStartPos);
		pToken->pStrPos = pStateMent + nNumStartPos;
	}
	else
	{
		pToken->pStrPos = pStateMent + g_nPos;
	}
	return pToken;
}
TOKEN_LINK_LIST* Parse(const char* pStateMent)
{
	TOKEN_LINK_LIST* pResult = new TOKEN_LINK_LIST();
	pResult->type = TYPE_UNKNOWN;
	pResult->value = 0;
	int nLength = strlen(pStateMent);

	TOKEN* pNextToken = GetNextToken(pStateMent);
	pResult->pNext = pNextToken;
	while (!g_bParseError && g_nPos < nLength)
	{
		pNextToken->pNext = GetNextToken(pStateMent);
		pNextToken = pNextToken->pNext;
	}
	if (g_bParseError)
	{
		printf("Parse error : %s \n", &(pStateMent[g_nPos]));
	}
	return pResult;
}
