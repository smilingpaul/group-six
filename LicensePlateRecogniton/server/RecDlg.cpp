// RecDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "RecDlg.h"
#include "ImageRecognize.h"
#include "serverDlg.h"

// CRecDlg 对话框

IMPLEMENT_DYNAMIC(CRecDlg, CDialog)

CRecDlg::CRecDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRecDlg::IDD, pParent)
, m_filePath(_T(""))
, p_timer(0)
{

}

CRecDlg::~CRecDlg()
{
}

void CRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPPLATECTRL1, m_recControl);
}


BEGIN_MESSAGE_MAP(CRecDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CRecDlg::OnBnClickedButton1)
	ON_WM_CREATE()
	//	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON2, &CRecDlg::OnBnClickedButton2)
	ON_WM_ACTIVATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRecDlg 消息处理程序
double GetPerFromPics(CString strFN,CString strPoke)
{
	CImageRecognize img;
	img.AttachFromFile(strPoke);
	CRect orgA;
	img.GetRect(&orgA);

	CImageRecognize imgCur;
	imgCur.AttachFromFile(strFN);
	CRect curA;
	imgCur.GetRect(&curA);

	if (orgA.Width() == curA.Width() + 1)
	{
		int dx = orgA.left - curA.left;
		int dy = orgA.top - curA.top;

		int sum = 0;
		for(int i=curA.left;i<curA.right;i++)
		{
			for(int j=curA.top;j<curA.bottom;j++)
			{
				if (imgCur.GetGray(i,j) == img.GetGray(i+dx-1,j+dy))
					sum++;
			}
		}

		int area = (curA.Width()+1) * (curA.Height()+1);

		double per = sum*1.0/area;

		return per;
	}

	if (orgA.Width() != curA.Width())
		return 0.1;

	if (orgA.Height() != curA.Height())
		return 0.1;

	int dx = orgA.left - curA.left;
	int dy = orgA.top - curA.top;

	int sum = 0;
	for(int i=curA.left;i<=curA.right;i++)
	{
		for(int j=curA.top;j<=curA.bottom;j++)
		{
			if (imgCur.GetGray(i,j) == img.GetGray(i+dx,j+dy))
				sum++;
		}
	}

	int area = (curA.Width()+1) * (curA.Height()+1);

	double per = sum*1.0/area;

	return per;
}

TCHAR GetCharFromPic(CString strFN)
{
	CFileFind ff;
	ff.FindFile(L".\\Templete\\*.*");

	double dMax = 0.0;
	double dCur;
	CString fn = L"";

	while(true)
	{
		if (ff.FindNextFile() == false)
		{
			break;
		}

		if (!ff.IsDots() && !ff.IsDirectory())
		{
			dCur = GetPerFromPics(strFN,ff.GetFilePath());

			if (dCur > dMax)
			{
				dMax = dCur;
				fn = ff.GetFileName();
			}
		}
	}

	return fn[0];
}
CString getFileName(CString filePath)
{
	int pos1,pos2;
	CString str1,str2,str3,result;
	str1 = filePath;
	pos1=filePath.ReverseFind('.');
	str2=str1.Left(pos1);
	pos2=str2.ReverseFind('\\');
	result = str2.Right(pos1-pos2-1);
	return result;
}
CString CRecDlg::GetPlateNum(CString filePath)
{
	CString strToRecPath,strToNumPath;

	CString filename;

	strToRecPath.Format(L".\\Plate\\%s.bmp",getFileName(filePath));

	//m_recControl.RecognisePicFile(filePath);
	//Sleep(2000);
	//return CString();
	//////////////////////////////////////////////////////////////////////////
	//取出车牌
	//////////////////////////////////////////////////////////////////////////

	CDC *dc = m_recControl.GetDC();
	CClientDC d(this);

	CRect rectReal;
	m_recControl.GetWindowRect(&rectReal);
	ScreenToClient(&rectReal);

	CImageRecognize rawImg,locatedImg,numImg;
	rawImg.AttachFromFile(filePath);
	locatedImg.AttachFromFile(L".\\q.bmp");
	numImg.AttachFromFile(L".\\p.bmp");

	//locatedImg.Resize(100,20,RGB(255,255,255));
	//numImg.Resize(70,20,RGB(255,255,255));

	for(int i=0;i<100;i++)
	{
		for(int j=rectReal.Height() - 20;j<rectReal.Height();j++)
		{
			locatedImg.SetPixel(i,j-rectReal.Height()+20,dc->GetPixel(i,j));
		}
	}

	for(int i=102;i<170-2;i++)
	{
		for(int j=rectReal.Height() - 20+2;j<rectReal.Height()-2;j++)
		{
			numImg.SetPixel(i-100,j-rectReal.Height()+20,dc->GetPixel(i,j));
		}
	}

	locatedImg.SaveToFile(strToRecPath);



	//////////////////////////////////////////////////////////////////////////
	//识别部分
	//////////////////////////////////////////////////////////////////////////

	numImg.Index2Gray();
	int count = 0;
	bool countflag = true;
	int iB = 0;
	int iE = 0;
	for(int i=0;i<70;i++)
	{
		bool flag = true;
		for(int j=0;j<20;j++)
		{
			if (numImg.GetGray(i,j) == 255)
			{
				continue;
			}
			else
			{
				flag = false;
				countflag = true;
				break;
			}
		}

		if (countflag == true && flag == true)
		{
			count++;
			countflag = false;
			iE = i;

			CImageRecognize imgSplit;
			imgSplit.AttachFromFile(L".\\0.bmp");

			for(int k=iB;k<iE;k++)
			{
				for(int j=0;j<20;j++)
				{
					imgSplit.SetPixel(k-iB,j,RGB(numImg.GetPixel(k,j),numImg.GetPixel(k,j),numImg.GetPixel(k,j)));
				}
			}

			CString fn;
			fn.Format(L".\\Split\\Split_%d.bmp",count);
			imgSplit.SaveToFile(fn);

			iB=iE;
		}
	}

	CString re;
	re = L"";

	for(int i=2;i<9;i++)
	{
		CString str;
		str.Format(L".\\Split\\Split_%d.bmp",i);
		re =  re + GetCharFromPic(str);
	}


	return re;
}

void CRecDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//GetPlateNum(m_filePath);

}

int CRecDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码


	return 0;
}

//void CRecDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialog::OnActivate(nState, pWndOther, bMinimized);
//
//	// TODO: 在此处添加消息处理程序代码
//	if (m_filePath.GetLength()!=0)
//	{
//		AfxMessageBox(GetPlateNum(m_filePath));
//	}
//}


void CRecDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_recControl.RecognisePicFile(m_filePath);
	OnBnClickedButton1();
	CString result;
	result = GetPlateNum(m_filePath);
	((CtestDlg*)GetParent())->m_showNum.SetWindowText(result);
	ShowWindow(SW_HIDE);

	HWND m_hWnd;
	((CtestDlg*)GetParent())->GetDlgItem(IDC_STATIC_PLATE,&m_hWnd);

	CString strToRecPath = m_filePath;
	strToRecPath = strToRecPath.TrimLeft(_T(".\\Wait\\"));
	CString strToPlatePath(".\\Plate\\");
	strToPlatePath += strToRecPath;

	((CtestDlg*)GetParent())->DisplayPic(strToPlatePath,m_hWnd,150, 40);
	//显示违章时间
	WIN32_FIND_DATA stFileTime ;
	FindFirstFile(m_filePath,&stFileTime);
	CTime ctFileTime(stFileTime.ftCreationTime);
	CString strFileTime = ctFileTime.Format("%y-%m-%d %H:%M:%S");
	((CtestDlg*)GetParent())->m_showTime.SetWindowText(strFileTime);

}

void CRecDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	if(m_filePath.GetLength()!=0)
	{m_recControl.RecognisePicFile(m_filePath);
	p_timer = SetTimer(1,100,NULL);}
}

void CRecDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedButton2();
	KillTimer(p_timer);
	CDialog::OnTimer(nIDEvent);
}
