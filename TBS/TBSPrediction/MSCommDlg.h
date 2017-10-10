#pragma once
#include "CMSComm.h"
#include "TBSScriptParse.h"
#include <regex>
#include <fstream>

// CMSCommDlg 对话框
 
class CMSCommDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMSCommDlg)

public:
	CMSCommDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMSCommDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MSCOMM_DIALOG};
#endif

public:

	CHAR		*pGetData;
	wregex		regEachTime[7];
	string		strEachLine;
	CString		cstrPort;
	CString		cstrParam;
	CString		cstrFileName;
	CString		cstrEachLine;
	CString		cstrFilePath;
	CMSComm		*m_MSCOMbutton;
	SYSTEMTIME	m_SysTime;
	basic_filebuf<wchar_t> 		pfsFile;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	virtual BOOL OnInitDialog();
	void OnMSComm();
	VOID tbs_init_MSComm();
	
	DECLARE_EVENTSINK_MAP()

protected:

	afx_msg LRESULT OnKeyboardMsgRec(WPARAM INum, LPARAM strSend);
	afx_msg LRESULT OnCMSCommRec(WPARAM cstrPort, LPARAM cstrParm);
	afx_msg LRESULT OnLogFileRec(WPARAM nullParameter, LPARAM cstrFileName);
	afx_msg LRESULT OnExpectationRec(WPARAM nullParameter, LPARAM cstrFileName);
	
	virtual void DoDataExchange(CDataExchange* pDX);
	void tbs_log_write(WCHAR *pData);
	void tbs_log_file_create();
	
	DECLARE_MESSAGE_MAP()

private:

	INT		iFlag;
	CHAR	*pCharLog;
	DOUBLE	dTime[5];
	DOUBLE	dTimeBegin;
	CString cstrProjectName;
	CTBSMainDlg *m_MainDlg;
	TBS_SCRIPT_EXPECTION_t	*m_Exptation;

};
