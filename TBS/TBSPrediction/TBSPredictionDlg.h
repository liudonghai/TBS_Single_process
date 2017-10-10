
// TBSPredictionDlg.h : 头文件
//

#pragma once
#include "CMSComm.h"
#include "TBSMainDlg.h"
#include "TBSMainThread.h"
#include "TBSProjectCreatDlg.h"
#include "TBSMainDlg.h"
#include "afxcmn.h"
#include "TBSTabCtrl.h"
// CTBSPredictionDlg 对话框
class CTBSPredictionDlg : public CDialogEx
{
// 构造
public:
	CTBSPredictionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TBSPREDICTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	CMenu m_MainMenu;
	static CMenu *pMenu;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);	// 生成的消息映射函数
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	void static tbs_menu_enable(INT *pId, INT iNum);
	void static tbs_menu_disable(INT *pId, INT iNum);

	//解析脚本名称，添加数据库
	void tbs_test_prase_scriptname(CHAR *pPath, CHAR *pName);
	//查找脚本文件
	void tbs_file_find(CString pDirPath, const CString  cstrKey = L".tbs", bool bSubDirectory = TRUE);
	
	// 写入测试报告基本信息函数
	void tbs_test_report_head_create(CString cstrFileName, CHAR **ppData, INT iCloum);
	//写入测试报告汇总信息
	void tbs_test_report_information_create(CString cstrFileName, CHAR **ppData, INT iCloum);
	//写入每一条测试报告的测试结果
	void tbs_test_report_case_result_create(CString cstrFileName);
	//写入每一条case的测试详细结果
	void tbs_test_report_case_information_create(CString cstrTestReportName);

	virtual void OnOK();
	
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
	afx_msg void OnTcnSelchangeTabControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnConnectIr();
	afx_msg void OnDisconIr();
	afx_msg void OnLogSave();

private:
	void tbs_init_menu();	// 初始化菜单

public:
	static CMutex		*m_Mutex;
	static CTBSTabCtrl	*m_TabControl;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTestFinish();
	afx_msg void OnIrValueIn();
	virtual void OnCancel();
};

