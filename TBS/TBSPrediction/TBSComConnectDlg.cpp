// TBSComConnectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSComConnectDlg.h"
#include "afxdialogex.h"
#include "TBSApp.h"
#include "TBSMSCOMThread.h"
// CTBSComConnectDlg 对话框

IMPLEMENT_DYNAMIC(CTBSComConnectDlg, CDialogEx)

CTBSComConnectDlg::CTBSComConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONNECT_DIALOG, pParent)
{
}

CTBSComConnectDlg::~CTBSComConnectDlg()
{
}

void CTBSComConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTBSComConnectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSComConnectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTBSComConnectDlg 消息处理程序


void CTBSComConnectDlg::OnBnClickedOk()
{
	//创建一个优先级较高的线程，读取串口数据
	CRuntimeClass* prt = RUNTIME_CLASS(CTBSMSCOMThread);
	CTBSCommon::m_PresentThread->m_ComThread= AfxBeginThread(prt, THREAD_PRIORITY_TIME_CRITICAL, 0, 0,NULL);
	if (CTBSCommon::m_PresentThread->m_ComThread == NULL)
	{
		AfxMessageBox(L"串口打开失败！");
		return;
	}
	HANDLE	hCom;
	CString cstrTitle;
	CString csPortocol;
	CString csBaud;
	CString csData;
	CString csParity;
	CString csStop;
	CString csParater;

	GetDlgItem(IDC_COMBO_PORT)->GetWindowTextW(csPortocol);
	GetDlgItem(IDC_COMBO_BAUD)->GetWindowTextW(csBaud);
	GetDlgItem(IDC_COMBO_DATA)->GetWindowTextW(csData);
	GetDlgItem(IDC_COMBO_PARITY)->GetWindowTextW(csParity);
	GetDlgItem(IDC_COMBO_STOP)->GetWindowTextW(csStop);
	if (csPortocol.IsEmpty() || csBaud.IsEmpty() || csData.IsEmpty() || csParity.IsEmpty() || csStop.IsEmpty())
	{
		AfxMessageBox(L"请选择正确的串口参数");
		return;
	}
	if (csParity == L"None")
	{
		csParity = L"n";
	}
	csParater = csBaud + "," + csParity + "," + csData + "," + csStop;
	
	CDialogEx::OnOK();
	
	//检查串口的有效性
	Sleep(200);//等待读取串口数据的线程初始化完成
	hCom = CreateFile(csPortocol, GENERIC_READ | GENERIC_WRITE, 0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom);
		AfxMessageBox(L"串口被占用或者不存在！！");
		CTBSCommon::m_PresentThread->m_ComThread->PostThreadMessage(WM_QUIT, NULL, NULL);
		return ;
	}
	CloseHandle(hCom);

	cstrTitle = L"MSComm" + CTBSCommon::m_PresentThread->cstrProjectName;
	::SendMessage(::FindWindow(NULL, cstrTitle), WM_PARAMER_SEND, (WPARAM)&csPortocol, (LPARAM)&csParater);
	CTBSCommon::m_PresentThread->m_pMainDlg->m_Event.SetEvent();
	
	INT arrMenuIDEnable[] = { IDM_DISCONNECT_STB };
	CTBSDlg::tbs_menu_enable(arrMenuIDEnable, 1);
}


BOOL CTBSComConnectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetDlgItem(IDC_COMBO_PORT)->SetWindowTextW(L"COM3");
	GetDlgItem(IDC_COMBO_BAUD)->SetWindowTextW(L"115200");
	GetDlgItem(IDC_COMBO_DATA)->SetWindowTextW(L"8");
	GetDlgItem(IDC_COMBO_PARITY)->SetWindowTextW(L"None");
	GetDlgItem(IDC_COMBO_STOP)->SetWindowTextW(L"1");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
