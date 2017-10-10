#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTBSScriptDeleteDlg 对话框

class CTBSScriptDeleteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSScriptDeleteDlg)

public:
	CTBSScriptDeleteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSScriptDeleteDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELETE_SCRIPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_DeleteFile;
	CListCtrl m_DeleteShow;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedSelectAll();
	
	virtual BOOL OnInitDialog();

private:
	string strProjectName;
};
