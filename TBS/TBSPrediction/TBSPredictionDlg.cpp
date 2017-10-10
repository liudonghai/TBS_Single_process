
// TBSPredictionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSRedRat.h"
#include "TBSPrediction.h"
#include "TBSPredictionDlg.h"
#include "afxdialogex.h"
#include "TBSComConnectDlg.h"
#include "TBSMainDlg.h"
#include "TBSMainThread.h"
#include "TBSProjectOpenDlg.h"
#include "TBSDataBase.h"
#include "TBSScriptDeleteDlg.h"
#include "TBSSelectTestScript.h"
#include "TBSScriptParse.h"
#include "TBSMainDlg.h"
#include "TBSTestDataInDlg.h"
#include "TBSLogSaveDlg.h"
#include "TBSSelectIR.h"
#include "TBSGlobal.h"
#include <regex>
#include <assert.h>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

//静态成员初始化
CMenu *CTBSPredictionDlg::pMenu = NULL;

CTBSPredictionDlg::CTBSPredictionDlg(CWnd* pParent )
	: CDialogEx(IDD_TBSPREDICTION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	

}

//初始化静态成员变量
CTBSTabCtrl *CTBSPredictionDlg::m_TabControl=new CTBSTabCtrl;
CMutex		*CTBSPredictionDlg::m_Mutex =new CMutex;
//CTabCtrl	*CTBSPredictionDlg::m_TabControl=new CTabCtrl;
void CTBSPredictionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CONTROL, *m_TabControl);
}

BEGIN_MESSAGE_MAP(CTBSPredictionDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_CONNECT_STB, &CTBSPredictionDlg::OnConnectStb)
	ON_COMMAND(IDM_DISCONNECT_STB, &CTBSPredictionDlg::OnDisconnectStb)
	ON_COMMAND(IDM_NEW_PROJECT, &CTBSPredictionDlg::OnNewProject)
	ON_COMMAND(IDM_CLOSE_PROJECT, &CTBSPredictionDlg::OnCloseProject)
	ON_COMMAND(IDM_ABOUT, &CTBSPredictionDlg::OnAbout)
	ON_COMMAND(IDM_ADD_SCRIPT, &CTBSPredictionDlg::OnAddScript)
	ON_COMMAND(IDM_OPEN_PROJECT, &CTBSPredictionDlg::OnOpenProject)
	ON_COMMAND(IDM_IR_IN, &CTBSPredictionDlg::OnIrIn)
	ON_COMMAND(IDM_UPDATE_SCRIPT, &CTBSPredictionDlg::OnUpdateScript)
	ON_COMMAND(IDM_DELETE_SCRIPT, &CTBSPredictionDlg::OnDeleteScript)
	ON_COMMAND(IDM_SELECT_RUN, &CTBSPredictionDlg::OnSelectRun)
	ON_COMMAND(IDM_TEST_DATA_IN, &CTBSPredictionDlg::OnTestDataIn)
	ON_COMMAND(IDM_TEST_REPORT_CREATE, &CTBSPredictionDlg::OnTestReportCreate)
	ON_COMMAND(IDM_LOG_SAVE, &CTBSPredictionDlg::OnLogSave)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONTROL, &CTBSPredictionDlg::OnTcnSelchangeTabControl)
	ON_COMMAND(IDM_CONNECT_IR, &CTBSPredictionDlg::OnConnectIr)
	ON_COMMAND(IDM_DISCON_IR, &CTBSPredictionDlg::OnDisconIr)
	ON_COMMAND(IDM_TEST_FINISH, &CTBSPredictionDlg::OnTestFinish)
	ON_COMMAND(ID_IR_VALUE_IN, &CTBSPredictionDlg::OnIrValueIn)
END_MESSAGE_MAP()

// CTBSPredictionDlg 消息处理程序

BOOL CTBSPredictionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CTBSLog	m_LogInfo;
	AfxBeginThread(CTBSLog::tbs_log_record_in,NULL);
	CFile m_ConfFile;
	CTBSDataBase m_DataBase(DATAPATH);

	m_ConfFile.Open(CONFIGFILE, CFile::modeCreate | CFile::modeNoTruncate);
	m_ConfFile.Close();
	m_MainMenu.LoadMenuW(IDR_MAIN_MENU);
	SetMenu(&m_MainMenu);
	pMenu = &m_MainMenu;
	tbs_init_menu();
	if (0 != m_DataBase.tbs_db_init())
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "数据库初始化失败");
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "数据库初始化成功");

	ShowWindow(SW_MAXIMIZE);
	return TRUE;
}

void CTBSPredictionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

HCURSOR CTBSPredictionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTBSPredictionDlg::OnConnectStb()
{
	CTBSComConnectDlg m_NewConnectDlg;
	m_NewConnectDlg.DoModal();


}

void CTBSPredictionDlg::OnOK()
{
}

void CTBSPredictionDlg::OnDisconnectStb()
{
	if (CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread != NULL)
	{
		CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread->PostThreadMessageW(WM_QUIT, NULL, NULL);
		WaitForSingleObject(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread, INFINITE);
		CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread = NULL;
		CloseHandle(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "断开串口连接");
		MessageBox(L"断开串口连接成功！");
	}
	else
	{
		MessageBox(L"未连接串口！");
	}
}

void CTBSPredictionDlg::OnNewProject()
{
	CTBSProjectCreatDlg	m_NewProject;
	m_NewProject.DoModal();
}

void CTBSPredictionDlg::OnCloseProject()
{
	CSingleLock m_SingleLock(m_Mutex);
	m_SingleLock.Lock();
	INT iPresent = CTBSCommon::iTBSPresent;
	UINT uiTab = CTBSPredictionDlg::m_TabControl->uiFlag;
	if (CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread != NULL)
	{
		CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread->PostThreadMessage(WM_QUIT, NULL, NULL);
	}
	CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_MainThread->PostThreadMessage(WM_QUIT, NULL, NULL);
	
	CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread = NULL;
	CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_MainThread = NULL;
	CloseHandle(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread);
	CloseHandle(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_MainThread);
	CTBSPredictionDlg::m_TabControl->DeleteItem(CTBSCommon::iTBSPresent);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName);
	for (INT i = CTBSCommon::iTBSPresent; i < CTBSCommon::iTBSNum; i++)
	{
		memcpy(&CTBSCommon::m_PresentThread[i], &CTBSCommon::m_PresentThread[i + 1], sizeof(TBS_PRESENT_THREAD_t));
		CTBSCommon::m_PresentThread[i].iTabNum--; 
	}

	CTBSCommon::iTBSNum--;
	if (CTBSCommon::iTBSNum == 0)
	{
		tbs_init_menu();
	}
	else
	{
		CTBSPredictionDlg::m_TabControl->uiFlag = CTBSPredictionDlg::m_TabControl->uiFlag >> (iPresent+1);
		CTBSPredictionDlg::m_TabControl->uiFlag = CTBSPredictionDlg::m_TabControl->uiFlag << iPresent;
		uiTab = uiTab << (8 - iPresent);
		uiTab &= 0x00FF;
		uiTab = uiTab >> (8 - iPresent);
		CTBSPredictionDlg::m_TabControl->uiFlag |= uiTab;

		CTBSCommon::iTBSPresent = 0;
		CTBSPredictionDlg::m_TabControl->SetCurSel(CTBSCommon::iTBSPresent);
		if (CTBSCommon::m_PresentThread[0].bLock)
		{
			CTBSCommon::m_PresentThread[0].m_ManualEvent.SetEvent();
			CTBSCommon::m_PresentThread[0].bLock = false;
		}
		CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_pMainDlg->ShowWindow(true);
		for (INT i = 1; i < CTBSCommon::iTBSNum; i++)
		{
			if (CTBSCommon::m_PresentThread[i].m_pMainDlg != NULL)
			{
				CTBSCommon::m_PresentThread[i].m_pMainDlg->ShowWindow(false);
			}
		}
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName);
	}
	m_SingleLock.Unlock();
}

void CTBSPredictionDlg::OnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CTBSPredictionDlg::OnAddScript()
{
	if (CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName == L"")
		return;

	WCHAR		BUFFER[80];
	CHAR		*pFilePath = new CHAR[260];
	CHAR		*pKeyWord = new CHAR[20];
	WCHAR		wcTargetPath[MAX_PATH];
	BROWSEINFO	m_ScriptFile;

	ZeroMemory(&m_ScriptFile, sizeof(m_ScriptFile));
	memset(pFilePath, 0, 260);
	memset(pKeyWord, 0, 20);
	m_ScriptFile.hwndOwner = NULL;
	m_ScriptFile.pidlRoot = NULL;
	m_ScriptFile.lpszTitle = L"脚本文件路径";
	m_ScriptFile.pszDisplayName = BUFFER;
	m_ScriptFile.ulFlags = /*BIF_BROWSEINCLUDEFILES |*/ BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST targetLocation = SHBrowseForFolder(&m_ScriptFile);
	if (targetLocation != NULL)
	{
		string	strProjectName;
		string	strSql;
		CString csProjectConf;
		CTBSDataBase m_DataBase(DATAPATH);
		strProjectName = TOSTRING(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName);
		SHGetPathFromIDList(targetLocation, wcTargetPath);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, wcTargetPath);
		strSql = "delete  from TestCase where ProjectName = '" + strProjectName + "'";
		m_DataBase.tbs_db_sql_exec(strSql);
		tbs_file_find(wcTargetPath);

		csProjectConf = PROJECTPATH;
		csProjectConf = csProjectConf + CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName + L"\\" + CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName + L".ini";
		WritePrivateProfileString(L"ScriptPath", L"Path", wcTargetPath, csProjectConf);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"脚本添加完成！");
		MessageBox(L"脚本添加完成");	
	}
}

void CTBSPredictionDlg::OnOpenProject()
{
	CTBSProjectOpenDlg m_OpenProjectdlg;
	m_OpenProjectdlg.DoModal();
}

void CTBSPredictionDlg::OnIrIn()
{
	assert(!CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName.IsEmpty());

	CString csProjectPath;
	CString csfilter;
	CString cstrIRFile;

	csfilter = L"File(*.xml)|*.xml|";
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, csfilter, NULL);
	if (IDOK == openFileDlg.DoModal())
	{
		cstrIRFile = openFileDlg.GetPathName();
		csProjectPath = PROJECTPATH + CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName + L"\\" + CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName + L".ini";

		WritePrivateProfileString(L"IRData", L"Path", cstrIRFile, csProjectPath);
		CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrIRFile = cstrIRFile;
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrIRFile);
}

//脚本更新
void CTBSPredictionDlg::OnUpdateScript()
{
	assert(!CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName.IsEmpty());
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName);
	
	CHAR	*pDirPath = NULL;
	WCHAR	pGetData[MAX_PATH];
	string	str1;
	string	str2;
	string	strSql;
	CString cstrIniPath = PROJECTPATH + CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName + L"\\" + CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName + L".ini";
	CString cstrScriptPath;
	CTBSDataBase m_DataBase(DATAPATH);

	GetPrivateProfileString(L"ScriptPath", L"Path", L"", pGetData, MAX_PATH, cstrIniPath);
	cstrScriptPath = pGetData;
	if (cstrScriptPath == L"")
	{
		AfxMessageBox(L"请添加脚本路径");
		return;
	}
	
	if (!PathIsDirectory(cstrScriptPath))
	{
		AfxMessageBox(L"路径不合法");
		return;
	}

	str1 = TOSTRING(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName);
	strSql = "delete  from TestCase where ProjectName = '" + str1 + "'";
	m_DataBase.tbs_db_sql_exec(strSql);

	str2 = TOSTRING(cstrScriptPath);
	pDirPath = new CHAR[str2.length()];
	strcpy_s(pDirPath, str2.length() + 1, str2.c_str());
	tbs_file_find(cstrScriptPath,L".tbs");
	MessageBox(L"更新完成！！");
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "脚本更新完成！");
}

void CTBSPredictionDlg::OnDeleteScript()
{
	CTBSScriptDeleteDlg m_DeleteDlg;
	m_DeleteDlg.DoModal();

}

void CTBSPredictionDlg::OnSelectRun()
{
	CTBSScriptSelectDlg m_SelectDlg;
	m_SelectDlg.DoModal();
}

// 初始化菜单
void CTBSPredictionDlg::tbs_init_menu()
{
	m_MainMenu.EnableMenuItem(IDM_LOG_SAVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_NEW_PROJECT, MF_BYCOMMAND | MF_ENABLED);
	m_MainMenu.EnableMenuItem(IDM_OPEN_PROJECT, MF_BYCOMMAND | MF_ENABLED);
	m_MainMenu.EnableMenuItem(IDM_ADD_SCRIPT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_UPDATE_SCRIPT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_DELETE_SCRIPT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_IR_IN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_CONNECT_STB, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_DISCONNECT_STB, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_SELECT_RUN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_TEST_BEGIN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_OPEN_TEST_MODULE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_TEST_FINISH, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_CLOSE_TEST_MODULE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_CLOSE_PROJECT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_TEST_DATA_IN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_TEST_REPORT_CREATE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_TEST_REPORT_SEND, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_CONNECT_IR, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	m_MainMenu.EnableMenuItem(IDM_DISCON_IR, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "菜单初始化完成！");
}

void CTBSPredictionDlg::tbs_menu_enable(INT *pId, INT iNum)
{
	for (INT i = 0; i < iNum; i++)
	{
		pMenu->EnableMenuItem(*(pId + i), MF_BYCOMMAND | MF_ENABLED);
	}
}

void CTBSPredictionDlg::tbs_menu_disable(INT *pId, INT iNum)
{
	for (INT i = 0; i < iNum; i++)
	{
		pMenu->EnableMenuItem(*(pId + i), MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
}

void CTBSPredictionDlg::OnSize(UINT nType, INT cx, INT cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	Invalidate();
}

void CTBSPredictionDlg::OnPaint()
{
	CPaintDC dc(this); 
	CRect m_Rect;
	CRect m_TabRect;
	if (CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_pMainDlg != NULL)
	{
		GetClientRect(&m_Rect);
		m_TabControl->MoveWindow(&m_Rect);

		m_TabControl->GetClientRect(&m_Rect);
		m_TabControl->GetItemRect(0, &m_TabRect);
		m_Rect.top += m_TabRect.Height();
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_Rect.top);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_TabRect.Height());
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_Rect.Height());

		CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_pMainDlg->MoveWindow(&m_Rect);
	}
	else
	{
		GetClientRect(&m_Rect);
		//m_Rect.top -= 5;
		m_TabControl->MoveWindow(&m_Rect);
	}
}

void CTBSPredictionDlg::OnTestDataIn()
{
	CTBSTestDataInDlg m_TestDataIn;
	m_TestDataIn.DoModal();

}
//生成测试报告
void CTBSPredictionDlg::OnTestReportCreate()
{
	INT		iRow	= 0;
	INT		iCloum	= 0;
	CHAR	**ppGetTestReport;
	CTime	m_Time;
	CFile	m_ProjectFile;
	string	ProjectName(TOSTRING(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName));
	string	strSqlSelect = "select *from TestReport where ProjectName='" + ProjectName + "'";
	CString cstrTime;
	CString cstrProjectName = CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName;
	CString cstrTestReportName;
	CString cstraReportPath;
	CTBSDataBase m_DataBase(DATAPATH);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"开始生成测试报告: "+ CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName);
	
	cstraReportPath = L"..\\testreport";
	if (0 == CTBSCommon::tbs_file_check(cstraReportPath))
	{
		if (!CreateDirectory(cstraReportPath, NULL))
		{
			AfxMessageBox(cstraReportPath + L"文件夹创建失败！");
			return;
		}
	}

	m_DataBase.tbs_db_data_query(strSqlSelect, &ppGetTestReport, &iRow, &iCloum);
	if (iCloum > 0 && iRow == 1)
	{
		m_Time				= CTime::GetCurrentTime();
		cstrTime			= m_Time.Format(_T("%Y-%m-%d-%H-%M-%S"));
		cstrTestReportName	= cstraReportPath + L"\\" + cstrProjectName + L"_" + cstrTime + L".html";
	
		if (m_ProjectFile.Open(cstrTestReportName, CFile::modeCreate))
		{
			m_ProjectFile.Close();
		}
		else
		{
			AfxMessageBox(L"创建测试报告文件失败");
			return;
		}
		tbs_test_report_head_create(cstrTestReportName, ppGetTestReport, iCloum);
		tbs_test_report_information_create(cstrTestReportName, ppGetTestReport, iCloum);
		tbs_test_report_case_result_create(cstraReportPath + L"\\" + CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName + L"_" + cstrTime);
		tbs_test_report_case_information_create(cstraReportPath + L"\\" + CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName + L"_" + cstrTime);
	}
	else
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"测试报告生成失败，数据库数据为空！");
	}
}
// 写入基本信息
void CTBSPredictionDlg::tbs_test_report_head_create(CString cstrFileName, CHAR **ppData, INT iCloum)
{
	assert(ppData != NULL);
	fstream fsTestreport(cstrFileName, ios::app | ios::in);
	fsTestreport << "<html style = \"background-color: #AEEEEE\">" << endl;
	fsTestreport << "<title>TBS Test Report</title>" << endl;
	string strTestReportName = *(ppData + iCloum);
	fsTestreport << "<h1 style=\"background-color:yellow\">" + strTestReportName + " test report</h1>" << endl;
	fsTestreport << "<h2 style=\"background-color:blue\">基本信息</h2>" << endl;
	fsTestreport << "<table border=\"1\"style=\"background-color:green\"width=\"1400\">" << endl;
	string strTestReport[5];
	strTestReport[0] = "产品名称";
	strTestReport[1] = "硬件版本";
	strTestReport[2] = "软件版本";
	strTestReport[3] = "产品序列号";
	strTestReport[4] = "CPU ID";
	string strTestReportValues[5] = { "" };
	for (INT i = 0; i < 5; i++)
	{
		if (*(ppData + iCloum +1+ i))
		{
			strTestReportValues[i] = *(ppData + iCloum + 1 + i);
		}
	}
	//写入html文件内的每一行数据
	fsTestreport << "<th rowspan=\"6\"width=\"300\">基本信息</th>" << endl;
	for (int i = 0; i < 5; i++)
	{
		fsTestreport << "<tr>\n<td width=\"300\">" + strTestReport[i] + "</td>\n<td width=\"800\">" + strTestReportValues[i] + "</td>\n</tr>" << endl;
	}
	fsTestreport << "</table>" << endl;
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
}
// 写入测试结果汇总
void CTBSPredictionDlg::tbs_test_report_information_create(CString cstrFileName, CHAR **ppData, INT iCloum)
{
	assert(ppData != NULL);
	fstream fsTestreport(cstrFileName, ios::app | ios::in);
	fsTestreport << "<h2 style=\"background-color:blue\">测试结果汇总</h2>" << endl;
	fsTestreport << "<table border=\"1\"style=\"background-color:green\"width=\"1400\">" << endl;
	fsTestreport << "<tr>\n<th rowspan=\"10\"width=\"300\">汇总</th>\n</tr>" << endl;
	string strTestReport[9];
	strTestReport[0] = "测试结果";
	strTestReport[1] = "原因备注";
	strTestReport[2] = "成功项目";
	strTestReport[3] = "失败项目";
	strTestReport[4] = "开始时间";
	strTestReport[5] = "结束时间";
	strTestReport[6] = "总计用时";
	strTestReport[7] = "测试人员";
	strTestReport[8] = "Log文件";
	string strTestReportValues[9] = { "" };
	for (INT i = 0; i < 9; i++)
	{
		if (*(ppData + iCloum + 6 + i))
		{
			strTestReportValues[i] = *(ppData + iCloum + 6 + i);
		}
	}
	//将前8个数据写入
	for (INT i = 0; i < 8; i++)
	{
		fsTestreport << "<tr>\n<td width=\"300\">" + strTestReport[i] + "</td>\n<td width =\"800\">" + strTestReportValues[i] + "</td>\n</tr>" << endl;
	}
	//写入第九个数据
	fsTestreport << "<td>Log文件</td><td>\n<a href=\"" + strTestReportValues[8] + "\">" + strTestReportValues[8] + "</a></td>" << endl;
	fsTestreport << "</table>" << endl;
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
}
// 写入测试报告(每个case的测试结果)
void CTBSPredictionDlg::tbs_test_report_case_result_create(CString cstrFileName)
{
	INT		iRow = 0;
	INT		iCloum = 0;
	CHAR	**ppGetData;
	CHAR	cRow[4];

	CTBSDataBase m_DataBase(DATAPATH);

	
	string	strFileName;
	string	strProjectName;
	string	strSqlselect;
	fstream fsTestReport(cstrFileName + L".html", ios::app | ios::in);

	strFileName = TOSTRING(cstrFileName);
	strProjectName = TOSTRING(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName);
	strSqlselect = "Select *from TestDetail where ProjectName='" + strProjectName + "' and CaseName='Describe'";
	m_DataBase.tbs_db_data_query(strSqlselect, &ppGetData, &iRow, &iCloum);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iRow);
	if (iRow > 0)
	{
		fsTestReport << "<h2 style =\"background-color:blue\">case详情</h2>" << endl;
		fsTestReport << "<table border=\"1\"style=\"background-color:green\"width=\"1400\">" << endl;
		for (INT i = 0; i < iRow; i++)
		{
			INT		iRowCase = 0;
			INT		iCloumCase = 0;
			CHAR	**ppGetDataCase;
			strSqlselect = "Select *from TestDetail where ProjectName='" + strProjectName + "' and ScriptName='" + *(ppGetData + (i+1)*iCloum + 3) + "' and CaseName!='Describe'";
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, *(ppGetData + (i+1)*iCloum + 3));
			m_DataBase.tbs_db_data_query(strSqlselect, &ppGetDataCase, &iRowCase, &iCloumCase);
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iRowCase);
			if (iRowCase > 0)
			{
				_itoa_s(iRowCase+1, cRow, 10);
				if (*(ppGetData + (i+1)*iCloum + 16))
				{
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, *(ppGetData + (i + 1)*iCloum + 16));
					fsTestReport << "<tr>\n<th rowspan =\"" + (string)cRow + "\"width =\"300\">" + *(ppGetData + (i + 1)*iCloum + 16) + "</th>\n</tr>" << endl;
				}
				else
				{
					fsTestReport << "<tr>\n<th rowspan =\"" + (string)cRow + "\"width =\"300\">未命名</th>\n</tr>" << endl;
				}
				for (INT ii = 1; ii < iRowCase + 1; ii++)
				{
					string strCaseName;
					string strCaseResult;
					if (*(ppGetDataCase + ii*iCloum + 7))
					{
						strCaseResult = *(ppGetDataCase + ii*iCloum + 7);
					}
					else
					{
						strCaseResult = "FAIL";
					}

					if (*(ppGetDataCase + ii*iCloum + 2))
					{
						strCaseName = *(ppGetDataCase + ii*iCloum + 2);
					}
					else
					{
						strCaseName = "";
					}
					fsTestReport << "<tr>\n<td>" + strCaseName + "</td>\n<td>\n<a href = \"" + strFileName + "\\" + strCaseName + ".html\">" + strCaseResult + "<a>\n</td>\n</tr>" << endl;
				}
				
			}
		}
		fsTestReport << "</table>\n</html>" << endl;
	}

	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__,"测试报告主页生成完成！");
}
//写入单个脚本的测试详细数据
void CTBSPredictionDlg::tbs_test_report_case_information_create(CString cstrTestReportName)
{
	//文件夹路径
	cstrTestReportName = L"..\\testreport\\" + cstrTestReportName;
	if (!CreateDirectory(cstrTestReportName, NULL))
	{
		AfxMessageBox(cstrTestReportName + L"文件夹创建失败！");
		return;
	}

	INT		iRow = 0;
	INT		iCloum = 0;
	CHAR	**ppGetData;
	CFile	m_TestReport;
	string	strSqlSelectDetail;
	string	strProjectName;
	string	strParseResult;
	string	strTestReport[11];
	string	strTestReportValues[11];
	CTBSDataBase m_DataBase(DATAPATH);

	strTestReport[0] = "Case标题";
	strTestReport[1] = "脚本名称";
	strTestReport[2] = "Case描述";
	strTestReport[3] = "开始时间";
	strTestReport[4] = "结束时间";
	strTestReport[5] = "测试结果";
	strTestReport[6] = "动作";
	strTestReport[7] = "发生时间";
	strTestReport[8] = "期望结果";
	strTestReport[9] = "实际结果";
	strTestReport[10] = "Case耗时";

	strProjectName		= TOSTRING(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName);
	strSqlSelectDetail	= "select *from TestDetail where ProjectName='" + strProjectName + "' and CaseName!='Describe'";
	m_DataBase.tbs_db_data_query(strSqlSelectDetail, &ppGetData, &iRow, &iCloum);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iRow);

	if (iRow > 0)
	{
		for (INT i = 1; i < iRow + 1; i++)
		{
			strParseResult = *(ppGetData + i*iCloum + 13);
			if (strParseResult=="Fail")
			{
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "生成解析失败的测试报告！");
				
				CString cstrCaseName(*(ppGetData + i*iCloum + 2));
				string strCaseName(*(ppGetData + i*iCloum + 2));
				CString cstrCaseFileName = cstrTestReportName + L"\\" + cstrCaseName + L".html";
				if (m_TestReport.Open(cstrCaseFileName, CFile::modeCreate))
				{
					m_TestReport.Close();

					fstream fsTestReport(cstrCaseFileName, ios::app | ios::in);
					fsTestReport << "<html style=\"background-color:#AEEEEE\">" << endl;
					fsTestReport << "<title>" + strCaseName + "</title>" << endl;
					fsTestReport << "<h1 style=\"background-color:yellow\"align=\"center\">" + strCaseName + "</h1>" << endl;
					fsTestReport << "<table border=\"1\"style=\"background-color:green\"width=\"1400\">" << endl;
					fsTestReport << "<tr>\n<th colspan=\"3\">" + strCaseName + "明细</th>\n</tr>" << endl;

					fsTestReport << "<tr>\n<th width=\"200\">脚本名称</th>\n<td width = \"1200\">" + strCaseName + "</td>\n</tr>" << endl;
					fsTestReport << "<tr>\n<th width=\"200\">解析结果</th>\n<td width = \"1200\">Fail</td>\n</tr>" << endl;
					fsTestReport << "<tr>\n<th width=\"200\">错误位置</th>\n<td width = \"1200\">" + string(*(ppGetData + i*iCloum + 14)) + "</td>\n</tr>" << endl;
					fsTestReport << "<tr>\n<th width=\"200\">错误信息</th>\n<td width = \"1200\">" + string(*(ppGetData + i*iCloum + 15)) + "</td>\n</tr>" << endl;
					fsTestReport.close();
				}
			}
			else
			{
				CString cstrCaseName(*(ppGetData + i*iCloum + 2));
				//CString cstrScriptName(*(ppGetData + i*iCloum + 3));
				string	strCaseName(*(ppGetData + i*iCloum + 2));
				string	strTestReportName(CW2A(cstrTestReportName.GetString()));
				CString cstrCaseFileName = cstrTestReportName + L"\\" + cstrCaseName + L".html";
				if (m_TestReport.Open(cstrCaseFileName, CFile::modeCreate))
				{
					m_TestReport.Close();
					fstream fsTestReport(cstrCaseFileName, ios::app | ios::in);
					fsTestReport << "<html style=\"background-color:#AEEEEE\">" << endl;
					fsTestReport << "<title>" + strCaseName + "</title>" << endl;
					fsTestReport << "<h1 style=\"background-color:yellow\"align=\"center\">" + strCaseName + "</h1>" << endl;
					fsTestReport << "<table border=\"1\"style=\"background-color:green\"width=\"1400\">" << endl;
					fsTestReport << "<tr>\n<th colspan=\"3\">" + strCaseName + "明细</th>\n</tr>" << endl;
					for (INT j = 2; j < 13; j++)
					{
						if (*(ppGetData + i*iCloum + j))
						{
							strTestReportValues[j - 2] = *(ppGetData + i*iCloum + j);
						}
						else
						{
							strTestReportValues[j - 2] = "";
						}
					}
					if (strTestReportValues[5] == "OK")
					{
						for (INT i = 0; i < 6; i++)
						{
							fsTestReport << "<tr>\n<th width=\"200\">" + strTestReport[i] + "</th>\n<td width = \"1200\">"
								+ strTestReportValues[i] + "</td>\n</tr>" << endl;
						}
						fsTestReport << "<tr>\n<th width=\"200\">" + strTestReport[10] + "</th>\n<td width = \"1200\">"
							+ strTestReportValues[10] + "</td>\n</tr>" << endl;
						fsTestReport << "</table >\n</html>" << endl;
					}
					else
					{
						for (INT i = 0; i < 6; i++)
						{
							if (i == 5)
							{
								fsTestReport << "<tr>\n<th width=\"200\">" + strTestReport[i] + "</th>\n<td width = \"1200\"style=\"color:red\">" + strTestReportValues[i] + "</td>\n</tr>" << endl;
								fsTestReport << "<tr>\n<th>测试结果备注</th>\n<td>" << endl;
								fsTestReport << "<table>\
											\n<tr>\
											\n<th align=\"right\">" + strTestReport[7] + "</th>\
											\n<td>" + strTestReportValues[7] + "</td>\
											\n</tr>\
											\n<tr>\
											\n<th align=\"right\">" + strTestReport[6] + "</th>\
											\n<td>" + strTestReportValues[6] + "</td>\
											\n</tr>\
											\n<tr>\
											\n<th align=\"right\">" + strTestReport[8] + "</th>\
											\n<td>" + strTestReportValues[8] + "</td>\
											\n</tr>\
											\n<tr style =\"color:red\">\
											\n<th align=\"right\">" + strTestReport[9] + "</th>\
											\n<td>" + strTestReportValues[9] + "</td>\
											\n</tr>\
											\n</table>\
											\n</td>\
											\n</tr>" << endl;
								fsTestReport << "<tr>\n<th>Case耗时</th><td>" + strTestReportValues[10] + "</td>\n</tr>" << endl;
								break;
							}
							fsTestReport << "<tr>\n<th width=\"200\">" + strTestReport[i] + "</th>\n<td width = \"1200\">" + strTestReportValues[i] + "</td>\n</tr>" << endl;
						}
					}
					fsTestReport.close();
				}
			}
		}
	}

	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "测试报告生成完成！");

}

void CTBSPredictionDlg::OnLogSave()
{
	CTBSLogSaveDlg m_LogSaveDlg;
	m_LogSaveDlg.DoModal();
}

void CTBSPredictionDlg::OnTcnSelchangeTabControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	INT iTabNum				= m_TabControl->GetCurSel();
	if (CTBSCommon::m_PresentThread[iTabNum].bLock)
	{
		CTBSCommon::m_PresentThread[iTabNum].m_ManualEvent.SetEvent();
		CTBSCommon::m_PresentThread[iTabNum].bLock = false;
	}
	CTBSCommon::iTBSPresent = iTabNum;
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__,iTabNum);

	for (INT i = 0; i < CTBSCommon::iTBSNum; i++)
	{
		if (CTBSCommon::m_PresentThread[i].m_pMainDlg == NULL)
			continue;
		if (i == iTabNum)
		{
			CTBSCommon::m_PresentThread[i].m_pMainDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			CTBSCommon::m_PresentThread[i].m_pMainDlg->ShowWindow(SW_HIDE);
		}
	}

	*pResult = 0;
}

void CTBSPredictionDlg::OnConnectIr()
{
	CTBSSelectIR m_IRDlg;
	m_IRDlg.DoModal();
}

void CTBSPredictionDlg::OnDisconIr()
{
	CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrRedRat = L"";
}

/*
功能：递归查找指定路径下，包含特定关键字的文件。
参数：
输出：
cstrDirPath：文件路径，可以是文件夹的路径，也可以是文件的路径；
pKeyName：关键字；
bSubDirectory：是否查找子文件夹，TRUE表示需要。
*/
VOID CTBSPredictionDlg::tbs_file_find(const CString cstrDirPath, const CString  cstrKey, bool bSubDirectory)
{
	assert(!cstrDirPath.IsEmpty());

	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrDirPath);
	CFileFind m_FileFind;
	BOOL bResult;

	bResult = m_FileFind.FindFile(cstrDirPath);
	while (bResult)
	{
		bResult = m_FileFind.FindNextFile();
		if (m_FileFind.IsDots())
			continue;
		if (m_FileFind.IsDirectory() && bSubDirectory)
		{
			CString cstrPath = m_FileFind.GetFilePath();
			//为什么路径后面要添加\*.*,这是为了去匹配当路径是一个文件夹的时候，能够匹配到文件夹下的文件
			tbs_file_find(cstrPath+L"\\*.*", cstrKey, bSubDirectory);
		}
		else
		{
			CString cstrPath = m_FileFind.GetFilePath();
			CString cstrName = m_FileFind.GetFileName();
			BSTR bstr = cstrName.AllocSysString();
			wregex m_wregex(cstrKey);
			if (regex_search(bstr, m_wregex))
			{
				tbs_test_prase_scriptname(TOSTRING(cstrPath), TOSTRING(cstrName));
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrName);
			}
		}
	}
	m_FileFind.Close();
}

void CTBSPredictionDlg::tbs_test_prase_scriptname(CHAR *pPath, CHAR *pName)
{

	string strPath(pPath);
	string pKeyWord[4];
	//0:projectName;
	//1:autuo or semi-autuo;
	//2:module;
	//3:scriptName;
	CTBSCommon::tbs_com_string_split(strPath, '_', pKeyWord);
	string strproject(TOSTRING(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName));
	string strValue = "'" + strproject + "','" + pKeyWord[2] + "'," + "NULL," + "'" + pName + "','" + pPath + "','" + pKeyWord[1] + "'";
	string strSql = "Insert  into TestCase values (" + strValue + ")";

	CTBSDataBase m_DataBase(DATAPATH);
	m_DataBase.tbs_db_sql_exec(strSql);
}

BOOL CTBSPredictionDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTBSPredictionDlg::OnTestFinish()
{
	INT iIndex = CTBSCommon::iTBSPresent;
	CTBSCommon::m_PresentThread[iIndex].m_ParseThread = NULL;
	CTBSCommon::m_PresentThread[iIndex].m_TestThread = NULL;
	wregex m_Regex1(L"(.*)(执行中)");
	wregex m_Regex2(L"(waiting...)");
	CString cstrStatus;
	for (INT i = 0; i >= 0; i++)
	{
		cstrStatus = L"";
		cstrStatus=CTBSCommon::m_PresentThread[iIndex].m_pMainDlg->m_TestList.GetItemText(i, 1);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrStatus);
		if (cstrStatus == L"")
		{
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrStatus);
			break;
		}
		if (regex_match(cstrStatus.AllocSysString(),m_Regex1))
		{
			CTBSCommon::m_PresentThread[iIndex].m_pMainDlg->m_TestList.SetItemText(i, 1, L"测试强制结束");
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrStatus);
			break;
		}
		if (regex_match(cstrStatus.AllocSysString(), m_Regex2))
		{
			CTBSCommon::m_PresentThread[iIndex].m_pMainDlg->m_TestList.SetItemText(i, 1, L"测试强制结束");
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrStatus);
			break;
		}

	}
}

void CTBSPredictionDlg::OnIrValueIn()
{
	/*INT iResult = WinExec("..\\SignalDBUtil\\SignalDBUtil.exe", SW_SHOW);
	if (31 > iResult)
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iResult);
	}*/
	PROCESS_INFORMATION m_pi;
	STARTUPINFO si = { sizeof(si) };
	CreateProcess(L"..\\SignalDBUtil\\SignalDBUtil.exe", NULL, NULL, NULL, 1, CREATE_UNICODE_ENVIRONMENT, NULL, NULL, &si, &m_pi);
	INT i=GetLastError();
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, i);

}

void CTBSPredictionDlg::OnCancel()
{
	HWND hWnd = ::FindWindow(NULL, L"RedRat IR Signal Database Utility");
	if (hWnd != NULL)
	{
		::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE| SWP_NOSIZE | SWP_SHOWWINDOW);
		::ShowWindow(hWnd, SW_RESTORE);
		return;
	}
	CDialogEx::OnCancel();
}
