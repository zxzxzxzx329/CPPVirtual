#include "Common.h"

// charתWCAHR  
void C2W(const char* szSrc, WCHAR* wszDst, int nMaxLen/*wszDst���ַ�����*/)
{
	int vMinLen = MultiByteToWideChar(CP_ACP, 0, szSrc, -1, NULL, 0);
	if(vMinLen > nMaxLen)
	{
		MessageBoxA(NULL, szSrc, "ת����UNICODE�ִ�ʧ��", MB_ICONWARNING);
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, szSrc, -1, wszDst, vMinLen); 
}

// WCHARתchar 
void W2C(const WCHAR* wszSrc, char* szDst, int nMaxLen/*szDst���ַ�����*/)
{
	int vMinLen = WideCharToMultiByte(CP_ACP, 0, wszSrc, -1, NULL, 0, NULL, FALSE);
	if(vMinLen > nMaxLen)
	{
		MessageBoxW(NULL, wszSrc, L"ת����ANSI�ִ�ʧ��", MB_ICONWARNING);
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, wszSrc, -1, szDst, vMinLen, NULL, FALSE);
}

CString ANSIToUnicode(char * szChar)
{
	int len = 0;
	len = MultiByteToWideChar(CP_ACP, 0, szChar, -1, NULL, 0);
	TCHAR * p;
	p = new TCHAR[len + 1];
	memset(p, 0, (len + 1) * sizeof(TCHAR));
	MultiByteToWideChar(CP_ACP, 0, szChar, -1, p, len);

	CString ss;
	ss.Format(_T("%s"), p);
	delete[]p;
	return ss;
}
string UnicodeToANSI(CString & szStr)
{
	int len = 0; 
	len = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0,NULL,NULL);

	char * pElementText;
	pElementText = new char[len + 1];
	memset(pElementText, 0, sizeof(len + 1));
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pElementText, len, NULL, NULL);

	string ss;
	ss = pElementText;
	delete[]pElementText;
	return ss;

}
