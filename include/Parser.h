#ifndef _PARSER_H_
#define _PARSER_H_

namespace MiniCalc
{
	enum class TOKEN_TYPE
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

	struct TOKEN
	{
		//data
		TOKEN_TYPE	Type = TOKEN_TYPE::TYPE_UNKNOWN;
		long long	llValue = 0;
		const char* pStrPos = nullptr;
		//link
		TOKEN*		pNext = nullptr;
	};
	typedef TOKEN TOKEN_LINK_LIST;

	class CParser
	{
	public:
		CParser() = default;
		CParser(const CParser& other) = delete;
		CParser(CParser&& other) = delete;
		~CParser();
		CParser& operator=(const CParser& other) = delete;
		CParser& operator=(const CParser&& other) = delete;
	public:
		bool Parse(const char* pStateMent);
		const TOKEN_LINK_LIST* GetResult() const;

		static void ShowTokenValue(const TOKEN* pToken);
		static void ShowToken(const TOKEN* pToken);
		static void ShowTokenList(const TOKEN_LINK_LIST* pTokenList);
	private:
		TOKEN* GetNextToken(const char* pStateMent);
	private:
		//current parser position
		int m_nParsePos = 0;
		//parser node link table
		TOKEN_LINK_LIST* m_pTokenList = nullptr;
		//parser  error flag
		bool m_bParseError = false;
	};
};

#endif //_PARSER_H_