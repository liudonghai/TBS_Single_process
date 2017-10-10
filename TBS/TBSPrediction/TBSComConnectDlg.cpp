// TBSComConnectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSComConnectDlg.h"
#include "afxdialogex.h"
#include "TBSApp.h"
#include "TBSMSCOMThread.h"
// CTBSComConnectDlg �Ի���

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


// CTBSComConnectDlg ��Ϣ�������


void CTBSComConnectDlg::OnBnClickedOk()
{
	//����һ�����ȼ��ϸߵ��̣߳���ȡ��������
	CRuntimeClass* prt = RUNTIME_CLASS(CTBSMSCOMThread);
	CTBSCommon::m_PresentThread->m_ComThread= AfxBeginThread(prt, THREAD_PRIORITY_TIME_CRITICAL, 0, 0,NULL);
	if (CTBSCommon::m_PresentThread->m_ComThread == NULL)
	{
		AfxMessageBox(L"���ڴ�ʧ�ܣ�");
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
		AfxMessageBox(L"��ѡ����ȷ�Ĵ��ڲ���");
		return;
	}
	if (csParity == L"None")
	{
		csParity = L"n";
	}
	csParater = csBaud + "," + csParity + "," + csData + "," + csStop;
	
	CDialogEx::OnOK();
	
	//��鴮�ڵ���Ч��
	Sleep(200);//�ȴ���ȡ�������ݵ��̳߳�ʼ�����
	hCom = CreateFile(csPortocol, GENERIC_READ | GENERIC_WRITE, 0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if (hCom == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hCom);
		AfxMessageBox(L"���ڱ�ռ�û��߲����ڣ���");
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}
