// Connect.cpp : 实现文件
//

#include "stdafx.h"
#include "Sender.h"
#include "Connect.h"
#include "Sender.h"
#include "Exit.h"


// CConnect 对话框

IMPLEMENT_DYNAMIC(CConnect, CDialog)

CConnect::CConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CConnect::IDD, pParent)
	, m_port(0)
{
	
}

CConnect::~CConnect()
{
}

void CConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_addressCtrl);
	DDX_Text(pDX, IDC_PORT_EDIT, m_port);
}


BEGIN_MESSAGE_MAP(CConnect, CDialog)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CConnect::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CConnect::OnBnClickedButtonExit)
END_MESSAGE_MAP()


 //CConnect 消息处理程序
 BOOL CConnect::PreTranslateMessage(MSG* pMsg) 
 {
 	if(pMsg->message == WM_KEYDOWN) 
 	{
 		if(pMsg->wParam == VK_RETURN)//当按下键为回车键时
 		{
 			OnBnClickedBtnConnect();//调用按钮函数
 			return TRUE;
 
 		}
 		else if(pMsg->wParam == VK_ESCAPE)//当按下键为ESC键时
 		{
 			OnBnClickedButtonExit();//调用按钮函数
 			return TRUE;
 
 		}
 
 	}
 	return CDialog::PreTranslateMessage(pMsg);
 }

//建立于服务器端的连接
void CConnect::OnBnClickedBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	//套接字初始化
	WSAData wsaData;
	connect_socket=socket(AF_INET,SOCK_STREAM,0);
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	if(connect_socket < 0)
	{
		AfxMessageBox(_T("获取SOCKET号失败！"));
		exit(0);
	}

	UpdateData(true);
	//连接服务器
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(m_port);  //目标端口号

	// 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址 

	BYTE bIP[4];
	m_addressCtrl.GetAddress(bIP[0],bIP[1],bIP[2],bIP[3]);
	CString sIP;
	sIP.Format(_T("%d.%d.%d.%d"),bIP[0],bIP[1],bIP[2],bIP[3]);
	servAddr.sin_addr.S_un.S_addr = inet_addr(sIP);  //目标地址

	if(connect(connect_socket, (sockaddr*)&servAddr, sizeof(struct sockaddr)) != 0)   //如果连接失败
	{ 
		closesocket(connect_socket);
		AfxMessageBox(_T("连接失败！"));
	}

	else
	{
		AfxMessageBox(_T("连接成功！"));
		char connectFlag={'C'};						//连接服务器标志变量
		send(connect_socket,&connectFlag,sizeof(connectFlag),0);
		struct stIPandPort                      //连接成功的IP和端口号的结构体
		{
			BYTE nField0;
			BYTE nField1;
			BYTE nField2;
			BYTE nField3;
			short port;
		}stIpAdress;

		stIpAdress.nField0 = bIP[0];
		stIpAdress.nField1 = bIP[1];
		stIpAdress.nField2 = bIP[2];
		stIpAdress.nField3 = bIP[3];
		stIpAdress.port = m_port;

		FILE * fWriteIP = fopen("IP.txt","wb");

		fwrite(&stIpAdress,sizeof(stIpAdress),1,fWriteIP);  //将成功连接的IP端口号写入文件

		fclose(fWriteIP);
		OnOK();	
		
	}
}

//服务器端退出程序
void CConnect::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CExit edlg;
	edlg.DoModal();
}




//与服务器端建立连接界面的额外初始化
BOOL CConnect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	struct stIPandPort					 //从文件中读出上次连接成功的IP号和端口号的结构体
	{
		BYTE nField0;
		BYTE nField1;
		BYTE nField2;
		BYTE nField3;
		short port;
	}stIpAdress;
	FILE * fReadIP = fopen("IP.txt","rb");
	if( fReadIP != NULL)
	{
		fread(&stIpAdress,sizeof(stIpAdress),1,fReadIP);
		m_addressCtrl.SetAddress(stIpAdress.nField0,stIpAdress.nField1,stIpAdress.nField2,stIpAdress.nField3);
		m_port = stIpAdress.port;
		UpdateData(false);
	}

	fclose(fReadIP);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
