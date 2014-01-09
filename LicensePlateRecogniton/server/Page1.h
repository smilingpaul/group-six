#pragma once
#include "afxcmn.h"
#include "serverDlg.h"


// CPage1 对话框

class CPage1 : public CDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnNMDblclkListNoidentify(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CListCtrl m_list_noidentify;

public:
	afx_msg void OnNMClickListNoidentify(NMHDR *pNMHDR, LRESULT *pResult);
};
