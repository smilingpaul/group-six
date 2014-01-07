
// LPRS_ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPRS_Client.h"
#include "LPRS_ClientDlg.h"
#include "stdlib.h"

#pragma comment(lib,"Ws2_32")
CString arr[10]={NULL};
CString arrName[10]={NULL};

// LPRS_ClientDlg 对话框

IMPLEMENT_DYNAMIC(LPRS_ClientDlg, CDialog)

UINT SendFile(void *p/*窗口指针*/)
{
    

	CString strError;
	ASSERT(p!=NULL);//
	LPRS_ClientDlg *pDlg = (LPRS_ClientDlg*)p;
	CString strIP;
	pDlg->m_cSvrIP.GetWindowText(strIP);

	sockaddr_in addr;
	addr.sin_family=AF_INET;//表示在INT上通信
	addr.sin_addr.S_un.S_addr=inet_addr((LPSTR)(LPCTSTR)strIP.GetBuffer(0));
	addr.sin_port=htons(pDlg->m_iPort); //转换为网络字节序

	int ret=0;
	int error=0;
	ret=connect(pDlg->m_hSocket,(LPSOCKADDR)&addr,sizeof(addr));//主动连接服务器
	if(ret==SOCKET_ERROR)
	{
		strError.Format(_T("Connect Error:%d "),error=WSAGetLastError());
		AfxMessageBox(strError);
		closesocket(pDlg->m_hSocket);
		return -1;
	}


	//文件操作
	int num = ((CListCtrl*)(pDlg->GetDlgItem(IDC_LIST1)))->GetItemCount();
	CString strNum;
	strNum.Format(_T("%d"),num);
	int ret1=send(pDlg->m_hSocket,(LPSTR)(LPCTSTR)strNum, 10, 0);	//发送文件的个数

	for(int j = 0; j < num; ++j)
	{
		
		int  iEnd =0;
		int  ilen =0;
		long iTmp =0;
		long lFileSize = 0;
		CString str = _T(" ");
		CString strPathName = arr[j];
		
		CFile file;
		if(!file.Open(strPathName, CFile::modeRead | CFile::typeBinary))
		{
			AfxMessageBox(_T("打开文件失败"));
			return -1;
		}
		lFileSize = file.GetLength();

		pDlg->m_Progress.SetRange32(0, lFileSize);	//设置进度条范围
		pDlg->m_Progress.SetStep(1);	            //设置进度条步长

		char temp[1024];   //修改         	            //首先用来存放文件大小，后面用作发送文件缓冲区
		memset(temp,0,sizeof(temp)); 
		ltoa(lFileSize, temp, 10);                  //转化为字符

	    str = arrName[j];     //获取文件名字
		str += "*";
		str += temp;
		int i=str.GetLength();

		ret1 = send(pDlg->m_hSocket, (LPSTR)(LPCTSTR)str, 100, 0);	//发送文件的名称和大小


		if (ret1==SOCKET_ERROR)
		{
				strError.Format(_T("Send File: %d "),error=WSAGetLastError());
				AfxMessageBox(strError);
				return -1;
		}
	    
		/////////////////////////发送文件///////////////////////////////////////////
		int flagFileSize = 0;
		while (1)
		{
			
			ilen = file.Read(temp, 1024);
			
			if (ilen < 1024)
			{
				flagFileSize += ilen;  //修改
				iEnd = send(pDlg->m_hSocket,temp,1024,0);
				break;
			}
			flagFileSize += ilen;  //修改
			iEnd = send(pDlg->m_hSocket,temp,ilen,0);
			if (iEnd == SOCKET_ERROR)
			{
				strError.Format(_T("Send File : %d"),error=WSAGetLastError());
				AfxMessageBox(strError);
				break;
			}
			iTmp += iEnd;
			pDlg -> m_Progress.SetPos( iTmp );
		
		}
        int n = 0;   
		file.Close();
		pDlg->m_ListControl.DeleteItem(0);
		
		/*if (iTmp == lFileSize)
		{
			//AfxMessageBox("文件发送成功！");
			
		}
		else
		{
			strError.Format(str+"文件发送失败");
			AfxMessageBox(strError);
			return 0;
		}*/
		

	}
	
	/////////////////判断发送是否成功、结束处理////////////////////////////////////
	closesocket(pDlg->m_hSocket);
	
	ExitThread(0);
	return 0;
}


LPRS_ClientDlg::LPRS_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LPRS_ClientDlg::IDD, pParent)
	, m_iPort(4000)
{
	m_hSocket=NULL;
}

LPRS_ClientDlg::~LPRS_ClientDlg()
{
}

void LPRS_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_cSvrIP);
	DDX_Text(pDX, IDC_EDIT1, m_iPort);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_LIST1, m_ListControl);
}


BEGIN_MESSAGE_MAP(LPRS_ClientDlg, CDialog)
	ON_BN_CLICKED(IDC_BTNBROWS, &LPRS_ClientDlg::OnBnClickedBtnbrows)
	ON_BN_CLICKED(IDC_BTNDELETE, &LPRS_ClientDlg::OnBnClickedBtndelete)
	ON_BN_CLICKED(IDOK, &LPRS_ClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTNSTOP, &LPRS_ClientDlg::OnBnClickedBtnstop)
	ON_BN_CLICKED(IDCANCEL, &LPRS_ClientDlg::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// LPRS_ClientDlg 消息处理程序

BOOL LPRS_ClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	this->m_ListControl.InsertColumn(0,_T("文件名"),LVCFMT_LEFT,120); 
	this->m_ListControl.InsertColumn(1,_T("文件路径"),LVCFMT_LEFT,250);
	
	this->UpdateData(false);
	LPRS_ClientDlg dlg;

	//显示ip和端口

	CString str2,str3;
	CString filename=_T("..\\LPRS_Client\\port.txt");
	CStdioFile f2;
	CFileException fileException;
	if(!f2.Open(filename,CFile::modeReadWrite,&fileException))
	{
		MessageBox(_T("文件打不开"));
	}
	else
	{
		
		f2.SeekToBegin();
		f2.ReadString(str2);
		m_cSvrIP.SetWindowTextW(str2);
		f2.ReadString(str3);
		m_iPort=_ttof(str3);
		UpdateData(false);
		f2.Close();
	}


	UpdateData(FALSE);


	//GetDlgItem(IDC_BTNDELETE)->EnableWindow(false);//将删除按钮设置为不可用

    //////////////
	WSADATA wsaData;
	WORD version=MAKEWORD(2,0);
	int ret=WSAStartup(version,&wsaData);
	if(ret!=0) 
	{
		MessageBox(_T("Init  Error"));
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void LPRS_ClientDlg::OnBnClickedBtnbrows()
{
	// TODO: 在此添加控件通知处理程序代码

	// 设置过滤器   
    TCHAR szFilter[] = _T("所有文件(*.*)|*.*|图片文件(*.bmp)|*.bmp|");   
    // 构造打开文件对话框   
    //CFileDialog fileDlg(TRUE, _T("bmp"), NULL, 0, szFilter, this);

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, _T("所有文件(*.*)|*.*|图片文件(*.bmp)|*.bmp|图片文件(*.jpg)|*.jpg||"), this);
	fileDlg.m_ofn.lpstrTitle = _T("打开");

	 if (IDOK == fileDlg.DoModal())   
    {   
        // 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		/*m_strFileName=fileDlg.GetFileName();
        m_strPathName = fileDlg.GetPathName(); 

		this->m_ListControl.InsertItem(0,m_strFileName);
		this->m_ListControl.SetItemText(0,1,m_strPathName);
		*/

		POSITION pos = fileDlg.GetStartPosition();

		while(pos)
		{
			m_strPathName = fileDlg.GetNextPathName(pos);
			//打开文件
			CFile file(m_strPathName, CFile::modeRead);
			m_strFileName = file.GetFileName();
			//关闭文件
			file.Close();
			UpdateData(FALSE);
			

			//加入文件列表
			this->m_ListControl.InsertItem(0,m_strFileName);
			this->m_ListControl.SetItemText(0,1,m_strPathName);
		}
			
		  
    }
}



void LPRS_ClientDlg::OnBnClickedBtndelete()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION p=m_ListControl.GetFirstSelectedItemPosition();

	//CString str1;
	//str1.Format("%d",p);
	//this->MessageBox(str1);

	while (p)
	{ 
		int nSelected = m_ListControl.GetNextSelectedItem(p); 

		//str1.Format("%d",nSelected);
		//this->MessageBox(str1);

		m_ListControl.DeleteItem(nSelected); 
		p=m_ListControl.GetFirstSelectedItemPosition(); 
	} 
}


void LPRS_ClientDlg::InitSock()//
{
    

	if(m_hSocket)//如果已经创建，先关闭
	{
		closesocket(m_hSocket);
        m_hSocket=NULL;
	}
	else
	{
		m_hSocket=socket(AF_INET,SOCK_STREAM,0);
		ASSERT(m_hSocket!=NULL);//
	}


}

void LPRS_ClientDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData();
	m_hSocket=NULL;
	InitSock();//初始化SOCK。BIND
    CFileFind find;
	int num = m_ListControl.GetItemCount();
	for(int i =0;i < num; ++i)
	{
		arr[i] = m_ListControl.GetItemText(i,1);
		arrName[i] = m_ListControl.GetItemText(i,0);
		
	}

  
	if(!find.FindFile(m_strPathName))
	{
		MessageBox(_T("文件路径无效","提醒"));
		return;
	}

	AfxBeginThread(SendFile, this);

	//OnOK();
}

void LPRS_ClientDlg::OnBnClickedBtnstop()
{
	// TODO: 在此添加控件通知处理程序代码
}

void LPRS_ClientDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	

	OnCancel();

}

void LPRS_ClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
        CPaintDC dc(this);
        GetClientRect(rect);
        dc.FillSolidRect(rect,RGB(167,220,234)); //设置为绿色背景
		CDialog::OnPaint();
	}
	 // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void LPRS_ClientDlg::OnDestroy()
{
	if(m_hSocket!=NULL)
	{
		::closesocket(m_hSocket);
	}

	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

BOOL LPRS_ClientDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	if(m_hSocket)
	{
		closesocket(m_hSocket);

	}
	WSACleanup();
	return CDialog::DestroyWindow();
}

HBRUSH LPRS_ClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor==CTLCOLOR_STATIC)
    {
       // pDC->SetTextColor(RGB(0,0,0));  //字体颜色
       pDC->SetBkColor(RGB(167,220,234));
	  //pDC->SetBkMode(HOLLOW_BRUSH); //设置字体背景为透明
    }
	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
