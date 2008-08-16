// MacStyleSkin.cpp: implementation of the CMacStyleSkin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "MacStyle.h"
#include "MacStyleSkin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static COLORREF clrBtnShadow = RGB (176, 186, 201);
static COLORREF clrBtnFace = RGB (230, 230, 230);
static COLORREF clrBtnLight = RGB (255, 255, 255);
static COLORREF clrHotText = RGB (150, 0, 24);
static COLORREF clrBkCaption = RGB (190, 199, 214);
static COLORREF clrFgCaption = RGB (181, 190, 206);
static COLORREF clrBtnText = RGB (20, 20, 20);
static COLORREF clrBtnDisabledText = RGB (128, 128, 128);

IMPLEMENT_DYNCREATE(CMacStyleSkin, CBCGPVisualManager)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMacStyleSkin::CMacStyleSkin()
{
	WORD hatchBitsBar [8] = { 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00 };

	CBitmap bmpBar;
	bmpBar.CreateBitmap (8, 8, 1, 1, hatchBitsBar);

	m_brBarBkgnd.CreatePatternBrush (&bmpBar);

	WORD hatchBitsMenu [8] = { 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00 };

	CBitmap bmpMenu;
	bmpMenu.CreateBitmap (8, 8, 1, 1, hatchBitsMenu);

	m_brMenuBkgnd.CreatePatternBrush (&bmpMenu);

	m_brMenuHighlighted.CreateSolidBrush (RGB (52, 108, 190));

	m_bLook2000 = TRUE;
}

CMacStyleSkin::~CMacStyleSkin()
{

}

void CMacStyleSkin::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
								CRect rectClient, CRect rectClip,
								BOOL bNCArea)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (!bNCArea)
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect (&rectClient);

		pDC->SelectClipRgn (&rgn);
	}

	FillArea (pDC, rectClient, pBar, pBar->IsFloating (), bNCArea);

	if (!bNCArea)
	{
		pDC->SelectClipRgn (NULL);
	}
}

void CMacStyleSkin::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	ASSERT_VALID(pBar);
	ASSERT_VALID(pDC);

	DWORD dwStyle = pBar->GetBarStyle ();
	if (!(dwStyle & CBRS_BORDER_ANY))
		return;

	COLORREF clrBckOld = pDC->GetBkColor ();	// FillSolidRect changes it

	if(pBar->GetBarStyle () & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, 0, 1, rect.Height() - 1, clrBtnShadow);
	if(pBar->GetBarStyle () & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.Width()-1 , 1, clrBtnShadow);
	if(pBar->GetBarStyle () & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect.right, 0/*RGL~:1*/, -1,
			rect.Height()/*RGL-: - 1*/, clrBtnShadow);	
	if(pBar->GetBarStyle () & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect.bottom, rect.Width()-1, -1, clrBtnShadow);

	if (dwStyle & CBRS_BORDER_LEFT)
		++rect.left;
	if (dwStyle & CBRS_BORDER_TOP)
		++rect.top;
	if (dwStyle & CBRS_BORDER_RIGHT)
		--rect.right;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		--rect.bottom;

	// Restore Bk color:
	pDC->SetBkColor (clrBckOld);
}

void CMacStyleSkin::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect)
{
	ASSERT_VALID (pDC);

	COLORREF clrTextOld = pDC->SetTextColor (clrBtnLight);
	COLORREF clrBkOld = pDC->SetBkColor (clrBtnFace);

	pDC->FillRect (rect, &m_brMenuBkgnd);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);

	pDC->Draw3dRect (&rect,  clrBtnShadow, clrBtnShadow);
}

void CMacStyleSkin::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz, 
									   CBCGPBaseControlBar* pBar)
{
	ASSERT_VALID (pBar);

	BOOL bSideBar = pBar->IsKindOf (RUNTIME_CLASS (CBCGPDockingControlBar));
	if (!bSideBar)
	{
		CBCGPVisualManager::OnDrawBarGripper (pDC, rectGripper, bHorz, pBar);
		return;
	}

	ASSERT_VALID (pDC);

	COLORREF clrTextOld = pDC->SetTextColor (clrFgCaption);
	COLORREF clrBkOld = pDC->SetBkColor (clrBkCaption);

    CWnd* pFocus = pBar->GetFocus();
    if (pFocus->GetSafeHwnd () != NULL && pBar->IsChild (pFocus))
	{
		pDC->FillRect (rectGripper, &m_brBarBkgnd);
	}

	//------------------
	// Draw bar caption:
	//------------------
	int nOldBkMode = pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (0);

	const CFont& font = CBCGPMenuBar::GetMenuFont (!bHorz);

	CFont* pOldFont = pDC->SelectObject ((CFont*) &font);

	CString strCaption;
	pBar->GetWindowText (strCaption);
	strCaption = _T(" ") + strCaption + _T(" ");

	CRect rectText = rectGripper;
	UINT uiTextFormat = 0;

	TEXTMETRIC tm;
	pDC->GetTextMetrics (&tm);

	CPoint ptTextOffset (2, 2);

	if (!bHorz)
	{
		ptTextOffset.y = (rectGripper.Height () - tm.tmHeight - 1) / 2;
		rectText.top += ptTextOffset.y;
		pDC->DrawText (strCaption, &rectText, uiTextFormat);
	}
	else
	{
		ptTextOffset.x = (rectGripper.Width () - tm.tmHeight + 1) / 2;
		rectText.left = rectText.right - ptTextOffset.x;
		rectText.top = rectGripper.top + ptTextOffset.y;
		rectText.bottom = rectGripper.top + 3 * ptTextOffset.y;

		uiTextFormat |= DT_NOCLIP;

		pDC->DrawText (strCaption, &rectText, uiTextFormat);
	}

	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nOldBkMode);
	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}

void CMacStyleSkin::OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton,BOOL bActive,  
										  BOOL bHorz, BOOL bMaximized, BOOL bDisabled)
{
	ASSERT_VALID (pDC);

	BOOL bPushed = pButton->m_bPushed;
	BOOL bFocused = pButton->m_bFocused;

	BOOL bIsDown = (!bDisabled && bFocused && bPushed);

	COLORREF clrShadow = clrBtnShadow;
	COLORREF clrLight = clrBtnLight;
	COLORREF clrFace = clrBtnFace;

	CBCGPDrawManager dm (*pDC);

	if (!bDisabled && (bFocused || bPushed))
	{
		BOOL bIsClose = pButton->m_nHit == HTCLOSE_BCG;	// Otherwise - maximize

		clrShadow = bIsClose ? RGB (236, 107, 118) : RGB (118, 107, 236);
		clrLight = bIsClose ? RGB (254, 248, 248) : RGB (248, 254, 248);
		clrFace = clrBtnLight;
	}

	CRect rect = pButton->GetRect ();
	rect.DeflateRect (2, 2);

	dm.DrawGradientRing (rect,
		bIsDown ? clrShadow : clrLight,
		bIsDown ? clrLight : clrShadow,
		clrShadow, 225, bIsDown ? 5 : 5,
		clrFace);

	pButton->m_bPushed = FALSE;
	pButton->m_bFocused = FALSE;

	CBCGPVisualManager::OnDrawCaptionButton (pDC, pButton, bActive, 
											bHorz, bMaximized, bDisabled);
	// Restore state
	pButton->m_bPushed = bPushed;
	pButton->m_bFocused = bFocused;
}

void CMacStyleSkin::OnFillButtonInterior (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);

	if (CBCGPToolBar::IsCustomizeMode () ||
		(state != ButtonsIsHighlighted && state != ButtonsIsPressed))
	{
		CBCGPVisualManager::OnFillButtonInterior (pDC, pButton, rect, state);
		return;
	}

	BOOL bIsMenuBarItem = FALSE;

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	if (pMenuButton != NULL)
	{
		bIsMenuBarItem = DYNAMIC_DOWNCAST (CBCGPMenuBar, 
			pMenuButton->GetParentWnd ()) != NULL;
	}

	if (bIsMenuBarItem)
	{
		pDC->FillRect (rect, &m_brMenuHighlighted);
		return;
	}

	if (!pButton->IsExtraSize ())
	{
		CBCGPVisualManager::OnFillButtonInterior (pDC, pButton, rect, state);
		return;
	}

	CBCGPDrawManager dm (*pDC);

	if (pButton->m_bTextBelow && pButton->IsHorizontal ())
	{
		int nTextHeight = pButton->GetTextSize ().cy;
		rect.bottom -= nTextHeight;

		if (!pButton->IsKindOf (RUNTIME_CLASS (CBCGPToolbarMenuButton)))
		{
			int xCenter = (rect.left + rect.right) / 2;
			rect.left = xCenter - rect.Height () / 2;
			rect.right = xCenter + rect.Height () / 2;
		}
	}

	rect.DeflateRect (1, 1);

	if (state != ButtonsIsHighlighted &&
		(pButton->m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
	{
		dm.DrawGradientRing (rect, 
			clrBtnShadow,
			clrBtnLight,
			clrBtnShadow, 225, 5,
			clrBtnFace);
		return;
	}

	switch (state)
	{
	case ButtonsIsHighlighted:
		if ((pButton->m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)) == 0)
		{
			dm.DrawGradientRing (rect, 
				clrBtnLight,
				clrBtnShadow,
				clrBtnShadow, 225, 3,
				clrBtnFace);
			return;
		}

	case ButtonsIsPressed:
		dm.DrawGradientRing (rect, 
			clrBtnShadow, 
			clrBtnLight,
			clrBtnShadow, 225, 3,
			clrBtnFace);
		return;
	}

	if (pButton->m_nStyle & TBBS_DISABLED)
	{
		dm.DrawGradientRing (rect, 
			clrBtnLight, 
			clrBtnShadow, 
			-1, 225, 2,
			clrBtnFace);
	}
	else
	{
		dm.DrawGradientRing (rect, 
			clrBtnLight, 
			clrBtnShadow, 
			-1, 225, 4,
			clrBtnFace);
	}
}

void CMacStyleSkin::OnDrawButtonBorder (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	if (CBCGPToolBar::IsCustomizeMode ())
	{
		CBCGPVisualManager::OnDrawButtonBorder (pDC, pButton, rect, state);
		return;
	}

	BOOL bIsMenuBarItem = FALSE;

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	if (pMenuButton != NULL)
	{
		bIsMenuBarItem = DYNAMIC_DOWNCAST (CBCGPMenuBar, 
			pMenuButton->GetParentWnd ()) != NULL;
	}

	if (!bIsMenuBarItem && !pButton->IsExtraSize ())
	{
		CBCGPVisualManager::OnDrawButtonBorder (pDC, pButton, rect, state);
	}

	// Border was drawn in CMacStyleSkin::OnFillButtonInterior
}

void CMacStyleSkin::OnHighlightMenuItem (CDC *pDC, CBCGPToolbarMenuButton* pButton,
	CRect rect, COLORREF& clrText)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuHighlighted);
	clrText = clrBtnLight;
}

void CMacStyleSkin::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
}

void CMacStyleSkin::OnEraseTabsArea (CDC* pDC, CRect rect, const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);

	COLORREF clrTextOld = pDC->SetTextColor (clrBtnLight);
	COLORREF clrBkOld = pDC->SetBkColor (clrBtnFace);

	pDC->FillRect (rect, &m_brBarBkgnd);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}

void CMacStyleSkin::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	#define TEXT_MARGIN				4
	#define IMAGE_MARGIN			4

	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	if (pTabWnd->IsFlatTab ())
	{
		CBCGPVisualManager::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	CPen penGray (PS_SOLID, 1, clrBtnShadow);
	CPen penDkGray (PS_SOLID, 1, RGB (0, 0, 0));
	CPen penLight (PS_SOLID, 1, RGB (0, 0, 0));
	CPen penHiLight (PS_SOLID, 1, clrBtnLight);
	
	CPen* pOldPen = pDC->SelectObject (&penGray);
	ASSERT (pOldPen != NULL);
	
	if (iTab != pTabWnd->GetActiveTab () - 1)
	{
		pDC->MoveTo (rectTab.right, rectTab.top + 3);
		pDC->LineTo (rectTab.right, rectTab.bottom - 3);
	}

	if (bIsActive)
	{
		CBCGPDrawManager dm (*pDC);

		if (pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_BOTTOM)
		{
			CRect rectFace = rectTab;
			rectFace.top -= pTabWnd->IsFlatFrame () ? 1 : 2;
			
			dm.FillGradient (rectFace, clrBtnFace, clrBkCaption, FALSE);
			
			pDC->SelectObject (&penDkGray);

			pDC->MoveTo (rectTab.right, rectTab.top);
			pDC->LineTo (rectTab.right, rectTab.bottom);
			pDC->LineTo (rectTab.left, rectTab.bottom);

			pDC->SelectObject (&penHiLight);
			pDC->LineTo (rectTab.left, rectTab.top);

			pDC->SelectObject (&penLight);
			pDC->MoveTo (rectTab.left + 1, rectTab.bottom - 1);
			pDC->LineTo (rectTab.right - 1, rectTab.bottom - 1);
		}
		else
		{
			CRect rectFace = rectTab;
			rectFace.bottom ++;
			rectFace.left ++;
			
			dm.FillGradient (rectFace, clrBtnFace, clrBkCaption, FALSE);
			
			pDC->SelectObject (&penDkGray);
			pDC->MoveTo (rectTab.right, rectTab.bottom);
			pDC->LineTo (rectTab.right, rectTab.top);

			pDC->SelectObject (&penHiLight);
			pDC->LineTo(rectTab.left, rectTab.top);
			pDC->LineTo(rectTab.left, rectTab.bottom);

			pDC->SelectObject (&penLight);
			pDC->MoveTo (rectTab.left + 1, rectTab.top + 1);
			pDC->LineTo (rectTab.right - 1, rectTab.top + 1);
		}
	}

	CSize sizeImage = pTabWnd->GetImageSize ();
	UINT uiIcon = pTabWnd->GetTabIcon (iTab);
	if (uiIcon == (UINT)-1)
	{
		sizeImage.cx = 0;
	}

	if (sizeImage.cx + IMAGE_MARGIN <= rectTab.Width ())
	{
		const CImageList* pImageList = pTabWnd->GetImageList ();
		if (pImageList != NULL && uiIcon != (UINT)-1)
		{
			//----------------------
			// Draw the tab's image:
			//----------------------
			CRect rectImage = rectTab;

			if (bIsActive && pTabWnd->GetLocation () == CBCGPTabWnd::LOCATION_BOTTOM)
				rectImage.top += ((rectTab.Height () - sizeImage.cy) / 2) - 1;
			else
				rectImage.top += (rectTab.Height () - sizeImage.cy) / 2;
			
			rectImage.bottom = rectImage.top + sizeImage.cy;

			rectImage.left += IMAGE_MARGIN;
			rectImage.right = rectImage.left + sizeImage.cx;

			ASSERT_VALID (pImageList);
			((CImageList*) pImageList)->Draw (pDC, uiIcon, rectImage.TopLeft (), ILD_TRANSPARENT);
		}

		//------------------------------
		// Finally, draw the tab's text:
		//------------------------------
		CRect rcText = rectTab;
		rcText.left += sizeImage.cx + 2 * TEXT_MARGIN;
		rcText.bottom -= (bIsActive) ? 2 : 0;
		
		pDC->SetTextColor (0);

		CString strText;
		pTabWnd->GetTabLabel (iTab, strText);

		pDC->DrawText (strText, rcText, 
					DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
	}

	pDC->SelectObject (pOldPen);
}

void CMacStyleSkin::FillArea (CDC* pDC, CRect rect, const CWnd* pWnd, BOOL bIsFloating,
							BOOL bNCArea)
{
	COLORREF clrTextOld = pDC->SetTextColor (clrBtnLight);
	COLORREF clrBkOld = pDC->SetBkColor (clrBtnFace);

	BOOL bIsMenu =	pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)) ||
					pWnd->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	int xOffset = 0;
	int yOffset = 0;

	if (!bIsFloating)
	{
		CRect rectWnd;

		if (bNCArea)
		{
			pWnd->GetWindowRect (rectWnd);
			rectWnd.OffsetRect (-rectWnd.TopLeft ());
		}
		else
		{
			pWnd->GetWindowRect (rectWnd);
			pWnd->ScreenToClient (&rectWnd);
		}

		xOffset = rectWnd.left % 8;
		yOffset = rectWnd.top % 8;
	}

	CBrush* pBrush = bIsMenu ? &m_brMenuBkgnd : &m_brBarBkgnd;
	pBrush->UnrealizeObject ();

	CPoint ptOriginOld = pDC->SetBrushOrg (xOffset, yOffset);

	pDC->FillRect (rect, pBrush);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
	pDC->SetBrushOrg (ptOriginOld);
}
//**************************************************************************************
COLORREF CMacStyleSkin::GetToolbarButtonTextColor (CBCGPToolbarButton* pButton, 
													CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	if (!CBCGPToolBar::IsCustomizeMode ())
	{
		BOOL bIsMenuBarItem = FALSE;
		CBCGPToolbarMenuButton* pMenuButton = 
			DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
		if (pMenuButton != NULL)
		{
			bIsMenuBarItem = DYNAMIC_DOWNCAST (CBCGPMenuBar, 
				pMenuButton->GetParentWnd ()) != NULL;
		}

		if (bIsMenuBarItem && (state == ButtonsIsHighlighted || state == ButtonsIsPressed))
		{
			return RGB (255, 255, 255);
		}
	}

	BOOL bDisabled = (CBCGPToolBar::IsCustomizeMode () && !pButton->IsEditable ()) ||
		(!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED));

	if (state == ButtonsIsHighlighted)
	{
		return clrHotText;
	}

	return bDisabled ? clrBtnDisabledText : clrBtnText;
}
//***************************************************************************************
BOOL CMacStyleSkin::OnFillOutlookPageButton (CBCGPButton* pButton,
											CDC* pDC, const CRect& rectClient,
											COLORREF& clrText)
{
	ASSERT_VALID (pButton);
	ASSERT_VALID (pDC);

	CBCGPDrawManager dm (*pDC);

	BOOL bHot = pButton->IsHighlighted () || pButton->IsPressed ();

	COLORREF clrFace = bHot ? clrBtnLight : clrBtnFace;
	COLORREF clrDark = bHot ? clrBtnShadow : clrBkCaption;

	dm.FillGradient (rectClient, clrFace, clrDark, FALSE);

	clrText = bHot ? clrHotText : clrBtnText;

	return TRUE;
}
//**************************************************************************************
void CMacStyleSkin::OnDrawComboDropButton (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	CPoint pointTriangle (
		rect.left + (rect.Width () - CMenuImages::Size ().cx) / 2 + 1,
		rect.top + (rect.Height () - CMenuImages::Size ().cy) / 2 + 1);

//	CMenuImages::IMAGES_IDS idImage = 
//		(bDisabled ? CMenuImages::IdArowDownDsbl : CMenuImages::IdArowDown);

	CMenuImages::IMAGES_IDS idImage = 
		(bDisabled ? CMenuImages::IdArowDown : CMenuImages::IdArowDown);

	CBCGPDrawManager dm (*pDC);

	if (bIsDropped || bIsHighlighted)
	{
		dm.FillGradient (rect, clrBtnShadow, clrBkCaption, FALSE);

		CPen pen (PS_SOLID, 1, clrBtnShadow);
		CPen* pOldPen = pDC->SelectObject (&pen);
		ASSERT (pOldPen != NULL);

		pDC->MoveTo (rect.left, rect.top);
		pDC->LineTo (rect.left, rect.bottom);

		pDC->SelectObject (pOldPen);
	}
	else
	{
		dm.FillGradient (rect, clrBtnFace, clrBkCaption, FALSE);
	}

	pDC->Draw3dRect (rect, clrBtnShadow, clrBtnShadow);
	CMenuImages::Draw (pDC, idImage, pointTriangle);
}
//*************************************************************************************
void CMacStyleSkin::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL /*bDisabled*/,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	rect.DeflateRect (1, 1);
	COLORREF color = (bIsHighlighted || bIsDropped) ? clrBtnText : clrBtnShadow;

	pDC->Draw3dRect (&rect,  color, color);
}
//*********************************************************************************
COLORREF CMacStyleSkin::GetCaptionBarTextColor (CBCGPCaptionBar* /*pBar*/)
{
	return clrBtnText;
}
//**********************************************************************************
COLORREF CMacStyleSkin::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* pBar, 
		BOOL bActive, CRect rectCaption, CRect rectButtons)
{
    CWnd* pFocus = pBar->GetFocus();
    if (pFocus->GetSafeHwnd () != NULL && pBar->IsChild (pFocus))
	{
		return clrHotText;
	}

	return clrBtnText;
}
