// Connect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Sender.h"
#include "Connect.h"
#include "Sender.h"
#include "Exit.h"


// CConnect �Ի���

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


 //CConnect ��Ϣ�������
 BOOL CConnect::PreTranslateMessage(MSG* pMsg) 
 {
 	if(pMsg->message == WM_KEYDOWN) 
 	{
 		if(pMsg->wParam == VK_RETURN)//�����¼�Ϊ�س���ʱ
 		{
 			OnBnClickedBtnConnect();//���ð�ť����
 			return TRUE;
 
 		}
 		else if(pMsg->wParam == VK_ESCAPE)//�����¼�ΪESC��ʱ
 		{
 			OnBnClickedButtonExit();//���ð�ť����
 			return TRUE;
 
 		}
 
 	}
 	return CDialog::PreTranslateMessage(pMsg);
 }

//�����ڷ������˵�����
void CConnect::OnBnClickedBtnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�׽��ֳ�ʼ��
	WSAData wsaData;
	connect_socket=socket(AF_INET,SOCK_STREAM,0);
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	if(connect_socket < 0)
	{
		AfxMessageBox(_T("��ȡSOCKET��ʧ�ܣ�"));
		exit(0);
	}

	UpdateData(true);
	//���ӷ�����
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(m_port);  //Ŀ��˿ں�

	// ע�⣬����Ҫ��д����������TCPServer�������ڻ�����IP��ַ 

	BYTE bIP[4];
	m_addressCtrl.GetAddress(bIP[0],bIP[1],bIP[2],bIP[3]);
	CString sIP;
	sIP.Format(_T("%d.%d.%d.%d"),bIP[0],bIP[1],bIP[2],bIP[3]);
	servAddr.sin_addr.S_un.S_addr = inet_addr(sIP);  //Ŀ���ַ

	if(connect(connect_socket, (sockaddr*)&servAddr, sizeof(struct sockaddr)) != 0)   //�������ʧ��
	{ 
		closesocket(connect_socket);
		AfxMessageBox(_T("����ʧ�ܣ�"));
	}

	else
	{
		AfxMessageBox(_T("���ӳɹ���"));
		char connectFlag={'C'};						//���ӷ�������־����
		send(connect_socket,&connectFlag,sizeof(connectFlag),0);
		struct stIPandPort                      //���ӳɹ���IP�Ͷ˿ںŵĽṹ��
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

		fwrite(&stIpAdress,sizeof(stIpAdress),1,fWriteIP);  //���ɹ����ӵ�IP�˿ں�д���ļ�

		fclose(fWriteIP);
		OnOK();	
		
	}
}

//���������˳�����
void CConnect::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CExit edlg;
	edlg.DoModal();
}




//��������˽������ӽ���Ķ����ʼ��
BOOL CConnect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	struct stIPandPort					 //���ļ��ж����ϴ����ӳɹ���IP�źͶ˿ںŵĽṹ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}
