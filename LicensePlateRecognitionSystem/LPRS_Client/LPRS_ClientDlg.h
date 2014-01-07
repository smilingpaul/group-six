
// LPRS_ClientDlg.h : ͷ�ļ�
//

#pragma once

#include <Winsock2.h>
#include "afxwin.h"
#include "afxcmn.h"

// LPRS_ClientDlg �Ի���

class LPRS_ClientDlg : public CDialog
{
	DECLARE_DYNAMIC(LPRS_ClientDlg)

public:
	LPRS_ClientDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LPRS_ClientDlg();

    // �Ի�������
	enum { IDD = IDD_LPRS_CLIENT_DIALOG };


	SOCKET  m_hSocket;
	CString m_strFileName;
	CString	m_strPathName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void  InitSock();

public:
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
	CIPAddressCtrl m_cSvrIP;
	int m_iPort;
	CProgressCtrl m_Progress;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnbrows();
	afx_msg void OnBnClickedBtndelete();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnstop();
	afx_msg void OnBnClickedCancel();
	CListCtrl m_ListControl;
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticlast();
};
