#pragma once


// CPageCarNoIdentify �Ի���

class CPageCarNoIdentify : public CDialog
{
	DECLARE_DYNAMIC(CPageCarNoIdentify)

public:
	CPageCarNoIdentify(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageCarNoIdentify();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
