#pragma once
#include "afxcmn.h"
#include "serverDlg.h"


// CPage1 �Ի���

class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage1();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnNMDblclkListNoidentify(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CListCtrl m_list_noidentify;

public:
	afx_msg void OnNMClickListNoidentify(NMHDR *pNMHDR, LRESULT *pResult);
};
