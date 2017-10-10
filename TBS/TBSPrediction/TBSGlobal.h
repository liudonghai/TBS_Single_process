#pragma once
#include "stdafx.h"
#include "TBSMainDlg.h"
#include "TBSScriptParse.h"
#include "TBSRedRat.h"
#include "TBSLog.h"

//CString to string
#define TOSTRING(x) CW2A((x).GetString())

//string to CString
#define TOCSTRING(x) (x).c_str()


typedef struct TBS_PRESENT_THREAD_t
{
	bool			bOpen			= false;	//check the project if opened;
	bool			bLock			= false;
	INT				iTabNum			= -1;		//the number in the tab control;
	CString			cstrRedRat;					//select the device of redrat
	INT				iNum			= -1;		//the start number of project,this value is setted for dialog name;
	INT				iScript			= -1;		//the index of m_pScriptParse;
	//CMutex			m_Mutex;					//synchronize the script parse and run;
	CEvent			m_Event;					//synchronize the message of log saved;
	CEvent			m_ManualEvent;				//Manual Test
 	CString			cstrProjectName	= L"";		//the project name of the present project
	CString			cstrIRFile		= L"";		//the name of the IR file
	CWinThread		*m_MainThread	= NULL;		//the pointer of MainThread;
	CWinThread		*m_ComThread	= NULL;		//the pointer of serial port thread;
	CWinThread		*m_ParseThread	= NULL;		//the pointer of script parse thread;
	CWinThread		*m_TestThread	= NULL;		//the pointer of script test thread;
	CTBSMainDlg		*m_pMainDlg		= NULL;		//the pointer of mian dialog;
}TBS_PRESENT_THREAD_t;

//synchronize the script parse and run,create in the CTBSScriptSelectDlg
typedef struct tbs_thread_script_t
{
	CString			*pcstrHead;
	INT				iScriptNum;
	//INT				iIndex;
	CTBSScriptParse *m_Script;
	CEvent			m_Event;
	CMutex			m_Mutex;
	CEvent			m_EventBegin;
	CWinThread		*pThreadExc;
	

}tbs_thread_script_t;

typedef struct tbs_test_detail_msg_t
{
	CString cstrScriptName;
	CString cstrCaseNmae;
	INT iIndex;
}tbs_test_detail_msg_t;