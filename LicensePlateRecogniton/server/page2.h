#pragma once
#include "afxcmn.h"
#include "serverDlg.h"


// CPage1 �Ի���

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage2();

	// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_list_identify;

public:
	afx_msg void OnNMClickListIdentify(NMHDR *pNMHDR, LRESULT *pResult);
};
