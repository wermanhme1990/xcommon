// MyVisualManager.h: interface for the CMyVisualManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYVISUALMANAGER_H__E1D49E71_1437_4493_A62B_FF9912DA8AF8__INCLUDED_)
#define AFX_MYVISUALMANAGER_H__E1D49E71_1437_4493_A62B_FF9912DA8AF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyVisualManager : public CBCGPVisualManager2003  
{
	DECLARE_DYNCREATE (CMyVisualManager)

public:
	CMyVisualManager();
	virtual ~CMyVisualManager();

	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);

	virtual void OnFillPopupWindowBackground (CDC* pDC, CRect rect);
	virtual void OnDrawPopupWindowBorder (CDC* pDC, CRect rect);
	virtual void OnDrawPopupWindowCaption (CDC* pDC, CRect rect);
	virtual void OnErasePopupWindowButton (CDC* pDC, CRect rectClient, CBCGPButton* pButton);
	virtual void OnDrawPopupWindowButtonBorder (CDC* pDC, CRect rectClient, CBCGPButton* pButton);
};

#endif // !defined(AFX_MYVISUALMANAGER_H__E1D49E71_1437_4493_A62B_FF9912DA8AF8__INCLUDED_)
