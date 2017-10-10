// TBSSelectIR.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TBSDlg.h"
#include "TBSSelectIRDlg.h"
#include "afxdialogex.h"
#include <regex>

// CTBSSelectIR �Ի���

IMPLEMENT_DYNAMIC(CTBSSelectIRDlg, CDialogEx)

CTBSSelectIRDlg::CTBSSelectIRDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOW_IR, pParent)
{

}

CTBSSelectIRDlg::~CTBSSelectIRDlg()
{
}

void CTBSSelectIRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_IR, m_IRList);
}


BEGIN_MESSAGE_MAP(CTBSSelectIRDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSSelectIRDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CTBSSelectIRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect	m_Rc;
	DWORD	dwStyle		= m_IRList.GetExtendedStyle();
	CString *cstrName	= new CString[8];
	WCHAR	arrRedRat[MAX_PATH];

	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;

	m_IRList.GetClientRect(&m_Rc);
	m_IRList.SetExtendedStyle(dwStyle);
	m_IRList.InsertColumn(1, _T("IR_Device"), LVCFMT_LEFT, m_Rc.Width());
	CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__);
	iIRNum = CTBSRedRat::tbs_ir_get_info(cstrName);
	for (INT i = 0; i < iIRNum; i++)
	{
		////����Ѿ���ʹ�õ�RedRat�豸
		//for (INT ii = 0; ii < 8; ii++)
		//{
		//	if (CTBSCommon::m_PresentThread->cstrRedRat == cstrName[i])
		//	{
		//		break;
		//	}
		//	if (ii == 7)
		//	{
		//		m_IRList.InsertItem(1, cstrName[i]);
		//	}
		//}
		GetPrivateProfileString(L"RedRat", L"Device", L"", arrRedRat, MAX_PATH, CONFIGFILE);
		wregex m_RegRedRat(cstrName[i]);
		if (regex_search(arrRedRat, m_RegRedRat))
		{
			continue;
		}
		m_IRList.InsertItem(1, cstrName[i]);
	}
	return TRUE;  
}

void CTBSSelectIRDlg::OnBnClickedOk()
{
	WCHAR	arrRedRat[MAX_PATH];
	CString cstrItem;
	CString	cstrNum;
	wstring cstrWrite;
	INT		IFlag = 0;//�Ƿ��Ѿ���IR�豸��������Ҫ����ѡ��ı�־λ��

	
	for (INT i = 0; i < iIRNum; i++)
	{
		if (m_IRList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			cstrItem=m_IRList.GetItemText(i, 0);
			if (CTBSCommon::m_PresentThread->cstrRedRat != L"")
			{
				if (IDCANCEL == ::MessageBox(NULL, L"���Ѿ�ѡ���������豸��\n" + CTBSCommon::m_PresentThread->cstrRedRat + "\n�Ƿ�Ҫ����ѡ��?", L"��ʾ", MB_OKCANCEL))
				{
					CDialogEx::OnOK();
					return;//������ѡ��
				}
				else
				{
					IFlag = 1;
				}
			}

			GetPrivateProfileString(L"RedRat", L"Device", L"", arrRedRat, MAX_PATH, CONFIGFILE);
			wregex m_RegRedRat(cstrItem);
			if (regex_search(arrRedRat, m_RegRedRat))
			{
				AfxMessageBox(L"���豸�ѱ�ռ�ã�������ѡ��");
				return;
			}

			if (IFlag == 1)
			{
				cstrWrite=regex_replace(arrRedRat, (wregex)CTBSCommon::m_PresentThread->cstrRedRat, cstrItem.GetBuffer());
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, (CString)cstrWrite.c_str());
			}
			else
			{
				cstrWrite = (CString)arrRedRat + cstrItem + L",";
				CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, (CString)cstrWrite.c_str());
			}
			
			WritePrivateProfileString(L"RedRat", L"Device", (CString)cstrWrite.c_str(), CONFIGFILE);
			CTBSCommon::m_PresentThread->cstrRedRat = cstrItem;
		}
	}
	CDialogEx::OnOK();
}