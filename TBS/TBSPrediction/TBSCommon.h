#pragma once
#include <string>
using namespace std;

class CTBSCommon
{
public:
	CTBSCommon();
	~CTBSCommon();

	// 分割字符串函数
	static void tbs_com_string_split(string srcStr, CHAR cSplit, string* pDesStr);
	// 判断文件及文件夹是否存在
	static bool tbs_is_file_exist(const CString csFilePath);
	static bool tbs_com_delete_directory(CString strPath, bool bDelete);
	static INT tbs_file_check(const CString cstrPath);
	static CString tbs_file_code_check(CString cstrFileName);
	static CString tbs_file_code_check(string  strFileName);
	static void ConvertUTF8ToANSI(CHAR* pUTF8, CHAR **pANSI);
public:
	static INT iTBSNum;
	static INT iTBSPresent;
	static TBS_PRESENT_THREAD_t *m_PresentThread;
	
};

