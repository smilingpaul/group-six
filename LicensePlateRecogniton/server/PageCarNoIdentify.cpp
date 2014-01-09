// PageCarNoIdentify.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "PageCarNoIdentify.h"


// CPageCarNoIdentify 对话框

IMPLEMENT_DYNAMIC(CPageCarNoIdentify, CDialog)

CPageCarNoIdentify::CPageCarNoIdentify(CWnd* pParent /*=NULL*/)
	: CDialog(CPageCarNoIdentify::IDD, pParent)
{

}

CPageCarNoIdentify::~CPageCarNoIdentify()
{
}

void CPageCarNoIdentify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageCarNoIdentify, CDialog)
END_MESSAGE_MAP()


// CPageCarNoIdentify 消息处理程序
