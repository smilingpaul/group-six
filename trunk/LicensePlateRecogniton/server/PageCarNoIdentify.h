#pragma once


// CPageCarNoIdentify 对话框

class CPageCarNoIdentify : public CDialog
{
	DECLARE_DYNAMIC(CPageCarNoIdentify)

public:
	CPageCarNoIdentify(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageCarNoIdentify();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
