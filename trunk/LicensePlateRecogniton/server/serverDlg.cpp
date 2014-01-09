// serverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxwin.h"
#include "server.h"
#include "serverDlg.h"
#include "stdio.h"
#include "winsock2.h"
#include "Page1.h"
#include "LoginDlg.h"
#include "RecDlg.h"
#include "DataBase.h"
#include "MessageDlg.h"
#include "SaveDlg.h"
#include "ConvertPage.h"
#include "atlimage.h"
#include "ImageRecognize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int m_loadPicFlag = 0;
int m_loadIdenPicFlag = 1;
int m_loadConvertPicFlag = 0;
CPage1 m_page;
CPage2 m_page_identify;
CConvertPage m_page_convert;
HWND m_hWnd1;
CWnd *pWnd1;
CString pathname;//打开文件的路径名
int serverThreadFlag = 2;
int engine = 2;
HANDLE hReceiveHandle;//afterconnect线程的句柄

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtestDlg 对话框




CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CtestDlg::IDD, pParent)
, recvFlag(10)
, m_port(0)
, pDlg(NULL)
, m_searchInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//DDX_Control(pDX, IDC_LIST_CAR, m_list);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Control(pDX, IDC_EDIT_NO, m_showNum);
	DDX_Text(pDX, IDC_EDIT4, m_searchInfo);
	DDX_Control(pDX, IDC_COMBO1, m_searchItem);
	DDX_Control(pDX, IDC_LIST_NCAR, m_peccancyList);
	DDX_Control(pDX, IDC_EDIT_T, m_showTime);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CtestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_START, &CtestDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CtestDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CtestDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_SHRINK, &CtestDlg::OnBnClickedBtnShrink)
	ON_MESSAGE(WM_RECVDATA,OnRecvData)
	ON_COMMAND(ID_OPEN, &CtestDlg::OnOpen)
	ON_COMMAND(ID_32776, &CtestDlg::On32776)
	//ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAR, &CtestDlg::OnNMDblclkListCar)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CtestDlg::OnTcnSelchangeTab)
	//ON_NOTIFY(NM_DBLCLK, IDC_LIST_NOIDENTIFY, &CtestDlg::OnNMDblclkListNoidentify)
	ON_BN_CLICKED(IDC_BTN_IDENTIFY, &CtestDlg::OnBnClickedBtnIdentify)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CtestDlg::OnBnClickedBtnSave)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON5, &CtestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BTN_CONFIRM, &CtestDlg::OnBnClickedBtnConfirm)
	ON_BN_CLICKED(IDC_BTN_CONVERT, &CtestDlg::OnBnClickedBtnConvert)
END_MESSAGE_MAP()


// CtestDlg 消息处理程序

BOOL CtestDlg::OnInitDialog()
{
	CLoginDlg dlg;
	int login = dlg.DoModal();
	if(login == 0)
	{
		exit(0);
	}

	CDialog::OnInitDialog();


	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//m_page = new CPage1();

	//创建违章信息对话框
	m_peccancyList.InsertColumn(0, _T("姓名"), LVCFMT_LEFT ,50);
	m_peccancyList.InsertColumn(1, _T("身份证"), LVCFMT_LEFT ,100);
	m_peccancyList.InsertColumn(2, _T("车牌号"), LVCFMT_LEFT ,100);
	m_peccancyList.InsertColumn(3, _T("违章时间"), LVCFMT_LEFT ,150);

	SetTimer(124,1000,NULL);

	//为Tab Control增加两个页面

	m_tab.InsertItem(0, _T("未识别"));
	m_tab.InsertItem(1, _T("已识别"));
	m_tab.InsertItem(2,_T("图像转换"));

	//创建两个对话框
	m_page.Create(IDD_DIALOG1, &m_tab);
	m_page_identify.Create(IDD_DIALOG2, &m_tab);
	m_page_convert.Create(IDD_DIALOG_CONVERT,&m_tab);

	m_page.m_list_noidentify.InsertColumn(0,_T("图片名"),LVCFMT_LEFT,50);
	m_page.m_list_noidentify.InsertColumn(1,_T("违章时间"),LVCFMT_LEFT,100);

	m_page_identify.m_list_identify.InsertColumn(0,_T("图片名"),LVCFMT_LEFT,50);
	m_page_identify.m_list_identify.InsertColumn(2,_T("车牌号"),LVCFMT_LEFT,100);
	m_page_identify.m_list_identify.InsertColumn(1,_T("违章时间"),LVCFMT_LEFT,100);
	m_page_identify.m_list_identify.SetExtendedStyle(LVS_EX_CHECKBOXES);

	m_page_convert.m_list_convert.InsertColumn(0,_T("图片名"),LVCFMT_LEFT,80);
	m_page_convert.m_list_convert.InsertColumn(1,_T("违章时间"),LVCFMT_LEFT,70);
	m_page_convert.m_list_convert.SetExtendedStyle(LVS_EX_CHECKBOXES);

	//设定在Tab内显示的范围
	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 8;
	rc.left += 8;
	rc.right -= 8;

	m_page.MoveWindow(&rc);
	m_page_identify.MoveWindow(&rc);
	m_page_convert.MoveWindow(&rc);

	//用数组把对话框对象指针保存起来
	pDialog[0] = &m_page;
	pDialog[1] = &m_page_identify;
	pDialog[2] = &m_page_convert;


	//显示初始页面
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	pDialog[2]->ShowWindow(SW_HIDE);

	//保存当前选择
	m_CurSelTab = 0;


	GetDlgItem(IDC_STATIC_PICTRUE,&m_hWnd1);
	pWnd1 = GetDlgItem(IDC_STATIC_PICTRUE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestDlg::OnPaint()
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
		HWND m_hWnd;
		GetDlgItem(IDC_STATIC_PICTRUE,&m_hWnd); 

		CWnd *pWnd = GetDlgItem(IDC_STATIC_PICTRUE);
		CDC *pDC = pWnd->GetDC();
		CRect rect;
		pWnd->GetWindowRect(rect);

		DisplayPic(pathname,m_hWnd,rect.Width(), rect.Height());
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CtestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}



void CtestDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	//closesocket(m_socket);
	//WSACleanup();
	serverThreadFlag = 0;
}

void CtestDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CtestDlg::OnBnClickedBtnShrink()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_BTN_SHRINK)->GetWindowText(str);
	if (str == "<")
	{
		GetDlgItem(IDC_BTN_SHRINK)->SetWindowText(_T(">"));
	}
	else
	{
		GetDlgItem(IDC_BTN_SHRINK)->SetWindowText(_T("<"));

	}
	static CRect rectLarge;
	static CRect rectSmall;

	if(rectLarge.IsRectNull())
	{
		CRect rectSeparator;
		GetWindowRect(&rectLarge);
		GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);

		rectSmall.left=rectLarge.left;
		rectSmall.top=rectLarge.top;
		rectSmall.bottom=rectLarge.bottom;
		rectSmall.right=rectSeparator.right;
	}
	if(str==_T("<"))
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),SWP_NOMOVE | SWP_NOZORDER);

	}
	else
	{
		SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}

}

void CtestDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(true);
	UpdateData(true);

	if (serverThreadFlag == 2)
	{
		serverThreadFlag = 1;
		InitSocket(m_port);
		RECVPARAM *pRecvParam=new RECVPARAM;
		pRecvParam->sock=m_socket;
		pRecvParam->hwnd=m_hWnd;
		HANDLE hThread=CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);
		CloseHandle(hThread);
	} 
	else
	{
		serverThreadFlag = 0;
	}	

}

BOOL CtestDlg::InitSocket(int port)
{
	WSAData   wsaData;
	WSAStartup(MAKEWORD(1,1),&wsaData);
	m_socket=socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET==m_socket)
	{
		AfxMessageBox(_T("套接字创建失败！"));
		return FALSE;
	}
	SOCKADDR_IN addrSock;
	addrSock.sin_family=AF_INET;
	addrSock.sin_port=htons(port);
	addrSock.sin_addr.S_un.S_addr=htonl(INADDR_ANY);

	int retval;
	retval=bind(m_socket,(SOCKADDR*)&addrSock,sizeof(SOCKADDR));
	if(SOCKET_ERROR==retval)
	{
		closesocket(m_socket);
		AfxMessageBox(_T("绑定失败!"));
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI CtestDlg::RecvProc(LPVOID lpParameter)
{
	SOCKET sock=((RECVPARAM*)lpParameter)->sock;
	HWND hwnd=((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;	//释放内存的操作

	int len=sizeof(SOCKADDR);

	accepSocket acSocket[100];


	if(::listen(sock, 5) == SOCKET_ERROR)   //监听端口
	{ 
		//AfxMessageBox(L"listen error");
	} 

	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	int acceptNumber = 0;
	while( serverThreadFlag == 1)
	{
		acSocket[acceptNumber].sock = accept(sock, (SOCKADDR*)&remoteAddr, &nAddrLen);  //接受连接请求
		if(acSocket[acceptNumber].sock == INVALID_SOCKET)   //建立连接失败
		{ 
			CString conInfo(L"failed connect！");
			::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)conInfo.GetBuffer());
			continue;
		} 
		CString connInfo(L"有新连接建立！");
		::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)connInfo.GetBuffer());
		accepSocket *lpAcceptSocket = &acSocket[acceptNumber];
		hReceiveHandle = CreateThread(NULL,0,AfterConnect,(LPVOID)lpAcceptSocket,0,NULL);  //创建一个线程用于建立连接后的通信
		++acceptNumber;
	}
	return 0;
}

DWORD WINAPI CtestDlg::AfterConnect( LPVOID lpParameter )
{
	SOCKET skChildSock = ((accepSocket*)lpParameter)->sock;
	char tempBuf[100000]={0};
	int iCommand ; //用于判断接收的是数据还是口令
	int retval = 10;

	iCommand = recv(skChildSock,tempBuf,100000,0);

	/*如果客户端除了连接什么也不干*/
	if( iCommand == 1 && tempBuf[0] == 'C')  
	{
		shutdown(skChildSock,SD_BOTH);
	}

	/*如果客户端要验证用户名和密码*/
	else if(iCommand == 1 && tempBuf[0] == 'L')   
	{
		while(true)
		{
			retval = recv(skChildSock,tempBuf,100000,0);   //接收用户名
			tempBuf[retval] = '\0';

			CString strUserName(tempBuf);				   //strUserName：用户名

			memset(tempBuf,0,100000);					   //数组清
			retval = recv(skChildSock,tempBuf,100000,0);   //接收密码
			tempBuf[retval] = '\0';
			CString strPassWord(tempBuf);				   //strPassWord：密码

			/////////////////////////////////////////////////
			//从数据库中获取用户名密码：

			CString strSql("");
			strSql.Format(_T("select count(*) from Login where username ='"+strUserName+"' and password='"+strPassWord+"'"));
			CDataBase db;
			db.QueryData(strSql);
			int nCount = db.m_QueryRecordSet->GetCollect(_variant_t((long)0));
			if(nCount > 0)       ////如果用户名密码正确则返回T
			{
				char cResult = {'T'};
				send(skChildSock,&cResult,sizeof(cResult),0);
				shutdown(skChildSock,SD_BOTH);
				break;
			}
			else			//否则返回F
			{
				char cResult = {'F'};
				send(skChildSock,&cResult,sizeof(cResult),0);

				retval = recv(skChildSock,tempBuf,100000,0);   //如果验证错误客户端会再发一个L，此句用来接收。
				memset(tempBuf,0,100000);
			}
		}

	}

	/*如果客户端要发送图片*/
	else if(iCommand == 1 && tempBuf[0] == 'P')
	{
		int endFlag = 1;

		CFile file;
		srand(time(NULL));
		CString strFileName;    //接收后的文件名
		while( true )
		{
			memset(tempBuf,0,100000);

			retval = recv(skChildSock,tempBuf,sizeof(tempBuf),0);

			if(retval == SOCKET_ERROR)  //如果接收出现错误
			{
				CString nopostInfo(L"连接断开！");
				/*::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)nopostInfo.GetBuffer());*/
				break;
			}

			else if( retval == 0 )
			{
				break;
			}

			else if( retval == 1 && tempBuf[0] == 'P')
			{
				continue ;
			}

			else
			{
				;
			}

			if (endFlag == 1)  //如果是一个新文件
			{
				CTime time = CTime::GetCurrentTime();

				strFileName.Format(L".\\Wait\\%d_%d_%d_%d_%d_%d_%d_%d.bmp",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond(),rand(),rand());

				//FILE *flWrite = fopen(strFileName,"wb");

				file.Open(strFileName,CFile::modeCreate|CFile::modeRead|CFile::modeWrite);
			}

			if (tempBuf[0] == 'E' && retval == 1)  //如果一个图片已发送完毕
			{
				file.Close();
				endFlag = 1;
				//////////////////////////////////////////////////////
				////更新文件列表
				CFileFind finder;
				BOOL x = finder.FindFile(strFileName);
				
				CString strFileNameCopy = strFileName;
				strFileNameCopy.TrimLeft(_T(".\\Wait\\"));

				int iListLine = m_page.m_list_noidentify.InsertItem(0,strFileNameCopy);     //向列表中添加文件名，iListLine用于获取行数
				WIN32_FIND_DATA stFileTime ;
				FindFirstFile(strFileName,&stFileTime);
				CTime ctFileTime(stFileTime.ftCreationTime);
				CString strFileTime = ctFileTime.Format("%y-%m-%d %H:%M:%S");
				m_page.m_list_noidentify.SetItemText(iListLine,1,strFileTime);
				
				continue;
			}

			//写入文件


			else  //如果是图片数据则写入文件
			{
				CString postInfo(L"正在传输文件中......");
				/*::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)postInfo.GetBuffer());*/
				// 			fwrite(tempBuf,1,retval+1,flWrite);  //将接收到的数据写入文件
				// 
				// 			fclose(flWrite);
				file.Write(tempBuf,retval);
				memset(tempBuf,0,100000);
				endFlag = 0;
			}

			CString sendInfo(L"文件传输完毕。");



			/*::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)sendInfo.GetBuffer());

			m_loadPicFlag = 0;*/

			//函数间通信
			//::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)tempBuf);
		}

	}

	else
	{
		;
	}
	CloseHandle(hReceiveHandle);
	return 0;
}

LRESULT CtestDlg::OnRecvData(WPARAM wParam,LPARAM lParam)
{

	//CString strTemp=(char*)lParam;
	CString strTemp;
	strTemp.Format(L"%s",lParam);
	SetDlgItemText(IDC_STATIC_MESSAGE,strTemp.GetBuffer());

	return TRUE;
}

void CtestDlg::OnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
	static CString BASED_CODE szFilter = L"JPG Files (*.jpg)|*.jpg|JPG Files (*.BMP)|*.bmp|JIF Files (*.bmp)|*.jif|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE,_T("BMP"),NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter.GetBuffer());
	if(dlg.DoModal() != IDOK)
		return;
	pathname = dlg.GetPathName();

	HWND m_hWnd;
	GetDlgItem(IDC_STATIC_PICTRUE,&m_hWnd);

	CWnd *pWnd = GetDlgItem(IDC_STATIC_PICTRUE);
	CDC *pDC = pWnd->GetDC();
	CRect rect;
	pWnd->GetWindowRect(rect);
	DisplayPic(pathname,m_hWnd,rect.Width(), rect.Height());
}
HRESULT CtestDlg::DisplayPic(CString lpImageFile, HWND hWnd, int nScrWidth, int nScrHeight)
{
	HDC hDC_Temp=::GetDC(hWnd);

	IPicture *pPic;
	IStream *pStm;

	BOOL bResult;

	HANDLE hFile=NULL;
	DWORD dwFileSize,dwByteRead;

	//打开硬盘中的图形文件
	hFile=CreateFile(lpImageFile,GENERIC_READ,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);


	if (hFile!=INVALID_HANDLE_VALUE)
	{
		dwFileSize=GetFileSize(hFile,NULL);//获取文件字节数

		if (dwFileSize==0xFFFFFFFF)
			return E_FAIL;
	}
	else
	{
		return E_FAIL;
	}

	//分配全局存储空间
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	LPVOID pvData = NULL;

	if (hGlobal == NULL)
	{
		AfxMessageBox(_T("图形文件分配出错."));
		return E_FAIL;
	}

	if ((pvData = GlobalLock(hGlobal)) == NULL)//锁定分配内存块
	{
		AfxMessageBox(_T("内存块锁定出问题"));
		return E_FAIL;
	}

	ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//把文件读入内存缓冲区

	GlobalUnlock(hGlobal);

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStm) != S_OK)
	{
		AfxMessageBox(_T("流初始化失败"));
		return E_FAIL;
	}

	//装入图形文件
	bResult=OleLoadPicture(pStm,dwFileSize,TRUE,IID_IPicture,(LPVOID*)&pPic);

	if(FAILED(bResult))
	{
		AfxMessageBox(_T("图形文件装载出错."));
		return E_FAIL;
	}

	OLE_XSIZE_HIMETRIC hmWidth;//图片的真实宽度
	OLE_YSIZE_HIMETRIC hmHeight;//图片的真实高度
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	//将图形输出到屏幕上
	bResult=pPic->Render(hDC_Temp,0,0,nScrWidth,nScrHeight,
		0,hmHeight,hmWidth,-hmHeight,NULL);

	CloseHandle(hFile);//关闭打开的文件

	pPic->Release();
	// Free memory.
	GlobalFree(hGlobal);

	if (SUCCEEDED(bResult))
	{
		return S_OK;
	}
	else
	{
		AfxMessageBox(_T("图形文件装载出错."));
		return E_FAIL;
	}
}
void CtestDlg::On32776()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}

HBRUSH CtestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_LOGO)
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BMP_LOGO);

		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(pDC);

		dcCompatible.SelectObject(&bitmap);

		CRect rect;
		GetDlgItem(IDC_STATIC_LOGO)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pDC->BitBlt(0,8,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);//,bmp.bmWidth,bmp.bmHeight

	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_HIT)
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BMP_HIT);

		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(pDC);

		dcCompatible.SelectObject(&bitmap);

		CRect rect;
		GetDlgItem(IDC_STATIC_HIT)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pDC->BitBlt(0,8,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);//,bmp.bmWidth,bmp.bmHeight

	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_PICTRUE)
	{
		pathname = ".\\hit\\hit_heard.bmp";

		HWND m_hWnd;
		GetDlgItem(IDC_STATIC_PICTRUE,&m_hWnd);

		CWnd *pWnd = GetDlgItem(IDC_STATIC_PICTRUE);
		CDC *pDC = pWnd->GetDC();
		CRect rect;
		pWnd->GetWindowRect(rect);
		DisplayPic(pathname,m_hWnd,rect.Width(), rect.Height());
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
void CtestDlg::OnTimer(UINT_PTR nIDEvent)
{
	//读取未识别的图片进入列表
	if (m_loadPicFlag == 0)
	{
		CString str1(_T(".\\Wait"));
		LPCTSTR pstr=str1;
		CFileFind finder;
		CString strWildcard(pstr);
		strWildcard += _T("\\*.bmp");

		// start working for files
		BOOL bWorking = finder.FindFile(strWildcard);

		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// skip . and .. files; otherwise, we'd
			// recur infinitely!

			if (finder.IsDots())
				continue;

			CString str =finder.GetFileName();
			////////////////////////////////////////////////////////////
			//添加显示文件创建时间功能
			int iListLine = m_page.m_list_noidentify.InsertItem(0,str);     //向列表中添加文件名，iListLine用于获取行数
			CTime  ctFileTime;												//文件创建时间
			finder.GetCreationTime(ctFileTime);								//获取文件创建时间
			CString strFileTime = ctFileTime.Format("%y-%m-%d,%H:%M:%S");	//将时间转换为CString类型
			m_page.m_list_noidentify.SetItemText(iListLine,1,strFileTime);	//在相应行上的第二列添加时间
			/////////////////////////////////////////////////////////////
		}
		finder.Close();
		m_loadPicFlag = 1;
	}

	
	//读取JPG图像进入图像转换列表
	if (m_loadConvertPicFlag == 0)
	{
		CString str1(_T(".\\Wait"));
		LPCTSTR pstr=str1;
		CFileFind finder;
		CString strWildcard(pstr);
		strWildcard += _T("\\*.jpg");

		// start working for files
		BOOL bWorking = finder.FindFile(strWildcard);

		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// skip . and .. files; otherwise, we'd
			// recur infinitely!

			if (finder.IsDots())
				continue;

			CString str =finder.GetFileName();
			////////////////////////////////////////////////////////////
			//添加显示文件创建时间功能
			int iListLine = m_page_convert.m_list_convert.InsertItem(0,str);     //向列表中添加文件名，iListLine用于获取行数
			CTime  ctFileTime;												//文件创建时间
			finder.GetCreationTime(ctFileTime);								//获取文件创建时间
			CString strFileTime = ctFileTime.Format("%y-%m-%d,%H:%M:%S");	//将时间转换为CString类型
			m_page_convert.m_list_convert.SetItemText(iListLine,1,strFileTime);	//在相应行上的第二列添加时间
			/////////////////////////////////////////////////////////////
		}
		finder.Close();
		m_loadConvertPicFlag = 1;
	}


	//显示系统时间
	CTime t=CTime::GetCurrentTime();
	CString str=t.Format("%H:%M:%S");
	SetDlgItemText(IDC_EDIT_TIME,str);

}

void CtestDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	m_CurSelTab = m_tab.GetCurSel();
	switch(m_CurSelTab)
	{
	case 0:
		pDialog[0]->ShowWindow(SW_SHOW);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_IDENTIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONVERT)->EnableWindow(FALSE);
		break;
	case 1:
		pDialog[0]->ShowWindow(SW_HIDE);
		pDialog[1]->ShowWindow(SW_SHOW);
		pDialog[2]->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_IDENTIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CONVERT)->EnableWindow(FALSE);
		break;
	case 2:
		pDialog[0]->ShowWindow(SW_HIDE);
		pDialog[1]->ShowWindow(SW_HIDE);
		pDialog[2]->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_IDENTIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONVERT)->EnableWindow(TRUE);
		break;
	default:
		;
	}
	*pResult = 0;
}

void CtestDlg::OnClickPic(CString str)
{
	CString straddr = str;
	pathname.Format(_T(".\\Wait\\"));
	pathname +=straddr;

	CDC *pDC = pWnd1->GetDC();
	CRect rect;
	pWnd1->GetWindowRect(rect);
	DisplayPic(pathname,m_hWnd1,rect.Width(), rect.Height());

}

void CtestDlg::OnClickIdenPic(CString str)
{
	CString straddr = str;
	pathname.Format(_T(".\\Finish\\"));
	pathname +=straddr;

	CDC *pDC = pWnd1->GetDC();
	CRect rect;
	pWnd1->GetWindowRect(rect);
	DisplayPic(pathname,m_hWnd1,rect.Width(), rect.Height());

}
void CtestDlg::OnBnClickedBtnIdentify()
{
	CString result;
	// 图像识别：pathname中存有图片路径
	if( pathname.GetLength() == 0)
	{
		AfxMessageBox(L"请选择一张图片！");
		return;
	}
	
	//调用图像识别：
	
	//识别引擎
	m_showNum.SetWindowText(CImageRecognize::RecognizePlate(pathname));
	HWND m_hWnd;
	GetDlgItem(IDC_STATIC_PLATE,&m_hWnd);

	CString strToRecPath = pathname;
	strToRecPath = strToRecPath.TrimLeft(_T(".\\Wait\\"));
	CString strToPlatePath(".\\Plate\\");
	strToPlatePath += strToRecPath;

	DisplayPic(strToPlatePath,m_hWnd,150, 40);
	//显示违章时间
	WIN32_FIND_DATA stFileTime ;
	FindFirstFile(pathname,&stFileTime);
	CTime ctFileTime(stFileTime.ftCreationTime);
	CString strFileTime = ctFileTime.Format("%y-%m-%d %H:%M:%S");
	m_showTime.SetWindowText(strFileTime);
	

}

void CtestDlg::OnBnClickedBtnSave()
{
	// 保存车牌号
	CString strCarNo;
	CString strPictureName;
	CString strPeccancyTime;
	CString sql;
	for(int i=0; i<m_page_identify.m_list_identify.GetItemCount(); i++)
	{
		if( m_page_identify.m_list_identify.GetCheck(i))
		{
			//如果被选中
			strPictureName= m_page_identify.m_list_identify.GetItemText(i,0);
			strPeccancyTime = m_page_identify.m_list_identify.GetItemText(i,1);
			strCarNo = m_page_identify.m_list_identify.GetItemText(i,2);
			sql.Format(_T("Insert into Peccancy(carNo,ntime,link) values ('"+strCarNo+"','"+strPeccancyTime+"','"+strPictureName+"')"));
			CDataBase db; 
			db.ExecuteSQL(sql);
			//把图片从Finish文件夹移动到Save文件夹
			CString strPicName = pathname;	//获取图片名称
			strPicName = strPicName.TrimLeft(_T(".\\Wait\\"));
			CString strSrcPath(".\\Finish\\");
			strSrcPath += strPicName;

			CString strDestPath(".\\Save\\");  //目标路径
			strDestPath += strPictureName;
			MoveFile(strSrcPath,strDestPath);		//移动文件
			
		}
	}
	for(int i=0; i<m_page_identify.m_list_identify.GetItemCount(); i++)
	{
		if( m_page_identify.m_list_identify.GetCheck(i))
		{
			//如果被选中
			m_page_identify.m_list_identify.DeleteItem(i);
		}
	}


	

	m_loadIdenPicFlag = 0;
	
	CSaveDlg saveDlg;
	saveDlg.DoModal();
	

}

int CtestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_RecDlg.Create(IDD_DIALOG4,this);
	m_RecDlg.ShowWindow(SW_HIDE);
	return 0;
}

//查询信息
void CtestDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	int iFlag = m_searchItem.GetCurSel();
	m_peccancyList.DeleteAllItems();
	CDataBase db;
	CString sql;
	UpdateData(true);
	vector<peccancy_t> ves;
	//按姓名查询
	if (iFlag == 2)
	{
		sql.Format(_T("SELECT carMaster.name,carMaster.idCard,carMaster.carNo,Peccancy.ntime FROM CarMaster,Peccancy \
					  WHERE name like '%%"+m_searchInfo+"%%'and carMaster.carNo = Peccancy.carNo" ));
		
		db.GetPeccancyData(ves,sql);
		for (int i=0;i<(int)ves.size();++i)
		{
			int index=m_peccancyList.InsertItem(i,ves[i].strName);
			m_peccancyList.SetItemText(index, 1, ves[i].strIdCard);
			m_peccancyList.SetItemText(index, 2, ves[i].strCarNo);
			m_peccancyList.SetItemText(index, 3, ves[i].strTime);
		}
	}
	//按身份证号
	if (iFlag == 1)
	{
		sql.Format(_T("SELECT carMaster.name,carMaster.idCard,carMaster.carNo,Peccancy.ntime FROM CarMaster,Peccancy \
					  WHERE idCard like '%%"+m_searchInfo+"%%'and carMaster.carNo = Peccancy.carNo" ));
		
		db.GetPeccancyData(ves,sql);
		for (int i=0;i<(int)ves.size();++i)
		{
			int index=m_peccancyList.InsertItem(i,ves[i].strName);
			m_peccancyList.SetItemText(index, 1, ves[i].strIdCard);
			m_peccancyList.SetItemText(index, 2, ves[i].strCarNo);
			m_peccancyList.SetItemText(index, 3, ves[i].strTime);
		}
	}
	//按车牌号
	if (iFlag == 0)
	{
		sql.Format(_T("SELECT carMaster.name,carMaster.idCard,carMaster.carNo,Peccancy.ntime FROM CarMaster,Peccancy \
					  WHERE CarMaster.carNo like '%%"+m_searchInfo+"%%'and carMaster.carNo = Peccancy.carNo" ));
		
		db.GetPeccancyData(ves,sql);
		for (int i=0;i<(int)ves.size();++i)
		{
			int index=m_peccancyList.InsertItem(i,ves[i].strName);
			m_peccancyList.SetItemText(index, 1, ves[i].strIdCard);
			m_peccancyList.SetItemText(index, 2, ves[i].strCarNo);
			m_peccancyList.SetItemText(index, 3, ves[i].strTime);
		}
	}
}

void CtestDlg::OnBnClickedBtnConfirm()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strCarNumber;	//用于读取识别后的车牌号
	GetDlgItemText(IDC_EDIT_NO,strCarNumber);

	CString strCarTime;		//用于读取编辑框中的违章时间
	GetDlgItemText(IDC_EDIT_T,strCarTime);

	CString strPicName = pathname;	//获取图片名称
	strPicName = strPicName.TrimLeft(_T(".\\Wait\\"));

	//在已识别列表中添加图片名、车牌号和违章时间
	int iListLine = m_page_identify.m_list_identify.InsertItem(0,strPicName);
	m_page_identify.m_list_identify.SetItemText(iListLine,1,strCarTime);
	m_page_identify.m_list_identify.SetItemText(iListLine,2,strCarNumber);

	//把图片从Wait文件夹移动到Finish文件夹
	CString strDestPath(".\\Finish\\");  //目标路径
	strDestPath += strPicName;
	MoveFile(pathname,strDestPath);		//移动文件
	//删除未识别列表中的item


	CString tempName;
	for(int i=0; i<m_page.m_list_noidentify.GetItemCount(); i++)
	{
		tempName = m_page.m_list_noidentify.GetItemText(i,0);
		if( tempName == strPicName)
		{
			//如果被选中，删除
			m_page.m_list_noidentify.DeleteItem(i);

		}
	}


	CMessageDlg messageDlg;

	messageDlg.DoModal();
	//Sleep(1000);
	//messageDlg.OnOK();

	//把未识别列表中的相应图片删掉
}


void CtestDlg::OnBnClickedBtnConvert()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPictureName;
	CString sql;
	for(int i=0; i<m_page_convert.m_list_convert.GetItemCount(); i++)
	{
		if( m_page_convert.m_list_convert.GetCheck(i))
		{
			//如果被选中
			strPictureName= m_page_convert.m_list_convert.GetItemText(i,0);
			CImage img;
			CString temp;
			temp.Format(L".\\Wait\\%s",strPictureName);

			img.Load(temp);
	
			CString toPath;
			toPath.Format(L".\\Wait\\%s.bmp",strPictureName.Left(strPictureName.ReverseFind(L'.')));
	
			img.Save(toPath);

			CString strDestPath;
			
			strDestPath = ".\\Convert\\";
			strDestPath += strPictureName;

			MoveFile(temp,strDestPath);		//移动文件

			m_page_convert.m_list_convert.DeleteItem(i);

		}
	}

	CMessageDlg messageDlg;
	messageDlg.DoModal();

}
