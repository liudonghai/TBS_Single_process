// TBSDataBase.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSDataBase.h"
#include "sqlite3.h"
#include "TBSLog.h"

// CTBSDataBase

CMutex *CTBSDataBase::m_pMutex = new CMutex(FALSE, TBS_DATABASE_METUX);

CTBSDataBase::CTBSDataBase(string strDBName)
{
	strDataPath = strDBName;
	m_pDataBase = NULL;
}
 
CTBSDataBase::~CTBSDataBase()
{
	m_pDataBase = NULL;
}

// 数据库除查询操作以外的操作，全部由该函数完成，只需传入相应的sql语句即可。
INT CTBSDataBase::tbs_db_sql_exec(string strExpression)
{
	CSingleLock m_Lock(m_pMutex);
	m_Lock.Lock();
	
	INT iOpenResult = sqlite3_open(strDataPath.c_str(), &m_pDataBase);
	
	if (iOpenResult == SQLITE_OK)
	{
		INT		iResult;
		CHAR	*pError;
		iResult=sqlite3_exec(this->m_pDataBase, strExpression.c_str(), NULL, NULL, &pError);
		if (iResult != SQLITE_OK)
		{
			AfxMessageBox(CString(pError));
			sqlite3_close(m_pDataBase);
			return -1;
		}
		sqlite3_close(m_pDataBase); 
	}
	else
	{
		AfxMessageBox(L"数据库打开失败！！");
		sqlite3_close(m_pDataBase);
		return -1;
	}	
	m_Lock.Unlock();
	return 0;
}
INT CTBSDataBase::tbs_db_data_query(string strExpression, CHAR*** pData,INT *iRow,INT *iColum)
{
	CSingleLock m_Lock(m_pMutex);
	m_Lock.Lock();
	INT iOpenResult = sqlite3_open(strDataPath.c_str(), &m_pDataBase);
	if (SQLITE_OK == iOpenResult)
	{
		INT		iResult;
		INT		iRowNum;
		INT		iCLounNum;
		CHAR	**ppGetData;
		CHAR	*pError;

		iResult=sqlite3_get_table(this->m_pDataBase, strExpression.c_str(), &ppGetData, &iRowNum, &iCLounNum, &pError);
		if (iResult != SQLITE_OK)
		{
			AfxMessageBox(CString(pError));
			return iResult;
		}
		if (pData != NULL)
		{
			*pData = ppGetData;
		}
		if (iRow != NULL)
		{
			*iRow = iRowNum;
		}
		if (iColum != NULL)
		{
			*iColum = iCLounNum;
		}
		sqlite3_close(m_pDataBase);
	}
	else
	{
		AfxMessageBox(L"数据库打开失败！！");
		sqlite3_close(m_pDataBase);
		return -1;
	}
	m_Lock.Unlock();
	return 0;
}
INT CTBSDataBase::tbs_db_init()
{
	CSingleLock m_Lock(m_pMutex);
	m_Lock.Lock();
	INT iResult;
	INT iOpenResult = sqlite3_open(strDataPath.c_str(), &m_pDataBase);
	if (SQLITE_OK == iOpenResult)
	{
		iResult = 0;
		sqlite3_exec(m_pDataBase, "Create table if not exists TestCase(ProjectName CHAR(20), ModuleName CHAR(20), CaseName CHAR(80),ScriptName CHAR(80),CaseDirectory CHAR(120),CaseTypeFlag CHAR(20))", NULL, NULL, NULL);
		sqlite3_exec(m_pDataBase, "Create table if not exists TestReport(ProjectName CHAR(20), ProductName CHAR(20), HardwareVersion CHAR(20), SoftwareVersion CHAR(20),SN CHAR(30),ChipID CHAR(20),Result CHAR(5), Reason CHAR(80),Success INT,Fail INT,TimeBegin DATETIME,TimeEnd DATETIME,Time CHAR(20),Tester CHAR(10),Log CHAR(40))", NULL, NULL, NULL);
		sqlite3_exec(m_pDataBase, "Create table if not exists TestDetail(ProjectName CHAR(20), Module CHAR(20), CaseName CHAR(40), ScriptName CHAR(20),Describe CHAR(80), TimeBegin DATETIME,TimeEnd DATETIME,Result CHAR(5),Action CHAR(20),TimeOccur DATETIME,Expect CHAR(240),Actual CHAR(240),Time CHAR(20),ParseResult CHAR(10),ErrorLine INT,ErrorInfo CHAR(80),Name CHAR(80),CaseNumber INT)", NULL, NULL, NULL);

		/*INT		iResultExec		= 0;
		CHAR	*pError			= NULL;*/
		/*string	sqlTestReport	= "delete from TestReport";
		string	sqlTestDetail	= "delete from TestDetail";

		iResultExec =sqlite3_exec(this->m_pDataBase, sqlTestReport.c_str(),NULL,NULL,&pError);
		if (iResultExec != SQLITE_OK)
		{
			iResult = -1;
		}
		iResultExec =sqlite3_exec(this->m_pDataBase, sqlTestDetail.c_str(), NULL, NULL, &pError);
		if (iResultExec != SQLITE_OK)
		{
			iResult = -1;
		}*/

	}
	else
	{
		iResult = -2;
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iResult);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"database create success");
	m_Lock.Unlock();
	return iResult;
}