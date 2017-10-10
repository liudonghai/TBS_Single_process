#pragma once


// CTBSProjectCreatDlg 对话框

class CTBSProjectCreatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSProjectCreatDlg)

public:
	CTBSProjectCreatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSProjectCreatDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREAT_PROJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CWinThread* hConnect;
};
