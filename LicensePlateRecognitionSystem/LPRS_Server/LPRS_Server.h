
// LPRS_Server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLPRS_ServerApp:
// �йش����ʵ�֣������ LPRS_Server.cpp
//

class CLPRS_ServerApp : public CWinApp
{
public:
	CLPRS_ServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLPRS_ServerApp theApp;