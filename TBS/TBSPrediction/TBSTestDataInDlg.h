#pragma once


// CTBSTestDataInDlg 对话框

class CTBSTestDataInDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSTestDataInDlg)

public:
	CTBSTestDataInDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTBSTestDataInDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DATA_IN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void tbs_write_pro_ini();
	void tbs_read_pro_ini();
	CString cstrProduct;
	CString cstrHard;
	CString cstrSoft;
	CString cstrSN;
	CString cstrChipId;
	CString cstrTester;
	CString cstrProject;
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
};
