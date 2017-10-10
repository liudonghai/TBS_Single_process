#pragma once
#include "TBSMainDlg.h"
#include "TBSComConnectDlg.h"


// CTBSMainThread

class CTBSMainThread : public CWinThread
{
	DECLARE_DYNCREATE(CTBSMainThread)

protected:
	CTBSMainThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CTBSMainThread();

public:
	virtual BOOL InitInstance();
	virtual INT ExitInstance();
	
	CTBSMainDlg* m_MainDlg;


protected:
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
};


