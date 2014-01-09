#pragma once


// CPageCarIdentify 对话框

class CPageCarIdentify : public CDialog
{
	DECLARE_DYNAMIC(CPageCarIdentify)

public:
	CPageCarIdentify(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageCarIdentify();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
