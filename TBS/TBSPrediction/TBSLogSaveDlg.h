#pragma once
#include "afxwin.h"


// CTBSLogSave �Ի���

class CTBSLogSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSLogSaveDlg)

public:
	CTBSLogSaveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTBSLogSaveDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOG_SAVE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
