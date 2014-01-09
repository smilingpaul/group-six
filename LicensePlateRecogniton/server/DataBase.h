#pragma once

#include <vector>
using namespace std;

//�洢˾��Υ����Ϣ�Ľṹ��
typedef struct tagPeccancyInfo_t
{
	CString strName;            //˾������
	CString strIdCard;          //˾�����֤
	CString strCarNo;           //���ƺ�
	CString strTime;            //Υ��ʱ��
}peccancy_t;

class CDataBase
{
	//interface
public: 
	static bool ConnectDB();
	static bool CloseDB();

public:
	CDataBase(void);
public:
	~CDataBase(void);

	//���ݿ���������ָ��
public:
	static _ConnectionPtr m_pConnection; 
	_RecordsetPtr m_QueryRecordSet;  

public:
	void GetPeccancyData(vector<peccancy_t>& ves,CString str);

public:
	int ExecuteSQL(const CString& sql);
	HRESULT QueryData(const CString & sql);
};