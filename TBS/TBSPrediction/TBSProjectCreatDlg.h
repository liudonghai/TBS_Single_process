#pragma once


// CTBSProjectCreatDlg �Ի���

class CTBSProjectCreatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSProjectCreatDlg)

public:
	CTBSProjectCreatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTBSProjectCreatDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREAT_PROJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CWinThread* hConnect;
};
