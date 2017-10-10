#pragma once
#include <string>
#include <fstream>
#include "stdafx.h"
using namespace std;


// CTBSScriptParse 命令目标

enum TBS_SCRIPT_OPREATION_TYPE_e { A，C，D，E，F，I，X};

//循环链表
typedef struct TBS_LOOP_INF_t
{
	INT				iLoop;							//循环级别
	INT				iLoopNum;						//该级别的循环次数
	TBS_LOOP_INF_t	*pNext;

}TBS_LOOP_INF_t;

//动作链表
typedef struct TBS_SCRIPT_ACTION_TYPE_t
{
	CHAR							cCondtion;			//条件标志位
	string							strCmdType;			//操作类型
	string							strCmd;				//操作内容
	TBS_LOOP_INF_t					pLoop_t;
	TBS_SCRIPT_OPREATION_TYPE_e		sOperationType_e;	//标签类型
	TBS_SCRIPT_ACTION_TYPE_t		*pNext;

}TBS_SCRIPT_ACTION_TYPE_t;

//测试期望结构体
typedef struct TBS_SCRIPT_EXPECTION_t
{
	INT		iNum		= 0;
	INT		iFlag		= 0;
	INT		iResult[6]	= { 0 };
	string	strExpType[5];
	string	strCmd[5];
	string	strTime[5];
	//string	strSIM;
	//string	strSavePath;
	CEvent	*pEvent;
	bool	bEffect = false;

}TBS_SCRIPT_EXPECTION_t;
typedef struct TBS_SCRIPT_PARSE_ERROR_INF_t
{
	INT			iLineNum;
	INT			iError;
	CString		cstrErrorInfo;

}TBS_SCRIPT_PARSE_ERROR_INF_t;

class CTBSScriptParse : public CObject
{

public:
	CTBSScriptParse();
	CTBSScriptParse(string strFilePath);
	virtual ~CTBSScriptParse();

public:
	CTime m_ScriptTime;

public:
	static UINT		tbs_test_script_parse(LPVOID lpParam);
	static UINT		tbs_test_script_execute(LPVOID lpParam);

protected:
	INT								iLoopFlag;
	INT								iConditionFlag;
	INT								iEnd;
	string							strCaseName;
	CString							cstrPageCode;
	fstream							*fsReadScript;
	TBS_LOOP_INF_t					*pHeadLoop;
	TBS_SCRIPT_ACTION_TYPE_t		*pListhead1;
	TBS_SCRIPT_ACTION_TYPE_t		*pListhead2;
	TBS_SCRIPT_PARSE_ERROR_INF_t	*pParseError;
	TBS_SCRIPT_ACTION_TYPE_t		*pHeadAction;



	void tbs_test_add_to_looplist(TBS_LOOP_INF_t* pLoop_t);
	void tbs_test_script_parse_error(string strScriptName,string strProjectName);
	void tbs_test_add_to_actionlist(TBS_SCRIPT_ACTION_TYPE_t* pActionType_t);

	INT tbs_test_parse_line(INT *pCase);
	INT tbs_test_parse_single_tab(CHAR *pBuffer);
	INT tbs_test_space_delete(CHAR *pBuffer);
	INT tbs_test_prase_c(CHAR *pLine, INT iLineNum);
	INT tbs_test_parse_keytype(CHAR *pLine, INT iLineNum, INT *pCase);
	INT tbs_test_parse_action(CHAR *pLine, INT iLineNum, CHAR **ppKey, INT ikeylength, CHAR *pEnd, TBS_SCRIPT_OPREATION_TYPE_e OPREATION_TYPE_e, INT *pCase=NULL);

	TBS_SCRIPT_ACTION_TYPE_t* tbs_test_get_script_value(INT *loopflag1, INT *loopflag2);
private:
	CMutex	*m_MutexDetail;
};


