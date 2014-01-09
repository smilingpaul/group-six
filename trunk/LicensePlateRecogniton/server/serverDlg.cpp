// serverDlg.cpp : ʵ���ļ�
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
CString pathname;//���ļ���·����
int serverThreadFlag = 2;
int engine = 2;
HANDLE hReceiveHandle;//afterconnect�̵߳ľ��

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CtestDlg �Ի���




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


// CtestDlg ��Ϣ�������

BOOL CtestDlg::OnInitDialog()
{
	CLoginDlg dlg;
	int login = dlg.DoModal();
	if(login == 0)
	{
		exit(0);
	}

	CDialog::OnInitDialog();


	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//m_page = new CPage1();

	//����Υ����Ϣ�Ի���
	m_peccancyList.InsertColumn(0, _T("����"), LVCFMT_LEFT ,50);
	m_peccancyList.InsertColumn(1, _T("���֤"), LVCFMT_LEFT ,100);
	m_peccancyList.InsertColumn(2, _T("���ƺ�"), LVCFMT_LEFT ,100);
	m_peccancyList.InsertColumn(3, _T("Υ��ʱ��"), LVCFMT_LEFT ,150);

	SetTimer(124,1000,NULL);

	//ΪTab Control��������ҳ��

	m_tab.InsertItem(0, _T("δʶ��"));
	m_tab.InsertItem(1, _T("��ʶ��"));
	m_tab.InsertItem(2,_T("ͼ��ת��"));

	//���������Ի���
	m_page.Create(IDD_DIALOG1, &m_tab);
	m_page_identify.Create(IDD_DIALOG2, &m_tab);
	m_page_convert.Create(IDD_DIALOG_CONVERT,&m_tab);

	m_page.m_list_noidentify.InsertColumn(0,_T("ͼƬ��"),LVCFMT_LEFT,50);
	m_page.m_list_noidentify.InsertColumn(1,_T("Υ��ʱ��"),LVCFMT_LEFT,100);

	m_page_identify.m_list_identify.InsertColumn(0,_T("ͼƬ��"),LVCFMT_LEFT,50);
	m_page_identify.m_list_identify.InsertColumn(2,_T("���ƺ�"),LVCFMT_LEFT,100);
	m_page_identify.m_list_identify.InsertColumn(1,_T("Υ��ʱ��"),LVCFMT_LEFT,100);
	m_page_identify.m_list_identify.SetExtendedStyle(LVS_EX_CHECKBOXES);

	m_page_convert.m_list_convert.InsertColumn(0,_T("ͼƬ��"),LVCFMT_LEFT,80);
	m_page_convert.m_list_convert.InsertColumn(1,_T("Υ��ʱ��"),LVCFMT_LEFT,70);
	m_page_convert.m_list_convert.SetExtendedStyle(LVS_EX_CHECKBOXES);

	//�趨��Tab����ʾ�ķ�Χ
	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 8;
	rc.left += 8;
	rc.right -= 8;

	m_page.MoveWindow(&rc);
	m_page_identify.MoveWindow(&rc);
	m_page_convert.MoveWindow(&rc);

	//������ѶԻ������ָ�뱣������
	pDialog[0] = &m_page;
	pDialog[1] = &m_page_identify;
	pDialog[2] = &m_page_convert;


	//��ʾ��ʼҳ��
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	pDialog[2]->ShowWindow(SW_HIDE);

	//���浱ǰѡ��
	m_CurSelTab = 0;


	GetDlgItem(IDC_STATIC_PICTRUE,&m_hWnd1);
	pWnd1 = GetDlgItem(IDC_STATIC_PICTRUE);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CtestDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}



void CtestDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	//closesocket(m_socket);
	//WSACleanup();
	serverThreadFlag = 0;
}

void CtestDlg::OnBnClickedBtnExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CtestDlg::OnBnClickedBtnShrink()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox(_T("�׽��ִ���ʧ�ܣ�"));
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
		AfxMessageBox(_T("��ʧ��!"));
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI CtestDlg::RecvProc(LPVOID lpParameter)
{
	SOCKET sock=((RECVPARAM*)lpParameter)->sock;
	HWND hwnd=((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;	//�ͷ��ڴ�Ĳ���

	int len=sizeof(SOCKADDR);

	accepSocket acSocket[100];


	if(::listen(sock, 5) == SOCKET_ERROR)   //�����˿�
	{ 
		//AfxMessageBox(L"listen error");
	} 

	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	int acceptNumber = 0;
	while( serverThreadFlag == 1)
	{
		acSocket[acceptNumber].sock = accept(sock, (SOCKADDR*)&remoteAddr, &nAddrLen);  //������������
		if(acSocket[acceptNumber].sock == INVALID_SOCKET)   //��������ʧ��
		{ 
			CString conInfo(L"failed connect��");
			::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)conInfo.GetBuffer());
			continue;
		} 
		CString connInfo(L"�������ӽ�����");
		::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)connInfo.GetBuffer());
		accepSocket *lpAcceptSocket = &acSocket[acceptNumber];
		hReceiveHandle = CreateThread(NULL,0,AfterConnect,(LPVOID)lpAcceptSocket,0,NULL);  //����һ���߳����ڽ������Ӻ��ͨ��
		++acceptNumber;
	}
	return 0;
}

DWORD WINAPI CtestDlg::AfterConnect( LPVOID lpParameter )
{
	SOCKET skChildSock = ((accepSocket*)lpParameter)->sock;
	char tempBuf[100000]={0};
	int iCommand ; //�����жϽ��յ������ݻ��ǿ���
	int retval = 10;

	iCommand = recv(skChildSock,tempBuf,100000,0);

	/*����ͻ��˳�������ʲôҲ����*/
	if( iCommand == 1 && tempBuf[0] == 'C')  
	{
		shutdown(skChildSock,SD_BOTH);
	}

	/*����ͻ���Ҫ��֤�û���������*/
	else if(iCommand == 1 && tempBuf[0] == 'L')   
	{
		while(true)
		{
			retval = recv(skChildSock,tempBuf,100000,0);   //�����û���
			tempBuf[retval] = '\0';

			CString strUserName(tempBuf);				   //strUserName���û���

			memset(tempBuf,0,100000);					   //������
			retval = recv(skChildSock,tempBuf,100000,0);   //��������
			tempBuf[retval] = '\0';
			CString strPassWord(tempBuf);				   //strPassWord������

			/////////////////////////////////////////////////
			//�����ݿ��л�ȡ�û������룺

			CString strSql("");
			strSql.Format(_T("select count(*) from Login where username ='"+strUserName+"' and password='"+strPassWord+"'"));
			CDataBase db;
			db.QueryData(strSql);
			int nCount = db.m_QueryRecordSet->GetCollect(_variant_t((long)0));
			if(nCount > 0)       ////����û���������ȷ�򷵻�T
			{
				char cResult = {'T'};
				send(skChildSock,&cResult,sizeof(cResult),0);
				shutdown(skChildSock,SD_BOTH);
				break;
			}
			else			//���򷵻�F
			{
				char cResult = {'F'};
				send(skChildSock,&cResult,sizeof(cResult),0);

				retval = recv(skChildSock,tempBuf,100000,0);   //�����֤����ͻ��˻��ٷ�һ��L���˾��������ա�
				memset(tempBuf,0,100000);
			}
		}

	}

	/*����ͻ���Ҫ����ͼƬ*/
	else if(iCommand == 1 && tempBuf[0] == 'P')
	{
		int endFlag = 1;

		CFile file;
		srand(time(NULL));
		CString strFileName;    //���պ���ļ���
		while( true )
		{
			memset(tempBuf,0,100000);

			retval = recv(skChildSock,tempBuf,sizeof(tempBuf),0);

			if(retval == SOCKET_ERROR)  //������ճ��ִ���
			{
				CString nopostInfo(L"���ӶϿ���");
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

			if (endFlag == 1)  //�����һ�����ļ�
			{
				CTime time = CTime::GetCurrentTime();

				strFileName.Format(L".\\Wait\\%d_%d_%d_%d_%d_%d_%d_%d.bmp",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond(),rand(),rand());

				//FILE *flWrite = fopen(strFileName,"wb");

				file.Open(strFileName,CFile::modeCreate|CFile::modeRead|CFile::modeWrite);
			}

			if (tempBuf[0] == 'E' && retval == 1)  //���һ��ͼƬ�ѷ������
			{
				file.Close();
				endFlag = 1;
				//////////////////////////////////////////////////////
				////�����ļ��б�
				CFileFind finder;
				BOOL x = finder.FindFile(strFileName);
				
				CString strFileNameCopy = strFileName;
				strFileNameCopy.TrimLeft(_T(".\\Wait\\"));

				int iListLine = m_page.m_list_noidentify.InsertItem(0,strFileNameCopy);     //���б�������ļ�����iListLine���ڻ�ȡ����
				WIN32_FIND_DATA stFileTime ;
				FindFirstFile(strFileName,&stFileTime);
				CTime ctFileTime(stFileTime.ftCreationTime);
				CString strFileTime = ctFileTime.Format("%y-%m-%d %H:%M:%S");
				m_page.m_list_noidentify.SetItemText(iListLine,1,strFileTime);
				
				continue;
			}

			//д���ļ�


			else  //�����ͼƬ������д���ļ�
			{
				CString postInfo(L"���ڴ����ļ���......");
				/*::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)postInfo.GetBuffer());*/
				// 			fwrite(tempBuf,1,retval+1,flWrite);  //�����յ�������д���ļ�
				// 
				// 			fclose(flWrite);
				file.Write(tempBuf,retval);
				memset(tempBuf,0,100000);
				endFlag = 0;
			}

			CString sendInfo(L"�ļ�������ϡ�");



			/*::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)sendInfo.GetBuffer());

			m_loadPicFlag = 0;*/

			//������ͨ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	//��Ӳ���е�ͼ���ļ�
	hFile=CreateFile(lpImageFile,GENERIC_READ,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);


	if (hFile!=INVALID_HANDLE_VALUE)
	{
		dwFileSize=GetFileSize(hFile,NULL);//��ȡ�ļ��ֽ���

		if (dwFileSize==0xFFFFFFFF)
			return E_FAIL;
	}
	else
	{
		return E_FAIL;
	}

	//����ȫ�ִ洢�ռ�
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	LPVOID pvData = NULL;

	if (hGlobal == NULL)
	{
		AfxMessageBox(_T("ͼ���ļ��������."));
		return E_FAIL;
	}

	if ((pvData = GlobalLock(hGlobal)) == NULL)//���������ڴ��
	{
		AfxMessageBox(_T("�ڴ������������"));
		return E_FAIL;
	}

	ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//���ļ������ڴ滺����

	GlobalUnlock(hGlobal);

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStm) != S_OK)
	{
		AfxMessageBox(_T("����ʼ��ʧ��"));
		return E_FAIL;
	}

	//װ��ͼ���ļ�
	bResult=OleLoadPicture(pStm,dwFileSize,TRUE,IID_IPicture,(LPVOID*)&pPic);

	if(FAILED(bResult))
	{
		AfxMessageBox(_T("ͼ���ļ�װ�س���."));
		return E_FAIL;
	}

	OLE_XSIZE_HIMETRIC hmWidth;//ͼƬ����ʵ���
	OLE_YSIZE_HIMETRIC hmHeight;//ͼƬ����ʵ�߶�
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	//��ͼ���������Ļ��
	bResult=pPic->Render(hDC_Temp,0,0,nScrWidth,nScrHeight,
		0,hmHeight,hmWidth,-hmHeight,NULL);

	CloseHandle(hFile);//�رմ򿪵��ļ�

	pPic->Release();
	// Free memory.
	GlobalFree(hGlobal);

	if (SUCCEEDED(bResult))
	{
		return S_OK;
	}
	else
	{
		AfxMessageBox(_T("ͼ���ļ�װ�س���."));
		return E_FAIL;
	}
}
void CtestDlg::On32776()
{
	// TODO: �ڴ���������������
	CAboutDlg dlg;
	dlg.DoModal();
}

HBRUSH CtestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
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

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
void CtestDlg::OnTimer(UINT_PTR nIDEvent)
{
	//��ȡδʶ���ͼƬ�����б�
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
			//�����ʾ�ļ�����ʱ�书��
			int iListLine = m_page.m_list_noidentify.InsertItem(0,str);     //���б�������ļ�����iListLine���ڻ�ȡ����
			CTime  ctFileTime;												//�ļ�����ʱ��
			finder.GetCreationTime(ctFileTime);								//��ȡ�ļ�����ʱ��
			CString strFileTime = ctFileTime.Format("%y-%m-%d,%H:%M:%S");	//��ʱ��ת��ΪCString����
			m_page.m_list_noidentify.SetItemText(iListLine,1,strFileTime);	//����Ӧ���ϵĵڶ������ʱ��
			/////////////////////////////////////////////////////////////
		}
		finder.Close();
		m_loadPicFlag = 1;
	}

	
	//��ȡJPGͼ�����ͼ��ת���б�
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
			//�����ʾ�ļ�����ʱ�书��
			int iListLine = m_page_convert.m_list_convert.InsertItem(0,str);     //���б�������ļ�����iListLine���ڻ�ȡ����
			CTime  ctFileTime;												//�ļ�����ʱ��
			finder.GetCreationTime(ctFileTime);								//��ȡ�ļ�����ʱ��
			CString strFileTime = ctFileTime.Format("%y-%m-%d,%H:%M:%S");	//��ʱ��ת��ΪCString����
			m_page_convert.m_list_convert.SetItemText(iListLine,1,strFileTime);	//����Ӧ���ϵĵڶ������ʱ��
			/////////////////////////////////////////////////////////////
		}
		finder.Close();
		m_loadConvertPicFlag = 1;
	}


	//��ʾϵͳʱ��
	CTime t=CTime::GetCurrentTime();
	CString str=t.Format("%H:%M:%S");
	SetDlgItemText(IDC_EDIT_TIME,str);

}

void CtestDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// ͼ��ʶ��pathname�д���ͼƬ·��
	if( pathname.GetLength() == 0)
	{
		AfxMessageBox(L"��ѡ��һ��ͼƬ��");
		return;
	}
	
	//����ͼ��ʶ��
	
	//ʶ������
	m_showNum.SetWindowText(CImageRecognize::RecognizePlate(pathname));
	HWND m_hWnd;
	GetDlgItem(IDC_STATIC_PLATE,&m_hWnd);

	CString strToRecPath = pathname;
	strToRecPath = strToRecPath.TrimLeft(_T(".\\Wait\\"));
	CString strToPlatePath(".\\Plate\\");
	strToPlatePath += strToRecPath;

	DisplayPic(strToPlatePath,m_hWnd,150, 40);
	//��ʾΥ��ʱ��
	WIN32_FIND_DATA stFileTime ;
	FindFirstFile(pathname,&stFileTime);
	CTime ctFileTime(stFileTime.ftCreationTime);
	CString strFileTime = ctFileTime.Format("%y-%m-%d %H:%M:%S");
	m_showTime.SetWindowText(strFileTime);
	

}

void CtestDlg::OnBnClickedBtnSave()
{
	// ���泵�ƺ�
	CString strCarNo;
	CString strPictureName;
	CString strPeccancyTime;
	CString sql;
	for(int i=0; i<m_page_identify.m_list_identify.GetItemCount(); i++)
	{
		if( m_page_identify.m_list_identify.GetCheck(i))
		{
			//�����ѡ��
			strPictureName= m_page_identify.m_list_identify.GetItemText(i,0);
			strPeccancyTime = m_page_identify.m_list_identify.GetItemText(i,1);
			strCarNo = m_page_identify.m_list_identify.GetItemText(i,2);
			sql.Format(_T("Insert into Peccancy(carNo,ntime,link) values ('"+strCarNo+"','"+strPeccancyTime+"','"+strPictureName+"')"));
			CDataBase db; 
			db.ExecuteSQL(sql);
			//��ͼƬ��Finish�ļ����ƶ���Save�ļ���
			CString strPicName = pathname;	//��ȡͼƬ����
			strPicName = strPicName.TrimLeft(_T(".\\Wait\\"));
			CString strSrcPath(".\\Finish\\");
			strSrcPath += strPicName;

			CString strDestPath(".\\Save\\");  //Ŀ��·��
			strDestPath += strPictureName;
			MoveFile(strSrcPath,strDestPath);		//�ƶ��ļ�
			
		}
	}
	for(int i=0; i<m_page_identify.m_list_identify.GetItemCount(); i++)
	{
		if( m_page_identify.m_list_identify.GetCheck(i))
		{
			//�����ѡ��
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

	// TODO:  �ڴ������ר�õĴ�������
	m_RecDlg.Create(IDD_DIALOG4,this);
	m_RecDlg.ShowWindow(SW_HIDE);
	return 0;
}

//��ѯ��Ϣ
void CtestDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iFlag = m_searchItem.GetCurSel();
	m_peccancyList.DeleteAllItems();
	CDataBase db;
	CString sql;
	UpdateData(true);
	vector<peccancy_t> ves;
	//��������ѯ
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
	//�����֤��
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
	//�����ƺ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strCarNumber;	//���ڶ�ȡʶ���ĳ��ƺ�
	GetDlgItemText(IDC_EDIT_NO,strCarNumber);

	CString strCarTime;		//���ڶ�ȡ�༭���е�Υ��ʱ��
	GetDlgItemText(IDC_EDIT_T,strCarTime);

	CString strPicName = pathname;	//��ȡͼƬ����
	strPicName = strPicName.TrimLeft(_T(".\\Wait\\"));

	//����ʶ���б������ͼƬ�������ƺź�Υ��ʱ��
	int iListLine = m_page_identify.m_list_identify.InsertItem(0,strPicName);
	m_page_identify.m_list_identify.SetItemText(iListLine,1,strCarTime);
	m_page_identify.m_list_identify.SetItemText(iListLine,2,strCarNumber);

	//��ͼƬ��Wait�ļ����ƶ���Finish�ļ���
	CString strDestPath(".\\Finish\\");  //Ŀ��·��
	strDestPath += strPicName;
	MoveFile(pathname,strDestPath);		//�ƶ��ļ�
	//ɾ��δʶ���б��е�item


	CString tempName;
	for(int i=0; i<m_page.m_list_noidentify.GetItemCount(); i++)
	{
		tempName = m_page.m_list_noidentify.GetItemText(i,0);
		if( tempName == strPicName)
		{
			//�����ѡ�У�ɾ��
			m_page.m_list_noidentify.DeleteItem(i);

		}
	}


	CMessageDlg messageDlg;

	messageDlg.DoModal();
	//Sleep(1000);
	//messageDlg.OnOK();

	//��δʶ���б��е���ӦͼƬɾ��
}


void CtestDlg::OnBnClickedBtnConvert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strPictureName;
	CString sql;
	for(int i=0; i<m_page_convert.m_list_convert.GetItemCount(); i++)
	{
		if( m_page_convert.m_list_convert.GetCheck(i))
		{
			//�����ѡ��
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

			MoveFile(temp,strDestPath);		//�ƶ��ļ�

			m_page_convert.m_list_convert.DeleteItem(i);

		}
	}

	CMessageDlg messageDlg;
	messageDlg.DoModal();

}
