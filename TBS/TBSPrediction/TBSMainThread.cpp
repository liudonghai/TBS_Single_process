// TBSMainThread.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSMainThread.h"
#include "TBSApp.h"
#include "TBSDataBase.h"
#include "TBSDlg.h"
#include "TBSMainDlg.h"
#include "TBSScriptParse.h"
#include "CMSComm.h"
#include <regex>
using namespace std;

IMPLEMENT_DYNCREATE(CTBSMainThread, CWinThread)

CTBSMainThread::CTBSMainThread()
{
	m_MainDlg = NULL;
}

CTBSMainThread::~CTBSMainThread()
{
}

BOOL CTBSMainThread::InitInstance()
{
	m_MainDlg = new CTBSMainDlg();
	CTBSCommon::m_PresentThread->m_pMainDlg = m_MainDlg;
	m_MainDlg->ShowWindow(SW_SHOW);
	
	//for (INT i = 0; i <= CTBSCommon::m_PresentThread->iTabNum ; i++)
	//{
	//	if (i == CTBSCommon::iTBSPresent)
	//	{
	//		CTBSCommon::m_PresentThread[i].m_pMainDlg->ShowWindow(SW_SHOW);
	//	}
	//	else
	//	{
	//		CTBSCommon::m_PresentThread[i].m_pMainDlg->ShowWindow(SW_HIDE);
	//	}
	//}
	//m_MainDlg->UpdateWindow();	//更新指定窗口的客户区;

	CTBSDataBase m_DataBase(DATAPATH);

	string strProjectName=TOSTRING(m_MainDlg->cstrProjectName);
	
	//delete Testreport
	string strSqlDeleteTestReport = "delete from TestReport where ProjectName='"+ strProjectName +"'";
	//delete TestDetail
	string strSqlDeleteTestDetail = "delete from TestDetail where ProjectName='" + strProjectName + "'";
	//Insert TestReport
	string strSqlInsert = "Insert into TestReport (ProjectName) values ('" + strProjectName + "')";
	
	m_DataBase.tbs_db_sql_exec(strSqlDeleteTestReport);
	m_DataBase.tbs_db_sql_exec(strSqlDeleteTestDetail);
	m_DataBase.tbs_db_sql_exec(strSqlInsert);
	return TRUE; 
}

INT CTBSMainThread::ExitInstance()
{
	m_MainDlg->DestroyWindow();
	m_MainDlg = NULL;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTBSMainThread, CWinThread)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CTBSMainThread, CWinThread)
END_EVENTSINK_MAP()