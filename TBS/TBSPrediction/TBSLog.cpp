// TBSLog.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSLog.h"
#include "TBSGlobal.h"
#include "TBSCommon.h"
#include <string>
using namespace std;

CMutex	*CTBSLog::pMutex=new CMutex;
string	CTBSLog::strLogInfo = "";

CTBSLog::CTBSLog()
{
	if (CTBSCommon::tbs_file_check(LOG_FILE_PATH))
	{
		DeleteFile(LOG_FILE_PATH);
	}
	CFile file;
	file.Open(LOG_FILE_PATH, CFile::modeCreate);
	file.Close();

}

CTBSLog::~CTBSLog()
{
}


void CTBSLog::tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction, const CString cstrInfo)
{
	INT		iLength = WideCharToMultiByte(CP_ACP, 0, cstrInfo, -1, NULL, 0, NULL, 0);
	CHAR	*pLogInfo = new CHAR[iLength];
	CHAR	*pLogInfoGet = new CHAR[iLength + MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, cstrInfo, -1, pLogInfo, iLength, NULL, 0);
	pMutex->Lock();
	sprintf_s(pLogInfoGet, iLength + MAX_PATH, "[%s]: %d [%s]:\t %s\n", pFile, iLine, pFunction, pLogInfo);
	strLogInfo = pLogInfoGet;
	pMutex->Unlock();
	delete[] pLogInfoGet;
	delete[] pLogInfo;
}

void CTBSLog::tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction, const string strInfo)
{
	INT		iLength = strInfo.length();
	CHAR	*pLogInfo = new CHAR[iLength + MAX_PATH];
	pMutex->Lock();
	sprintf_s(pLogInfo, iLength + MAX_PATH, "[%s]: %d [%s]:\t %s\n", pFile, iLine, pFunction, strInfo.c_str());
	strLogInfo = pLogInfo;
	pMutex->Unlock();
	delete[] pLogInfo;
}

void CTBSLog::tbs_log_info(const CHAR *pFile, INT iLine,const CHAR *pFunction,const CHAR *pInfo)
{	
	INT		iLength = strlen(pInfo);
	CHAR	*pLogInfo = new CHAR[MAX_PATH + iLength];
	pMutex->Lock();
	sprintf_s(pLogInfo, MAX_PATH + iLength,"[%s]: %d [%s]:\t %s\n", pFile, iLine, pFunction, pInfo);
	strLogInfo = pLogInfo;
	pMutex->Unlock();
	delete[] pLogInfo;
}

void CTBSLog::tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction,  INT iInfo)
{
	
	CHAR pLogInfo[MAX_PATH];
	pMutex->Lock();
	sprintf_s(pLogInfo, MAX_PATH, "[%s]: %d [%s]:\t the value is:%d\n", pFile, iLine, pFunction, iInfo);
	strLogInfo = pLogInfo;
	pMutex->Unlock();
}

void CTBSLog::tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction)
{
	CHAR pLogInfo[MAX_PATH];
	pMutex->Lock();
	sprintf_s(pLogInfo, MAX_PATH, "[%s]: %d [%s]\n", pFile, iLine, pFunction);
	strLogInfo = pLogInfo;
	pMutex->Unlock();
}

UINT CTBSLog::tbs_log_record_in(LPVOID lpParam)
{
	fstream pfsLogWrite(LOG_FILE_PATH, ios::in | ios::app);
	while (1)
	{
		pMutex->Lock();
		if (!strLogInfo.empty())
		{
			pfsLogWrite << strLogInfo << flush;
			strLogInfo = "";
		}
		pMutex->Unlock();
	}
	pfsLogWrite.close();
	return 0;
}