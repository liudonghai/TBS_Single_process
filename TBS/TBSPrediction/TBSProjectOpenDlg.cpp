// TBSProjectOpenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSGlobal.h"
#include "TBSPrediction.h"
#include "TBSProjectOpenDlg.h"
#include "afxdialogex.h"
#include "TBSCommon.h"
#include "TBSMainThread.h"
#include "TBSPredictionDlg.h"
#include "TBSMainDlg.h"
#include <fstream>
#include <string>
using namespace std;


IMPLEMENT_DYNAMIC(CTBSProjectOpenDlg, CDialogEx)

CTBSProjectOpenDlg::CTBSProjectOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDC_PROJECT_SELECT, pParent)
	, csItemString(_T(""))
{
	iItem = 0;
}

CTBSProjectOpenDlg::~CTBSProjectOpenDlg()
{
}

void CTBSProjectOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECT_LIST, m_ProjectList);
	DDX_Control(pDX, IDC_PROJECT_SELECT, m_SelectName);
}


BEGIN_MESSAGE_MAP(CTBSProjectOpenDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_CLICK, IDC_PROJECT_LIST, &CTBSProjectOpenDlg::OnClickProjectList)
	ON_BN_CLICKED(IDOK, &CTBSProjectOpenDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DELETE_PROJECT, &CTBSProjectOpenDlg::OnBnClickedDeleteProject)
END_MESSAGE_MAP()

BOOL CTBSProjectOpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	INT		iListItem = 0;
	INT		iResult;
	CRect	m_Rc;
	CString	csProjectPath = PROJECTPATH;
	CString cstrProjectDirctory;
	CFileFind m_FileFind;

	m_ProjectList.GetClientRect(&m_Rc);
	m_ProjectList.InsertColumn(0, _T("项目名称"), 0, m_Rc.Width(), 0);
	iResult=m_FileFind.FindFile(csProjectPath + L"*.*");
	while(iResult)
	{
		iResult = m_FileFind.FindNextFile();
		if (m_FileFind.IsDots())
			continue;
		if(m_FileFind.IsDirectory())
		{
			cstrProjectDirctory = m_FileFind.GetFileTitle();
			m_ProjectList.InsertItem(iListItem, cstrProjectDirctory);

			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"初始化project名称："+cstrProjectDirctory);
			iListItem++;
		}
	}
	return TRUE;  
}

void CTBSProjectOpenDlg::OnClickProjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	HD_NOTIFY *lpNMItemActivate = (HD_NOTIFY*)pNMHDR;
	if (lpNMItemActivate != NULL)
	{
		iItem = lpNMItemActivate->iItem;
		csItemString = m_ProjectList.GetItemText(lpNMItemActivate->iItem, 0);
	}
	((CEdit*)GetDlgItem(IDC_PROJECT_SELECT))->SetWindowTextW(csItemString);
	*pResult = 0;
}

void CTBSProjectOpenDlg::OnBnClickedOk()
{
	static INT iStartNum = 0;

	if (csItemString.IsEmpty())
	{
		AfxMessageBox(L"请选择需要打开的project！");
		return;
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iStartNum+1);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, csItemString);
	if (tbs_check_project(csItemString))
	{
		AfxMessageBox(L"该Project已经打开");
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "Project打开失败： 打开重复的Project！");
		return;
	}
	if(CTBSCommon::iTBSNum < 8)
	{	
		CString csProjectPath = PROJECTPATH + csItemString + L"\\" + csItemString + L".ini";
		if (CTBSCommon::tbs_file_check(csProjectPath))
		{
			CRuntimeClass* prt = RUNTIME_CLASS(CTBSMainThread);
			CTBSCommon::m_PresentThread[CTBSCommon::iTBSNum].m_MainThread = AfxBeginThread(prt, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
			if (CTBSCommon::m_PresentThread[CTBSCommon::iTBSNum].m_MainThread == NULL)
			{
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "Project打开失败： 线程创建失败！");
				AfxMessageBox(L"工程打开失败！");
				return;
			}
			iStartNum++;
			CString cstrIrData;
			CTBSCommon::iTBSPresent = CTBSCommon::iTBSNum;
			CTBSCommon::m_PresentThread[CTBSCommon::iTBSNum].iTabNum			= CTBSCommon::iTBSNum;
			CTBSCommon::m_PresentThread[CTBSCommon::iTBSNum].cstrProjectName	= csItemString;
			CTBSCommon::m_PresentThread[CTBSCommon::iTBSNum].iNum				= iStartNum;
			CTBSCommon::m_PresentThread[CTBSCommon::iTBSNum].bOpen				= true;

			//showwing the menu.
			if (CTBSCommon::iTBSNum == 0)
			{
				INT arrMenuIDEnable[] = { IDM_ADD_SCRIPT ,IDM_UPDATE_SCRIPT ,IDM_DELETE_SCRIPT ,IDM_IR_IN ,IDM_CONNECT_STB ,IDM_OPEN_TEST_MODULE ,IDM_CLOSE_PROJECT,IDM_SELECT_RUN,IDM_TEST_DATA_IN, IDM_LOG_SAVE,IDM_CONNECT_IR,IDM_DISCON_IR };
				CTBSPredictionDlg::tbs_menu_enable(arrMenuIDEnable, 12);
			}

			GetPrivateProfileString(L"IRData", L"Path", L"", cstrIrData.GetBuffer(MAX_PATH), MAX_PATH, csProjectPath);
			if (cstrIrData != L"")
			{
				CTBSCommon::m_PresentThread[CTBSCommon::iTBSNum].cstrIRFile = cstrIrData;
				cstrIrData.ReleaseBuffer();
			}
			CTBSCommon::iTBSNum++;
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, csItemString+L" 打开成功！");
		}
		else
		{
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "Project打开失败： 配置文件丢失！");
			AfxMessageBox(L"配置文件丢失！");
			CDialogEx::OnOK();
			return;
		}
		CDialogEx::OnOK();
	}
	else
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "Project打开失败： Project数量达到上限！");
		AfxMessageBox(L"Project打开已达上限");
		CDialogEx::OnOK();
	}
}

void CTBSProjectOpenDlg::OnBnClickedDeleteProject()
{
	if (csItemString.IsEmpty())
	{
		AfxMessageBox(L"请选择需要删除的project！");
		return;
	}
	CString csProjectPath = PROJECTPATH + csItemString;
	
	if (CTBSCommon::tbs_com_delete_directory(csProjectPath, 0))
	{
		m_SelectName.SetWindowTextW(L"");
		m_ProjectList.DeleteItem(iItem);
		m_ProjectList.UpdateData(0);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"删除工程："+ csItemString);
		AfxMessageBox(L"删除成功");
	}
	else
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"删除工程失败：" + csItemString);
		AfxMessageBox(L"删除失败");
	}
}

BOOL CTBSProjectOpenDlg::tbs_check_project(const CString cstrProjectName)
{
	for (INT i = 0; i < CTBSCommon::iTBSNum; i++)
	{
		if (cstrProjectName == CTBSCommon::m_PresentThread[i].cstrProjectName)
		{
			return 1;
		}
	}
	return 0;
}