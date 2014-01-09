#pragma once

#include <vector>
using namespace std;

//存储司机违章信息的结构体
typedef struct tagPeccancyInfo_t
{
	CString strName;            //司机名字
	CString strIdCard;          //司机身份证
	CString strCarNo;           //车牌号
	CString strTime;            //违章时间
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

	//数据库连接智能指针
public:
	static _ConnectionPtr m_pConnection; 
	_RecordsetPtr m_QueryRecordSet;  

public:
	void GetPeccancyData(vector<peccancy_t>& ves,CString str);

public:
	int ExecuteSQL(const CString& sql);
	HRESULT QueryData(const CString & sql);
};