#pragma once


// CTBSTabCtrl

class CTBSTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CTBSTabCtrl)

public:
	CTBSTabCtrl();
	virtual ~CTBSTabCtrl();
	UINT uiFlag;
	UINT uiHeight;
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);

};


