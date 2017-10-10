#pragma once
#include "CMSComm.h"
#include "MSCommDlg.h"

// CTBSMSCOMThread

class CTBSMSCOMThread : public CWinThread
{
	DECLARE_DYNCREATE(CTBSMSCOMThread)

protected:
	CTBSMSCOMThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CTBSMSCOMThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CMSCommDlg *m_ConnectDlg;

protected:
	DECLARE_MESSAGE_MAP()
};


