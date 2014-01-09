// Page1.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "Page1.h"
#include "serverDlg.h"

// CPage1 对话框


IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CPage1::IDD, pParent)
{
	
	//serverDlg.Create(IDD_TEST_DIALOG);
}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NOIDENTIFY, m_list_noidentify);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)

	ON_NOTIFY(NM_CLICK, IDC_LIST_NOIDENTIFY, &CPage1::OnNMClickListNoidentify)
END_MESSAGE_MAP()


// CPage1 消息处理程序

//void CPage1::OnNMDblclkListNoidentify(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CPage1::OnNMClickListNoidentify(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CtestDlg* serverDlg = new CtestDlg();
	CPoint pt;
	GetCursorPos(&pt);//此时获得是相对屏幕的位置
	ScreenToClient(&pt);//相对窗口
	MapWindowPoints(&m_list_noidentify,&pt,1);//相对控件

	UINT uflags;
	int nItem =m_list_noidentify.HitTest(pt,&uflags);
	if(uflags & LVHT_ONITEMLABEL)
	{
		CString str1;
		str1=m_list_noidentify.GetItemText(nItem,0);
		
		serverDlg->OnClickPic(str1);
	}
	*pResult = 0;
}
