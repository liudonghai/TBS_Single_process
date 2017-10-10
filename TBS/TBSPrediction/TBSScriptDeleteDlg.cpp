// TBSScriptDeleteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSScriptDeleteDlg.h"
#include "afxdialogex.h"
#include "TBSApp.h"
#include "TBSDataBase.h"
#include <string>
using namespace std;
// CTBSScriptDeleteDlg 对话框

IMPLEMENT_DYNAMIC(CTBSScriptDeleteDlg, CDialogEx)

CTBSScriptDeleteDlg::CTBSScriptDeleteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DELETE_SCRIPT, pParent)
{
	strProjectName = TOSTRING(CTBSCommon::m_PresentThread->cstrProjectName);
}

CTBSScriptDeleteDlg::~CTBSScriptDeleteDlg()
{
}

void CTBSScriptDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELETE_SCRIPT, m_DeleteShow);
	DDX_Control(pDX, IDC_INCLUDE_FILE, m_DeleteFile);
}

BEGIN_MESSAGE_MAP(CTBSScriptDeleteDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSScriptDeleteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CTBSScriptDeleteDlg::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_SELECT_ALL, &CTBSScriptDeleteDlg::OnBnClickedSelectAll)
END_MESSAGE_MAP()

BOOL CTBSScriptDeleteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	INT				iRow		= 0, iColum = 0;
	CHAR			**ppGetData = NULL;
	DWORD			dwStyle = m_DeleteShow.GetExtendedStyle();
	string			strsql		= "select * from TestCase where ProjectName='" + strProjectName+"'";
	CString			cstrScriptName;
	CTBSDataBase	m_DataBase(DATAPATH);

	dwStyle |= LVS_EX_CHECKBOXES;
	m_DeleteShow.SetExtendedStyle(dwStyle);
	m_DeleteShow.InsertColumn(0, _T("ScriptName"), 0, 300, 0);
	m_DataBase.tbs_db_data_query(strsql, &ppGetData, &iRow, &iColum);
	if (iRow > 0)
	{
		for (INT i = 1; i <= iRow; i++)
		{
			cstrScriptName = *(ppGetData+i*iColum+3);
			m_DeleteShow.InsertItem(i-1, cstrScriptName);
		}
	}
	return TRUE;  
}

void CTBSScriptDeleteDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CTBSScriptDeleteDlg::OnBnClickedDeleteButton()
{
	CTBSDataBase m_DataBase(DATAPATH);

	for (INT i = m_DeleteShow.GetItemCount() - 1; i >= 0; i--)
	{
		if (m_DeleteShow.GetCheck(i))
		{
			string strDeleteFile (CW2A(m_DeleteShow.GetItemText(i, 0).GetString()));
			if (m_DeleteFile.GetCheck())
			{
				INT		iRow			= 0;
				INT		iColum			= 0;
				CHAR	**ppGetData;
				string	strSqlSelect	= "select * from TestCase where ProjectName='" + strProjectName + "' and ScriptName='" + strDeleteFile + "'";
				m_DataBase.tbs_db_data_query(strSqlSelect, &ppGetData, &iRow, &iColum);
				if (iRow == 1)
				{
					string strGetPath = *(ppGetData+ iColum+4);
					CString cstrPath(strGetPath.c_str());
					DeleteFile(cstrPath);
				}
			}
			string strSqlDelete = "delete from TestCase where ProjectName='" + strProjectName + "' and ScriptName='" + strDeleteFile + "'";
			m_DataBase.tbs_db_sql_exec(strSqlDelete);
			m_DeleteShow.DeleteItem(i);
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "删除脚本："+strDeleteFile);
		}
	}
	UpdateData(0);
}

void CTBSScriptDeleteDlg::OnBnClickedSelectAll()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstrButton;
	GetDlgItem(IDC_SELECT_ALL)->GetWindowTextW(cstrButton);
	if (cstrButton == L"全选")
	{

		GetDlgItem(IDC_SELECT_ALL)->SetWindowTextW(L"取消全选");
		for (INT i = 0; i < m_DeleteShow.GetItemCount(); i++)
		{
			m_DeleteShow.SetCheck(i);
		}
		UpdateData(false);
	}
	else
	{
		GetDlgItem(IDC_SELECT_ALL)->SetWindowTextW(L"全选");
		for (INT i = 0; i < m_DeleteShow.GetItemCount(); i++)
		{
			m_DeleteShow.SetCheck(i,0);
		}
		UpdateData(false);
	}
}


