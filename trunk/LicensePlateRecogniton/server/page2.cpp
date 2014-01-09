// Page1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "server.h"
#include "Page2.h"
#include "serverDlg.h"

// CPage1 �Ի���


IMPLEMENT_DYNAMIC(CPage2, CDialog)

CPage2::CPage2(CWnd* pParent /*=NULL*/)
: CDialog(CPage2::IDD, pParent)
{

	//serverDlg.Create(IDD_TEST_DIALOG);
}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IDENTIFY, m_list_identify);
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)

	ON_NOTIFY(NM_CLICK, IDC_LIST_IDENTIFY, &CPage2::OnNMClickListIdentify)
END_MESSAGE_MAP()


// CPage1 ��Ϣ�������

//void CPage1::OnNMDblclkListNoidentify(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


void CPage2::OnNMClickListIdentify(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CtestDlg* serverDlg = new CtestDlg();
	CPoint pt;
	GetCursorPos(&pt);//��ʱ����������Ļ��λ��
	ScreenToClient(&pt);//��Դ���
	MapWindowPoints(&m_list_identify,&pt,1);//��Կؼ�

	UINT uflags;
	int nItem =m_list_identify.HitTest(pt,&uflags);
	if(uflags & LVHT_ONITEMLABEL)
	{
		CString str1;
		str1=m_list_identify.GetItemText(nItem,0);

		serverDlg->OnClickIdenPic(str1);
	}
	*pResult = 0;
}
