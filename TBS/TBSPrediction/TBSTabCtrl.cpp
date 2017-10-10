// TBSTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TBSPrediction.h"
#include "TBSTabCtrl.h"


// CTBSTabCtrl

IMPLEMENT_DYNAMIC(CTBSTabCtrl, CTabCtrl)

CTBSTabCtrl::CTBSTabCtrl()
{
	uiFlag = 0;
	uiHeight = 0;
}

CTBSTabCtrl::~CTBSTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CTBSTabCtrl, CTabCtrl)
END_MESSAGE_MAP()



// CTBSTabCtrl 消息处理程序


void CTBSTabCtrl::PreSubclassWindow()
{
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
	CTabCtrl::PreSubclassWindow();
}

void CTBSTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType == ODT_TAB)
	{
		INT		iTabIndex = lpDrawItemStruct->itemID;
		INT		iSavedDC;
		CRect	m_Rect = lpDrawItemStruct->rcItem;
		uiHeight = m_Rect.Height();
		WCHAR	label[64];

		COLORREF m_BackColor;
		COLORREF m_TextColor;
		TC_ITEM	 tci;

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		if (iTabIndex < 0 || !pDC)
		{
			return;
		}
		iSavedDC = pDC->SaveDC();
		tci.mask = TCIF_TEXT | TCIF_IMAGE;
		tci.pszText = label;
		tci.cchTextMax = 63;
		GetItem(iTabIndex, &tci);
		
		//填充背景色  
		if (lpDrawItemStruct->itemState == CDIS_SELECTED)
		{
			m_BackColor = RGB(255, 255, 255);
			
		}
		else if (uiFlag != 0 && (1 << iTabIndex) & uiFlag)
		{
			m_BackColor = RGB(255, 0, 0);
		}
		else
		{
			m_BackColor = GetSysColor(COLOR_BTNFACE);
		}

		pDC->FillSolidRect(m_Rect, m_BackColor);
		m_Rect.top += ::GetSystemMetrics(SM_CYEDGE);
		pDC->SetBkMode(TRANSPARENT);

		CTBSLog::tbs_log_info(__FILE__, __LINE__, __FUNCTION__, m_Rect.Height());


		//绘制图片  
		CImageList* pImageList = GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			m_Rect.left += pDC->GetTextExtent(_T(" ")).cx;       // Margin  

															   // Get height of image so we   
			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect ImageRect(info.rcImage);
			INT nYpos = m_Rect.top;

			pImageList->Draw(pDC, tci.iImage, CPoint(m_Rect.left, nYpos), ILD_TRANSPARENT);
			m_Rect.left += ImageRect.Width();
		}

		//绘制字体  
		if (lpDrawItemStruct->itemState & CDIS_SELECTED)
		{
			m_Rect.top -= ::GetSystemMetrics(SM_CYEDGE);
			m_TextColor = RGB(0, 0, 0);
		}
		else
		{
			m_TextColor = GetSysColor(COLOR_WINDOWTEXT);
		}

		pDC->SetTextColor(m_TextColor);
		pDC->DrawText(label, m_Rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		pDC->RestoreDC(iSavedDC);
	}

}

//BOOL CTBSTabCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//{
//	dwStyle |= TCS_OWNERDRAWFIXED;
//	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//}