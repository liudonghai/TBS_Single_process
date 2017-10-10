#pragma once
#include "CMSComm.h"
#include "MSCommDlg.h"

// CTBSMSCOMThread

class CTBSMSCOMThread : public CWinThread
{
	DECLARE_DYNCREATE(CTBSMSCOMThread)

protected:
	CTBSMSCOMThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTBSMSCOMThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CMSCommDlg *m_ConnectDlg;

protected:
	DECLARE_MESSAGE_MAP()
};


