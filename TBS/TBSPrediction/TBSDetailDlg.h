#pragma once
#include "stdafx.h"
#include "afxcmn.h"
#include "TBSScriptParse.h"
#include "TBSGlobal.h"
using namespace std;

// CTBSDetailDlg 对话框

class CTBSDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSDetailDlg)

private:
	CTBSDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	CTBSDetailDlg(CString cstrProject, CString cstrScript, CString cstrResult/*, CWnd* pParent = NULL*/);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETAIL_PARSE_DIALIG};
#endif

protected:
	void tbs_test_result_show_detail();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:

	virtual ~CTBSDetailDlg();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	static CTBSDetailDlg *GetInstance();
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnActionChange(WPARAM nullParameter, LPARAM cstrFileName);
	
	
	bool	bFirst;
	bool	bFirstRun;
	static	INT iCount;
	static	TBS_SCRIPT_ACTION_TYPE_t *m_ActionNow;
	static	TBS_SCRIPT_EXPECTION_t *m_ExpectionNow;
	
	tbs_test_detail_msg_t *m_TestDetail;
	CString cstrActionType;
	CString cstrActionValue;
	CString cstrActionTypeOld;
	CString cstrActionValueOld;
	CString cstrScriptName;
	CString cstrProjectName;
	CString cstrResult;
	CListCtrl m_ListControl;
	virtual void OnCancel();
};
