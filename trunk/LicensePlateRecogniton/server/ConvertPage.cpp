// ConvertPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "server.h"
#include "ConvertPage.h"


// CConvertPage �Ի���

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


// CConvertPage ��Ϣ�������
