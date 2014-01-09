// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "server.h"
#include "LoginDlg.h"
#include "DataBase.h"

// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_user(_T(""))
	, m_password(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_password);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������

void CLoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	// �ж��û����������Ƿ�Ϊ��
	if(m_user == "")
	{
		MessageBox(_T("�û���Ϊ��"));
	}
	else if (m_password == "")
	{
		MessageBox(_T("����Ϊ��"));
		m_user = "";
		UpdateData(false);
	}
	// ��֤�û��������Ƿ���ȷ
	else
	{
		CString strSql("");
		strSql.Format(_T("select count(*) from Login where username ='"+m_user+"' and password='"+m_password+"'"));
		CDataBase db;
		db.QueryData(strSql);
		int nCount = db.m_QueryRecordSet->GetCollect(_variant_t((long)0));
		if(nCount > 0)
		{
			CStdioFile  file;
			try
			{
				CString filename(_T(".\\mylogin.txt"));
				file.Open(filename,CFile::modeWrite|CFile::typeText);
				file.WriteString(m_user);
			}
			catch (CFileException *e)
			{
				e->ReportError();	
			}
			EndDialog(1);
		}
		else
		{
			MessageBox(_T("�û������������"));
			m_user = "";
			m_password = "";
			UpdateData(false);
		}
	}
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(0);
}
BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CStdioFile  file;
	try
	{
		CString filename(_T(".\\mylogin.txt"));
		file.Open(filename,CFile::modeRead|CFile::typeText);
		file.ReadString(m_user);
		UpdateData(false);
	}
	catch (CFileException *e)
	{
		e->ReportError();	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}