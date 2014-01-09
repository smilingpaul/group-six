// Exit.cpp : 实现文件
//

#include "stdafx.h"
#include "Sender.h"
#include "Exit.h"


// CExit 对话框

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


// CExit 消息处理程序

void CExit::OnBnClickedButtonSure()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}

void CExit::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}
