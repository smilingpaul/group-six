// Exit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Sender.h"
#include "Exit.h"


// CExit �Ի���

IMPLEMENT_DYNAMIC(CExit, CDialog)

CExit::CExit(CWnd* pParent /*=NULL*/)
	: CDialog(CExit::IDD, pParent)
{

}

CExit::~CExit()
{
}

void CExit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExit, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SURE, &CExit::OnBnClickedButtonSure)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CExit::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CExit ��Ϣ�������

void CExit::OnBnClickedButtonSure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	exit(0);
}

void CExit::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(0);
}
