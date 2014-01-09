#pragma once
#include "afxcmn.h"
#include "Sender.h"


// CConnect 对话框

class CConnect : public CDialog
{
	DECLARE_DYNAMIC(CConnect)

public:
	CConnect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConnect();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnect();
public:
	SOCKET connect_socket;
public:
	afx_msg void OnBnClickedButtonExit();
public:
	CIPAddressCtrl m_addressCtrl;
public:
	int m_port;
 public:
 	BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual BOOL OnInitDialog();
};
