#pragma once
#include "supplatectrl1.h"
#include "afxwin.h"


// CRecDlg �Ի���

class CRecDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecDlg)

public:
	CRecDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString GetPlateNum(CString filePath);
public:
	CSupplatectrl1 m_recControl;
public:
	afx_msg void OnBnClickedButton1();
public:
	CString m_filePath;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	afx_msg void OnBnClickedButton2();
public:
	CStatic m_Hide;
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	UINT_PTR p_timer;
};
