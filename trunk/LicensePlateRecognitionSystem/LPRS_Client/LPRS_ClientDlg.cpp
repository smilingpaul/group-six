
// LPRS_ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPRS_Client.h"
#include "LPRS_ClientDlg.h"
#include "stdlib.h"

#pragma comment(lib,"Ws2_32")
CString arr[10]={NULL};
CString arrName[10]={NULL};

// LPRS_ClientDlg �Ի���

IMPLEMENT_DYNAMIC(LPRS_ClientDlg, CDialog)

UINT SendFile(void *p/*����ָ��*/)
{
    

	CString strError;
	ASSERT(p!=NULL);//
	LPRS_ClientDlg *pDlg = (LPRS_ClientDlg*)p;
	CString strIP;
	pDlg->m_cSvrIP.GetWindowText(strIP);

	sockaddr_in addr;
	addr.sin_family=AF_INET;//��ʾ��INT��ͨ��
	addr.sin_addr.S_un.S_addr=inet_addr((LPSTR)(LPCTSTR)strIP.GetBuffer(0));
	addr.sin_port=htons(pDlg->m_iPort); //ת��Ϊ�����ֽ���

	int ret=0;
	int error=0;
	ret=connect(pDlg->m_hSocket,(LPSOCKADDR)&addr,sizeof(addr));//�������ӷ�����
	if(ret==SOCKET_ERROR)
	{
		strError.Format(_T("Connect Error:%d "),error=WSAGetLastError());
		AfxMessageBox(strError);
		closesocket(pDlg->m_hSocket);
		return -1;
	}


	//�ļ�����
	int num = ((CListCtrl*)(pDlg->GetDlgItem(IDC_LIST1)))->GetItemCount();
	CString strNum;
	strNum.Format(_T("%d"),num);
	int ret1=send(pDlg->m_hSocket,(LPSTR)(LPCTSTR)strNum, 10, 0);	//�����ļ��ĸ���

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
			AfxMessageBox(_T("���ļ�ʧ��"));
			return -1;
		}
		lFileSize = file.GetLength();

		pDlg->m_Progress.SetRange32(0, lFileSize);	//���ý�������Χ
		pDlg->m_Progress.SetStep(1);	            //���ý���������

		char temp[1024];   //�޸�         	            //������������ļ���С���������������ļ�������
		memset(temp,0,sizeof(temp)); 
		ltoa(lFileSize, temp, 10);                  //ת��Ϊ�ַ�

	    str = arrName[j];     //��ȡ�ļ�����
		str += "*";
		str += temp;
		int i=str.GetLength();

		ret1 = send(pDlg->m_hSocket, (LPSTR)(LPCTSTR)str, 100, 0);	//�����ļ������ƺʹ�С


		if (ret1==SOCKET_ERROR)
		{
				strError.Format(_T("Send File: %d "),error=WSAGetLastError());
				AfxMessageBox(strError);
				return -1;
		}
	    
		/////////////////////////�����ļ�///////////////////////////////////////////
		int flagFileSize = 0;
		while (1)
		{
			
			ilen = file.Read(temp, 1024);
			
			if (ilen < 1024)
			{
				flagFileSize += ilen;  //�޸�
				iEnd = send(pDlg->m_hSocket,temp,1024,0);
				break;
			}
			flagFileSize += ilen;  //�޸�
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
			//AfxMessageBox("�ļ����ͳɹ���");
			
		}
		else
		{
			strError.Format(str+"�ļ�����ʧ��");
			AfxMessageBox(strError);
			return 0;
		}*/
		

	}
	
	/////////////////�жϷ����Ƿ�ɹ�����������////////////////////////////////////
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


// LPRS_ClientDlg ��Ϣ�������

BOOL LPRS_ClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	this->m_ListControl.InsertColumn(0,_T("�ļ���"),LVCFMT_LEFT,120); 
	this->m_ListControl.InsertColumn(1,_T("�ļ�·��"),LVCFMT_LEFT,250);
	
	this->UpdateData(false);
	LPRS_ClientDlg dlg;

	//��ʾip�Ͷ˿�

	CString str2,str3;
	CString filename=_T("..\\LPRS_Client\\port.txt");
	CStdioFile f2;
	CFileException fileException;
	if(!f2.Open(filename,CFile::modeReadWrite,&fileException))
	{
		MessageBox(_T("�ļ��򲻿�"));
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


	//GetDlgItem(IDC_BTNDELETE)->EnableWindow(false);//��ɾ����ť����Ϊ������

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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void LPRS_ClientDlg::OnBnClickedBtnbrows()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ���ù�����   
    TCHAR szFilter[] = _T("�����ļ�(*.*)|*.*|ͼƬ�ļ�(*.bmp)|*.bmp|");   
    // ������ļ��Ի���   
    //CFileDialog fileDlg(TRUE, _T("bmp"), NULL, 0, szFilter, this);

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, _T("�����ļ�(*.*)|*.*|ͼƬ�ļ�(*.bmp)|*.bmp|ͼƬ�ļ�(*.jpg)|*.jpg||"), this);
	fileDlg.m_ofn.lpstrTitle = _T("��");

	 if (IDOK == fileDlg.DoModal())   
    {   
        // ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		/*m_strFileName=fileDlg.GetFileName();
        m_strPathName = fileDlg.GetPathName(); 

		this->m_ListControl.InsertItem(0,m_strFileName);
		this->m_ListControl.SetItemText(0,1,m_strPathName);
		*/

		POSITION pos = fileDlg.GetStartPosition();

		while(pos)
		{
			m_strPathName = fileDlg.GetNextPathName(pos);
			//���ļ�
			CFile file(m_strPathName, CFile::modeRead);
			m_strFileName = file.GetFileName();
			//�ر��ļ�
			file.Close();
			UpdateData(FALSE);
			

			//�����ļ��б�
			this->m_ListControl.InsertItem(0,m_strFileName);
			this->m_ListControl.SetItemText(0,1,m_strPathName);
		}
			
		  
    }
}



void LPRS_ClientDlg::OnBnClickedBtndelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
    

	if(m_hSocket)//����Ѿ��������ȹر�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData();
	m_hSocket=NULL;
	InitSock();//��ʼ��SOCK��BIND
    CFileFind find;
	int num = m_ListControl.GetItemCount();
	for(int i =0;i < num; ++i)
	{
		arr[i] = m_ListControl.GetItemText(i,1);
		arrName[i] = m_ListControl.GetItemText(i,0);
		
	}

  
	if(!find.FindFile(m_strPathName))
	{
		MessageBox(_T("�ļ�·����Ч","����"));
		return;
	}

	AfxBeginThread(SendFile, this);

	//OnOK();
}

void LPRS_ClientDlg::OnBnClickedBtnstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void LPRS_ClientDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	

	OnCancel();

}

void LPRS_ClientDlg::OnPaint()
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
		CRect rect;
        CPaintDC dc(this);
        GetClientRect(rect);
        dc.FillSolidRect(rect,RGB(167,220,234)); //����Ϊ��ɫ����
		CDialog::OnPaint();
	}
	 // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void LPRS_ClientDlg::OnDestroy()
{
	if(m_hSocket!=NULL)
	{
		::closesocket(m_hSocket);
	}

	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

BOOL LPRS_ClientDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���

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
       // pDC->SetTextColor(RGB(0,0,0));  //������ɫ
       pDC->SetBkColor(RGB(167,220,234));
	  //pDC->SetBkMode(HOLLOW_BRUSH); //�������屳��Ϊ͸��
    }
	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
