#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TBSMainDlg.h"

// CTBSProjectOpenDlg 对话框

class CTBSProjectOpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSProjectOpenDlg)

public:
	CTBSProjectOpenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSProjectOpenDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_PROJECT_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	INT		iItem;
	CEdit	m_SelectName;
	CString csItemString;
	CListCtrl m_ProjectList;
public:
	afx_msg void OnClickProjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDeleteProject();

	virtual BOOL OnInitDialog();
	BOOL tbs_check_project(const CString cstrProjectName);
};
