// MacStyleSkin.h: interface for the CMacStyleSkin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACSTYLESKIN_H__AE245B2C_F22B_4B8C_93F0_2BF2FC81FFCB__INCLUDED_)
#define AFX_MACSTYLESKIN_H__AE245B2C_F22B_4B8C_93F0_2BF2FC81FFCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMacStyleSkin : public CBCGPVisualManager
{
	DECLARE_DYNCREATE(CMacStyleSkin)

public:
	CMacStyleSkin();
	virtual ~CMacStyleSkin();

	virtual void OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL bNCArea = FALSE);
	virtual void OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect);
	virtual void OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect);
	virtual void OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, CBCGPBaseControlBar* pBar);
	virtual void OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, BOOL bActive, BOOL bHorz, BOOL bMaximized, BOOL bDisabled);
	virtual void OnFillButtonInterior (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual void OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual void OnHighlightMenuItem (CDC *pDC, CBCGPToolbarMenuButton* pButton,
		CRect rect, COLORREF& clrText);
	virtual void OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed);
	virtual void OnEraseTabsArea (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd);
	virtual void OnDrawTab (CDC* pDC, CRect rectTab,
							int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd);

	virtual COLORREF OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons);
	virtual COLORREF GetToolbarButtonTextColor (CBCGPToolbarButton* pButton,
												CBCGPVisualManager::BCGBUTTON_STATE state);
	virtual COLORREF GetCaptionBarTextColor (CBCGPCaptionBar* pBar);
	virtual CSize GetButtonExtraBorder () const
	{
		return CSize (8, 8);
	}

	virtual BOOL IsLook2000Allowed () const
	{
		return FALSE;
	}

	virtual BOOL OnFillOutlookPageButton (	CBCGPButton* pButton,
											CDC* pDC, const CRect& rectClient,
											COLORREF& clrText);

	virtual void OnDrawComboDropButton (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	virtual void OnDrawComboBorder (CDC* pDC, CRect rect,
										BOOL bDisabled,
										BOOL bIsDropped,
										BOOL bIsHighlighted,
										CBCGPToolbarComboBoxButton* pButton);
	CBrush	m_brBarBkgnd;
	CBrush	m_brMenuBkgnd;
	CBrush	m_brMenuHighlighted;

	void FillArea (CDC* pDC, CRect rect, const CWnd* pWnd, BOOL bIsFloating, BOOL bNCArea);
};

#endif // !defined(AFX_MACSTYLESKIN_H__AE245B2C_F22B_4B8C_93F0_2BF2FC81FFCB__INCLUDED_)
