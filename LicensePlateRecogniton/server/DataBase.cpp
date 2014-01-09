#include "StdAfx.h"
#include "DataBase.h"

_ConnectionPtr CDataBase::m_pConnection = NULL; 

CDataBase::CDataBase(void)
{
	m_QueryRecordSet.CreateInstance(__uuidof(Recordset));    
}

CDataBase::~CDataBase(void)
{

}

//�������ݿ�
bool CDataBase::ConnectDB()
{
	CoInitialize(NULL); 

	try
	{ 
		//��ʼ��Connectionָ��
		m_pConnection.CreateInstance(__uuidof(Connection));
		CString strConnectStr;
		HRESULT hr;

		//�������ݿ���ڴ��޸������ַ���
		strConnectStr = _T("Provider=Microsoft.Jet.OLEDB.4.0; \
						   Data Source=.\\TrafficDataBase.mdb; \
						   Jet OLEDB:Database Password=111");

		//�������ݿ���ڴ��޸��û���������
		hr = m_pConnection->Open((_bstr_t)strConnectStr, _T("admin"), _T(""), adModeUnknown);
	}
	catch(_com_error& e)
	{
		AfxMessageBox(e.Description());    //��ʾ�쳣��Ϣ
		return FALSE;
	}

	return TRUE;
}

//�ر����ݿ�
bool CDataBase::CloseDB()
{
	HRESULT hr = TRUE;
	if (adStateOpen == m_pConnection->State)
	{
		try
		{
			hr = m_pConnection->Close();
			m_pConnection = NULL;
		}
		catch(_com_error &e)
		{
			AfxMessageBox(e.Description());
		}
	}
	CoUninitialize();
	return (SUCCEEDED(hr)) ? TRUE : FALSE;
}

//����ɾ����
int CDataBase::ExecuteSQL(const CString& sql)
{
	_variant_t RecordsAffected;	
	m_pConnection->Execute((_bstr_t)sql, &RecordsAffected, adCmdText);
	return RecordsAffected.intVal;
}

//��
HRESULT CDataBase::QueryData(const CString & sql)
{
	if (adStateOpen == m_QueryRecordSet->State)
	{
		m_QueryRecordSet->Close();
	}
	return m_QueryRecordSet->Open(_variant_t(sql), 
		m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockPessimistic, adCmdText); 
}

void CDataBase::GetPeccancyData(vector<peccancy_t>& ves,CString str)
{
	QueryData(str);
	while (!m_QueryRecordSet->adoEOF)
	{
		peccancy_t info;
		//info.id = m_QueryRecordSet->GetCollect(_T("id")).intVal;
		info.strName = (CString)m_QueryRecordSet->GetCollect(_T("name")).bstrVal;
		info.strIdCard = (CString)m_QueryRecordSet->GetCollect(_T("idCard")).bstrVal;
		info.strCarNo = (CString)m_QueryRecordSet->GetCollect(_T("carNo")).bstrVal;
		info.strTime = m_QueryRecordSet->GetCollect(_T("ntime"));
		ves.push_back(info);
		m_QueryRecordSet->MoveNext();
	}
}

