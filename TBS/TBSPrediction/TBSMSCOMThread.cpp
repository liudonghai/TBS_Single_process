// TBSMSCOMThread.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSMSCOMThread.h"
#include "MSCommDlg.h"


// CTBSMSCOMThread

IMPLEMENT_DYNCREATE(CTBSMSCOMThread, CWinThread)

CTBSMSCOMThread::CTBSMSCOMThread()
{
}

CTBSMSCOMThread::~CTBSMSCOMThread()
{
}

BOOL CTBSMSCOMThread::InitInstance()
{
	m_ConnectDlg = new CMSCommDlg;
	m_ConnectDlg->Create(IDD_MSCOMM_DIALOG, NULL);
	m_ConnectDlg->ShowWindow(SW_HIDE);
	return TRUE;
}

int CTBSMSCOMThread::ExitInstance()
{
	delete m_ConnectDlg;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTBSMSCOMThread, CWinThread)
END_MESSAGE_MAP()


// CTBSMSCOMThread 消息处理程序
