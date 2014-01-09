// SenderDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSenderDlg 对话框
class CSenderDlg : public CDialog
{
// 构造
public:
	CSenderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SENDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrowse();
public:
	afx_msg void OnBnClickedBtnSeleteall();
public:
	CListCtrl m_list;
public:
	afx_msg void OnLvnItemchangedListPic(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedBtnSend();
public:
	afx_msg void OnBnClickedBtnPause();
public:
	afx_msg void OnBnClickedBtnOppselete();
public:
	afx_msg void OnBnClickedBtnContinue();
public:
	afx_msg void OnBnClickedBtnDelete();
public:
	afx_msg void OnBnClickedBtnExit();
public:
	CString GetFileName(const CString & strFilePath);
public:
	CProgressCtrl m_progress;
public:
	SOCKET g_socket;
public:
	CIPAddressCtrl m_addressctrlRemb;
public:
	int m_portRemb;
public:
	SOCKET sender_socket;
public:
	HRESULT DisplayPic(CString lpImageFile, HWND hWnd, int nScrWidth, int nScrHeight);

public:
	CStatic cProgNum;
public:
	CString progNum;
public:
	afx_msg void OnNMClickListPic(NMHDR *pNMHDR, LRESULT *pResult);
};
