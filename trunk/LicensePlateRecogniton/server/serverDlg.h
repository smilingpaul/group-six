// serverDlg.h : ͷ�ļ�
//
#if !defined(AFX_TESTDLG_H__CA158545_90FE_4D51_8EB9_8AD271AFBB86__INCLUDED_)
#define AFX_TESTDLG_H__CA158545_90FE_4D51_8EB9_8AD271AFBB86__INCLUDED_

#include "afxwin.h"
#include "stdafx.h"
#include "winsock2.h"
#include "afxcmn.h"
#include "Page1.h"
#include "Page2.h"
#include "recdlg.h"

#if _MSC_VER > 1000
#pragma once
#endif

#define	WM_RECVDATA		WM_USER+1
struct RECVPARAM
{
	SOCKET sock;
	HWND hwnd;
};
struct accepSocket
{
	SOCKET sock;
	int enable;
};

// CtestDlg �Ի���
class CtestDlg : public CDialog
{
// ����
public:
	CtestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRecvData(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedBtnStart();
public:
	afx_msg void OnBnClickedBtnStop();
public:
	afx_msg void OnBnClickedBtnExit();
public:
	afx_msg void OnBnClickedBtnShrink();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	static DWORD WINAPI AfterConnect(LPVOID lpParameter);
	BOOL InitSocket(int );
public:
	int recvFlag;
public:
	SOCKET m_socket;
public:
	int m_port;
public:
	afx_msg void OnOpen();
public:
	char *lpImageFile; //ͼ���ļ���
	HWND hWnd ; //Ҫ���Ӷ�����
	int nScrWidth ; //��ʾ���
	int nScrHeight ; //��ʾ�߶�
	
	HRESULT DisplayPic(CString lpImageFile, HWND hWnd, int nScrWidth, int nScrHeight);
public:
	afx_msg void On32776();

public:
	CTabCtrl m_tab;
	CDialog *pDialog[3];
	int m_CurSelTab;
	CDialog m_page1,m_page2;
//public:
//	CPage1 m_page;
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void OnClickPic(CString str);
	void OnClickIdenPic(CString str);
public:
	afx_msg void OnBnClickedBtnIdentify();
public:
	afx_msg void OnBnClickedBtnSave();

public:
	CRecDlg m_RecDlg;
public:
	CRecDlg* pDlg;
public:
	CRecDlg m_recDlg;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	CEdit m_showNum;
public:
	CString m_searchInfo;
public:
	afx_msg void OnBnClickedButton5();
public:
	CComboBox m_searchItem;
public:
	CListCtrl m_peccancyList;
public:
	afx_msg void OnBnClickedBtnConfirm();
public:
	CEdit m_showTime;
public:
	afx_msg void OnBnClickedBtnConvert();
};
#endif