#pragma once
#include "afxcmn.h"


// CTBSSelectIR 对话框

class CTBSSelectIRDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSSelectIRDlg)

public:
	CTBSSelectIRDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSSelectIRDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW_IR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CListCtrl m_IRList;
	
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

private:
	INT	iIRNum;//Redrat设备数量
};
