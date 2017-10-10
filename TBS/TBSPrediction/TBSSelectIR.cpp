// TBSSelectIR.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSSelectIR.h"
#include "afxdialogex.h"


// CTBSSelectIR 对话框

IMPLEMENT_DYNAMIC(CTBSSelectIR, CDialogEx)

CTBSSelectIR::CTBSSelectIR(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOW_IR, pParent)
{

}

CTBSSelectIR::~CTBSSelectIR()
{
}

void CTBSSelectIR::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_IR, m_IRList);
}


BEGIN_MESSAGE_MAP(CTBSSelectIR, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSSelectIR::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CTBSSelectIR::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect	m_Rc;
	DWORD	dwStyle		= m_IRList.GetExtendedStyle();
	CString *cstrName	= new CString[8];

	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;

	m_IRList.GetClientRect(&m_Rc);
	m_IRList.SetExtendedStyle(dwStyle);
	m_IRList.InsertColumn(1, _T("IR_Device"), LVCFMT_LEFT, m_Rc.Width());

	iIRNum = CTBSRedRat::tbs_ir_get_info(cstrName);
	for (INT i = 0; i < iIRNum; i++)
	{
		//检测已经被使用的RedRat设备
		for (INT ii = 0; ii < 8; ii++)
		{
			if (CTBSCommon::m_PresentThread[ii].cstrRedRat == cstrName[i])
			{
				break;
			}
			if (ii == 7)
			{
				m_IRList.InsertItem(1, cstrName[i]);
			}
		}
	}
	return TRUE;  
}

void CTBSSelectIR::OnBnClickedOk()
{
	CString cstrItem;
	CString	cstrNum;
	for (INT i = 0; i < iIRNum; i++)
	{
		if (m_IRList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			cstrItem=m_IRList.GetItemText(i, 0);
			CTBSCommon::m_PresentThread[CTBSCommon::iTBSPresent].cstrRedRat = cstrItem;
		}
	}
	CDialogEx::OnOK();
}