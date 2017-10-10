#pragma once
#include "CMSComm.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <string>
using namespace std;
// CTBSMainDlg 对话框

class CTBSMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSMainDlg)

public:
	CTBSMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSMainDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:

	char *pGetData;

	CEdit m_Edit;
	CEvent m_Event;
	CString cstrProjectName;
	CListCtrl m_TestList;
	CRichEditCtrl m_RichEdit;

	static CString sIrPath;

	afx_msg void OnBnClickedOk();
	afx_msg void OnButtonPort(UINT nId);
	afx_msg void OnSize(UINT nType, INT cx, INT cy);
	afx_msg void OnPaint();
	afx_msg void OnNMClickTestList(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
private:
};
