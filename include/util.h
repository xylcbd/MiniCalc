#ifndef _UTIL_H_
#define _UTIL_H_

//mypow(2,10)
inline double mypow(double x, int times,double curvalue = 1.0f)
{
	if (times == 0)
	{
		return curvalue;
	}
	times--;
	curvalue *= x;
	return mypow(x,times,curvalue);
}
inline double Translate2Num(const char* pNumber, int nLen)
{
	double result = 0;
	if (nLen == 0)
	{
		result = pNumber[0] - '0';
	}
	else
	{
		//.32 => 0.32
		//"12345.213+0)" => 12345.213
		int dotpos = -1;
		for (int i = nLen - 1; i >= 0; i--)
		{
			if (pNumber[i] == '.')
			{
				dotpos = nLen - i;
				break;
			}
		}

		double nStep = 1.0f;
		if (dotpos != -1)
			nStep /= mypow(10.0f, dotpos-1);
		for (int i = nLen-1; i >= 0; i--)
		{
			if (pNumber[i] != '.')
			{
				result += (pNumber[i] - '0') * nStep;
				nStep *= 10;
			}
		}
	}
	return result;
}

#endif //_UTIL_H_