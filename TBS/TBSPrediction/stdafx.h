
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
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
#define ERROR_MSG				L"分配内存失败"
#define CONFIGFILE				L"..\\config.ini"

//脚本执行状态
#define SCRIPT_WAIT				L"waiting..."
#define SCRIPT_PARSE			L"脚本解析完成,等待执行..."
#define SCRIPT_ERROR			L"脚本语法有错，跳过该脚本。"
#define SCRIPT_RUNNING			L"脚本执行中..."
#define SCRIPT_TEST_FAIL		L"Fail"
#define SCRIPT_TEST_FAIL_STR	"Fail"
#define SCRIPT_TEST_SUCCESS		L"Success"
#define SCRIPT_TEST_SUCCESS_STR "Success"
#define SCRIPT_TEST_FINISH		L"执行完成"

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


