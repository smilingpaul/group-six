// SaveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "server.h"
#include "SaveDlg.h"


// CSaveDlg �Ի���

IMPLEMENT_DYNAMIC(CSaveDlg, CDialog)

CSaveDlg::CSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDlg::IDD, pParent)
{

}

CSaveDlg::~CSaveDlg()
{
}

void CSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaveDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSaveDlg ��Ϣ�������

void CSaveDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnOK();
}

BOOL CSaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(123,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSaveDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnOK();

	CDialog::OnTimer(nIDEvent);
}
