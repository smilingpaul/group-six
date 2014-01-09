// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "LoginDlg.h"
#include "DataBase.h"

// CLoginDlg 对话框

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


// CLoginDlg 消息处理程序

void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	// 判断用户名、密码是否为空
	if(m_user == "")
	{
		MessageBox(_T("用户名为空"));
	}
	else if (m_password == "")
	{
		MessageBox(_T("密码为空"));
		m_user = "";
		UpdateData(false);
	}
	// 验证用户、密码是否正确
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
			MessageBox(_T("用户名或密码错误"));
			m_user = "";
			m_password = "";
			UpdateData(false);
		}
	}
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}
BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常: OCX 属性页应返回 FALSE
}