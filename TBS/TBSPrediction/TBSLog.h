#pragma once
#include "TBSGlobal.h"
#include <fstream>
using namespace std;


class CTBSLog : public CObject
{
public:
	CTBSLog();
	virtual ~CTBSLog();

public:
	fstream *pfsLogWrite;
	static CMutex *pMutex;
	static string strLogInfo;
	static UINT tbs_log_record_in(LPVOID lpParam);

	static void tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction, const CString cstrInfo);
	static void tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction, const string strInfo);
	static void tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction, const CHAR *pInfo);
	static void tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction, INT iInfo);
	static void tbs_log_info(const CHAR *pFile, INT iLine, const CHAR *pFunction);
};
