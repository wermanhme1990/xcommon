// FunnyStyle.h: interface for the CFunnyStyle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNNYSTYLE_H__B895C338_1BB4_415B_B79A_BF53BC9BB036__INCLUDED_)
#define AFX_FUNNYSTYLE_H__B895C338_1BB4_415B_B79A_BF53BC9BB036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFunnyStyle : public CBCGPVisualManager2003  
{
	DECLARE_DYNCREATE(CFunnyStyle)

public:
	CFunnyStyle();
	virtual ~CFunnyStyle();

	virtual void OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, 
									CBCGPBaseControlBar* pBar);
	virtual COLORREF OnFillMiniFrameCaption (CDC* pDC, CRect rectCaption, 
											CBCGPMiniFrameWnd* pFrameWnd, 
											BOOL bActive);
	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);
	virtual void OnHighlightMenuItem (CDC* pDC, CBCGPToolbarMenuButton* pButton,
									CRect rect, COLORREF& clrText);
	virtual void OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar, CRect rect, BOOL bHorz);
	virtual void OnEraseTabsArea (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);

	virtual COLORREF OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons);

	virtual void OnFillOutlookPageButton (CDC* pDC, const CRect& rect,
										BOOL bIsHighlighted, BOOL bIsPressed,
										COLORREF& clrText);
	virtual void OnDrawOutlookPageButtonBorder (CDC* pDC, CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed);
	virtual COLORREF GetToolbarButtonTextColor (CBCGPToolbarButton* pButton,
												CBCGPVisualManager2003::BCGBUTTON_STATE state);
	virtual void OnDrawSlider (CDC* pDC, CBCGPSlider* pSlider, CRect rect, BOOL bAutoHideMode);
};

#endif // !defined(AFX_FUNNYSTYLE_H__B895C338_1BB4_415B_B79A_BF53BC9BB036__INCLUDED_)
