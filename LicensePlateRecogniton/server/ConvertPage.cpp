// ConvertPage.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "ConvertPage.h"


// CConvertPage 对话框

IMPLEMENT_DYNAMIC(CConvertPage, CDialog)

CConvertPage::CConvertPage(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertPage::IDD, pParent)
{

}

CConvertPage::~CConvertPage()
{
}

void CConvertPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONVERT, m_list_convert);
}


BEGIN_MESSAGE_MAP(CConvertPage, CDialog)
END_MESSAGE_MAP()


// CConvertPage 消息处理程序
