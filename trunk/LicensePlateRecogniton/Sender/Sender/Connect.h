#pragma once
#include "afxcmn.h"
#include "Sender.h"


// CConnect �Ի���

class CConnect : public CDialog
{
	DECLARE_DYNAMIC(CConnect)

public:
	CConnect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConnect();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
