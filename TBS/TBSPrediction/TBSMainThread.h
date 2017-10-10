#pragma once
#include "TBSMainDlg.h"
#include "TBSComConnectDlg.h"


// CTBSMainThread

class CTBSMainThread : public CWinThread
{
	DECLARE_DYNCREATE(CTBSMainThread)

protected:
	CTBSMainThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


