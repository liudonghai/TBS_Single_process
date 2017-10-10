#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TBSMainDlg.h"

// CTBSProjectOpenDlg �Ի���

class CTBSProjectOpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSProjectOpenDlg)

public:
	CTBSProjectOpenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTBSProjectOpenDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDC_PROJECT_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
