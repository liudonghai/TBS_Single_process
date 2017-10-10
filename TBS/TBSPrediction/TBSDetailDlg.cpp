// TBSDetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSDetailDlg.h"
#include "afxdialogex.h"
#include "TBSGlobal.h"

// CTBSDetailDlg 对话框
TBS_SCRIPT_ACTION_TYPE_t *CTBSDetailDlg::m_ActionNow = new TBS_SCRIPT_ACTION_TYPE_t;
TBS_SCRIPT_EXPECTION_t *CTBSDetailDlg::m_ExpectionNow = new TBS_SCRIPT_EXPECTION_t;
INT CTBSDetailDlg::iCount = 1;

CTBSDetailDlg *CTBSDetailDlg::GetInstance()
{
	if (iCount >0)
	{
		iCount--;
		return new CTBSDetailDlg();
	}
	return NULL;
}
IMPLEMENT_DYNAMIC(CTBSDetailDlg, CDialogEx)

CTBSDetailDlg::CTBSDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DETAIL_PARSE_DIALIG, pParent)
{
	bFirst = true;
	bFirstRun = true;
}
CTBSDetailDlg::CTBSDetailDlg(CString cstrProject, CString cstrScript, CString cstrResult/* CWnd* pParent = NULL*/)
{
	cstrProjectName	= cstrProject;
	cstrScriptName	= cstrScript;
	cstrActionType	= L"";
	cstrActionValue = L"";
	this->cstrResult = cstrResult;
	bFirst = true;
}
CTBSDetailDlg::~CTBSDetailDlg()
{
	iCount++;
}

void CTBSDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListControl);
}


BEGIN_MESSAGE_MAP(CTBSDetailDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSDetailDlg::OnBnClickedOk)
	ON_MESSAGE(WN_DETAIL_ACTON_CHANGE, &CTBSDetailDlg::OnActionChange)
END_MESSAGE_MAP()


// CTBSDetailDlg 消息处理程序


BOOL CTBSDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rc;
	m_ListControl.GetWindowRect(&rc);
	m_ListControl.InsertColumn(0, L"名称", LVCFMT_LEFT, rc.Width() * 3 / 10, 0);
	m_ListControl.InsertColumn(1, L"结果", LVCFMT_LEFT, rc.Width() * 7 / 10, 0);
	tbs_test_result_show_detail();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTBSDetailDlg::PostNcDestroy()
{
	delete this;
	CDialogEx::PostNcDestroy();
}


void CTBSDetailDlg::OnBnClickedOk()
{
	this->DestroyWindow();
}

afx_msg LRESULT CTBSDetailDlg::OnActionChange(WPARAM iFinish, LPARAM pMsg)
{
	 m_TestDetail = (tbs_test_detail_msg_t*)pMsg;
	INT iFinishFlag = iFinish;

	if (cstrScriptName == m_TestDetail->cstrScriptName)
	{
		//iFinishFlag: 0 is running, 1 is success,2 is fail
		if (iFinishFlag == 1)
		{
			cstrResult = SCRIPT_TEST_SUCCESS;
		}
		else if (iFinishFlag == 2)
		{
			cstrResult = SCRIPT_TEST_FAIL;
		}
		else
		{
			cstrResult = SCRIPT_RUNNING;
		}
		tbs_test_result_show_detail();
	}
	return 0;
}

void CTBSDetailDlg::tbs_test_result_show_detail()
{
	//the script has some grammar error 
	if (cstrResult == SCRIPT_ERROR)
	{
		string strSql = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "'";
		INT iCloum = 0, iRow = 0;
		string strGetData;
		CHAR **ppGetData;
		CTBSDataBase m_DataBase(DATAPATH);
		m_DataBase.tbs_db_data_query(strSql, &ppGetData, &iRow, &iCloum);
		if (iRow == 1)
		{
			strGetData = *(ppGetData + iCloum + 13);
			if (strGetData == SCRIPT_TEST_FAIL_STR)
			{
				//projectName
				m_ListControl.InsertItem(0, L"工程名");
				m_ListControl.SetItemText(0, 1, cstrProjectName);
				//scriptName
				m_ListControl.InsertItem(1, L"脚本名称");
				m_ListControl.SetItemText(1, 1, cstrScriptName);
				//parse result
				m_ListControl.InsertItem(2, L"解析结果");
				m_ListControl.SetItemText(2, 1, L"fail");
				//Line number
				string strLine;
				strLine = *(ppGetData + iCloum + 14);
				CString cstrLine(TOCSTRING(strLine));
				m_ListControl.InsertItem(3, L"错误行");
				m_ListControl.SetItemText(3, 1, cstrLine);
				//Error information
				string strErrorInf;
				strErrorInf = *(ppGetData + iCloum + 15);
				CString cstrErrorInf(TOCSTRING(strErrorInf));
				m_ListControl.InsertItem(3, L"错误信息");
				m_ListControl.SetItemText(3, 1, cstrErrorInf);
			}
			else
			{
				//数据库数据和显示不相符
			}
		}
		return;
	}
	//This is the first time enter into this funtion.
	if (bFirst)
	{
		//projectName
		string strSqlDescribe = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "' and CaseName='Describe'";
		INT iCloum = 0, iRow = 0;
		CString cstrGetData;
		CHAR **ppGetData;
		CTBSDataBase m_DataBase(DATAPATH);
		m_DataBase.tbs_db_data_query(strSqlDescribe, &ppGetData, &iRow, &iCloum);
		if (iRow > 0)
		{
			m_ListControl.InsertItem(0, L"工程名");
			m_ListControl.SetItemText(0, 1, cstrProjectName);
			//scriptName
			m_ListControl.InsertItem(1, L"脚本名称");
			m_ListControl.SetItemText(1, 1, cstrScriptName);

			//Name
			if (*(ppGetData + iCloum + 16))
			{
				cstrGetData = *(ppGetData + iCloum + 16);
			}
			m_ListControl.InsertItem(2, L"模块名称");
			m_ListControl.SetItemText(2, 1, cstrGetData);
			//CaseNumber
			if (*(ppGetData + iCloum + 17))
			{
				cstrGetData = *(ppGetData + iCloum + 17);
			}
			m_ListControl.InsertItem(3, L"case数量");
			m_ListControl.SetItemText(3, 1, cstrGetData);
			bFirst = 0;
		}


		iCloum = 0;
		iRow = 0;
		ppGetData = NULL;
		string strSqlAll = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "' and CaseName!='Describe'";
		m_DataBase.tbs_db_data_query(strSqlAll, &ppGetData, &iRow, &iCloum);
		if (iRow > 0)
		{
			for (INT i = 1; i < iRow + 1; i++)
			{
				if (*(ppGetData + i*iCloum + 7)==NULL)
				{
					break;
				}
				
				m_ListControl.InsertItem(4 + (i - 1) * 10, L"Case名称");
				if (*(ppGetData + i*iCloum + 2))
				{
					m_ListControl.SetItemText(4 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 2));

				}
				m_ListControl.InsertItem(5 + (i - 1) * 10, L"Case序号");
				if (*(ppGetData + i*iCloum + 17))
				{
					m_ListControl.SetItemText(5 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 17));
				}


				if (*(ppGetData + i*iCloum + 7) && (CString)*(ppGetData + i*iCloum + 7) == L"OK")
				{
					m_ListControl.InsertItem(6 + (i - 1) * 10, L"开始时间");
					if (*(ppGetData + i*iCloum + 5))
					{
						m_ListControl.SetItemText(6 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 5));
					}
					m_ListControl.InsertItem(7 + (i - 1) * 10, L"结束时间");
					if (*(ppGetData + i*iCloum + 6))
					{
						m_ListControl.SetItemText(7 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 6));
					}
					m_ListControl.InsertItem(8 + (i - 1) * 10, L"测试结果");
					if (*(ppGetData + i*iCloum + 7))
					{
						m_ListControl.SetItemText(8 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 7));
					}
					m_ListControl.InsertItem(9 + (i - 1) * 10, L"耗时");
					if (*(ppGetData + i*iCloum + 12))
					{
						m_ListControl.SetItemText(9 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 12));
					}
					m_ListControl.InsertItem(10 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(11 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(12 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(13 + (i - 1) * 10, L"");
				}


				if (*(ppGetData + i*iCloum + 7) && (CString)*(ppGetData + i*iCloum + 7) == L"FAIL")
				{
					m_ListControl.InsertItem(6 + (i - 1) * 10, L"开始时间");
					if (*(ppGetData + i*iCloum + 5))
					{
						m_ListControl.SetItemText(6 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 5));
					}
					m_ListControl.InsertItem(7 + (i - 1) * 10, L"结束时间");
					if (*(ppGetData + i*iCloum + 6))
					{
						m_ListControl.SetItemText(7 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 6));
					}
					m_ListControl.InsertItem(8 + (i - 1) * 10, L"测试结果");
					if (*(ppGetData + i*iCloum + 7))
					{
						m_ListControl.SetItemText(8 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 7));
					}

					m_ListControl.InsertItem(9 + (i - 1) * 10, L"耗时");
					if (*(ppGetData + i*iCloum + 12))
					{
						m_ListControl.SetItemText(9 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 12));
					}

					m_ListControl.InsertItem(10 + (i - 1) * 10, L"动作");
					if (*(ppGetData + i*iCloum + 8))
					{
						m_ListControl.SetItemText(10 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 8));
					}
					m_ListControl.InsertItem(11 + (i - 1) * 10, L"动作时间");
					if (*(ppGetData + i*iCloum + 9))
					{
						m_ListControl.SetItemText(11 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 9));
					}
					m_ListControl.InsertItem(12 + (i - 1) * 10, L"期望结果");
					if (*(ppGetData + i*iCloum + 10))
					{
						m_ListControl.SetItemText(12 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 10));
					}
					m_ListControl.InsertItem(13 + (i - 1) * 10, L"实际结果");
					if (*(ppGetData + i*iCloum + 11))
					{
						m_ListControl.SetItemText(13 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 11));
					}
				}
			}
		}
	/*	m_ListControl.InsertItem(4, L"");
		m_ListControl.InsertItem(5, L"");
		m_ListControl.InsertItem(6, L"");
		m_ListControl.InsertItem(7, L"");
		m_ListControl.InsertItem(8, L"");
		m_ListControl.InsertItem(9, L"");
		m_ListControl.InsertItem(10, L"");
		m_ListControl.InsertItem(11, L"");
		m_ListControl.InsertItem(12, L"");
		m_ListControl.InsertItem(13, L"");
		m_ListControl.InsertItem(14, L"");
		m_ListControl.InsertItem(15, L"");
		m_ListControl.InsertItem(16, L"");
		m_ListControl.InsertItem(17, L"");
		m_ListControl.InsertItem(18, L"");*/
	}
	//the script is right,it is waiting for running
	if (cstrResult == SCRIPT_PARSE)
	{
		//projectName
		//m_ListControl.InsertItem(0, L"工程名");
		//m_ListControl.SetItemText(0, 1, cstrProjectName);
		////scriptName
		//m_ListControl.InsertItem(1, L"脚本名称");
		//m_ListControl.SetItemText(1, 1, cstrScriptName);
		////parse result
		//m_ListControl.InsertItem(2, L"解析结果");
		//m_ListControl.SetItemText(2, 1, L"success");
		////status
		//m_ListControl.InsertItem(3, L"状态");
		//m_ListControl.SetItemText(3, 1, L"等待执行");
	}
	//the script is running
	else if (SCRIPT_RUNNING == cstrResult)
	{
		
		if (bFirstRun)
		{
			bFirstRun = 0;
			//获取到空行的位置
			CString cstrLine;
			INT iItem;
			for (INT i = 0; i >= 0; i++)
			{
				cstrLine = m_ListControl.GetItemText(i, 0);
				if (cstrLine == L"")
				{
					iItem = i;
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iItem);
					break;
				}
			}
			m_ListControl.InsertItem(iItem, L"case名称");
			m_ListControl.SetItemText(iItem, 1, m_TestDetail->cstrCaseNmae);

			string strSqlDescribe = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "' and CaseName='" + (string)TOSTRING(m_TestDetail->cstrCaseNmae) + "'";
			CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_TestDetail->cstrCaseNmae);

			INT iCloum = 0, iRow = 0;
			CString cstrGetData;
			CHAR **ppGetData;
			CTBSDataBase m_DataBase(DATAPATH);
			m_DataBase.tbs_db_data_query(strSqlDescribe, &ppGetData, &iRow, &iCloum);
			if (iRow == 1)
			{
				m_ListControl.InsertItem(iItem + 1, L"case序号");
				if (*(ppGetData + iCloum + 17))
				{
					m_ListControl.SetItemText(iItem+1, 1, (CString)*(ppGetData + iCloum + 17));
				}
			}
			m_ListControl.InsertItem(iItem+2, L"当前动作类型");
			m_ListControl.SetItemText(iItem+2, 1, L"当前动作值");
			cstrActionTypeOld = cstrActionType;
			cstrActionValueOld = cstrActionValue;
			cstrActionType = (TOCSTRING(m_ActionNow->strCmdType));
			cstrActionValue = (TOCSTRING(m_ActionNow->strCmd));
			m_ListControl.InsertItem(iItem + 3, cstrActionType);
			m_ListControl.SetItemText(iItem + 3, 1, cstrActionValue);
			m_ListControl.InsertItem(iItem + 4, cstrActionTypeOld);
			m_ListControl.SetItemText(iItem + 4, 1, cstrActionValueOld);

			if (m_ExpectionNow->bEffect == 1)
			{
				m_ExpectionNow->bEffect = 0;
			}
			{
				m_ListControl.InsertItem(iItem + 5, L"期望类型");
				m_ListControl.SetItemText(iItem + 5, 1, L"期望值");
				INT i;
				for (i = 0; i < 5; i++)
				{
					CString cstrExpType(TOCSTRING(*(m_ExpectionNow->strExpType + i)));
					CString cstrExpValue(TOCSTRING(*(m_ExpectionNow->strCmd + i)));
					m_ListControl.InsertItem(iItem + 6+ i * 2, cstrExpType);
					m_ListControl.SetItemText(iItem + 6+ i * 2, 1, cstrExpValue);
					CString cstrDelay(TOCSTRING(*(m_ExpectionNow->strTime + i)));
					CString	cstrDelayType=L"";
					if (cstrDelay != L"")
					{
						cstrDelayType = L"超时时间";
					}
					m_ListControl.InsertItem(iItem + 7 + i * 2, cstrExpType);
					m_ListControl.SetItemText(iItem + 7 + i * 2, 1, cstrDelay);
				}
			}
		}
		else
		{
			//获取到空行的位置
			CString cstrLine;
			INT iItem;
			for (INT i = 0; i >= 0; i++)
			{
				cstrLine = m_ListControl.GetItemText(i, 1);
				if (cstrLine == m_TestDetail->cstrCaseNmae)
				{
					
					iItem = i;
					CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, iItem);
					break;
				}
			}
			//m_ListControl.InsertItem(iItem, L"case名称");
			//m_ListControl.SetItemText(iItem, 1, m_TestDetail->cstrCaseNmae);

			string strSqlDescribe = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "' and CaseName='" + (string)TOSTRING(m_TestDetail->cstrCaseNmae) + "'";
			INT iCloum = 0, iRow = 0;
			CString cstrGetData;
			CHAR **ppGetData;
			CTBSDataBase m_DataBase(DATAPATH);
			m_DataBase.tbs_db_data_query(strSqlDescribe, &ppGetData, &iRow, &iCloum);
			if (iRow == 1)
			{
				//m_ListControl.InsertItem(iItem + 1, L"case序号");
				if (*(ppGetData + iCloum + 17))
				{
					m_ListControl.SetItemText(iItem + 1, 1, (CString)*(ppGetData + iCloum + 17));
				}
			}
			//m_ListControl.InsertItem(iItem + 2, L"当前动作类型");
			//m_ListControl.SetItemText(iItem + 2, 1, L"当前动作值");
			cstrActionTypeOld = cstrActionType;
			cstrActionValueOld = cstrActionValue;
			cstrActionType = (TOCSTRING(m_ActionNow->strCmdType));
			cstrActionValue = (TOCSTRING(m_ActionNow->strCmd));
			m_ListControl.SetItemText(iItem + 3, 0, cstrActionType);
			m_ListControl.SetItemText(iItem + 3, 1, cstrActionValue);
			m_ListControl.SetItemText(iItem + 4, 0, cstrActionTypeOld);
			m_ListControl.SetItemText(iItem + 4, 1, cstrActionValueOld);

			if (m_ExpectionNow->bEffect == 1)
			{
				m_ExpectionNow->bEffect = 0;
				//m_ListControl.InsertItem(iItem + 5, L"期望类型");
				//m_ListControl.SetItemText(iItem + 5, 1, L"期望值");
				INT i;
				for (i = 0; i < m_ExpectionNow->iNum; i++)
				{
					CString cstrExpType(TOCSTRING(*(m_ExpectionNow->strExpType + i)));
					CString cstrExpValue(TOCSTRING(*(m_ExpectionNow->strCmd + i)));
					m_ListControl.SetItemText(iItem + 6 + i * 2, 0, cstrExpType);
					m_ListControl.SetItemText(iItem + 6 + i * 2, 1, cstrExpValue);
					CString cstrDelay(TOCSTRING(*(m_ExpectionNow->strTime + i)));
					m_ListControl.SetItemText(iItem + 7 + i * 2, 0, L"超时时间");
					m_ListControl.SetItemText(iItem + 7 + i * 2, 1, cstrDelay);
				}
			}
		}
	}
	//exectue successful
	else if (SCRIPT_TEST_SUCCESS == cstrResult)
	{
		bFirstRun = 1;
		m_ListControl.DeleteAllItems();
		string strSqlDescribe = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "' and CaseName='Describe'";
		INT iCloum = 0, iRow = 0;
		CString cstrGetData;
		CHAR **ppGetData;
		CTBSDataBase m_DataBase(DATAPATH);
		m_DataBase.tbs_db_data_query(strSqlDescribe, &ppGetData, &iRow, &iCloum);
		if (iRow > 0)
		{
			m_ListControl.InsertItem(0, L"工程名");
			m_ListControl.SetItemText(0, 1, cstrProjectName);
			//scriptName
			m_ListControl.InsertItem(1, L"脚本名称");
			m_ListControl.SetItemText(1, 1, cstrScriptName);

			//Name
			if (*(ppGetData + iCloum + 16))
			{
				cstrGetData = *(ppGetData + iCloum + 16);
			}
			m_ListControl.InsertItem(2, L"模块名称");
			m_ListControl.SetItemText(2, 1, cstrGetData);
			//CaseNumber
			if (*(ppGetData + iCloum + 17))
			{
				cstrGetData = *(ppGetData + iCloum + 17);
			}
			m_ListControl.InsertItem(3, L"case数量");
			m_ListControl.SetItemText(3, 1, cstrGetData);
			bFirst = 0;
		}


		iCloum = 0;
		iRow = 0;
		ppGetData = NULL;
		string strSqlAll = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "' and CaseName!='Describe'";
		m_DataBase.tbs_db_data_query(strSqlAll, &ppGetData, &iRow, &iCloum);
		if (iRow > 0)
		{
			for (INT i = 1; i < iRow + 1; i++)
			{
				if (*(ppGetData + i*iCloum + 7) == NULL)
				{
					break;
				}
				m_ListControl.InsertItem(4 + (i - 1) * 10, L"Case名称");
				if (*(ppGetData + i*iCloum + 2))
				{
					m_ListControl.SetItemText(4 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 2));

				}
				m_ListControl.InsertItem(5 + (i - 1) * 10, L"Case序号");
				if (*(ppGetData + i*iCloum + 17))
				{
					m_ListControl.SetItemText(5 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 17));
				}


				if (*(ppGetData + i*iCloum + 7) && (CString)*(ppGetData + i*iCloum + 7) == L"OK")
				{
					m_ListControl.InsertItem(6 + (i - 1) * 10, L"开始时间");
					if (*(ppGetData + i*iCloum + 5))
					{
						m_ListControl.SetItemText(6 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 5));
					}
					m_ListControl.InsertItem(7 + (i - 1) * 10, L"结束时间");
					if (*(ppGetData + i*iCloum + 6))
					{
						m_ListControl.SetItemText(7 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 6));
					}
					m_ListControl.InsertItem(8 + (i - 1) * 10, L"测试结果");
					if (*(ppGetData + i*iCloum + 7))
					{
						m_ListControl.SetItemText(8 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 7));
					}
					m_ListControl.InsertItem(9 + (i - 1) * 10, L"耗时");
					if (*(ppGetData + i*iCloum + 12))
					{
						m_ListControl.SetItemText(9 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 12));
					}
					m_ListControl.InsertItem(10 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(11 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(12 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(13 + (i - 1) * 10, L"");
				}


				if (*(ppGetData + i*iCloum + 7) && (CString)*(ppGetData + i*iCloum + 7) == L"FAIL")
				{
					m_ListControl.InsertItem(6 + (i - 1) * 10, L"开始时间");
					if (*(ppGetData + i*iCloum + 5))
					{
						m_ListControl.SetItemText(6 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 5));
					}
					m_ListControl.InsertItem(7 + (i - 1) * 10, L"结束时间");
					if (*(ppGetData + i*iCloum + 6))
					{
						m_ListControl.SetItemText(7 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 6));
					}
					m_ListControl.InsertItem(8 + (i - 1) * 10, L"测试结果");
					if (*(ppGetData + i*iCloum + 7))
					{
						m_ListControl.SetItemText(8 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 7));
					}

					m_ListControl.InsertItem(9 + (i - 1) * 10, L"耗时");
					if (*(ppGetData + i*iCloum + 12))
					{
						m_ListControl.SetItemText(9 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 12));
					}

					m_ListControl.InsertItem(10 + (i - 1) * 10, L"动作");
					if (*(ppGetData + i*iCloum + 8))
					{
						m_ListControl.SetItemText(10 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 8));
					}
					m_ListControl.InsertItem(11 + (i - 1) * 10, L"动作时间");
					if (*(ppGetData + i*iCloum + 9))
					{
						m_ListControl.SetItemText(11 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 9));
					}
					m_ListControl.InsertItem(12 + (i - 1) * 10, L"期望结果");
					if (*(ppGetData + i*iCloum + 10))
					{
						m_ListControl.SetItemText(12 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 10));
					}
					m_ListControl.InsertItem(13 + (i - 1) * 10, L"实际结果");
					if (*(ppGetData + i*iCloum + 11))
					{
						m_ListControl.SetItemText(13 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 11));
					}
				}

			}
		}
	}
	//execute fail
	else if (SCRIPT_TEST_FAIL == cstrResult)
	{
		bFirstRun = 1;
		m_ListControl.DeleteAllItems();
		string strSqlDescribe = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "' and CaseName='Describe'";
		INT iCloum = 0, iRow = 0;
		CString cstrGetData;
		CHAR **ppGetData;
		CTBSDataBase m_DataBase(DATAPATH);
		m_DataBase.tbs_db_data_query(strSqlDescribe, &ppGetData, &iRow, &iCloum);
		if (iRow > 0)
		{
			m_ListControl.InsertItem(0, L"工程名");
			m_ListControl.SetItemText(0, 1, cstrProjectName);
			//scriptName
			m_ListControl.InsertItem(1, L"脚本名称");
			m_ListControl.SetItemText(1, 1, cstrScriptName);

			//Name
			if (*(ppGetData + iCloum + 16))
			{
				cstrGetData = *(ppGetData + iCloum + 16);
			}
			m_ListControl.InsertItem(2, L"模块名称");
			m_ListControl.SetItemText(2, 1, cstrGetData);
			//CaseNumber
			if (*(ppGetData + iCloum + 17))
			{
				cstrGetData = *(ppGetData + iCloum + 17);
			}
			m_ListControl.InsertItem(3, L"case数量");
			m_ListControl.SetItemText(3, 1, cstrGetData);
			bFirst = 0;
		}


		iCloum = 0;
		iRow = 0;
		ppGetData = NULL;
		string strSqlAll = "select *from TestDetail where ProjectName='" + string(TOSTRING(cstrProjectName)) + "' and ScriptName='" + string(TOSTRING(cstrScriptName)) + "' and CaseName!='Describe'";
		m_DataBase.tbs_db_data_query(strSqlAll, &ppGetData, &iRow, &iCloum);
		if (iRow > 0)
		{
			for (INT i = 1; i < iRow + 1; i++)
			{
				if (*(ppGetData + i*iCloum + 7) == NULL)
				{
					break;
				}
				m_ListControl.InsertItem(4 + (i - 1) * 10, L"Case名称");
				if (*(ppGetData + i*iCloum + 2))
				{
					m_ListControl.SetItemText(4 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 2));

				}
				m_ListControl.InsertItem(5 + (i - 1) * 10, L"Case序号");
				if (*(ppGetData + i*iCloum + 17))
				{
					m_ListControl.SetItemText(5 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 17));
				}


				if (*(ppGetData + i*iCloum + 7) && (CString)*(ppGetData + i*iCloum + 7) == L"OK")
				{
					m_ListControl.InsertItem(6 + (i - 1) * 10, L"开始时间");
					if (*(ppGetData + i*iCloum + 5))
					{
						m_ListControl.SetItemText(6 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 5));
					}
					m_ListControl.InsertItem(7 + (i - 1) * 10, L"结束时间");
					if (*(ppGetData + i*iCloum + 6))
					{
						m_ListControl.SetItemText(7 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 6));
					}
					m_ListControl.InsertItem(8 + (i - 1) * 10, L"测试结果");
					if (*(ppGetData + i*iCloum + 7))
					{
						m_ListControl.SetItemText(8 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 7));
					}
					m_ListControl.InsertItem(9 + (i - 1) * 10, L"耗时");
					if (*(ppGetData + i*iCloum + 12))
					{
						m_ListControl.SetItemText(9 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 12));
					}
					m_ListControl.InsertItem(10 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(11 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(12 + (i - 1) * 10, L"");
					m_ListControl.InsertItem(13 + (i - 1) * 10, L"");
				}


				if (*(ppGetData + i*iCloum + 7) && (CString)*(ppGetData + i*iCloum + 7) == L"FAIL")
				{
					m_ListControl.InsertItem(6 + (i - 1) * 10, L"开始时间");
					if (*(ppGetData + i*iCloum + 5))
					{
						m_ListControl.SetItemText(6 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 5));
					}
					m_ListControl.InsertItem(7 + (i - 1) * 10, L"结束时间");
					if (*(ppGetData + i*iCloum + 6))
					{
						m_ListControl.SetItemText(7 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 6));
					}
					m_ListControl.InsertItem(8 + (i - 1) * 10, L"测试结果");
					if (*(ppGetData + i*iCloum + 7))
					{
						m_ListControl.SetItemText(8 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 7));
					}

					m_ListControl.InsertItem(9 + (i - 1) * 10, L"耗时");
					if (*(ppGetData + i*iCloum + 12))
					{
						m_ListControl.SetItemText(9 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 12));
					}

					m_ListControl.InsertItem(10 + (i - 1) * 10, L"动作");
					if (*(ppGetData + i*iCloum + 8))
					{
						m_ListControl.SetItemText(10 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 8));
					}
					m_ListControl.InsertItem(11 + (i - 1) * 10, L"动作时间");
					if (*(ppGetData + i*iCloum + 9))
					{
						m_ListControl.SetItemText(11 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 9));
					}
					m_ListControl.InsertItem(12 + (i - 1) * 10, L"期望结果");
					if (*(ppGetData + i*iCloum + 10))
					{
						m_ListControl.SetItemText(12 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 10));
					}
					m_ListControl.InsertItem(13 + (i - 1) * 10, L"实际结果");
					if (*(ppGetData + i*iCloum + 11))
					{
						m_ListControl.SetItemText(13 + (i - 1) * 10, 1, (CString)*(ppGetData + i*iCloum + 11));
					}
				}
			}
		}
	}
	//nothing
	else
	{
		//nothing
	}
	UpdateData(0);
}


void CTBSDetailDlg::OnCancel()
{
	this->DestroyWindow();	
}
