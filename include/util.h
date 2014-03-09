#ifndef _UTIL_H_
#define _UTIL_H_

inline long long Translate2Num(const char* pNumber, int nLen)
{
	long long result = 0;
	if (nLen == 0)
	{
		result = pNumber[0] - '0';
	}
	else
	{
		//"12345+0)" => 12345
		int nStep = 1;
		for (int i = nLen - 1; i >= 0; i--, nStep *= 10)
		{
			result += (pNumber[i] - '0') * nStep;
		}
	}
	return result;
}

#endif //_UTIL_H_