
// CTPMFCTEST.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCTPMFCTESTApp:
// �йش����ʵ�֣������ CTPMFCTEST.cpp
//

class CCTPMFCTESTApp : public CWinAppEx
{
public:
	CCTPMFCTESTApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCTPMFCTESTApp theApp;