// server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestApp:
// �йش����ʵ�֣������ server.cpp
//

class CtestApp : public CWinApp
{
public:
	CtestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

// �˳�
public:
	virtual int ExitInstance();
};

extern CtestApp theApp;