// TBSLogSave.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSLogSaveDlg.h"
#include "afxdialogex.h"
#include "TBSCommon.h"
#include "TBSApp.h"
// CTBSLogSave 对话框

IMPLEMENT_DYNAMIC(CTBSLogSaveDlg, CDialogEx)

CTBSLogSaveDlg::CTBSLogSaveDlg(CWnd* pParent)
	: CDialogEx(IDD_LOG_SAVE_DIALOG, pParent)
	, cstrFilePath(_T(""))
	, cstrFileName(_T(""))
	, cstrEachLine(_T(""))
{
}

CTBSLogSaveDlg::~CTBSLogSaveDlg()
{
}

void CTBSLogSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOGSAVE_DIRECTORY, cstrFilePath);
	DDX_Text(pDX, IDC_LOGSAVE_FILE, cstrFileName);
	DDX_Text(pDX, IDC_EACHLINE, cstrEachLine);
	DDX_Control(pDX, IDC_SAVE_ATUO, m_Check);
}


BEGIN_MESSAGE_MAP(CTBSLogSaveDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSLogSaveDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SEEK_DIRECTORY, &CTBSLogSaveDlg::OnBnClickedSeekDirectory)
END_MESSAGE_MAP()


// CTBSLogSave 消息处理程序
UINT tbs_logfile_message_send(LPVOID lpParameter)
{
	//从数组到void指针，再到LPARAM，再到数组，中间一定要转回来，即是在转到void以后，先转为数组在转为LPARAM发送。
	/*CHAR cNum[10];*/
	CString cstrTitle;
	CString *cstrParamter = (CString*)lpParameter;
	CString cstrLogParameter[3];

	cstrLogParameter[0] = cstrParamter[0];
	cstrLogParameter[1] = cstrParamter[1];
	cstrLogParameter[2] = cstrParamter[2];

	CTBSCommon::m_PresentThread->m_pMainDlg->m_Event.Lock();
	/*_itoa_s(CTBSCommon::m_PresentThread->iNum, cNum, 10);*/
	cstrTitle = L"MSComm" + CTBSCommon::m_PresentThread->cstrProjectName;
	::SendMessage(::FindWindow(NULL, cstrTitle), WM_LOG_FILENAME, 0, (LPARAM)cstrLogParameter);
	return 0;
}

void CTBSLogSaveDlg::OnBnClickedOk()
{
	UpdateData();
	if (cstrFileName.IsEmpty() || cstrFilePath.IsEmpty())
	{
		AfxMessageBox(L"请输入LOG文件名和路径");
		return;
	}
	CMutex m_ConfigMutex(FALSE, TBS_CONFIG_MUREX);
	CSingleLock m_SingleLock(&m_ConfigMutex);
	m_SingleLock.Lock();
	if (m_Check.GetCheck())
	{

		WritePrivateProfileString(L"LOGFile", L"Path", this->cstrFilePath, CONFIGFILE);
		WritePrivateProfileString(L"LOGFile", L"Name", this->cstrFileName, CONFIGFILE);
		WritePrivateProfileString(L"LOGFile", L"Line", this->cstrEachLine, CONFIGFILE);
	}
	else
	{
		WritePrivateProfileString(L"LOGFileTmp", L"Project", CTBSCommon::m_PresentThread->cstrProjectName, CONFIGFILE);
		WritePrivateProfileString(L"LOGFileTmp", L"Path", this->cstrFilePath, CONFIGFILE);
		WritePrivateProfileString(L"LOGFileTmp", L"Name", this->cstrFileName, CONFIGFILE);
		WritePrivateProfileString(L"LOGFileTmp", L"Line", this->cstrEachLine, CONFIGFILE);
	}
	m_SingleLock.Unlock();
	/*CString cstrLogParameter[3];
	cstrLogParameter[0] = this->cstrFilePath;
	cstrLogParameter[1] = this->cstrFileName;
	cstrLogParameter[2] = this->cstrEachLine;*/

	if (CTBSCommon::m_PresentThread->m_ComThread == NULL)
	{
		/*CWinThread *m_thread = AfxBeginThread(tbs_logfile_message_send, NULL);*/
	}
	else
	{
		
		CString cstrTitle;
	
		cstrTitle = L"MSComm" + CTBSCommon::m_PresentThread->cstrProjectName;
		::SendMessage(::FindWindow(NULL, cstrTitle), WM_LOG_FILENAME, 0, 0);
	}

	CDialogEx::OnOK();
}


void CTBSLogSaveDlg::OnBnClickedSeekDirectory()
{
	CFolderPickerDialog m_SaveFile(NULL, 0, NULL, 0);
	if (m_SaveFile.DoModal() == IDOK)
	{
		cstrFilePath = m_SaveFile.GetPathName();
		UpdateData(0);
	}
}
BOOL CTBSLogSaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	WCHAR pFileName[MAX_PATH] = { 0 };
	WCHAR pFilePath[MAX_PATH] = { 0 };
	WCHAR pEachLine[MAX_PATH] = { 0 };
	CMutex m_ConfigMutex(FALSE, TBS_CONFIG_MUREX);
	CSingleLock m_SingleLock(&m_ConfigMutex);
	m_SingleLock.Lock();
	GetPrivateProfileString(L"LOGFile", L"Name", L"", pFileName, MAX_PATH, CONFIGFILE);
	GetPrivateProfileString(L"LOGFile", L"Path", L"", pFilePath, MAX_PATH, CONFIGFILE);
	GetPrivateProfileString(L"LOGFile", L"Line", L"", pEachLine, MAX_PATH, CONFIGFILE);
	m_SingleLock.Unlock();
	this->cstrFilePath = pFilePath;
	this->cstrFileName = pFileName;
	this->cstrEachLine = pEachLine;
	if (!cstrFileName.IsEmpty())
		m_Check.SetCheck(BST_CHECKED);
	UpdateData(0);
	return TRUE; 
}