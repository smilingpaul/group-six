// SenderDlg.cpp : ʵ���ļ�
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


// CSenderDlg �Ի���



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


//�ͻ���������ĳ�ʼ��

BOOL CSenderDlg::OnInitDialog()
{
	//�������ӶԻ���
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
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT   |   LVS_EX_GRIDLINES   |   LVS_EX_INFOTIP   |   LVS_EX_CHECKBOXES); //����CHEXBOX�ؼ�
	//��LISTCTRL�������
	m_list.InsertColumn(0,_T("·����"),LVCFMT_LEFT,250);          
	m_list.InsertColumn(1,_T("�ļ���"),LVCFMT_LEFT,130);
	//��ʼ���׽�������
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
		servAddr.sin_port = htons(IpAddress.Port);  //Ŀ��˿ں�
		CString csIp;
		csIp.Format(_T("%d.%d.%d.%d"),IpAddress.Field0,IpAddress.Field1,IpAddress.Field2,IpAddress.Field3);
		servAddr.sin_addr.S_un.S_addr = inet_addr(csIp); 
		if(connect(sender_socket, (sockaddr*)&servAddr, sizeof(struct sockaddr)) != 0)   //�������ʧ��
		{ 
			closesocket(sender_socket);
			AfxMessageBox(_T("����ʧ�ܣ�"));
		}
		else
			/*AfxMessageBox("s");*/
			fclose(fReadIp);
	}

	UpdateData(false);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// CSenderDlg ��Ϣ�������

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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

char *lpImageFile; //ͼ���ļ���
HWND hWnd ; //Ҫ���Ӷ�����
int nScrWidth ; //��ʾ���
int nScrHeight ; //��ʾ�߶�
CString pathName;//���ļ���·����


void CSenderDlg::OnPaint()
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
		//������LOGO��ʾ����Ӧ�ؼ���
		CString path;
		CImage image;
		CWnd *psWnd = NULL;
		CRect rec;
		path = _T("D:\\LOGO.bmp");
		image.Load(path);
		psWnd = GetDlgItem(IDC_LOGO);
		psWnd->GetClientRect(rec);
		image.Draw(psWnd->GetDC()->GetSafeHdc(),rec);
		//��ʾ����ͼ�Ŀؼ���ʼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int count = 0;                 /*����ͳ������ѡ�е�ͼƬ */
HANDLE hThreadSend;			   /*�����������������߳�  */

//��ʾ����ͼ����
HRESULT CSenderDlg::DisplayPic(CString lpImageFile, HWND hWnd, int nScrWidth, int nScrHeight)
{
	HDC hDC_Temp=::GetDC(hWnd);

	IPicture *pPic;
	IStream *pStm;

	BOOL bResult;

	HANDLE hFile=NULL;
	DWORD dwFileSize,dwByteRead;

	//��Ӳ���е�ͼ���ļ�
	// hFile=CreateFile(lpstrFile,GENERIC_READ,
	// FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
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
		AfxMessageBox("ͼ���ļ��������.");
		return E_FAIL;
	}

	if ((pvData = GlobalLock(hGlobal)) == NULL)//���������ڴ��
	{
		AfxMessageBox("�ڴ������������");
		return E_FAIL;
	}

	ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//���ļ������ڴ滺����

	GlobalUnlock(hGlobal);

	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStm) != S_OK)
	{
		AfxMessageBox("����ʼ��ʧ��");
		return E_FAIL;
	}

	//װ��ͼ���ļ�
	bResult=OleLoadPicture(pStm,dwFileSize,TRUE,IID_IPicture,(LPVOID*)&pPic);

	if(FAILED(bResult))
	{
		AfxMessageBox("ͼ���ļ�װ�س���.");
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
		AfxMessageBox("ͼ���ļ�װ�س���.");
		return E_FAIL;
	}

}
//����ļ������
void CSenderDlg::OnBnClickedBtnBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, 
		_T("�����ļ�(*.*)|*.*||"));
	if (IDOK==fileDlg.DoModal())
	{
		POSITION pos = fileDlg.GetStartPosition();
		int thisFileCount,receiveCount;      //thisFileCount ���ν���ͼƬ����  receiveCount ���ܱ������ǰ�б��е��ļ�����
		CString filePath[50];                //�ļ���ͼƬ��·��
		CString fileName;                    //�ļ���
		for (thisFileCount=0;pos!=NULL;++thisFileCount)
		{
			filePath[thisFileCount]=fileDlg.GetNextPathName(pos);
			fileName = GetFileName(filePath[thisFileCount]);
	
			m_list.InsertItem(thisFileCount,filePath[thisFileCount]);   //����ļ�·��
			m_list.SetItemText(thisFileCount,1,fileName);               //����ļ���

		}
		receiveCount = count;                          //���ܱ������ǰ�б��е��ļ�����
		count = thisFileCount + receiveCount;          //�����������ļ�����
	}	
}
//��ȡ�ļ�������
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
//���б���ͼƬȫѡ
void CSenderDlg::OnBnClickedBtnSeleteall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int num = 0; num < count+1; num++)
	{
		ListView_SetCheckState(m_list,num,true);
	}
}

void CSenderDlg::OnLvnItemchangedListPic(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

//��������ͼƬ����
DWORD WINAPI SendPictures(LPVOID lpParameter);

void CSenderDlg::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������߳�
	hThreadSend=CreateThread(NULL,0,SendPictures,this,0,NULL);
}

//����ͼƬ����ʵ��
DWORD WINAPI SendPictures(LPVOID lpParameter)
{
	CSenderDlg *sendlg = (CSenderDlg*)lpParameter;

	SOCKADDR_IN addrTo;
	CString strSend;
	int iSelectItems = 0;          //ѡ����ļ���
	int iPictNumItems = 0;		  //�ļ�����
	int errorFlag = 0;
	//�õ���ѡ���ļ���
	for (int i=0;i<sendlg->m_list.GetItemCount();i++)
	{
		if (sendlg->m_list.GetCheck(i))
		{
			iSelectItems +=1;
		}
	}
	CString strTotalNum;
	strTotalNum.Format("%d",iSelectItems);
	char SendPicturesStartFlag = {'P'};       //���������˷��Ϳ�ʼ����ͼƬ�ı�Ǳ���
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
			FILE* file=fopen(strText,"rb"); // ��д�������ļ��Ǽ� b			
			char data[100000]={0};
			int len;
			while (true)
			{
				len=fread(data,1,sizeof(data),file);
				if( len == 0 )
				{
					break;
				}
				//����ͼƬ
				send(sendlg->sender_socket,data,len,0);
				Sleep(200);

			}
			char SendPicturesFinshedFlag = {'E'};  //���������˷��ͽ���ͼƬ���͵ı�Ǳ���
			int lenfli ;
			lenfli=sizeof(SendPicturesFinshedFlag);
			send(sendlg->sender_socket,&SendPicturesFinshedFlag,lenfli,0);
			fclose(file);
			Sleep(20);

			//��ȡ�����ļ����Լ��ѷ����� ���㷢�ͽ���
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
				AfxMessageBox("�빴ѡҪ���͵�ͼƬ");
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
//��ͣ����
void CSenderDlg::OnBnClickedBtnPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int receiveCheckNum;
	receiveCheckNum = pauseCheckNum;
	if (pauseCheckNum = receiveCheckNum+1)
	{
			SuspendThread(hThreadSend);
	}

}
//��ѡ����
void CSenderDlg::OnBnClickedBtnOppselete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ResumeThread(hThreadSend);
}

//ɾ��ѡ���ļ�
void CSenderDlg::OnBnClickedBtnDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CExit edlg;
	edlg.DoModal();
}



//�����б���ͼƬ��ʾ����ͼ�¼�����
void CSenderDlg::OnNMClickListPic(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
