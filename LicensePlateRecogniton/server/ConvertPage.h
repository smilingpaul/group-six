#pragma once
#include "afxcmn.h"


// CConvertPage �Ի���

class CConvertPage : public CDialog
{
	DECLARE_DYNAMIC(CConvertPage)

public:
	CConvertPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConvertPage();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONVERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_convert;
};
