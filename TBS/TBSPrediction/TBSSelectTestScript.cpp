// TBSSelectTestScript.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSSelectTestScript.h"
#include "afxdialogex.h"
#include "TBSApp.h"
#include "TBSDataBase.h"
#include "TBSScriptParse.h"
#include "TBSGlobal.h"
#include <string>
using namespace std;

IMPLEMENT_DYNAMIC(CTBSScriptSelectDlg, CDialogEx)

CTBSScriptSelectDlg::CTBSScriptSelectDlg(CWnd* pParent)
	: CDialogEx(IDD_SELECT_DIALOG, pParent)
	, m_iRadio(0)
{

	/*m_MainDlg		= CTBSCommon::m_PresentThread->m_pMainDlg;
	cstrProjectName = CTBSCommon::m_PresentThread->cstrProjectName;*/

}

CTBSScriptSelectDlg::~CTBSScriptSelectDlg()
{
}

void CTBSScriptSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELETE_SCRIPT, m_ScriptShow);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadio);
}

BEGIN_MESSAGE_MAP(CTBSScriptSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SELECT_ALL, &CTBSScriptSelectDlg::OnBnClickedSelectAll)
	ON_BN_CLICKED(IDOK, &CTBSScriptSelectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CTBSScriptSelectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTBSScriptSelectDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CTBSScriptSelectDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BEGIN_TEST, &CTBSScriptSelectDlg::OnBnClickedBeginTest)
END_MESSAGE_MAP()

BOOL CTBSScriptSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	INT				iRow = 0, iColum = 0;
	CHAR			**ppGetData = NULL;
	CRect			m_Rc;
	DWORD			dwStyle = m_ScriptShow.GetExtendedStyle();
	string			strProjectName = TOSTRING(CTBSCommon::m_PresentThread->cstrProjectName);
	string			strsql = "select * from TestCase where ProjectName='" + strProjectName + "'";
	CString			cstrScriptName;
	CTBSDataBase	m_DataBase(DATAPATH);

	dwStyle |= LVS_EX_CHECKBOXES;
	m_ScriptShow.SetExtendedStyle(dwStyle);
	m_ScriptShow.GetClientRect(&m_Rc);
	m_ScriptShow.InsertColumn(0, _T("ScriptName"), 0, m_Rc.Width(), 0);
	
	m_DataBase.tbs_db_data_query(strsql, &ppGetData, &iRow, &iColum);
	if (iRow > 0)
	{
		for (INT i = 1; i <= iRow; i++)
		{
			cstrScriptName = *(ppGetData + i*iColum + 3);
			m_ScriptShow.InsertItem(i - 1, cstrScriptName);
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrScriptName);
		}
	}
	return TRUE;
}

void CTBSScriptSelectDlg::OnBnClickedSelectAll()
{
	CString cstrButton;
	GetDlgItem(IDC_SELECT_ALL)->GetWindowTextW(cstrButton);
	if (cstrButton == L"全选")
	{
		GetDlgItem(IDC_SELECT_ALL)->SetWindowTextW(L"取消全选");
		for (INT i = 0; i < m_ScriptShow.GetItemCount(); i++)
		{
			m_ScriptShow.SetCheck(i);
		}
		UpdateData(false);
	}
	else
	{
		GetDlgItem(IDC_SELECT_ALL)->SetWindowTextW(L"全选");
		for (INT i = 0; i < m_ScriptShow.GetItemCount(); i++)
		{
			m_ScriptShow.SetCheck(i, 0);
		}
		UpdateData(false);
	}
}

void CTBSScriptSelectDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CTBSScriptSelectDlg::OnBnClickedRadio1()
{
	if (m_iRadio != 0)
	{
		INT				iRow = 0, iColum = 0;
		CHAR			**ppGetData = NULL;
		string			strProjectName = TOSTRING(CTBSCommon::m_PresentThread->cstrProjectName);
		string			strsql = "select * from TestCase where ProjectName='" + strProjectName + "'";
		CString			cstrScriptName;
		CTBSDataBase	m_DataBase(DATAPATH);
		 
		m_ScriptShow.DeleteAllItems();
		m_DataBase.tbs_db_data_query(strsql, &ppGetData, &iRow, &iColum);
		if (iRow > 0)
		{
			for (INT i = 1; i <= iRow; i++)
			{
				cstrScriptName = *(ppGetData + i*iColum + 3);
				m_ScriptShow.InsertItem(i - 1, cstrScriptName);
			}
		}
		m_iRadio = 0;
		UpdateData(0);
	}
}

void CTBSScriptSelectDlg::OnBnClickedRadio2()
{
	if (m_iRadio != 1)
	{
		INT				iRow = 0, iColum = 0;
		CHAR			**ppGetData = NULL;
		string			strProjectName=TOSTRING(CTBSCommon::m_PresentThread->cstrProjectName);
		string			strsql = "select * from TestCase where ProjectName='" + strProjectName + "' and CaseTypeFlag='auto'";
		CString			cstrScriptName;
		CTBSDataBase	m_DataBase(DATAPATH);

		m_ScriptShow.DeleteAllItems();
		m_DataBase.tbs_db_data_query(strsql, &ppGetData, &iRow, &iColum);
		if (iRow > 0)
		{
			for (INT i = 1; i <= iRow; i++)
			{
				cstrScriptName = *(ppGetData + i*iColum + 3);
				m_ScriptShow.InsertItem(i - 1, cstrScriptName);
			}
		}
		m_iRadio = 1;
		UpdateData(0);
	}
}

void CTBSScriptSelectDlg::OnBnClickedRadio3()
{
	if (m_iRadio != 2)
	{
		INT				iRow = 0, iColum = 0;
		CHAR			**ppGetData = NULL;
		string			strProjectName(TOSTRING(CTBSCommon::m_PresentThread->cstrProjectName));
		string			strsql = "select * from TestCase where ProjectName='" + strProjectName + "' and CaseTypeFlag='semi'";
		CString			cstrScriptName;
		CTBSDataBase	m_DataBase(DATAPATH);

		m_ScriptShow.DeleteAllItems();
		m_DataBase.tbs_db_data_query(strsql, &ppGetData, &iRow, &iColum);
		if (iRow > 0)
		{

			for (INT i = 1; i <= iRow; i++)
			{
				cstrScriptName = *(ppGetData + i*iColum + 3);
				m_ScriptShow.InsertItem(i - 1, cstrScriptName);
			}
		}
		m_iRadio = 2;
		UpdateData(0);
	}
}

void CTBSScriptSelectDlg::OnBnClickedBeginTest()
{
	INT				i, iItemNum = 0;
	CTime			m_Time;
	string			strTime;
	string			strSqlUpdate;
	string			strProject	= TOSTRING(CTBSCommon::m_PresentThread->cstrProjectName);
	CString			*pItemValue = new CString[MAX_PATH];
	CString			*pHeadItem	= pItemValue;
	CString			cstrTime;
	CTBSDataBase	m_DataBase(DATAPATH);
	tbs_thread_script_t *m_ThreadScript = NULL;

	if (CTBSCommon::m_PresentThread->m_TestThread != NULL)
	{
		AfxMessageBox(L"请等待当前测试项执行完成，或者强制结束当前的测试项。");
		return;
	}


	if (CTBSCommon::m_PresentThread->cstrIRFile.IsEmpty())
	{
		AfxMessageBox(L"IR数据文件为空，请导入IR数据文件！");
		return;
	}

	if (CTBSCommon::m_PresentThread->cstrRedRat == L"")
	{
		AfxMessageBox(L"IR设备未指定，请指定IR设备！");
		return;
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, strProject+"开始测试");
	CTBSCommon::m_PresentThread->m_pMainDlg->m_TestList.DeleteAllItems();
	for (i = 0; i < m_ScriptShow.GetItemCount(); i++)
	{
		if (m_ScriptShow.GetCheck(i))
		{
			*pItemValue=m_ScriptShow.GetItemText(i, 0);
			CTBSCommon::m_PresentThread->m_pMainDlg->m_TestList.InsertItem(iItemNum, *pItemValue);
			CTBSCommon::m_PresentThread->m_pMainDlg->m_TestList.SetItemText(iItemNum,1, SCRIPT_WAIT);
			pItemValue++;
			iItemNum++;
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"选择测试脚本："+ *pItemValue);
		}																	
	}

	if (iItemNum == 0)
	{
		AfxMessageBox(L"所选脚本为空！");
		return;
	}
	pItemValue = NULL;
	m_Time = CTime::GetCurrentTime();
	cstrTime = m_Time.Format(_T("%Y-%m-%d %H:%M:%S"));
	strTime = TOSTRING(cstrTime);
	strSqlUpdate = "Update TestReport set TimeBegin='" + strTime + "' where ProjectName='" + strProject + "'";
	m_DataBase.tbs_db_sql_exec(strSqlUpdate);
	
	m_ThreadScript = new tbs_thread_script_t;
	m_ThreadScript->pcstrHead = pHeadItem;
	m_ThreadScript->iScriptNum = iItemNum;
	m_ThreadScript->m_Script = NULL;
	
	CTBSCommon::m_PresentThread->m_TestThread = AfxBeginThread(CTBSScriptParse::tbs_test_script_execute, m_ThreadScript, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (CTBSCommon::m_PresentThread->m_TestThread == NULL)
	{
		AfxMessageBox(L"创建后台脚本执行线程失败！");
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "脚本执行线程创建成功");
	
	m_ThreadScript->pThreadExc = CTBSCommon::m_PresentThread->m_TestThread;

	CTBSCommon::m_PresentThread->m_ParseThread = AfxBeginThread(CTBSScriptParse::tbs_test_script_parse, m_ThreadScript);
	if (CTBSCommon::m_PresentThread->m_ParseThread == NULL)
	{
		AfxMessageBox(L"创建后台脚本解析线程失败！");
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "脚本解析线程创建成功");
	
	INT arrMenuIDEnable[] = { IDM_TEST_FINISH };
	CTBSDlg::tbs_menu_enable(arrMenuIDEnable, 1);
	CDialogEx::OnOK();
}

UINT CTBSScriptSelectDlg::tbs_test_manager(LPVOID lpParam)
{
	return 0;
}