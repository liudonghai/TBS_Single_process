// TBSTestDataInDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSTestDataInDlg.h"
#include "afxdialogex.h"
#include "TBSDataBase.h"
#include "TBSPredictionDlg.h"	
#include <string>
using namespace std;
// CTBSTestDataInDlg 对话框

IMPLEMENT_DYNAMIC(CTBSTestDataInDlg, CDialogEx)

CTBSTestDataInDlg::CTBSTestDataInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEST_DATA_IN_DIALOG, pParent)
	, cstrProduct(_T(""))
	, cstrHard(_T(""))
	, cstrSoft(_T(""))
	, cstrSN(_T(""))
	, cstrChipId(_T(""))
	, cstrTester(_T(""))
{
	//TODO：读取配置文件内的相关数据
	cstrProject = CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrProjectName;
	tbs_read_pro_ini();
}

CTBSTestDataInDlg::~CTBSTestDataInDlg()
{
}

void CTBSTestDataInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRODUCTNAME, cstrProduct);
	DDX_Text(pDX, IDC_HARD, cstrHard);
	DDX_Text(pDX, IDC_SOFT, cstrSoft);
	DDX_Text(pDX, IDC_SN, cstrSN);
	DDX_Text(pDX, IDC_CHIPID, cstrChipId);
	DDX_Text(pDX, IDC_TESTER, cstrTester);
}


BEGIN_MESSAGE_MAP(CTBSTestDataInDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSTestDataInDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void CTBSTestDataInDlg::OnBnClickedOk()
{
	UpdateData();
	tbs_write_pro_ini();
	string strChipId = TOSTRING(cstrChipId);
	string strHard = TOSTRING(cstrHard);
	string strProduct = TOSTRING(cstrProduct);
	string strSoft = TOSTRING(cstrSoft);
	string strSN=TOSTRING(cstrSN);
	string strTester=TOSTRING(cstrTester);
	string strProject=TOSTRING(cstrProject);
	string strSqlUpdate = "Update TestReport set ProductName='" + strProduct + "',HardwareVersion='" + strHard + "',SoftwareVersion='" + strSoft + "',SN='" + strSN + "',ChipID='" + strChipId + "',Tester='" + strTester + "' where ProjectName='" + strProject + "'";
	
	CTBSDataBase m_DataBase(DATAPATH);
	m_DataBase.tbs_db_sql_exec(strSqlUpdate);

	CDialogEx::OnOK();
}

void CTBSTestDataInDlg::tbs_write_pro_ini()
{
	CString cstrconfig = L"..\\project\\"+ cstrProject+L"\\"+ cstrProject+L".ini";
	CString cstrTestReportBaseInfo = L"TestInfo";
	CString cstrInfoName[6] = {L"ProductName",L"HardVersion",L"SoftWareVersion",L"SN",L"ChipId",L"Tester"};
	WritePrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[0], cstrProduct, cstrconfig);
	WritePrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[1], cstrHard, cstrconfig);
	WritePrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[2], cstrSoft, cstrconfig);
	WritePrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[3], cstrSN, cstrconfig);
	WritePrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[4], cstrChipId, cstrconfig);
	WritePrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[5], cstrTester, cstrconfig);
}

void CTBSTestDataInDlg::tbs_read_pro_ini()
{
	CString cstrTestReportBaseInfo = L"TestInfo";
	CString cstrInfoName[6] = { L"ProductName",L"HardVersion",L"SoftWareVersion",L"SN",L"ChipId",L"Tester" };
	CString cstrconfig = L"..\\project\\" + cstrProject + L"\\" + cstrProject + L".ini";
	WCHAR wcData[MAX_PATH];
	GetPrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[0], L"", wcData, MAX_PATH, cstrconfig);
	cstrProduct = wcData;
	GetPrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[1], L"", wcData, MAX_PATH, cstrconfig);
	cstrHard = wcData;
	GetPrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[2], L"", wcData, MAX_PATH, cstrconfig);
	cstrSoft = wcData;
	GetPrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[3], L"", wcData, MAX_PATH, cstrconfig);
	cstrSN = wcData;
	GetPrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[4], L"", wcData, MAX_PATH, cstrconfig);
	cstrChipId = wcData;
	GetPrivateProfileString(cstrTestReportBaseInfo, cstrInfoName[5], L"", wcData, MAX_PATH, cstrconfig);
	cstrTester = wcData;
}