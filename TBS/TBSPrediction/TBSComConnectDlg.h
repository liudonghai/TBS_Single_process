#pragma once
// CTBSComConnectDlg �Ի���

class CTBSComConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSComConnectDlg)

public:
	CTBSComConnectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTBSComConnectDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
