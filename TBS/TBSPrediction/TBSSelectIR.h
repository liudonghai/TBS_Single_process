#pragma once
#include "afxcmn.h"


// CTBSSelectIR �Ի���

class CTBSSelectIR : public CDialogEx
{
	DECLARE_DYNAMIC(CTBSSelectIR)

public:
	CTBSSelectIR(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTBSSelectIR();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW_IR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CListCtrl m_IRList;
	
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

private:
	INT	iIRNum;//Redrat�豸����
};
