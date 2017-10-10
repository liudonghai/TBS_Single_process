
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��
#include <afx.h>
#include <afxdisp.h>









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "TBSApp.h"
#include "TBSLog.h"
#include "TBSGlobal.h"
#include "TBSDetailDlg.h"
#include "TBSDataBase.h"
#include "TBSDlg.h"
#include "TBSCommon.h"
#include <string>
#include <fstream>
using namespace std;



#define IR_FILE_PATH			"IR_KEYCODE.keyword"
#define DATAPATH				"..\\database\\TBSDatabase.db"
#define PROJECTPATH				L"..\\project\\"
#define ERROR_MSG				L"�����ڴ�ʧ��"
#define CONFIGFILE				L"..\\config.ini"

//�ű�ִ��״̬
#define SCRIPT_WAIT				L"waiting..."
#define SCRIPT_PARSE			L"�ű��������,�ȴ�ִ��..."
#define SCRIPT_ERROR			L"�ű��﷨�д������ýű���"
#define SCRIPT_RUNNING			L"�ű�ִ����..."
#define SCRIPT_TEST_FAIL		L"Fail"
#define SCRIPT_TEST_FAIL_STR	"Fail"
#define SCRIPT_TEST_SUCCESS		L"Success"
#define SCRIPT_TEST_SUCCESS_STR "Success"
#define SCRIPT_TEST_FINISH		L"ִ�����"

#define LOG_FILE_PATH			L"..\\logfile\\TBSLog.log"
//the Mutex for DataBase.
#define TBS_DATABASE_METUX		L"TBS_DATABASE_METUX_NAME"
#define TBS_CONFIG_MUREX		L"TBS_CONFIG_METUX_NAME"
#define TBS_LOG_RECORDE_MUTEX	L"TBS_LOG_RECOEDE_MUTEX_NAME"

#define WM_KEYBOARD_MSG WM_USER+1 
#define WM_PARAMER_SEND WM_USER+2
#define WM_LOG_FILENAME WM_USER+3
#define WN_DETAIL_ACTON_CHANGE WM_USER+4
#define WM_EXPECTATION_CHECK WM_USER+5


