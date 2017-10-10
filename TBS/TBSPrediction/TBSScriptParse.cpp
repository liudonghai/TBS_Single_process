// TBSScriptParse.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSScriptParse.h"
#include "TBSPredictionDlg.h"
#include "TBSDataBase.h"
#include "TBSRedRat.h"
#include "TBSScriptParseError.h"
#include "TBSLog.h"
#include <regex>
#include <string>
using namespace std;

#define IFNULL(x) if((x) == NULL) { return 0; }

CTBSScriptParse::CTBSScriptParse()
{
	fsReadScript	= new fstream;
	iEnd			= 0;
	pHeadAction		= NULL;
	pHeadLoop		= NULL;
	pListhead1		= NULL;
	pListhead2		= NULL;
	iLoopFlag		= 0;
	iConditionFlag	= 0;
	m_MutexDetail	= new CMutex;
	pParseError		= new TBS_SCRIPT_PARSE_ERROR_INF_t;
}

CTBSScriptParse::CTBSScriptParse(string strFilePath)
{
	iEnd = 0;
	fsReadScript	= new fstream;
	pListhead1		= NULL;
	pListhead2		= NULL;
	pHeadAction		= NULL;
	pHeadLoop		= NULL;
	iLoopFlag		= 0;
	iConditionFlag	= 0;
	m_MutexDetail	= new CMutex;
	pParseError		= new TBS_SCRIPT_PARSE_ERROR_INF_t;
	fsReadScript->open(strFilePath, ios::in);

}

CTBSScriptParse::~CTBSScriptParse()
{
	fsReadScript->close();
	delete fsReadScript;
	delete pParseError;
}

//�������ܣ�
//		�����ű����̵߳���ں���
//	������
//		lpParam��	�ű�������ɵ��ַ���ָ��
//	����ֵ��
//		0	��	�ɹ���
UINT CTBSScriptParse::tbs_test_script_parse(LPVOID lpParam)
{
	//we should use the member of TBS_PRESENT_THREAD_t to synchronize the  thread;
	TBS_PRESENT_THREAD_t *m_PresentThread = &CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent];
	tbs_thread_script_t *m_Parameter = (tbs_thread_script_t*)lpParam;

	
	INT				iFlag = 1;
	INT				iItemNum = 0;
	INT				iResult;
	INT				iThreadID = m_PresentThread->m_ParseThread->m_nThreadID;
	string			strScrptPath;
	string			strProjectName;
	CString			*pScriptName	= m_Parameter->pcstrHead;
	CSingleLock		m_ParseAndRun(&m_Parameter->m_Mutex);
	CTBSDataBase	m_DataBase(DATAPATH);
	CTBSMainDlg		*m_MainDlg		= m_PresentThread->m_pMainDlg;
	CTBSScriptParse	*m_Script		= NULL;
	CSingleLock		m_SingkeLock(CTBSPredictionDlg::m_Mutex);
	strProjectName = TOSTRING(m_PresentThread->cstrProjectName);

	while (!(*pScriptName).IsEmpty() && m_Script == NULL)
	{
		INT				iRow	= 0;
		INT				iColum	= 0;
		INT				iCase	= 0;
		INT				iCaseNumber = 0;
		CHAR			**ppScriptData;
		string			strScriptName;
		string			strSqlDelete;
		string			strSqlselect;
		string			strSqlinsert;
		string			strSqlUpdate;
		CString			cstrPageCode;

		strScriptName	= TOSTRING(*pScriptName);
		strSqlselect	= "select * from TestCase where ProjectName='" + strProjectName + "' and ScriptName='" + strScriptName + "'";
		strSqlDelete	= "Delete from TestDetail where ProjectName='" + strProjectName + "' and ScriptName='" + strScriptName + "'";
		strSqlinsert	= "Insert into TestDetail (ProjectName,ScriptName,CaseName) values ('" + strProjectName + "','" + strScriptName + "','Describe')";
		
		m_DataBase.tbs_db_sql_exec(strSqlDelete);
		m_DataBase.tbs_db_sql_exec(strSqlinsert);
		m_DataBase.tbs_db_data_query(strSqlselect, &ppScriptData, &iRow, &iColum);
		if (iRow == 1)
		{
			strScrptPath = *(ppScriptData + iColum + 4);
		}
		//m_Script = new CTBSScriptParse(strScrptPath);
		cstrPageCode = CTBSCommon::tbs_file_code_check(strScrptPath);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrPageCode);
		fstream fsScript(strScrptPath, ios::in);
		if (cstrPageCode == L"UTF-8")
		{
			fsScript.seekg(3);
		}
		if (fsScript.is_open() && (cstrPageCode == L"ANSI" || cstrPageCode == L"UTF-8"))
		{
			do
			{/*
				if(!m_ParseAndRun.IsLocked())
					m_ParseAndRun.Lock();*/
				m_Script = new CTBSScriptParse();
				m_Script->fsReadScript = &fsScript;
				m_Script->cstrPageCode = cstrPageCode;
				iResult = m_Script->tbs_test_parse_line(&iCaseNumber);
				if (iCaseNumber == 0)
				{
					iCaseNumber = 1;
				}
				CHAR pCaseNumber[10];
				_itoa_s(iCaseNumber, pCaseNumber,10,10);
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iResult);
				strSqlUpdate = "Update TestDetail set CaseNumber='"+ (string)pCaseNumber +"' where ProjectName='"+ strProjectName +"' and ScriptName='"+ strScriptName +"' and CaseName='Describe'";
				m_DataBase.tbs_db_sql_exec(strSqlUpdate);
				
				if (iCase < iCaseNumber - 1)
				{
					m_Script->iEnd = 1;
				}
				else
				{
					m_Script->iEnd = 0;//���һ���ű��������
				}
				if (iResult != SCRIPT_EOF)
				{
					strSqlinsert = "Insert into TestDetail (ProjectName,ScriptName,CaseName) values ('" + strProjectName + "','" + strScriptName + "','" + m_Script->strCaseName + "')";
					m_DataBase.tbs_db_sql_exec(strSqlinsert);
				}
				if (iResult==SCRIPT_FINISH)
				{
					//�����ɹ�
					//m_MainDlg->m_TestList.SetItemText(iItemNum, 1, SCRIPT_PARSE);
					m_Script->pParseError->iError = iResult;
					m_Script->pParseError->iLineNum = 0;
					m_Script->tbs_test_script_parse_error(strScriptName, strProjectName);
					m_Parameter->m_Script = m_Script;
					if (iFlag == 1)
					{
						iFlag = 0;
						ResumeThread(m_Parameter->pThreadExc->m_hThread);
					}
					m_Parameter->m_Event.Lock();
				}
				else
				{
					//������ʾ
					//m_MainDlg->m_TestList.SetItemText(iItemNum, 1, SCRIPT_ERROR);
					m_Script->pParseError->iError = iResult;
					m_Script->tbs_test_script_parse_error(strScriptName, strProjectName);
					if (iResult == SCRIPT_EOF)
					{
						break;
					}
					//�ƶ������case��FINISH��ǩ
					if (m_Script->iEnd == 1)
					{
						CHAR *pBuffer = new CHAR[MAX_PATH];
						while (!m_Script->fsReadScript->eof())
						{
							m_Script->fsReadScript->getline(pBuffer, MAX_PATH);
							if (0 == strcmp(pBuffer,"<D FINISH>"))
							{
								break;
							}
						}
					}
					/*delete  m_Script;
					m_Script = NULL;*/
				}
				//if (m_Script != NULL)
				//{
				//	//m_Script = NULL;
				//	//m_ParseAndRun.Unlock();
				//	m_Parameter->m_Event.Lock();
				//}
				iCase++;
			} while (iCase < iCaseNumber);
		}
		else
		{
			//m_MainDlg->m_TestList.SetItemText(iItemNum, 1, SCRIPT_ERROR);
			if (fsScript.is_open()&&(cstrPageCode != L"ANSI" && cstrPageCode != L"UTF-8"))
			{
				m_Script->pParseError->iError = 13;
			}
			else
			{
				m_Script->pParseError->iError = 1;
			}
			m_Script->pParseError->iLineNum = 0;
			m_Script->tbs_test_script_parse_error(strScriptName, strProjectName);
			delete  m_Script;
			m_Script = NULL;
		}
		//delete  m_Script;
		m_Script = NULL;
		pScriptName++;
		iItemNum++;

		m_SingkeLock.Lock();
		for (INT i = 0; i < CTBSCommon::iTBSNum; i++)
		{
			if (CTBSCommon::m_PresentThread[i].m_ParseThread != NULL && iThreadID == CTBSCommon::m_PresentThread[i].m_ParseThread->m_nThreadID)
			{
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
				break;
			}
			if (i == CTBSCommon::iTBSNum - 1)
			{
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}
		}
		m_SingkeLock.Unlock();
	}
	//if (m_ParseAndRun.IsLocked())
	//{
	//	m_ParseAndRun.Unlock();
	//}
	return 0;
}

//�������ܣ�
//		ƥ�亯��ÿһ�е����ͣ�
//	������
//		void;
//	����ֵ��
//		SCRIPT_EOF	��	�ɹ���
//		����				��	�μ���������롣
INT CTBSScriptParse::tbs_test_parse_line(INT *pCase)
{
	INT iResult;
	INT iLineNum = 0;
	CHAR *pBuffer_Get = new CHAR[MAX_PATH];
	
	//ƥ����Ҫ�����Ĳ���
	regex regParser1("((\\s|\t)*)(<[ACDEFI] )([A-z]+>)(.*)(</[ACDEFI]>)((\\s|\t)*)");
	regex regParser2("((\\s|\t)*)(<[ACDEFI] )([A-z]+>)((\\s|\t)*)");
	//ƥ�䲻��Ҫ�����Ĳ���
	regex regAnnotation("((\\s|\t)*)(<!--(.*)-->)((\\s|\t)*)|((\\s|\t)*)");
	regex regSpace("((\\s|\t)*)");
	while (!(this->fsReadScript->eof()))
	{
		iLineNum++;
		memset(pBuffer_Get, 0, MAX_PATH);
		CHAR *pBuffer = NULL;
		this->fsReadScript->getline(pBuffer_Get, MAX_PATH);
		if (cstrPageCode == L"UTF-8")
		{
			CTBSCommon::ConvertUTF8ToANSI(pBuffer_Get, &pBuffer);
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, pBuffer);

		}
		else
		{
			pBuffer = pBuffer_Get;
		}
		//CTBSCommon::ConvertUTF8ToANSI(pBuffer_Get, pBuffer);
		//����<>....<>
		if (regex_match(pBuffer, regParser1))
		{
			//ȥ���������˵Ŀո���Ʊ�λ
			tbs_test_space_delete(pBuffer);
			iResult = tbs_test_parse_keytype(pBuffer, iLineNum, pCase);
			delete[] pBuffer;
			if (iResult != SCRIPT_EOF)
			{
				this->pParseError->iLineNum = iLineNum;
				return iResult;
			}
		}
		//����<>
		else if (regex_match(pBuffer, regParser2))
		{
			//ȥ���������˵Ŀո���Ʊ�λ
			tbs_test_space_delete(pBuffer);
			//��������
			iResult = tbs_test_parse_single_tab(pBuffer);
			delete[] pBuffer;
			if (iResult != SCRIPT_EOF||iResult==SCRIPT_FINISH)
			{
				this->pParseError->iLineNum = iLineNum;
				return iResult;
			}
		}
		//����ע��
		else if (regex_match(pBuffer, regAnnotation) || regex_match(pBuffer, regSpace))
		{
			//
			delete[] pBuffer;
		}
		//�������
		else
		{
			//�﷨����,���в���ƥ��
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, pBuffer);
			if (pBuffer == NULL)
			{
				//�����ǿ���
				delete[] pBuffer;
			}
			else
			{
				//�﷨����,���в���ƥ��
				delete[] pBuffer;
				this->pParseError->iLineNum = iLineNum;
				return SCRIPT_LINE;
			}
		}
	}
	if (iConditionFlag != 0)
	{
		//�﷨����
		this->pParseError->iLineNum = iLineNum;
		return SCRIPT_COND_NOTEND;
	}
	if (iLoopFlag != 0)
	{
		//�﷨����
		this->pParseError->iLineNum = iLineNum;
		return SCRIPT_LOOP_NOTEND;
	}
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "�ű�������ɣ�OK");
	return SCRIPT_EOF;
}

//�ַ���ͬ���͵����
INT CTBSScriptParse::tbs_test_parse_keytype(CHAR *pLine, INT iLineNum,INT *pCase)
{
	CHAR* pAkey[] = { "IR","CMD","DELAY","MANUAL","SNAPSHOT","MSG" };
	CHAR* pDkey[] = { "CASENAME","DESCRIBE","NAME","CASENUMBER","BEGIN" };
	CHAR* pEkey[] = { "COM","HDMI","TIMES","SIM","PSAVE" };
	INT iResult = 0;
	switch (pLine[1])
	{
	case 'A':
		iResult = tbs_test_parse_action(pLine, iLineNum, pAkey, 6, "</A>", (TBS_SCRIPT_OPREATION_TYPE_e)0);
		break;
	case 'C':
		iResult = tbs_test_prase_c(pLine, iLineNum);
		break;
	case 'D':
		iResult = tbs_test_parse_action(pLine, iLineNum, pDkey, 5, "</D>", (TBS_SCRIPT_OPREATION_TYPE_e)2, pCase);
		break;
	case 'E':
		iResult = tbs_test_parse_action(pLine, iLineNum, pEkey, 5, "</E>", (TBS_SCRIPT_OPREATION_TYPE_e)3);
		break;
	case 'F':
		//δʵ��
		break;
	case 'I':
		//δʵ��
		break;
	}
	return iResult;
}

//����ÿһ���������
INT CTBSScriptParse::tbs_test_parse_action(CHAR *pLine, INT iLineNum, CHAR **ppKey, INT ikeylength, CHAR *pEnd, TBS_SCRIPT_OPREATION_TYPE_e OPREATION_TYPE_e, INT *pCase)
{
	INT i = 0;
	INT ilength = 0;
	CHAR *ckeyword = new CHAR[20];
	TBS_SCRIPT_ACTION_TYPE_t *pAction_t = new TBS_SCRIPT_ACTION_TYPE_t;
	
	memset(pAction_t, 0, sizeof(TBS_SCRIPT_ACTION_TYPE_t));
	memset(ckeyword, 0, 20);
	//�����ؼ���ckeyword��
	pLine += 3;
	while (pLine[i] != '>')
	{
		i++;
	}
	memmove(ckeyword, pLine, i);
	//�����������ݡ�
	pLine += i;
	pLine++;
	i = 0;
	while (i <= ikeylength - 1)
	{
		if (0 == strcmp(ckeyword, ppKey[i]))
		{
			ilength = strlen(pLine);
			if (0 == strcmp(&pLine[ilength - 4], pEnd))
			{
				pLine[ilength - 4] = '\0';
				//ȥ��˫����
				if (*pLine == '"'&&pLine[ilength - 5] == '"')
				{
					pLine[ilength - 5] = '\0';
					pLine++;
				}
				pAction_t->pLoop_t.iLoop = iLoopFlag;
				pAction_t->pLoop_t.iLoopNum = 0;
				TBS_LOOP_INF_t *pHeadTmp = pHeadLoop;
				if (iLoopFlag != 0)
				{
					while (pHeadTmp->iLoop != iLoopFlag)
					{
						pHeadTmp = pHeadTmp->pNext;
					}
					pAction_t->pLoop_t.iLoopNum = pHeadTmp->iLoopNum;
				}

				pAction_t->cCondtion = iConditionFlag;
				pAction_t->strCmdType = ckeyword;
				pAction_t->strCmd = pLine;
				pAction_t->sOperationType_e = OPREATION_TYPE_e;
				pAction_t->pNext = NULL;
				if (0 == strcmp(ckeyword ,"CASENUMBER") && pCase!=NULL)
				{
					*pCase = atoi(pLine);
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, *pCase);
				}
				if (0 == strcmp(ckeyword,"CASENAME"))
				{
					strCaseName = pLine;
				}
				tbs_test_add_to_actionlist(pAction_t);
				break;
			}
			else
			{
				//�﷨���󣬱�ǩ��ʼ�ͽ�����ƥ��
				return SCRIPT_NOT_MATCH;

			}
		}
		else
		{
			if (i == ikeylength - 1)
			{
				//�﷨����,����Ĺؼ���
				return SCRIPT_KEYWORD;
			}
		}
		i++;
	}
	delete[] ckeyword;
	//CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
	return SCRIPT_EOF;
}

//����ѭ���������Ŀ�ʼ
INT CTBSScriptParse::tbs_test_prase_c(CHAR *pLine, INT iLineNum)
{
	INT i = 0;
	INT ilength = 0;
	CHAR *ckeyword = new CHAR[20];
	TBS_LOOP_INF_t *pNode = new TBS_LOOP_INF_t;
	memset(pNode, 0, sizeof(TBS_LOOP_INF_t));
	memset(ckeyword, 0, 20);
	pLine += 3;
	while (pLine[i] != '>')
	{
		i++;
	}
	memmove(ckeyword, pLine, i);
	pLine += i;
	pLine++;
	i = 0;
	if (0 == strcmp(ckeyword, "IBEGIN"))
	{
		if (0 == iLoopFlag)
		{
			iConditionFlag = 1;
		}
		else
		{
			iConditionFlag = iConditionFlag * 10 + 1;
		}
	}
	else if (0 == strcmp(ckeyword, "CBEGIN"))
	{
		if (0 == iLoopFlag)
		{
			iLoopFlag = 1;
		}
		else
		{
			iLoopFlag = iLoopFlag * 10 + 1;
		}
		TBS_LOOP_INF_t *pHeadLooptmp = pHeadLoop;
		bool bcheck = true;
		while (bcheck)
		{
			while (pHeadLooptmp != NULL&&pHeadLooptmp->iLoop != iLoopFlag)
			{
				pHeadLooptmp = pHeadLooptmp->pNext;
			}
			if (pHeadLooptmp == NULL)
			{
				bcheck = false;
				break;
			}
			iLoopFlag += 1;
			//ÿһ��iLoopFlag��Ҫ����Ƿ�����ͬ��ѭ������
			pHeadLooptmp = pHeadLoop;
		}
	}
	else
	{
		//�﷨���󣬴���Ĺؼ���
		return SCRIPT_KEYWORD;
	}

	ilength = strlen(pLine);
	if (0 == strcmp(&pLine[ilength - 4], "</C>"))
	{
		pLine[ilength - 4] = '\0';
		//�ɹ��������ؼ��ֺ�����
		if (*pLine == '"'&&pLine[ilength - 5] == '"')
		{
			pLine[ilength - 5] = '\0';
			pLine++;
		}

		if (0 == strcmp(ckeyword, "CBEGIN"))
		{
			regex regNum("([0-9]+)");
			if (regex_match(pLine, regNum))
			{
				INT i = atoi(pLine);
				pNode->iLoop = iLoopFlag;
				pNode->iLoopNum = i;
				pNode->pNext = NULL;
				tbs_test_add_to_looplist(pNode);
				//add addtional action
				TBS_SCRIPT_ACTION_TYPE_t *pAction_t = new TBS_SCRIPT_ACTION_TYPE_t;
				pAction_t->pLoop_t.iLoop = iLoopFlag;
				pAction_t->pLoop_t.iLoopNum = 0;
				TBS_LOOP_INF_t *pHeadTmp = pHeadLoop;
				if (iLoopFlag != 0)
				{
					while (pHeadTmp->iLoop != iLoopFlag)
					{
						pHeadTmp = pHeadTmp->pNext;
					}
					pAction_t->pLoop_t.iLoopNum = pHeadTmp->iLoopNum;
				}

				pAction_t->cCondtion = iConditionFlag;
				pAction_t->strCmdType = "CBEGIN";
				pAction_t->strCmd = "CBEGIN";
				pAction_t->sOperationType_e = TBS_SCRIPT_OPREATION_TYPE_e(6);
				pAction_t->pNext = NULL;
				tbs_test_add_to_actionlist(pAction_t);
			}
			else
			{
				//�﷨����ѭ������Ϊ�����ֱ��ʽ
				return SCRIPT_NOT_NUM;
			}
		}
		else
		{
			//TODO:	�˷�֧��IBEGIN,�˴���ʱû��ʵ�֡�
		}
	}
	else
	{
		return SCRIPT_NOT_MATCH;
	}

	delete[] ckeyword;
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
	return SCRIPT_EOF;
}

//�����Ӧ��Action��Loop
void CTBSScriptParse::tbs_test_add_to_actionlist(TBS_SCRIPT_ACTION_TYPE_t* pActionType_t)
{
	if (pHeadAction == NULL)
	{
		pHeadAction = pActionType_t;
	}
	else
	{
		TBS_SCRIPT_ACTION_TYPE_t *pHeadTmp = pHeadAction;
		while (pHeadTmp->pNext != NULL)
		{
			pHeadTmp = pHeadTmp->pNext;
		}
		pHeadTmp->pNext = pActionType_t;
	}
}

void CTBSScriptParse::tbs_test_add_to_looplist(TBS_LOOP_INF_t* pLoop_t)
{
	if (pHeadLoop == NULL)
	{
		pHeadLoop = pLoop_t;
	}
	else
	{
		TBS_LOOP_INF_t *pHeadTmp = pHeadLoop;
		while (pHeadTmp->pNext != NULL)
		{
			pHeadTmp = pHeadTmp->pNext;
		}
		pHeadTmp->pNext = pLoop_t;
	}
}

//��ȡ��ǰ�Ķ���
TBS_SCRIPT_ACTION_TYPE_t* CTBSScriptParse::tbs_test_get_script_value(INT *pLoopFlag1, INT *pLoopFlag2)
{
	INT iLoopFlag1 = *pLoopFlag1;
	INT iLoopFlag2 = *pLoopFlag2;
	TBS_LOOP_INF_t* pHeadLooptmp = pHeadLoop;
	TBS_SCRIPT_ACTION_TYPE_t* pHeadActiontmp = pHeadAction;
	while (pHeadActiontmp != NULL)
	{
		if (pHeadActiontmp->pLoop_t.iLoop == 0)
		{
			if (pListhead1 != NULL&&pListhead1->pLoop_t.iLoopNum > 0)
			{
				pHeadActiontmp = pListhead1;
				continue;
			}
			
			pHeadAction = pHeadActiontmp->pNext;
			return pHeadActiontmp;
		}
		else if (pHeadActiontmp->pLoop_t.iLoop >= 1 && pHeadActiontmp->pLoop_t.iLoop <= 9)
		{
			iLoopFlag1 = iLoopFlag2;
			iLoopFlag2 = pHeadActiontmp->pLoop_t.iLoop;
			*pLoopFlag2 = iLoopFlag2;
			*pLoopFlag1 = iLoopFlag1;
			//����ѭ��
			if (iLoopFlag1 < iLoopFlag2)
			{
				pListhead1 = pHeadActiontmp;
			}
			//�˳�ѭ��
			if (iLoopFlag1 > iLoopFlag2)
			{
				if (pListhead2 != NULL&&pListhead2->pLoop_t.iLoopNum > 0)
				{
					pHeadActiontmp = pListhead2;
					continue;
				}
				if (pListhead2 != NULL&&pListhead2->pLoop_t.iLoopNum == 0)
				{
					while (pListhead2->pLoop_t.iLoop != pHeadLooptmp->iLoop&&pHeadLooptmp != NULL)
					{
						pHeadLooptmp = pHeadLooptmp->pNext;
					}
					pListhead2->pLoop_t.iLoopNum = pHeadLooptmp->iLoopNum;
					pHeadLooptmp = pHeadLoop;
				}
			}
			--pHeadActiontmp->pLoop_t.iLoopNum;			
			pHeadAction = pHeadActiontmp->pNext;
			return pHeadActiontmp;
		}
		else
		{
			iLoopFlag1 = iLoopFlag2;
			iLoopFlag2 = pHeadActiontmp->pLoop_t.iLoop;
			*pLoopFlag2 = iLoopFlag2;
			*pLoopFlag1 = iLoopFlag1;
			if (iLoopFlag1 < iLoopFlag2)
			{
				pListhead2 = pHeadActiontmp;
			}
			--pHeadActiontmp->pLoop_t.iLoopNum;
			pHeadAction = pHeadActiontmp->pNext;
			return pHeadActiontmp;
		}
	}
	return NULL;
}

//�������ܣ�
//		�ű�ִ���̵߳���ں���
//	������
//		lpParam��	ͬ���ű�ִ�кͽű������Ľṹ��
//	����ֵ��
//		0��	�ɹ���
UINT CTBSScriptParse::tbs_test_script_execute(LPVOID lpParam)
{
	TBS_PRESENT_THREAD_t *m_PresentThread = &CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent];
	tbs_thread_script_t *m_Parameter = (tbs_thread_script_t*)lpParam;

	INT				iScriptNum		= m_Parameter->iScriptNum;
	INT				iItemNum		= 0;
	INT				iSuccess		= 0;
	INT				iFail			= 0;
	CTime			m_Time;
	string			strProjectName	= TOSTRING(m_PresentThread->cstrProjectName);
	string			strIRFile		= TOSTRING(m_PresentThread->cstrIRFile);
	CString			cstrTime;
	CTimeSpan		m_CostTime;
	CTBSDataBase	m_DataBase(DATAPATH);
	CSingleLock		m_ParseAndRun(&(m_Parameter->m_Mutex));
	CTBSScriptParse	*m_pScriptRun	= NULL;
	CTBSMainDlg		*m_MainDlg		= m_PresentThread->m_pMainDlg;
	CEvent			*m_Event		= &m_PresentThread->m_Event;
	CString			cstrRedRat		= m_PresentThread->cstrRedRat;
	INT				iNameNum		= m_PresentThread->iNum;
	INT				iThreadID		= m_PresentThread->m_TestThread->m_nThreadID;
	INT				iIndex			= CTBSCommon::iTBSPresent;
	if (m_PresentThread->cstrProjectName == L"")
		return -1;
	CTBSLog::tbs_log_info(__FILE__,__LINE__,__FUNCTION__, m_PresentThread->cstrProjectName);
	

	CHAR *pIRFile = new CHAR[strIRFile.length() + 1];
	memmove(pIRFile, strIRFile.c_str(), strIRFile.length()+1);
	
	while (iItemNum < iScriptNum)
	{
		//��ʼһ���ű���ִ��
		//��������Ϊ�����нű��������ΪFAIL������£���������иýű�����������ʱִ�нű�Ӧ�������Ľű���
		//��ֹm_pScriptRun���ظ���ֵ�����½ű�ִ�д��ң�ʵ�����Ǳ���m_Parameter->m_Script��ֵ
		//�������ɱ�����������������
		/*if (!m_ParseAndRun.IsLocked())
		{
			m_ParseAndRun.Lock();
		}*/

		if (m_Parameter->m_Script != NULL)
		{
			m_pScriptRun = m_Parameter->m_Script;
			m_Parameter->m_Script = NULL;
		}
		if (m_pScriptRun == NULL)
		{
			m_MainDlg->m_TestList.SetItemText(iItemNum, 1, L"ִ�����");
			break;
		}
		IFNULL(m_MainDlg);
		INT			iRow;
		CString		cstrScriptName	= m_MainDlg->m_TestList.GetItemText(iItemNum, 0);
		string		strScriptName(CW2A(cstrScriptName.GetString()));
		CString		cstrCaseName(TOCSTRING(m_pScriptRun->strCaseName));
		tbs_test_detail_msg_t m_TestDeail;
		m_TestDeail.cstrScriptName = cstrScriptName;
		m_TestDeail.cstrCaseNmae = cstrCaseName;

		string		strSqlCheck		= "select *from TestDetail where ProjectName='" + strProjectName + "' and ScriptName='" + strScriptName + "'and CaseName='"+ m_pScriptRun->strCaseName +"' and ParseResult='Success'";
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"��ʼִ�нű���"+ cstrCaseName);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, "select *from TestDetail where ProjectName='" + strProjectName + "' and ScriptName='" + strScriptName + "'and CaseName='" + m_pScriptRun->strCaseName + "' and ParseResult='Success'");
		m_DataBase.tbs_db_data_query(strSqlCheck, NULL, &iRow, NULL);
		if (iRow == 0)
		{
			if (m_pScriptRun->iEnd == 1)
			{
				iItemNum--;
			}
			if (m_pScriptRun->iEnd == 0)
			{
				m_MainDlg->m_TestList.SetItemText(iItemNum, 1, L"ִ�����");
			}
			m_MainDlg->m_TestList.SetItemState(iItemNum, 0, LVIF_STATE);
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iRow);
			iItemNum++;
			continue;
		}
	/*	m_ParseAndRun.Unlock();*/
		m_Parameter->m_Event.SetEvent();

		INT		iLoopFlag1	= 0;
		INT		iLoopFlag2	= 0;
		INT		iExpFlag	= 0;
		INT		iFailFLag	= 0;
		INT		iDelayTime	= 0;
		CTime	m_TimeOccur;
		string	strAction;
		string	strExpect;
		string	strActual;
		TBS_SCRIPT_EXPECTION_t		m_Expection;
		TBS_SCRIPT_ACTION_TYPE_t	*m_Action[2] = { NULL,NULL };
		
		CTBSDetailDlg::m_ExpectionNow = &m_Expection;
		m_pScriptRun->m_ScriptTime = CTime::GetCurrentTime();
		m_MainDlg->m_TestList.SetItemText(iItemNum, 1, cstrCaseName +L"����ִ����");
		m_MainDlg->m_TestList.SetItemState(iItemNum, LVIS_DROPHILITED, LVIF_STATE);
	

		//��ʼִ�нű�
		//Ϊ��һ���ڵ㸳ֵ��
		if (m_Action[0] == NULL&&m_pScriptRun->pHeadAction != NULL)
		{
			m_Action[0] = m_pScriptRun->tbs_test_get_script_value(&iLoopFlag1, &iLoopFlag2);
		}
		if (m_Action[0]->sOperationType_e == 3)
		{
			AfxMessageBox(L"�ű������ܵ�һ��������<E>��ʼ����");
			break;
		}

		while (m_pScriptRun->pHeadAction != NULL || m_Action[0] != NULL)
		{
			//��ȡAction�ڵ�
			if (m_Action[1] == NULL&&m_pScriptRun->pHeadAction != NULL)
			{

				m_Action[1] = m_pScriptRun->tbs_test_get_script_value(&iLoopFlag1, &iLoopFlag2);
				if (m_Action[1]->sOperationType_e == 3)
				{
					if (m_Action[1]->strCmdType == "COM")
					{
						*(m_Expection.strExpType + m_Expection.iNum)= "COM";
						*(m_Expection.strCmd + m_Expection.iNum)	= m_Action[1]->strCmd;
						*(m_Expection.strTime + m_Expection.iNum)	= "-1";
						m_Expection.iNum++;
					}
					else if (m_Action[1]->strCmdType == "TIMES")
					{
						*(m_Expection.strTime + m_Expection.iNum - 1) = m_Action[1]->strCmd;
						INT iNum = m_Expection.iNum - 1;
						while (iNum > 0)
						{
							if (*(m_Expection.strTime + iNum - 1) == "-1")
							{
								*(m_Expection.strTime + iNum - 1) = m_Action[1]->strCmd;
								iNum--;
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						//TODO:	add for additional expectation
					}

					iExpFlag	= 1;
					m_Action[1] = NULL;
					continue;
				}
			}

			//Now m_Action[0] is struct is the executing action. the m_Expection is the expection if have.
			//the m_Action[1] is next one,when m_Action[0] is executed,we need to get the next action for a new value.
			
			
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_Action[0]->strCmd.c_str());
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iExpFlag);
			
			CSingleLock m_SingkeLock(CTBSPredictionDlg::m_Mutex);
			//�жϵ�ǰִ����û�б�ǿ�ƽ���
			m_SingkeLock.Lock();
			for (INT i = 0; i < CTBSCommon::iTBSNum; i++)
			{
				if(CTBSCommon::m_PresentThread[i].m_TestThread != NULL && iThreadID ==CTBSCommon::m_PresentThread[i].m_TestThread->m_nThreadID)
				{
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
					break;
				}
				if (i == CTBSCommon::iTBSNum - 1)
				{
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
					return 0;
				}
			}
			m_SingkeLock.Unlock();
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iExpFlag);
			//��ʼ��ǰ�ڵ�
			//����Ҫ���н����֤��ʱ��
		

			if (iExpFlag == 1)
			{
				CSingleLock m_Lock(m_pScriptRun->m_MutexDetail);

				if (m_Action[0]->sOperationType_e != TBS_SCRIPT_OPREATION_TYPE_e(6))
				{
					m_Lock.Lock();
					CTBSDetailDlg::m_ActionNow = m_Action[0];
					CTBSDetailDlg::m_ExpectionNow->bEffect = TRUE;
					m_Lock.Unlock();
					SendMessage(::FindWindow(NULL, L"script error"), WN_DETAIL_ACTON_CHANGE, 0, (LPARAM)&m_TestDeail);
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_Action[0]->sOperationType_e);
				}				
				if (iExpFlag == 1)
				{
					INT		iTime;
					CHAR	cNum[10];
					CString cstrTitle;
					
					_itoa_s(iNameNum, cNum, 10);
					cstrTitle = L"MSComm" + CString(cNum);
					m_Expection.pEvent = new CEvent;
					SendMessage(::FindWindow(NULL, cstrTitle), WM_EXPECTATION_CHECK, 0, (LPARAM)&m_Expection);

					//���ж���ڴ������ʱ�򣬶�ȡ������ʱʱ�䡣
					iDelayTime = 0;
					for (INT i = 0; i < m_Expection.iNum; i++)
					{
						iTime = stoi(*(m_Expection.strTime + i));
						if (iDelayTime < iTime)
						{
							iDelayTime = iTime;
						}
					}
				}
				
				//Type A;
				if (m_Action[0]->sOperationType_e == 0)
				{
					if (m_Action[0]->strCmdType == "IR")
					{
						CHAR *pCmd = new CHAR[m_Action[0]->strCmd.length() + 1];
						memmove(pCmd, m_Action[0]->strCmd.c_str(), m_Action[0]->strCmd.length()+1);
						CTBSRedRat::tbs_ir_send_key(cstrRedRat, pIRFile, pCmd);
					}
					else if (m_Action[0]->strCmdType == "DELAY")
					{
						Sleep(stoi(m_Action[0]->strCmd));
					}
					else if (m_Action[0]->strCmdType == "CMD")
					{
						INT		n;
						CHAR	cNum[10];
						CString	cstrTitle;
						const CHAR	*pCMD;

						_itoa_s(iNameNum, cNum, 10);
						cstrTitle = L"MSComm" + CString(cNum);
						::SendMessage(::FindWindow(NULL, cstrTitle), WM_KEYBOARD_MSG, 1, 0x03);

						m_Action[0]->strCmd += "\n";
						pCMD = m_Action[0]->strCmd.c_str();
						n = m_Action[0]->strCmd.length();
						::SendMessage(::FindWindow(NULL, cstrTitle), WM_KEYBOARD_MSG, n + 1, (LPARAM)pCMD);
					}
					else if (m_Action[0]->strCmdType == "MANUAL")
					{
						INT		iTabIndex = -1;
						INT		ii = 0;
						WCHAR	label[64];
						TC_ITEM	m_Tci;
						
						m_Tci.mask = TCIF_TEXT;
						m_Tci.pszText = label;
						m_Tci.cchTextMax = 63;
						
						for (ii; ii < CTBSCommon::iTBSNum; ii++)
						{
							CTBSPredictionDlg::m_TabControl->GetItem(ii, &m_Tci);
							if (CString(label) == m_PresentThread->cstrProjectName)
							{
								iTabIndex = ii;
								break;
							}	
						}
						CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iTabIndex);
						if (iTabIndex != -1)
						{
							if (iTabIndex == 0)
								iTabIndex = 1;
							else
							{
								iTabIndex = 1 << iTabIndex;
							}
							CTBSPredictionDlg::m_TabControl->uiFlag |= iTabIndex;
							CTBSPredictionDlg::m_TabControl->Invalidate();
							INT iTabNum = CTBSPredictionDlg::m_TabControl->GetCurSel();
							CHAR cCmd[MAX_PATH];
							strcpy_s(cCmd, m_Action[0]->strCmd.c_str());
							INT iLength = MultiByteToWideChar(CP_ACP, 0, cCmd, -1, NULL, 0);
							WCHAR *pBuffer = new WCHAR[iLength];
							MultiByteToWideChar(CP_ACP, 0, cCmd, -1, pBuffer, iLength);
							if (iTabNum != ii)
							{
								m_PresentThread->bLock = true;
								m_PresentThread->m_ManualEvent.Lock();
							}
							iTabIndex = 1 << CTBSCommon::iTBSPresent;
							if (IDCANCEL == ::MessageBox(NULL, pBuffer, L"MANUAL", MB_OKCANCEL))
							{
								iFail++;
								iFailFLag = 1;
								strAction = m_Action[0]->strCmd;
								m_TimeOccur = CTime::GetCurrentTime();
								m_pScriptRun->pHeadAction = NULL;
								m_Action[0] = NULL;
								strExpect += *(m_Expection.strCmd);
								strActual += "�޷���������";
								CTBSPredictionDlg::m_TabControl->uiFlag &= (~iTabIndex);
								CTBSPredictionDlg::m_TabControl->Invalidate();
								break;
							}
							delete[] pBuffer;
							CTBSPredictionDlg::m_TabControl->uiFlag &= (~iTabIndex);
							CTBSPredictionDlg::m_TabControl->Invalidate();
						}				
					}
					else
					{
						//TODO:add more executing
					}
				}
				//Type D
				else if (m_Action[0]->sOperationType_e == 2)
				{
					if (m_Action[0]->strCmdType == "DESCRIBE")
					{
						string strSqlDescribe = "Update TestDetail set Describe='" + m_Action[0]->strCmd + "' where ProjectName='" + strProjectName + "' and ScriptName='" + strScriptName + "' and CaseName='" + m_pScriptRun->strCaseName + "'";
						m_DataBase.tbs_db_sql_exec(strSqlDescribe);
					}
					else if (m_Action[0]->strCmdType == "NAME")
					{
						string strSqlCasename = "Update TestDetail set Name='" + m_Action[0]->strCmd + "' where ProjectName='"+strProjectName+"' and ScriptName='" + strScriptName + "' and CaseName='Describe'";
						m_DataBase.tbs_db_sql_exec(strSqlCasename);
						CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, strSqlCasename);
					}
					else if(m_Action[0]->strCmdType == "FINISH")
					{
						break;
						//TODO:	Add additional excute for more type;
					}
					else
					{

					}
				}
				else
				{

				}
				
				m_SingkeLock.Lock();
				for (INT i = 0; i < CTBSCommon::iTBSNum; i++)
				{
					if (CTBSCommon::m_PresentThread[i].m_TestThread != NULL && iThreadID == CTBSCommon::m_PresentThread[i].m_TestThread->m_nThreadID)
					{
						CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
						iIndex = i;
						break;
					}
					if (i == CTBSCommon::iTBSNum - 1)
					{
						CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
						return 0;
					}
				}
				if (CTBSCommon::m_PresentThread[iIndex].m_ComThread == NULL)
				{
					m_SingkeLock.Unlock();
					Sleep(iDelayTime);
					m_Expection.iResult[5] = -2;
				}
				else
				{
					m_SingkeLock.Unlock();
					m_Expection.pEvent->Lock();
				}

				if (m_Expection.iResult[5] != 0)
				{
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
					//m_MainDlg->m_TestList.SetItemText(iItemNum, 1, SCRIPT_TEST_FAIL);
					
					iFail++;
					iFailFLag	= 1;
					strAction	= m_Action[0]->strCmd;
					//TODO:	�޸�iCheckResultFlag��ʹ���ʶʧ�ܵ�ԭ��
					m_TimeOccur = CTime::GetCurrentTime();
					m_pScriptRun->pHeadAction = NULL;
					m_Action[0] = NULL;

					for (INT i = 0; i < m_Expection.iNum; i++)
					{
						strExpect += *(m_Expection.strCmd + i);
						if (m_Expection.iResult[5] == -2)
						{
							strActual += "Not connect STB";
							break;
						}
						if (*(m_Expection.iResult + i) == -1)
						{
							strActual += "DELAY";
						}
						if (*(m_Expection.iResult + i) == 0)
						{
							strActual += "SUCCESS";
							
						}
						if (*(m_Expection.iResult + i) == -2)
						{
							strActual += "Not STB";

						}
						if (i == m_Expection.iNum - 1)
						{
							break;
						}
						strActual += ",";
						strExpect += ",";
					}
					break;
				}
				m_Action[0]			= m_Action[1];
				m_Action[1]			= NULL;
				iExpFlag			= 0;
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_Action[0]->strCmd + "��ִ����ɣ�");
				memset(&m_Expection, 0, sizeof(TBS_SCRIPT_EXPECTION_t));

				
			}
			else
			{
				CSingleLock m_Lock(m_pScriptRun->m_MutexDetail);
				if (m_Action[0]->sOperationType_e != TBS_SCRIPT_OPREATION_TYPE_e(6))
				{
					m_Lock.Lock();
					CTBSDetailDlg::m_ActionNow = m_Action[0];
					CTBSDetailDlg::m_ExpectionNow->bEffect = FALSE;
					m_Lock.Unlock();
					SendMessage(::FindWindow(NULL, L"script error"), WN_DETAIL_ACTON_CHANGE, 0, (LPARAM)&m_TestDeail);
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_Action[0]->sOperationType_e);
				}				
				//A
				if (m_Action[0]->sOperationType_e == 0)
				{
					if (m_Action[0]->strCmdType == "IR")
					{
						CHAR *pCmd = new CHAR[m_Action[0]->strCmd.length() + 1];
						memmove(pCmd, m_Action[0]->strCmd.c_str(), m_Action[0]->strCmd.length()+1);
						CTBSRedRat::tbs_ir_send_key(cstrRedRat, pIRFile, pCmd);
					}
					else if (m_Action[0]->strCmdType == "DELAY")
					{
						Sleep(stoi(m_Action[0]->strCmd));
					}
					else if (m_Action[0]->strCmdType == "CMD")
					{
						INT		n;
						CHAR	cNum[10];
						CString	cstrTitle;
						const CHAR	*pCMD;
						
						_itoa_s(iNameNum, cNum, 10);
						cstrTitle = L"MSComm" + CString(cNum);
						::SendMessage(::FindWindow(NULL, cstrTitle), WM_KEYBOARD_MSG, 1, 0x03);

						m_Action[0]->strCmd += "\n";
						pCMD = m_Action[0]->strCmd.c_str();
						n = m_Action[0]->strCmd.length();
						::SendMessage(::FindWindow(NULL, cstrTitle), WM_KEYBOARD_MSG, n + 1, (LPARAM)pCMD);
					}
					else if (m_Action[0]->strCmdType == "MANUAL")
					{
						INT		iTabIndex = -1;
						INT		ii = 0;
						WCHAR	label[64];
						TC_ITEM	m_Tci;

						m_Tci.mask = TCIF_TEXT;
						m_Tci.pszText = label;
						m_Tci.cchTextMax = 63;

						for (ii; ii < CTBSCommon::iTBSNum; ii++)
						{
							CTBSPredictionDlg::m_TabControl->GetItem(ii, &m_Tci);
							if (CString(label) == m_PresentThread->cstrProjectName)
							{
								iTabIndex = ii;
								break;
							}
						}
						
						if (iTabIndex != -1)
						{
							if (iTabIndex == 0)
								iTabIndex = 1;
							else
							{
								iTabIndex = 1 << iTabIndex;
							}
							CTBSPredictionDlg::m_TabControl->uiFlag |= iTabIndex;
							CTBSPredictionDlg::m_TabControl->Invalidate();
							INT iTabNum = CTBSPredictionDlg::m_TabControl->GetCurSel();
							CHAR cCmd[MAX_PATH];
							strcpy_s(cCmd, m_Action[0]->strCmd.c_str());
							INT iLength = MultiByteToWideChar(CP_ACP, 0, cCmd, -1, NULL, 0);
							WCHAR *pBuffer = new WCHAR[iLength];
							iLength=MultiByteToWideChar(CP_ACP, 0, cCmd, -1, pBuffer, iLength);
							if (iTabNum != ii)
							{
								m_PresentThread->bLock = true;
								m_PresentThread->m_ManualEvent.Lock();
							}
							
							iTabIndex = 1 << CTBSCommon::iTBSPresent;
							if (IDCANCEL == ::MessageBox(NULL, pBuffer, L"MANUAL", MB_OKCANCEL))
							{
								iFail++;
								iFailFLag = 1;
								strAction = m_Action[0]->strCmd;
								m_TimeOccur = CTime::GetCurrentTime();
								m_pScriptRun->pHeadAction = NULL;
								m_Action[0] = NULL;
								strExpect += *(m_Expection.strCmd);
								strActual += "�޷���������";
								CTBSPredictionDlg::m_TabControl->uiFlag &= (~(UINT(iTabIndex)));
								CTBSPredictionDlg::m_TabControl->Invalidate();
								break;
							}
							delete[] pBuffer;
							CTBSPredictionDlg::m_TabControl->uiFlag &= (~(UINT(iTabIndex)));
							CTBSPredictionDlg::m_TabControl->Invalidate();
						}
					}
					else 
					{
						//TODO:add more executing
					}
				}
				//D
				else if (m_Action[0]->sOperationType_e == 2)
				{
					//�������ж�
					if (m_Action[0]->strCmdType == "DESCRIBE")
					{
						string strSqlDescribe = "Update TestDetail set Describe='" + m_Action[0]->strCmd + "' where ProjectName='" + strProjectName + "' and ScriptName='" + strScriptName + "' and CaseName='" + m_pScriptRun->strCaseName + "'";
						m_DataBase.tbs_db_sql_exec(strSqlDescribe);
					}
					else if (m_Action[0]->strCmdType == "NAME")
					{
						string strSqlCasename = "Update TestDetail set Name='" + m_Action[0]->strCmd + "' where ProjectName='" + strProjectName + "' and ScriptName='" + strScriptName + "' and CaseName='Describe'";
						m_DataBase.tbs_db_sql_exec(strSqlCasename);
						CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, strSqlCasename);
					}
					else if (m_Action[0]->strCmdType == "BEGIN")
					{
						string strSqlName = "Update TestDetail set CaseNumber='" + m_Action[0]->strCmd + "' where ScriptName='" + strScriptName + "' and ProjectName='" + strProjectName + "' and CaseName='" + m_pScriptRun->strCaseName + "'";
						m_DataBase.tbs_db_sql_exec(strSqlName);
					}
					else if (m_Action[0]->strCmdType == "FINISH")
					{
						break;
					}
					else
					{
						//TODO:add more executing
					}
				}
				else
				{
				}
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_Action[0]->strCmd+"��ִ����ɣ�");
				m_Action[0] = m_Action[1];
				m_Action[1] = NULL;
			}
		}

		//һ��caseִ�����
		if (m_pScriptRun->iEnd == 0)
		{
			m_MainDlg->m_TestList.SetItemText(iItemNum, 1, L"ִ�����");
		}
		m_MainDlg->m_TestList.SetItemState(iItemNum, 0, LVIF_STATE);
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, cstrCaseName+L"��ִ����ɣ�д�뵱ǰ�ű���ִ�н�������ݿ�");
		
		if (iFailFLag == 0)
		{
			//m_MainDlg->m_TestList.SetItemText(iItemNum, 1, SCRIPT_TEST_SUCCESS);
		}
		////д���case��ִ�н��,��Ҫ�������Խ����ʧ�ܲ��������Ժ�ʱ
		//����ʱ��
		m_Time = CTime::GetCurrentTime();
		CString cstrTimeEnd = m_Time.Format(_T("%Y-%m-%d %H:%M:%S"));
		string strTimeEnd(CW2A(cstrTimeEnd.GetString()));
		//��ʼʱ��
		CString cstrTimeBegin = m_pScriptRun->m_ScriptTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		string strTimeBegin(CW2A(cstrTimeBegin.GetString()));
		//��ʱ
		m_CostTime = m_Time - m_pScriptRun->m_ScriptTime;
		cstrTime = m_CostTime.Format(_T("%D:%H:%M:%S"));
		string strScriptTime(CW2A(cstrTime.GetString()));
		//ʱ������д�����ݿ�
		string strSqlUpdate = "Update TestDetail set TimeBegin='" + strTimeBegin + "',TimeEnd='" + strTimeEnd + "',Time='" + strScriptTime + "'where ScriptName='" + strScriptName + "' and ProjectName='" + strProjectName + "' and CaseName='" + m_pScriptRun->strCaseName + "'";
		m_DataBase.tbs_db_sql_exec(strSqlUpdate);
		//���Խ��
		string strSqlResult;
		if (iFailFLag == 0)
		{

			strSqlResult = "Update TestDetail set Result='OK' where ScriptName='" + strScriptName + "' and ProjectName='" + strProjectName + "' and CaseName='" + m_pScriptRun->strCaseName + "'";

		}
		else
		{
			CString cstrTimeOccur = m_TimeOccur.Format(_T("%Y-%m-%d %H:%M:%S"));
			string strTimeOccur(CW2A(cstrTimeOccur.GetString()));
			strSqlResult = "Update TestDetail set Result='FAIL',Action='" + strAction + "',Expect='" + strExpect + "',Actual='" + strActual + "',TimeOccur='" + strTimeOccur + "' where ScriptName='" + strScriptName + "' and ProjectName='" + strProjectName + "' and CaseName='" + m_pScriptRun->strCaseName + "'";
		}
		m_DataBase.tbs_db_sql_exec(strSqlResult);
		
		if (iFailFLag == 0)
		{
			SendMessage(::FindWindow(NULL, L"script error"), WN_DETAIL_ACTON_CHANGE, 1, (LPARAM)&m_TestDeail);
		}
		else
		{
			SendMessage(::FindWindow(NULL, L"script error"), WN_DETAIL_ACTON_CHANGE, 2, (LPARAM)&m_TestDeail);
		}

		iFailFLag = 0;
		if (m_pScriptRun->iEnd == 1)
		{
			iItemNum--;
		}
		m_pScriptRun = NULL;
		iItemNum++;
		iSuccess++;
	}
	
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"�ű�ִ����ɣ�����д��������ݵ����ݿ⡣");
	//Ϊ���ݿ���Ӵ˴β�����������
	INT arrMenuArr[2] = { IDM_TEST_REPORT_CREATE ,IDM_TEST_REPORT_SEND };
	CTBSPredictionDlg::tbs_menu_enable(arrMenuArr, 2);

	m_Time = CTime::GetCurrentTime();
	cstrTime = m_Time.Format(_T("%Y-%m-%d %H:%M:%S"));
	string strTime(CW2A(cstrTime.GetString()));
	char cSuccess[4];
	_itoa_s(iSuccess - iFail, cSuccess, 4);
	char cFail[4];
	_itoa_s(iFail, cFail, 4);
	string strSqlUpdate = "Update TestReport set TimeEnd='" + strTime + "',Success='" + cSuccess + "',Fail='" + cFail + "' where ProjectName='" + strProjectName + "'";
	m_DataBase.tbs_db_sql_exec(strSqlUpdate);
	if (iFail == 0)
	{
		string strSql = "Update TestReport set Result='OK' where ProjectName='" + strProjectName + "'";
		m_DataBase.tbs_db_sql_exec(strSql);
	}
	else
	{
		string strSql = "Update TestReport set Result='FAIL' where ProjectName='" + strProjectName + "'";
		m_DataBase.tbs_db_sql_exec(strSql);
	}
	//���㱾�β��Ժ�ʱ
	string strSelectTime = "select *from TestReport where ProjectName='" + strProjectName + "'";
	INT iRow = 0, iCloum = 0; 
	CHAR **ppGetTime;
	CString sFormat = _T("%d-%d-%d %d:%d:%d");
	TCHAR *charFormat = (TCHAR*)(LPCTSTR)sFormat;
	m_DataBase.tbs_db_data_query(strSelectTime, &ppGetTime, &iRow, &iCloum);
	if (iRow > 0)
	{
		INT iYear;
		INT iMonth;
		INT iDay;
		INT iHour;
		INT iMinute;
		INT iSecond;
		CString cstrTime(*(ppGetTime + iCloum + 10));

		swscanf_s(cstrTime, charFormat, &iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond);
		CTime m_TestTime(iYear, iMonth, iDay, iHour, iMinute, iSecond);
		m_CostTime	= m_Time - m_TestTime;
		cstrTime	= m_CostTime.Format(_T("%D:%H:%M:%S"));
		string strTimeCost(CW2A(cstrTime.GetString()));
		string strSqlTimeCost = "Update TestReport set Time='" + strTimeCost + "' where ProjectName='" + strProjectName + "'";
		m_DataBase.tbs_db_sql_exec(strSqlTimeCost);
		CSingleLock m_SingkeLock(CTBSPredictionDlg::m_Mutex);
		m_SingkeLock.Lock();
		for (INT i = 0; i < CTBSCommon::iTBSNum; i++)
		{
			if (CTBSCommon::m_PresentThread[i].m_TestThread != NULL && iThreadID == CTBSCommon::m_PresentThread[i].m_TestThread->m_nThreadID)
			{
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
				iIndex = i;
				break;
			}
			if (i == CTBSCommon::iTBSNum - 1)
			{
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}
		}
		CTBSCommon::m_PresentThread[iIndex].m_TestThread = NULL;
		m_SingkeLock.Unlock();
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"���β�����ɣ�");
	}
	else
	{
		AfxMessageBox(L"��ȡ���Կ�ʼʱ��ʧ�ܣ�");
	}
	return 0;
}

//�������ܣ�
//	ȥ�������ַ�����<ǰ��>��Ķ����ַ���
//	������
//		pBuffer��in\out��:	pBuffer��Ҫ�޸ĵ��ַ�����
//	����ֵ��
//		0��	�ɹ���
//		-1��	�������Ϊ�ա�
INT CTBSScriptParse::tbs_test_space_delete(CHAR *pBuffer)
{
	if (pBuffer == NULL)
		return -1;
	INT iLineLength = strlen(pBuffer);
	INT i = 0;
	iLineLength--;
	//ȥ����β�Ŀո�
	while (pBuffer[iLineLength] != '>')
	{
		iLineLength--;
	}
	pBuffer[iLineLength + 1] = '\0';
	//ȥ����ʼ�Ŀո�
	while (pBuffer[i] != '<')
	{
		i++;
	}
	if (i != 0)
	{
		memmove(pBuffer, &pBuffer[i], iLineLength - i + 2);
	}
	return 0;
}

//�������ܣ�
//	����<***>
//	������
//		pBuffer(in):	��ʽΪ<***>�������У�
//	����ֵ��
//		SCRIPT_EOF	��	�ɹ���
//		����				��	�μ����������
INT CTBSScriptParse::tbs_test_parse_single_tab(CHAR* pBuffer)
{
	if (0 == strcmp(pBuffer, "<C CEND>"))
	{
		if (this->iLoopFlag != 0)
		{
			iLoopFlag = iLoopFlag / 10;
			TBS_SCRIPT_ACTION_TYPE_t *pAction_t = new TBS_SCRIPT_ACTION_TYPE_t;
			pAction_t->pLoop_t.iLoop = iLoopFlag;
			pAction_t->pLoop_t.iLoopNum = 0;
			TBS_LOOP_INF_t *pHeadTmp = pHeadLoop;
			if (iLoopFlag != 0)
			{
				while (pHeadTmp->iLoop != iLoopFlag)
				{
					pHeadTmp = pHeadTmp->pNext;
				}
				pAction_t->pLoop_t.iLoopNum = pHeadTmp->iLoopNum;
			}

			pAction_t->cCondtion = iConditionFlag;
			pAction_t->strCmdType = "CEND";
			pAction_t->strCmd = "CEND";
			pAction_t->sOperationType_e = TBS_SCRIPT_OPREATION_TYPE_e(6);
			pAction_t->pNext = NULL;
			tbs_test_add_to_actionlist(pAction_t);
		}
		else
		{
			//�﷨���󣬶����ѭ������
			return SCRIPT_EXC_LOOP_END;
		}
	}
	else if (0 == strcmp(pBuffer, "<C IEND>"))
	{
		if (iConditionFlag != 0)
		{
			iConditionFlag = iConditionFlag / 10;
		}
		else
		{
			//�﷨���󣬶������������
			return SCRIPT_EXC_COND_END;
		}
	}
	else if (0 == strcmp(pBuffer, "<D FINISH>"))
	{
		TBS_SCRIPT_ACTION_TYPE_t *pAction_t = new TBS_SCRIPT_ACTION_TYPE_t;
		//�����ڽ�β������һ���ڵ�
		if (1)
		{
			//TODO:	ADD for FININSH
			pAction_t->pLoop_t.iLoop = 0;
			pAction_t->pLoop_t.iLoopNum = 0;
			pAction_t->cCondtion = 0;
			pAction_t->strCmdType = "FINISH";
			pAction_t->strCmd = "FINISH";
			pAction_t->sOperationType_e = (TBS_SCRIPT_OPREATION_TYPE_e)2;
			pAction_t->pNext = NULL;
			tbs_test_add_to_actionlist(pAction_t);
			return SCRIPT_FINISH;
		}
		else
		{
			//�﷨���󣬶�ν����ű�;
			return SCRIPT_EXC_FININSH;
		}
	}
	else if (0 == strcmp(pBuffer, "<D BEGIN>"))
	{

	}
	//�﷨���󣬷Ƿ�<...>���
	else
	{
		return SCRIPT_SINGLE;
	}
	return SCRIPT_EOF;
}

//�������ܣ�
//	�����ڽű����������еĴ�����Ϣ
//	������
//		strScriptName���ű�����
//		strProjectName:Project����
//	����ֵ��
//		void;
void CTBSScriptParse::tbs_test_script_parse_error(string strScriptName, string strProjectName)
{
	if (pParseError == NULL)
	{
		return;
	}

	CHAR			cLine[10];
	string			strResult;
	string			strErrorInfo;
	string			strSqlUpdate;
	CTBSDataBase	m_DataBase(DATAPATH);

	pParseError->cstrErrorInfo	= cstrErrorInf[pParseError->iError];
	strErrorInfo				= TOSTRING(this->pParseError->cstrErrorInfo);			

	if (pParseError->iError==SCRIPT_FINISH)
	{
		strResult = "Success";
	}
	else if(pParseError->iError == SCRIPT_EOF)
	{
		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, L"��case������");
		return;
	}
	else
	{
		strResult = "Fail";
	}
	_itoa_s(pParseError->iLineNum, cLine, 10);
	strSqlUpdate = "Update TestDetail set ParseResult='" + strResult + "',ErrorLine='" + cLine + "',ErrorInfo='" + strErrorInfo + "' where ProjectName='" + strProjectName + "' and ScriptName='" + strScriptName + "' and ProjectName='" + strProjectName + "' and CaseName='" + this->strCaseName + "'";
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, strResult+","+ cLine+","+ strErrorInfo);
	m_DataBase.tbs_db_sql_exec(strSqlUpdate);
}
