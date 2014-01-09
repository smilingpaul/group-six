// SenderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sender.h"
#include "SenderDlg.h"
#include "Connect.h"
#include <sys/stat.h>
#include "atlimage.h"
#include "Exit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int pauseCheckNum = 0;

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


// CSenderDlg 对话框



CSenderDlg::CSenderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSenderDlg::IDD, pParent)
	, progNum(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PIC, m_list);
	DDX_Control(pDX, IDC_SEND_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_PROGRESS, cProgNum);
	DDX_Text(pDX, IDC_PROGRESS, progNum);
}

BEGIN_MESSAGE_MAP(CSenderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CSenderDlg::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_SELETEALL, &CSenderDlg::OnBnClickedBtnSeleteall)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PIC, &CSenderDlg::OnLvnItemchangedListPic)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSenderDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CSenderDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_OPPSELETE, &CSenderDlg::OnBnClickedBtnOppselete)
	ON_BN_CLICKED(IDC_BTN_CONTINUE, &CSenderDlg::OnBnClickedBtnContinue)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CSenderDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CSenderDlg::OnBnClickedBtnExit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PIC, &CSenderDlg::OnNMClickListPic)
END_MESSAGE_MAP()


//客户端主界面的初始化

BOOL CSenderDlg::OnInitDialog()
{
	//弹出连接对话框
 	//CLogin ldlg;
 	//INT_PTR nResponseLogin = ldlg.DoModal();
 
	CConnect cdlg;
	//m_pMainWnd = &cdlg;
	INT_PTR nResponseConnect = cdlg.DoModal();

 	if (nResponseConnect == IDOK)
 	{
 		;
 	}
 	else
 	{
 		EndDialog(0);
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
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT   |   LVS_EX_GRIDLINES   |   LVS_EX_INFOTIP   |   LVS_EX_CHECKBOXES); //增加CHEXBOX控件
	//向LISTCTRL中添加列
	m_list.InsertColumn(0,_T("路径名"),LVCFMT_LEFT,250);          
	m_list.InsertColumn(1,_T("文件名"),LVCFMT_LEFT,130);
	//初始化套接字连接
	WSAData send_wsaData;
	sender_socket=socket(AF_INET,SOCK_STREAM,0);
	WSAStartup(MAKEWORD(1, 1), &send_wsaData);
	struct IPandADDRESS
	{
		BYTE Field0;
		BYTE Field1;
		BYTE Field2;
		BYTE Field3;
		short Port;
	}IpAddress;
	FILE *fReadIp = fopen("IP.txt","rb");
	if (fReadIp!=NULL)
	{
		fread(&IpAddress,sizeof(IpAddress),1,fReadIp);
		sockaddr_in servAddr;
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(IpAddress.Port);  //目标端口号
		CString csIp;
		csIp.Format(_T("%d.%d.%d.%d"),IpAddress.Field0,IpAddress.Field1,IpAddress.Field2,IpAddress.Field3);
		servAddr.sin_addr.S_un.S_addr = inet_addr(csIp); 
		if(connect(sender_socket, (sockaddr*)&servAddr, sizeof(struct sockaddr)) != 0)   //如果连接失败
		{ 
			closesocket(sender_socket);
			AfxMessageBox(_T("连接失败！"));
		}
		else
			/*AfxMessageBox("s");*/
			fclose(fReadIp);
	}

	UpdateData(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// CSenderDlg 消息处理程序

void CSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

char *lpImageFile; //图像文件名
HWND hWnd ; //要显视对象句柄
int nScrWidth ; //显示宽度
int nScrHeight ; //显示高度
CString pathName;//打开文件的路径名


void CSenderDlg::OnPaint()
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
		//将东软LOGO显示在相应控件中
		CString path;
		CImage image;
		CWnd *psWnd = NULL;
		CRect rec;
		path = _T("D:\\LOGO.bmp");
		image.Load(path);
		psWnd = GetDlgItem(IDC_LOGO);
		psWnd->GetClientRect(rec);
		image.Draw(psWnd->GetDC()->GetSafeHdc(),rec);
		//显示缩略图的控件初始化
		CDialog::OnPaint();
		HWND m_hWnd;
		GetDlgItem(IDC_PIC_VIEW,&m_hWnd); 
		CWnd *pWnd = GetDlgItem(IDC_PIC_VIEW);
		CDC *pDC = pWnd->GetDC();
		CRect rect;
		pWnd->GetWindowRect(rect);
		DisplayPic(pathName,m_hWnd,rect.Width(), rect.Height());
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int count = 0;                 /*用于统计所有选中的图片 */
HANDLE hThreadSend;			   /*创建发送数据所用线程  */

//显示缩略图函数
HRESULT CSenderDlg::DisplayPic(CString lpImageFile, HWND hWnd, int nScrWidth, int nScrHeight)
{
	HDC hDC_Temp=::GetDC(hWnd);

	IPicture *pPic;
	IStream *pStm;

	BOOL bResult;

	HANDLE hFile=NULL;
	DWORD dwFileSize,dwByteRead;

	//打开硬盘中的图形文件
	// hFile=CreateFile(lpstrFile,GENERIC_READ,
	// FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
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
		AfxMessageBox("图形文件分配出错.");
		return E_FAIL;
	}

	if ((pvData = GlobalLock(hGlobal)) == NULL)//锁定分配内存块
	{
		AfxMessageBox("内存块锁定出问题");
		return E_FAIL;
	}

	ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//把文件读入内存缓冲区

	GlobalUnlock(hGlobal);

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStm) != S_OK)
	{
		AfxMessageBox("流初始化失败");
		return E_FAIL;
	}

	//装入图形文件
	bResult=OleLoadPicture(pStm,dwFileSize,TRUE,IID_IPicture,(LPVOID*)&pPic);

	if(FAILED(bResult))
	{
		AfxMessageBox("图形文件装载出错.");
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
		AfxMessageBox("图形文件装载出错.");
		return E_FAIL;
	}

}
//浏览文件并添加
void CSenderDlg::OnBnClickedBtnBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, 
		_T("所有文件(*.*)|*.*||"));
	if (IDOK==fileDlg.DoModal())
	{
		POSITION pos = fileDlg.GetStartPosition();
		int thisFileCount,receiveCount;      //thisFileCount 本次接受图片数量  receiveCount 接受本次浏览前列表中的文件数量
		CString filePath[50];                //文件（图片）路径
		CString fileName;                    //文件名
		for (thisFileCount=0;pos!=NULL;++thisFileCount)
		{
			filePath[thisFileCount]=fileDlg.GetNextPathName(pos);
			fileName = GetFileName(filePath[thisFileCount]);
	
			m_list.InsertItem(thisFileCount,filePath[thisFileCount]);   //添加文件路径
			m_list.SetItemText(thisFileCount,1,fileName);               //添加文件名

		}
		receiveCount = count;                          //接受本次浏览前列表中的文件数量
		count = thisFileCount + receiveCount;          //本次浏览后的文件数量
	}	
}
//获取文件名函数
CString CSenderDlg::GetFileName(const CString & strFilePath)
{
	CString strTemp;
	int nPos = -1;
	int nOldPos = -1;
	while((nPos = strFilePath.Find(_T("\\"),nPos+1)) > 0)
		nOldPos = nPos;
	strTemp = strFilePath.Right(strFilePath.GetLength()-nOldPos-1);
	strTemp=strTemp.Left(strTemp.GetLength()-4);
	return strTemp;
}
//将列表中图片全选
void CSenderDlg::OnBnClickedBtnSeleteall()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int num = 0; num < count+1; num++)
	{
		ListView_SetCheckState(m_list,num,true);
	}
}

void CSenderDlg::OnLvnItemchangedListPic(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

//声明发送图片函数
DWORD WINAPI SendPictures(LPVOID lpParameter);

void CSenderDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	//创建发送线程
	hThreadSend=CreateThread(NULL,0,SendPictures,this,0,NULL);
}

//发送图片函数实现
DWORD WINAPI SendPictures(LPVOID lpParameter)
{
	CSenderDlg *sendlg = (CSenderDlg*)lpParameter;

	SOCKADDR_IN addrTo;
	CString strSend;
	int iSelectItems = 0;          //选择的文件数
	int iPictNumItems = 0;		  //文件总数
	int errorFlag = 0;
	//得到勾选的文件数
	for (int i=0;i<sendlg->m_list.GetItemCount();i++)
	{
		if (sendlg->m_list.GetCheck(i))
		{
			iSelectItems +=1;
		}
	}
	CString strTotalNum;
	strTotalNum.Format("%d",iSelectItems);
	char SendPicturesStartFlag = {'P'};       //给服务器端发送开始发送图片的标记变量
	int lenfl ;
	lenfl=sizeof(SendPicturesStartFlag);
	send(sendlg->sender_socket,&SendPicturesStartFlag,lenfl,0);
	Sleep(20);
	for(int i=0;i<sendlg->m_list.GetItemCount();i++)
	{
		if (sendlg->m_list.GetCheck(i))
		{
			++pauseCheckNum;
			CString strText;
			CString strProgNum;
			CString strSemicolon("/");
			iPictNumItems++;
			strText =  sendlg->m_list.GetItemText(i,0);
			FILE* file=fopen(strText,"rb"); // 读写二进制文件记加 b			
			char data[100000]={0};
			int len;
			while (true)
			{
				len=fread(data,1,sizeof(data),file);
				if( len == 0 )
				{
					break;
				}
				//发送图片
				send(sendlg->sender_socket,data,len,0);
				Sleep(200);

			}
			char SendPicturesFinshedFlag = {'E'};  //给服务器端发送结束图片发送的标记变量
			int lenfli ;
			lenfli=sizeof(SendPicturesFinshedFlag);
			send(sendlg->sender_socket,&SendPicturesFinshedFlag,lenfli,0);
			fclose(file);
			Sleep(20);

			//获取发送文件数以及已发送数 计算发送进度
			strProgNum.Format("%d",iPictNumItems);
			strProgNum += strSemicolon;
			strProgNum += strTotalNum;
			sendlg->progNum.SetString(strProgNum);
			sendlg->cProgNum.SetWindowText(strProgNum);
			sendlg->m_progress.SetRange(0,iSelectItems);
			sendlg->m_progress.SetPos(iPictNumItems);
		}
		else
		{
			if (iSelectItems == 0 && errorFlag == 0)
			{
				AfxMessageBox("请勾选要发送的图片");
				errorFlag = 1;
			}

		}

		if(ListView_GetCheckState(sendlg->m_list,i)==true)
		{
			sendlg->m_list.DeleteItem(i);
			i--;
			count--;
		}		
		else if(ListView_GetCheckState(sendlg->m_list,i)==false)
		{
			//do nothing

		}
	}
	CloseHandle(hThreadSend);
	return 0;
	
}
//暂停发送
void CSenderDlg::OnBnClickedBtnPause()
{
	// TODO: 在此添加控件通知处理程序代码
	int receiveCheckNum;
	receiveCheckNum = pauseCheckNum;
	if (pauseCheckNum = receiveCheckNum+1)
	{
			SuspendThread(hThreadSend);
	}

}
//反选函数
void CSenderDlg::OnBnClickedBtnOppselete()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int num = 0; num < count+1; num++)
	{
		if(ListView_GetCheckState(m_list,num)==true)
		{
			ListView_SetCheckState(m_list,num,false);
		}		
		else if(ListView_GetCheckState(m_list,num)==false)
		{
			ListView_SetCheckState(m_list,num,true);
		}

	}
}

void CSenderDlg::OnBnClickedBtnContinue()
{
	// TODO: 在此添加控件通知处理程序代码
	ResumeThread(hThreadSend);
}

//删除选中文件
void CSenderDlg::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int num=0; num<count+1; num++)
	{

		if(ListView_GetCheckState(m_list,num)==true)
		{
			m_list.DeleteItem(num);
			num--;
			count--;
		}		
		else if(ListView_GetCheckState(m_list,num)==false)
		{
			//do nothing

		}

	}

}

void CSenderDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CExit edlg;
	edlg.DoModal();
}



//单击列表中图片显示缩略图事件函数
void CSenderDlg::OnNMClickListPic(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	CString strText;
	if (pos!=NULL)
	{
		int nItem = m_list.GetNextSelectedItem(pos);
		strText =  m_list.GetItemText(nItem,0);
		pos = 0;
	}
	HWND m_hWnd;
	GetDlgItem(IDC_PIC_VIEW,&m_hWnd);
	pathName = strText;
	CWnd *pWnd = GetDlgItem(IDC_PIC_VIEW);
	CDC *pDC = pWnd->GetDC();
	CRect rect;
	pWnd->GetWindowRect(rect);
	DisplayPic(pathName,m_hWnd,rect.Width(), rect.Height());
	*pResult = 0;
}
