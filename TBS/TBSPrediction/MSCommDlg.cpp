// MSCommDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "MSCommDlg.h"
#include "afxdialogex.h"
#include "CMSComm.h"
#include "TBSApp.h"
#include "TBSScriptParse.h"
#include <regex>
#include <string>
using namespace std;

IMPLEMENT_DYNAMIC(CMSCommDlg, CDialogEx)

CMSCommDlg::CMSCommDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	cstrProjectName = CTBSCommon::m_PresentThread->cstrProjectName;
	m_MainDlg		= CTBSCommon::m_PresentThread->m_pMainDlg;
	m_Exptation		= NULL;
	m_MSCOMbutton	= new CMSComm;
	pGetData		= NULL;
	cstrFileName	= L"";
	dTimeBegin		= 0;
	iFlag			= 0;
	pCharLog		= NULL;
	regEachTime[0]	= L"(%Y)";
	regEachTime[1]	= L"(%M)";
	regEachTime[2]	= L"(%D)";
	regEachTime[3]	= L"(%h)";
	regEachTime[4]	= L"(%m)";
	regEachTime[5]	= L"(%s)";
	regEachTime[6]	= L"(%t)";

	memset(dTime, 0, 5*sizeof(dTime[0]));
}

CMSCommDlg::~CMSCommDlg()
{
	cstrFileName = L"";

}

void CMSCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOM1, *m_MSCOMbutton);
}


BEGIN_MESSAGE_MAP(CMSCommDlg, CDialogEx)
	ON_MESSAGE(WM_KEYBOARD_MSG, CMSCommDlg::OnKeyboardMsgRec)
	ON_MESSAGE(WM_PARAMER_SEND, CMSCommDlg::OnCMSCommRec)
	ON_MESSAGE(WM_LOG_FILENAME, CMSCommDlg::OnLogFileRec)
	ON_MESSAGE(WM_EXPECTATION_CHECK, CMSCommDlg::OnExpectationRec)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMSCommDlg 消息处理程序
BEGIN_EVENTSINK_MAP(CMSCommDlg, CDialogEx)
	ON_EVENT(CMSCommDlg, IDC_MSCOM1, 1, CMSCommDlg::OnMSComm, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CMSCommDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	CString cstrTitle;
	/*WCHAR	pFileName[MAX_PATH] = { 0 };
	WCHAR	pFilePath[MAX_PATH] = { 0 };
	WCHAR	pEachLine[MAX_PATH] = { 0 };
	WCHAR	pProjectName[MAX_PATH] = { 0 };*/


	
	cstrTitle = L"MSComm" + CTBSCommon::m_PresentThread->cstrProjectName;
	SetWindowText(cstrTitle);
	tbs_get_log_info();
	/*CMutex m_ConfigMutex(FALSE, TBS_CONFIG_MUREX);
	CSingleLock m_SingleLock(&m_ConfigMutex);
	m_SingleLock.Lock();
	GetPrivateProfileString(L"LOGFileTmp", L"Project", L"", pProjectName, MAX_PATH, CONFIGFILE);
	if (cstrProjectName == (CString)pProjectName)
	{
		GetPrivateProfileString(L"LOGFileTmp", L"Name", L"", pFileName, MAX_PATH, CONFIGFILE);
		GetPrivateProfileString(L"LOGFileTmp", L"Path", L"", pFilePath, MAX_PATH, CONFIGFILE);
		GetPrivateProfileString(L"LOGFileTmp", L"Line", L"", pEachLine, MAX_PATH, CONFIGFILE);
	}
	else
	{
		GetPrivateProfileString(L"LOGFile", L"Name", L"", pFileName, MAX_PATH, CONFIGFILE);
		GetPrivateProfileString(L"LOGFile", L"Path", L"", pFilePath, MAX_PATH, CONFIGFILE);
		GetPrivateProfileString(L"LOGFile", L"Line", L"", pEachLine, MAX_PATH, CONFIGFILE);
	}
	m_SingleLock.Unlock();

	this->cstrFilePath = pFilePath;
	this->cstrFileName = pFileName;
	this->cstrEachLine = pEachLine;
	if (!cstrFilePath.IsEmpty() && !cstrFileName.IsEmpty())
	{
		tbs_log_file_create();
	}*/
	return TRUE;
}

VOID CMSCommDlg::OnMSComm()
{
	INT				iLength;
	WCHAR*			lpInBufferw = NULL;
	CString			cstr;
	COleSafeArray	safearray_inp;

	switch (m_MSCOMbutton->get_CommEvent())
	{
	case 1:
		break;
	case 2:
		safearray_inp	= m_MSCOMbutton->get_Input();
		iLength			= safearray_inp.GetOneDimSize();
		pGetData		= new CHAR[iLength + 2];
		
		if (pGetData == NULL)
		{
			AfxMessageBox(ERROR_MSG);
			return;
		}
		for (LONG n = 0; n < iLength; n++)
		{
			safearray_inp.GetElement(&n, &pGetData[n]);
		}
		pGetData[iLength] = '\0';
		if (iFlag != 0)
		{
			for (INT i = 0; i < m_Exptation->iNum; i++)
			{
				regex regParser(m_Exptation->strCmd[i]);
				if (dTime[i] == 0)
				{
					if (regex_search(pGetData, regParser))
					{
						dTime[i] = GetTickCount();
						break;
					}
					else
					{
						if (pCharLog != NULL)
						{
							string strLog = (string)pCharLog+ (string)pGetData;
							CString cstrLog(TOCSTRING(strLog));
							CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrLog);
							if (regex_search(strLog, regParser))
							{
								dTime[i] = GetTickCount();
							}
						}
					}
				}
			}
			pCharLog = pGetData;
		} 

		/*pGetData[iLength] = '\n';
		pGetData[iLength + 1] = '\0';*/
		iLength		= MultiByteToWideChar(CP_UTF8, 0, pGetData, -1, NULL, 0);
		lpInBufferw = new WCHAR[iLength];
		if (lpInBufferw == NULL)
		{
			MessageBox(ERROR_MSG);
			return;
		}
		MultiByteToWideChar(CP_UTF8, 0, pGetData, -1, lpInBufferw, iLength);
		if (pfsFile.is_open())
		{
			tbs_log_write(lpInBufferw);
		}
		m_MainDlg->m_RichEdit.SetSel(-1, -1);
		m_MainDlg->m_RichEdit.ReplaceSel(lpInBufferw);

		delete[] pGetData;
		delete[] lpInBufferw;
		break;

	default:
		break;
	}
}

VOID CMSCommDlg::tbs_init_MSComm()
{
	INT		iComNumber;
	WCHAR	wcPort;

	wcPort = this->cstrPort[3];
	iComNumber = _ttoi(&wcPort);
	this->m_MSCOMbutton->put_CommPort(iComNumber);
	this->m_MSCOMbutton->put_InBufferSize(1024);
	this->m_MSCOMbutton->put_OutBufferSize(1024);
	//设置为0，表示全部读取，用以清空缓存区。
	this->m_MSCOMbutton->put_InputLen(0);
	if (!this->m_MSCOMbutton->get_PortOpen())
	{
		this->m_MSCOMbutton->put_PortOpen(true);
	}
	this->m_MSCOMbutton->put_InputMode(1);
	//this->m_MSCom->put_Settings(L"115200,n,8,1");
	this->m_MSCOMbutton->put_Settings(this->cstrParam);
	this->m_MSCOMbutton->put_RThreshold(1);
}

//接收键盘消息
LRESULT CMSCommDlg::OnKeyboardMsgRec(WPARAM INum, LPARAM strSend)
{
	
	INT			iNum	= INum;
	CHAR		*pCout	= NULL;
	CByteArray	m_Send;

	if (iNum == 1 && strSend == 0x03)
	{
		m_Send.SetSize(1);
		m_Send.SetAt(0, 0x03);
		m_MSCOMbutton->put_Output(COleVariant(m_Send));
	}
	else
	{
		pCout = (CHAR*)strSend;
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, pCout);

		m_Send.SetSize(iNum);
		for (INT i = 0; i < iNum; i++)
		{
			m_Send.SetAt(i, pCout[i]);
		}
		m_MSCOMbutton->put_Output(COleVariant(m_Send));
	}
	return 0;
}

//接收串口参数
LRESULT CMSCommDlg::OnCMSCommRec(WPARAM cstrPort, LPARAM cstrParam)
{
	
	CString *pParameter = (CString*)cstrParam;
	CString *pPort		= (CString*)cstrPort;
	
	this->cstrParam		= *pParameter;
	this->cstrPort		= *pPort;
	tbs_init_MSComm();
	
	return 0;
}

//写文件
VOID CMSCommDlg::tbs_log_write(WCHAR *pData)
{
	if (wcslen(pData) <= 0)
		return;
	WCHAR	*wcBuff = NULL;
	if (!cstrEachLine.IsEmpty())
	{
		BSTR	bsEach = cstrEachLine.AllocSysString();
		WCHAR	*pEachLine = bsEach;
		CString cstrTime[7];
		wregex	regEnter(L"(\\n)");
		wstring wstrOut= pEachLine;

		GetLocalTime(&m_SysTime);
		cstrTime[0].Format(L"%4d", m_SysTime.wYear);
		cstrTime[1].Format(L"%2d", m_SysTime.wMonth);
		cstrTime[2].Format(L"%2d", m_SysTime.wDay);
		cstrTime[3].Format(L"%2d", m_SysTime.wHour);
		cstrTime[4].Format(L"%2d", m_SysTime.wMinute);
		cstrTime[5].Format(L"%2d", m_SysTime.wSecond);
		cstrTime[6].Format(L"%3d", m_SysTime.wMilliseconds);

		for (INT i = 0; i < 7; i++)
		{
			wstrOut =regex_replace(wstrOut, regEachTime[i], cstrTime[i].GetBuffer());
		}
		wstrOut += L" ";
		wstrOut = regex_replace(pData, regEnter, wstrOut);
		wcBuff = new WCHAR[wstrOut.length()];
		pfsFile.pubsetbuf(wcBuff, (streamsize)wstrOut.length());
		pfsFile.sputn(wstrOut.c_str(), (streamsize)wstrOut.length());
	}
	else
	{
		wcBuff = new WCHAR[wcslen(pData)];
		pfsFile.pubsetbuf(wcBuff, (streamsize)wcslen(pData));
		pfsFile.sputn(pData, (streamsize)wcslen(pData));
	}
}

//接收log参数
LRESULT CMSCommDlg::OnLogFileRec(WPARAM nullParameter, LPARAM cstrFileName)
{
	if (pfsFile.is_open())
	{
		pfsFile.close();
	}
	/*CString *cstrLogFile = (CString*)cstrFileName;
	this->cstrFilePath = cstrLogFile[0];
	this->cstrFileName = cstrLogFile[1];
	this->cstrEachLine = cstrLogFile[2];
	tbs_log_file_create();*/
	tbs_get_log_info();
	return 0;
}

//创建log文件，和数据写入文件流
void CMSCommDlg::tbs_log_file_create()
{
	CFile m_File;
	//解析文件名称，是否含有时间参数
	string strFileName(TOSTRING(cstrFileName));
	regex m_RegTime("(%)");
	if (regex_search(strFileName, m_RegTime))
	{
		SYSTEMTIME m_SystemTime;
		GetLocalTime(&m_SystemTime);
		regex m_RegYear("(%Y)");
		regex m_RegMonth("(%M)");
		regex m_RegDay("(%D)");
		regex m_RegHour("(%h)");
		regex m_RegMinute("(%m)");
		regex m_RegSecond("(%s)");
		regex m_RegMSecond("(%t)");
		char cYear[10];
		char cMonth[10];
		char cDay[10];
		char cHour[10];
		char cMinutes[10];
		string strSencond;
		char cMilliSencond[10];
		CString cstrSecond;
		cstrSecond.Format(L"%2d", m_SystemTime.wSecond);
		strSencond = CW2A(cstrSecond.GetString());
		_itoa_s(m_SystemTime.wYear, cYear, 10);
		_itoa_s(m_SystemTime.wMonth, cMonth, 10);
		_itoa_s(m_SystemTime.wDay, cDay, 10);
		_itoa_s(m_SystemTime.wHour, cHour, 10);
		_itoa_s(m_SystemTime.wMinute, cMinutes, 10);
		_itoa_s(m_SystemTime.wMilliseconds, cMilliSencond, 10);
		strFileName=regex_replace(strFileName, m_RegYear, cYear);
		strFileName = regex_replace(strFileName, m_RegMonth, cMonth);
		strFileName = regex_replace(strFileName, m_RegDay, cDay);
		strFileName = regex_replace(strFileName, m_RegHour, cHour);
		strFileName = regex_replace(strFileName, m_RegMinute, cMinutes);
		strFileName = regex_replace(strFileName, m_RegSecond, strSencond);
		strFileName = regex_replace(strFileName, m_RegMSecond, cMilliSencond);
		cstrFileName = strFileName.c_str();
	}
	if (!this->cstrEachLine.IsEmpty())
	{
		strEachLine = CW2A(cstrEachLine.GetString());
		strEachLine += " ";
	}
	CString cstrLog = this->cstrFilePath + L"\\" + this->cstrFileName;
	string strLog(TOSTRING(cstrLog));
	if (m_File.Open(cstrLog, CFile::modeCreate))
	{
		m_File.Close();
		CTBSDataBase m_DataBase(DATAPATH);
		string strProjectName(TOSTRING(cstrProjectName));
		string strSqlLog = "update TestReport set Log='"+ strLog +"' where ProjectName='"+ strProjectName +"'";
		m_DataBase.tbs_db_sql_exec(strSqlLog);
		pfsFile.open(cstrLog, ios::out | ios::app | ios_base::binary);
	}
	else
	{
		AfxMessageBox(L"LOG文件创建失败,请检查文件路径是否存在，文件名是否合法，注意文件名中不能含有<>,/,\\,|,:,"",*,?符号");
		return;
	}
}

LRESULT CMSCommDlg::OnExpectationRec(WPARAM nullParameter, LPARAM pExp)
{
	INT iDelayTime = 0;
	INT iTime;
	m_Exptation = (TBS_SCRIPT_EXPECTION_t*)pExp;
	dTimeBegin	= GetTickCount();
	iFlag		= 1;
	
	
	for (INT i = 0; i < m_Exptation->iNum; i++)
	{
		iTime = stoi(*(m_Exptation->strTime + i));
		if (iDelayTime < iTime)
		{
			iDelayTime = iTime;
		}
	}
	SetTimer(1, iDelayTime, NULL);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iDelayTime);
	return 0;
}

void CMSCommDlg::OnTimer(UINT_PTR nIDEvent)
{
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, dTime[0]);
	KillTimer(nIDEvent);
	DOUBLE dSecond;
	iFlag = 0;
	for (INT i = 0; i < m_Exptation->iNum; i++)
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, dTime[i]);
		if (dTime[i] != 0)
		{
			dSecond = dTime[i] - dTimeBegin;
			if (dSecond > atoi(m_Exptation->strTime[i].c_str()))
			{
				m_Exptation->iResult[i] = -1;
				m_Exptation->iResult[5] = -1;
			}
		}
		else
		{
			m_Exptation->iResult[i] = -1;
			m_Exptation->iResult[5] = -1;
		}
	}
	m_Exptation->pEvent->SetEvent();
	memset(dTime, 0, 5 *sizeof(dTime[0]));
	m_Exptation = NULL;
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__,L"定时器结束！");
	CDialogEx::OnTimer(nIDEvent);
}
void CMSCommDlg::tbs_get_log_info()
{
	WCHAR	pFileName[MAX_PATH] = { 0 };
	WCHAR	pFilePath[MAX_PATH] = { 0 };
	WCHAR	pEachLine[MAX_PATH] = { 0 };
	WCHAR	pProjectName[MAX_PATH] = { 0 };
	

	CMutex m_ConfigMutex(FALSE, TBS_CONFIG_MUREX);
	CSingleLock m_SingleLock(&m_ConfigMutex);
	m_SingleLock.Lock();
	GetPrivateProfileString(L"LOGFileTmp", L"Project", L"", pProjectName, MAX_PATH, CONFIGFILE);
	if (cstrProjectName == (CString)pProjectName)
	{
		GetPrivateProfileString(L"LOGFileTmp", L"Name", L"", pFileName, MAX_PATH, CONFIGFILE);
		GetPrivateProfileString(L"LOGFileTmp", L"Path", L"", pFilePath, MAX_PATH, CONFIGFILE);
		GetPrivateProfileString(L"LOGFileTmp", L"Line", L"", pEachLine, MAX_PATH, CONFIGFILE);
	}
	else
	{
		GetPrivateProfileString(L"LOGFile", L"Name", L"", pFileName, MAX_PATH, CONFIGFILE);
		GetPrivateProfileString(L"LOGFile", L"Path", L"", pFilePath, MAX_PATH, CONFIGFILE);
		GetPrivateProfileString(L"LOGFile", L"Line", L"", pEachLine, MAX_PATH, CONFIGFILE);
	}
	m_SingleLock.Unlock();

	this->cstrFilePath = pFilePath;
	this->cstrFileName = pFileName;
	this->cstrEachLine = pEachLine;
	if (!cstrFilePath.IsEmpty() && !cstrFileName.IsEmpty())
	{
		tbs_log_file_create();
	}
}