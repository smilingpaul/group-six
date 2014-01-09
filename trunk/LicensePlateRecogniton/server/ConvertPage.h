#pragma once
#include "afxcmn.h"


// CConvertPage 对话框

class CConvertPage : public CDialog
{
	DECLARE_DYNAMIC(CConvertPage)

public:
	CConvertPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConvertPage();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONVERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_convert;
};
