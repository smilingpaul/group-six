
// LPRS_ClientDlg.h : ͷ�ļ�
//

#pragma once
#include <Winsock2.h>
#include "afxwin.h"
#include "afxcmn.h"
#include "DAL.h"


// CLPRS_ClientDlg �Ի���
class CLPRS_ClientDlg : public CDialogEx
{
// ����
public:
	CLPRS_ClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LPRS_CLIENT_DIALOG };

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
	DECLARE_MESSAGE_MAP()


};
