#pragma once
// CTBSComConnectDlg 对话框

class CTBSComConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSComConnectDlg)

public:
	CTBSComConnectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSComConnectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
