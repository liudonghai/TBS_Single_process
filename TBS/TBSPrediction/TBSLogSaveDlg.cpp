// TBSLogSave.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSLogSaveDlg.h"
#include "afxdialogex.h"
#include "TBSCommon.h"
#include "TBSPredictionDlg.h"
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
	CHAR cNum[10];
	CString cstrTitle;
	CString *cstrParamter = (CString*)lpParameter;
	CString cstrLogParameter[3];

	cstrLogParameter[0] = cstrParamter[0];
	cstrLogParameter[1] = cstrParamter[1];
	cstrLogParameter[2] = cstrParamter[2];

	CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_pMainDlg->m_Event.Lock();
	_itoa_s(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].iNum, cNum, 10);
	cstrTitle = L"MSComm" + CString(cNum);
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
	if (m_Check.GetCheck())
	{

		WritePrivateProfileString(L"LOGFilePath", L"Path", this->cstrFilePath, CONFIGFILE);
		WritePrivateProfileString(L"LOGFileName", L"Name", this->cstrFileName, CONFIGFILE);
		WritePrivateProfileString(L"LOGFileLine", L"Line", this->cstrEachLine, CONFIGFILE);
	}
	else
	{
		WritePrivateProfileString(L"LOGFilePath", L"Path", L"", CONFIGFILE);
		WritePrivateProfileString(L"LOGFileName", L"Name", L"", CONFIGFILE);
		WritePrivateProfileString(L"LOGFileLine", L"Line", L"", CONFIGFILE);
	}

	CString cstrLogParameter[3];
	cstrLogParameter[0] = this->cstrFilePath;
	cstrLogParameter[1] = this->cstrFileName;
	cstrLogParameter[2] = this->cstrEachLine;

	if (CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].m_ComThread == NULL)
	{
		CWinThread *m_thread = AfxBeginThread(tbs_logfile_message_send, cstrLogParameter);
	}
	else
	{
		CHAR cNum[10];
		CString cstrTitle;
		_itoa_s(CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].iNum, cNum, 10);
		cstrTitle = L"MSComm" + CString(cNum);
		::SendMessage(::FindWindow(NULL, cstrTitle), WM_LOG_FILENAME, 0, (LPARAM)cstrLogParameter);
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
	GetPrivateProfileString(L"LOGFileName", L"Name", L"", pFileName, MAX_PATH, CONFIGFILE);
	GetPrivateProfileString(L"LOGFilePath", L"Path", L"", pFilePath, MAX_PATH, CONFIGFILE);
	GetPrivateProfileString(L"LOGFileLine", L"Line", L"", pEachLine, MAX_PATH, CONFIGFILE);
	this->cstrFilePath = pFilePath;
	this->cstrFileName = pFileName;
	this->cstrEachLine = pEachLine;
	if (!cstrFileName.IsEmpty())
		m_Check.SetCheck(BST_CHECKED);
	UpdateData(0);
	return TRUE; 
}