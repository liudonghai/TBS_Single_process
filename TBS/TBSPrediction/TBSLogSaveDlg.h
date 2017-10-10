#pragma once
#include "afxwin.h"


// CTBSLogSave 对话框

class CTBSLogSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSLogSaveDlg)

public:
	CTBSLogSaveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSLogSaveDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOG_SAVE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedSeekDirectory();
	afx_msg void OnBnClickedOk();
	
	CString cstrFilePath;
	CString cstrFileName;
	CString cstrEachLine;
	CButton m_Check;
	fstream *pfsConfig;
};
