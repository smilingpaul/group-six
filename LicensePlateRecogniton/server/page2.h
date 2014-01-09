#pragma once
#include "afxcmn.h"
#include "serverDlg.h"


// CPage1 对话框

class CPage2 : public CDialog
{
	DECLARE_DYNAMIC(CPage2)

public:
	CPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage2();

	// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_list_identify;

public:
	afx_msg void OnNMClickListIdentify(NMHDR *pNMHDR, LRESULT *pResult);
};
