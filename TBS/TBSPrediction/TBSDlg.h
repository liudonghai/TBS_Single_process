
// TBSApp.h : ͷ�ļ�
//

#pragma once
#include "CMSComm.h"
#include "TBSMainDlg.h"
#include "TBSMainThread.h"
#include "TBSProjectCreatDlg.h"
#include "TBSMainDlg.h"
#include "afxcmn.h"
// CTBSPredictionDlg �Ի���
class CTBSDlg : public CDialogEx
{
// ����
public:
	CTBSDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CTBSDlg();				//����
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TBSPREDICTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	CMenu m_MainMenu;
	static CMenu *pMenu;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);	// ���ɵ���Ϣӳ�亯��
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	void static tbs_menu_enable(INT *pId, INT iNum);
	void static tbs_menu_disable(INT *pId, INT iNum);

	//�����ű����ƣ�������ݿ�
	void tbs_test_prase_scriptname(CHAR *pPath, CHAR *pName);
	//���ҽű��ļ�
	void tbs_file_find(CString pDirPath, const CString  cstrKey = L".tbs", bool bSubDirectory = TRUE);
	
	// д����Ա��������Ϣ����
	void tbs_test_report_head_create(CString cstrFileName, CHAR **ppData, INT iCloum);
	//д����Ա��������Ϣ
	void tbs_test_report_information_create(CString cstrFileName, CHAR **ppData, INT iCloum);
	//д��ÿһ�����Ա���Ĳ��Խ��
	void tbs_test_report_case_result_create(CString cstrFileName);
	//д��ÿһ��case�Ĳ�����ϸ���
	void tbs_test_report_case_information_create(CString cstrTestReportName);

	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnConnectStb();
	afx_msg void OnDisconnectStb();
	afx_msg void OnNewProject();
	afx_msg void OnCloseProject();
	afx_msg void OnAbout();
	afx_msg void OnAddScript();
	afx_msg void OnOpenProject();
	afx_msg void OnIrIn();
	afx_msg void OnUpdateScript();
	afx_msg void OnDeleteScript();
	afx_msg void OnSelectRun();
	afx_msg void OnSize(UINT nType, INT cx, INT cy);
	afx_msg void OnPaint();
	afx_msg void OnTestDataIn();
	afx_msg void OnTestReportCreate();
	afx_msg void OnConnectIr();
	afx_msg void OnDisconIr();
	afx_msg void OnLogSave();
	afx_msg void OnTestFinish();
	afx_msg void OnIrValueIn();
	afx_msg void On32826();

private:
	void tbs_init_menu();	// ��ʼ���˵�

public:
	static CMutex		*m_Mutex;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};