// PageCarIdentify.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "PageCarIdentify.h"


// CPageCarIdentify 对话框

IMPLEMENT_DYNAMIC(CPageCarIdentify, CDialog)

CPageCarIdentify::CPageCarIdentify(CWnd* pParent /*=NULL*/)
	: CDialog(CPageCarIdentify::IDD, pParent)
{

}

CPageCarIdentify::~CPageCarIdentify()
{
}

void CPageCarIdentify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageCarIdentify, CDialog)
END_MESSAGE_MAP()


// CPageCarIdentify 消息处理程序
