
// LPRS_Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLPRS_ClientApp:
// �йش����ʵ�֣������ LPRS_Client.cpp
//

class CLPRS_ClientApp : public CWinApp
{
public:
	CLPRS_ClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLPRS_ClientApp theApp;