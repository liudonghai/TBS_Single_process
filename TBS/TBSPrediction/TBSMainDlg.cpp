// TBSMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSMainDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include "TBSCommon.h"
#include "TBSRedRat.h"
#include "TBSApp.h"
#include "TBSScriptParse.h"
#include <regex>

using namespace std;
// CTBSMainDlg 对话框

IMPLEMENT_DYNAMIC(CTBSMainDlg, CDialogEx)
CString CTBSMainDlg::sIrPath = L"";

CTBSMainDlg::CTBSMainDlg(CWnd* pParent)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{
	
	pGetData = NULL;
	cstrProjectName = CTBSCommon::m_PresentThread->cstrProjectName;
	Create(IDD_MAIN_DIALOG, GetDlgItem(IDC_TAB_CONTROL));
}

CTBSMainDlg::~CTBSMainDlg()
{
	delete[] pGetData;
	pGetData = NULL;
}

void CTBSMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_SHOW, m_RichEdit);
	DDX_Control(pDX, IDC_COM_WRITE, m_Edit);
	DDX_Control(pDX, IDC_TEST_LIST, m_TestList);
}


BEGIN_MESSAGE_MAP(CTBSMainDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSMainDlg::OnBnClickedOk)
	ON_COMMAND_RANGE(IDC_BUTTON1, IDC_BUTTON21, &CTBSMainDlg::OnButtonPort)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_TEST_LIST, &CTBSMainDlg::OnNMClickTestList)
END_MESSAGE_MAP()


// CTBSMainDlg 消息处理程序

BEGIN_EVENTSINK_MAP(CTBSMainDlg, CDialogEx)
END_EVENTSINK_MAP()
void CTBSMainDlg::OnBnClickedOk()
{
}

BOOL CTBSMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// 接收键盘的输入，将接收到的数据发送给另外串口的独有线程。
	if (pMsg->message == WM_KEYDOWN)
	{
		INT n;
		CHAR *pComOut = new CHAR[80];
		/*CHAR cNum[10];*/
		CString cstr;
		CString cstrTitle;
		/*_itoa_s(CTBSCommon::m_PresentThread->iNum, cNum, 10);*/
		cstrTitle = L"MSComm" + CTBSCommon::m_PresentThread->cstrProjectName;
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		if ((pMsg->wParam == VK_RETURN) && GetFocus() == GetDlgItem(IDC_COM_WRITE))
		{
			GetDlgItem(IDC_COM_WRITE)->GetWindowTextW(cstr);
			if (cstr.IsEmpty())
			{
				return CDialogEx::PreTranslateMessage(pMsg);
			}
			n = WideCharToMultiByte(CP_ACP, 0, cstr, -1, NULL, 0, NULL, NULL);
			memset(pComOut, 0, 80);
			WideCharToMultiByte(CP_ACP, 0, cstr, -1, pComOut, n, NULL, NULL);
			n = strlen(pComOut);
			pComOut[n] = '\n';
			pComOut[n + 1] = '\0';
			::SendMessage(::FindWindow(NULL, cstrTitle), WM_KEYBOARD_MSG, n+1, (LPARAM)pComOut);
			GetDlgItem(IDC_COM_WRITE)->SetWindowTextW(L"");
		}
		if (pMsg->wParam == 'C' && (GetKeyState(VK_CONTROL) & 0x80) && GetFocus() == GetDlgItem(IDC_COM_WRITE))
		{
			memset(pComOut, 0, 80);
			::SendMessage(::FindWindow(NULL, cstrTitle), WM_KEYBOARD_MSG, 1, 0x03);
		}
		delete[] pComOut;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CTBSMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ShowWindow(SW_MAXIMIZE);
	CRect m_Rect;
	m_RichEdit.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	DWORD dwStyle = m_TestList.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_TestList.SetExtendedStyle(dwStyle);
	m_TestList.InsertColumn(0, _T("ScriptName"));
	m_TestList.InsertColumn(1, _T("TestResult"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CTBSMainDlg::OnButtonPort(UINT nId)
{
	if(CTBSCommon::m_PresentThread->cstrIRFile==L"")
	{
		AfxMessageBox(L"IR数据文件为空，请导入IR数据文件！");
		return;
	}
	CWnd *pWnd = GetFocus();
	bool bCondition = TRUE;
	INT i = 1;
	fstream fReadKey;
	CHAR *pBuffer = new CHAR[80];
	memset(pBuffer, 0, 80);
	string key[4];
	fReadKey.open(IR_FILE_PATH, ios::in);
	if (!fReadKey.is_open())
	{
		AfxMessageBox(L"键值数据文件打开失败！");
		return;
	}
	while (bCondition)
	{
		if (pWnd == GetDlgItem(1004 + i))
		{
			while (!fReadKey.eof())
			{
				fReadKey.getline(pBuffer, 80);
				string sKeyLine(pBuffer);
				CTBSCommon::tbs_com_string_split(sKeyLine, ' ', key);
				if (i == atoi(key[1].c_str()))
				{
					CHAR *pIRFile = CW2A(CTBSCommon::m_PresentThread->cstrIRFile.GetString());
					CMutex m_Mutex;
					m_Mutex.Lock();
					
					//CTBSRedRat::tbs_ir_send_key(CTBSCommon::m_PresentThread->cstrRedRat, pIRFile, key[0]);
					m_Mutex.Unlock();
					break;
				}
			}
			break;
		}
		i++;
	}
	delete[] pBuffer;
	pBuffer = NULL;
	fReadKey.close();
}

void CTBSMainDlg::OnSize(UINT nType, INT cx, INT cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	Invalidate();
}


void CTBSMainDlg::OnPaint()
{
	CPaintDC dc(this); 
	Invalidate();
	//无IR控件的布局
	CRect m_Rc;
	//CRect m_TabRect;
	//CTBSPredictionDlg::m_TabControl->GetClientRect(&m_Rc);
	CTBSCommon::m_PresentThread->m_TBSDialog->GetClientRect(&m_Rc);
	//m_Rc.top += CTBSPredictionDlg::m_TabControl->uiHeight;
	//CTBSPredictionDlg::m_TabControl->GetItemRect(0, &m_TabRect);
	//m_Rc.top += m_TabRect.Height();
	//m_Rc.top += 19;
	this->MoveWindow(&m_Rc);

	this->GetClientRect(&m_Rc);
	m_Rc.right = m_Rc.right * 3 / 5 ;
	m_Rc.bottom = m_Rc.bottom *4 / 5;
	m_RichEdit.MoveWindow(&m_Rc);

	this->GetClientRect(&m_Rc);
	m_Rc.top = m_Rc.bottom * 4 / 5;
	m_Edit.MoveWindow(&m_Rc);

	this->GetClientRect(&m_Rc);
	m_Rc.left = m_Rc.right * 3 / 5;
	m_Rc.bottom = m_Rc.bottom * 4 / 5;
	m_TestList.MoveWindow(&m_Rc);

	m_TestList.GetClientRect(&m_Rc);
	m_TestList.SetColumnWidth(0, m_Rc.Width() * 2 / 5);
	m_TestList.SetColumnWidth(1, m_Rc.Width() * 3 / 5);

	/*CRect rc;
	CTBSPredictionDlg::m_TabControl->GetClientRect(&rc);
	
	rc.top += 23;
	this->MoveWindow(&rc);

	this->GetClientRect(&rc);
	rc.right = rc.right * 3 / 5 + 190;
	rc.bottom -= 130;
	m_RichEdit.MoveWindow(&rc);

	this->GetClientRect(&rc);
	rc.top = rc.bottom - 130;
	m_Edit.MoveWindow(&rc);

	this->GetClientRect(&rc);
	rc.left = rc.right * 3 / 5 + 190;
	rc.bottom -= 130;
	m_TestList.MoveWindow(&rc);

	m_TestList.GetClientRect(&rc);
	m_TestList.SetColumnWidth(0, rc.Width() / 2);
	m_TestList.SetColumnWidth(1, rc.Width() / 2);*/

	CDialogEx::OnPaint();
}

void CTBSMainDlg::OnNMClickTestList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	HD_NOTIFY *lpNMItemActivate = (HD_NOTIFY*)pNMHDR;
	if (lpNMItemActivate != NULL)
	{
		CString cstrItemResult;
		CString cstrItemScript;
		cstrItemResult = m_TestList.GetItemText(lpNMItemActivate->iItem, 1);
		cstrItemScript = m_TestList.GetItemText(lpNMItemActivate->iItem, 0);
		if ((cstrItemResult.IsEmpty() && cstrItemScript.IsEmpty())||CTBSCommon::m_PresentThread->m_TestThread==NULL)
			return;
		CTBSDetailDlg *m_DetailDlg = CTBSDetailDlg::GetInstance();
		if (m_DetailDlg != NULL)
		{
			m_DetailDlg->cstrProjectName = CTBSCommon::m_PresentThread->cstrProjectName;
			m_DetailDlg->cstrScriptName = cstrItemScript;
			m_DetailDlg->cstrResult = cstrItemResult;
			m_DetailDlg->Create(IDD_DETAIL_PARSE_DIALIG);
			m_DetailDlg->ShowWindow(SW_SHOW);
		}
	}
	*pResult = 0;
}
