
// TBSDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "TBSApp.h"

// CTBSPredictionApp: 
// �йش����ʵ�֣������ TBSPrediction.cpp
//

class CTBSApp : public CWinApp
{
public:
	CTBSApp();
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTBSApp theApp;