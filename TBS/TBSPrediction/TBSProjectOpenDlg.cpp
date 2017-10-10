// TBSProjectOpenDlg.cpp : ʵ���ļ�
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
	m_ProjectList.InsertColumn(0, _T("��Ŀ����"), 0, m_Rc.Width(), 0);
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

			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"��ʼ��project���ƣ�"+cstrProjectDirctory);
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
		AfxMessageBox(L"��ѡ����Ҫ�򿪵�project��");
		return;
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iStartNum+1);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, csItemString);
	if (tbs_check_project(csItemString))
	{
		AfxMessageBox(L"��Project�Ѿ���");
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "Project��ʧ�ܣ� ���ظ���Project��");
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
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "Project��ʧ�ܣ� �̴߳���ʧ�ܣ�");
				AfxMessageBox(L"���̴�ʧ�ܣ�");
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
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, csItemString+L" �򿪳ɹ���");
		}
		else
		{
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "Project��ʧ�ܣ� �����ļ���ʧ��");
			AfxMessageBox(L"�����ļ���ʧ��");
			CDialogEx::OnOK();
			return;
		}
		CDialogEx::OnOK();
	}
	else
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "Project��ʧ�ܣ� Project�����ﵽ���ޣ�");
		AfxMessageBox(L"Project���Ѵ�����");
		CDialogEx::OnOK();
	}
}

void CTBSProjectOpenDlg::OnBnClickedDeleteProject()
{
	if (csItemString.IsEmpty())
	{
		AfxMessageBox(L"��ѡ����Ҫɾ����project��");
		return;
	}
	CString csProjectPath = PROJECTPATH + csItemString;
	
	if (CTBSCommon::tbs_com_delete_directory(csProjectPath, 0))
	{
		m_SelectName.SetWindowTextW(L"");
		m_ProjectList.DeleteItem(iItem);
		m_ProjectList.UpdateData(0);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"ɾ�����̣�"+ csItemString);
		AfxMessageBox(L"ɾ���ɹ�");
	}
	else
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"ɾ������ʧ�ܣ�" + csItemString);
		AfxMessageBox(L"ɾ��ʧ��");
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