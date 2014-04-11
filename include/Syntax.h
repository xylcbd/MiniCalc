#ifndef _SYNTAX_H_
#define _SYNTAX_H_

namespace MiniCalc
{
	struct SYNTAXNODE
	{
		const TOKEN* pToken = nullptr;
		SYNTAXNODE* lChild = nullptr;
		SYNTAXNODE* rChild = nullptr;
	};
	typedef SYNTAXNODE SYNTAXTREE;

	class CSyntax
	{
	public:
		CSyntax() = default;
		CSyntax(const CSyntax& other) = delete;
		CSyntax(CSyntax&& other) = delete;
		~CSyntax();
		CSyntax& operator=(const CSyntax& other) = delete;
		CSyntax& operator=(const CSyntax&& other) = delete;

	public:
		bool SyntaxAnalyse(const TOKEN_LINK_LIST* pTokenList);
		const SYNTAXTREE* GetResult() const;
		static void ShowSyntaxTree(const SYNTAXTREE* pSyntaxTree);
	private:
		SYNTAXNODE* Analyse();
		SYNTAXNODE* AnalyseExp();
		SYNTAXNODE* AnalyseTerm();
		SYNTAXNODE* AnalyseFactor();
		bool Match(const TOKEN_TYPE type);

		void ShowSyntaxError(const TOKEN_TYPE type)  const;
	private:
		//current token
		const TOKEN* m_pCurToken = nullptr;
		//syntax analyze tree
		SYNTAXTREE* m_pSyntaxTree = nullptr;
		//syntax analyze error flag
		bool m_bSyntaxError = false;
	};
};

#endif //_SYNTAX_H_