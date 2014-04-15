#include <cstdio>
#include <cstring>
#include <cassert>
#include "../include/version.h"
#include "../include/parser.h"
#include "../include/Syntax.h"
#include "../include/Walker.h"

using namespace MiniCalc;

//version 2014/03/08
//only support : 
//输入   ： 数字全部为整数
//运算符 ： ()+-*/
//输出   ： 小数
//注意   ： 输入式子中不含空格 ，不支持负号， 每个数字不超过sizeof(long long)
//sample (1+2)*5+2/3

#define SHOW_TOKENS 0
#define SHOW_SYNTAX 0

int CalcMain(const char* pStateMent)
{
	printf("input statement is : %s \n\n", pStateMent);

	CParser Parser;
	CSyntax Syntax;

	printf("parse statement begin ...\n");
	if (!Parser.Parse(pStateMent))
	{
		printf("parse error\n");
		return -1;
	}
	const TOKEN_LINK_LIST* pTokenList = Parser.GetResult();
	assert(pTokenList);
#if SHOW_TOKENS
	if (pTokenList)
	{
		Parser.ShowTokenList(pTokenList);
	}
#endif
	printf("parse statement end ...\n\n");

	printf("analyze syntax begin ...\n");
	if (!Syntax.SyntaxAnalyse(pTokenList))
	{
		printf("syntax analyze error\n");
		return -1;
	}
	const SYNTAXTREE* pSynTaxTree = Syntax.GetResult();
	assert(pSynTaxTree);
#if SHOW_SYNTAX
	if (pSynTaxTree)
	{
		Syntax.ShowSyntaxTree(pSynTaxTree);
	}
#endif
	printf("analyze syntax end ...\n\n");

	long long llResult = CWalker::GetResult(pSynTaxTree);
	printf("%s=%I64d\n\n", pStateMent, llResult);
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

	char strStatement[1024];
	printf(">:");
	while (gets(strStatement))
	{
		CalcMain(strStatement);
		printf(">:");
	}

	return 0;
}