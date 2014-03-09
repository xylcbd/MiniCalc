#ifndef _PARSER_H_
#define _PARSER_H_

enum PARSER_STATE
{
	STATE_STOP,
	STATE_START,
	STATE_APPED,
	STATE_COUNT
};

enum VALUE_TYPE
{
	TYPE_UNKNOWN,
	TYPE_NUMBER,		// 234 , 10 ,etc.
	TYPE_OP_PLUS,		// +
	TYPE_OP_MINUS,		// -
	TYPE_OP_TIME,		// *
	TYPE_OP_DIVIDE,		// /
	TYPE_LEFT_BRACKET,	// (
	TYPE_RIGHT_BRACKET	// )
};


typedef struct _TOKEN_
{
	VALUE_TYPE type;
	long long value;
	const char* pStrPos;

	_TOKEN_* pNext;
}TOKEN;

//first node is the header , value is invalidate
typedef TOKEN TOKEN_LINK_LIST;

TOKEN_LINK_LIST* Parse(const char* pStateMent);
extern bool g_bParseError;

void ShowToken(const TOKEN* pToken);

#endif //_PARSER_H_