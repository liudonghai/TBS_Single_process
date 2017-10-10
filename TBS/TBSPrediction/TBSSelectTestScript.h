#pragma once
#include "TBSScriptParse.h"
#include "afxcmn.h"


// CTBSSelectTestScript 对话框


class CTBSScriptSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSScriptSelectDlg)

public:
	CTBSScriptSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSScriptSelectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_RUN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	INT			m_iRadio;
	CListCtrl	m_ScriptShow;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedSelectAll();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedBeginTest();

	static UINT	tbs_test_manager(LPVOID lpParam);

private:

	INT			iIndex;
	CTBSMainDlg *m_MainDlg;
	CString		cstrProjectName;
};
