#pragma warning(disable : 4996)
#ifndef _COMMON_H_
#define _COMMON_H_

#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "stdafx.h"
#include <string>
using namespace std;

const int WM_SHOW_QUOTE = WM_USER + 0x200;							//��ʾ��Ʒ������Ϣ

// charתWCAHR  
void C2W(const char* szSrc, WCHAR* wszDst, int nMaxLen/*wszDst���ַ�����*/);

// WCHARתchar 
void W2C(const WCHAR* wszSrc, char* szDst, int nMaxLen/*szDst���ַ�����*/);

CString ANSIToUnicode(char * szChar);
string UnicodeToANSI(CString & szStr);

static int iRequestID = 0;											//������

#endif