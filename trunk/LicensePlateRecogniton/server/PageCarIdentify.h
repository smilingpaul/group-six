#pragma once


// CPageCarIdentify �Ի���

class CPageCarIdentify : public CDialog
{
	DECLARE_DYNAMIC(CPageCarIdentify)

public:
	CPageCarIdentify(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageCarIdentify();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
