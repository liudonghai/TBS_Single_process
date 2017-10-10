// TBSProjectCreatDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSProjectCreatDlg.h"
#include "afxdialogex.h"
#include "TBSMainThread.h"
#include "TBSCommon.h"
#include <fstream>
#include "TBSPredictionDlg.h"
using namespace std;
// CTBSProjectCreatDlg �Ի���

IMPLEMENT_DYNAMIC(CTBSProjectCreatDlg, CDialogEx)

CTBSProjectCreatDlg::CTBSProjectCreatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CREAT_PROJECT, pParent)
{
	hConnect = NULL;
}

CTBSProjectCreatDlg::~CTBSProjectCreatDlg()
{
}

void CTBSProjectCreatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTBSProjectCreatDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTBSProjectCreatDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTBSProjectCreatDlg ��Ϣ�������


void CTBSProjectCreatDlg::OnBnClickedOk()
{
	CString csProjectName;
	CString csProjectPath;
	CString csProjectRootPath;
	CString csProjectConf;
	CFile	m_ProjectFile;
	GetDlgItem(IDC_PROJECT_NAME)->GetWindowTextW(csProjectName);

	if (csProjectName.IsEmpty())
	{
		AfxMessageBox(L"�����빤������");
		return;
	}
	//PROJECTPATH=L"..\\project";
	csProjectRootPath = PROJECTPATH;
	if(!PathIsDirectory(csProjectRootPath))
	{
		if (!CreateDirectory(csProjectRootPath, NULL))
		{
			AfxMessageBox(L"project�ļ��д���ʧ�ܣ�");
			return;
		}
	}

	csProjectPath = PROJECTPATH + csProjectName;
	if (!PathIsDirectory(csProjectPath))
	{
		if (!CreateDirectory(csProjectPath, NULL))
		{
			AfxMessageBox(csProjectPath+L"�ļ��д���ʧ�ܣ�");
			return;
		}
	}

	csProjectConf = csProjectPath + L"\\" + csProjectName + L".ini";
	if (CTBSCommon::tbs_is_file_exist(csProjectConf))
	{
		AfxMessageBox(L"�ù������Ѵ��ڣ�");
		return;
	}

	m_ProjectFile.Open(csProjectConf, CFile::modeCreate);
	m_ProjectFile.Close();
	WritePrivateProfileString(L"ProjectName", L"Name", csProjectName, csProjectConf);

	CDialogEx::OnOK();
}
