//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
// BCGPVisualManagerXP.cpp: implementation of the CBCGPVisualManagerXP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BCGPVisualManagerXP.h"
#include "BCGPDrawManager.h"
#include "BCGPMenuBar.h"
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPOutlookBarPane.h"
#include "BCGPColorBar.h"
#include "BCGPBaseTabWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPAutoHideDockBar.h"
#include "BCGGlobals.h"
#include "BCGPOutlookButton.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPTaskPaneMiniFrameWnd.h"
#include "BCGPToolbarEditBoxButton.h"
#include "BCGPTasksPane.h"
#include "BCGPCalendarBar.h"
#include "BCGPCalculator.h"
#include "BCGPToolBox.h"
#include "BCGPPopupWindow.h"
#include "BCGPPropList.h"
#include "CustomizeButton.h"
#include "BCGPRibbonButton.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonComboBox.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonLabel.h"
#include "BCGPRibbonStatusBarPane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBCGPVisualManagerXP, CBCGPVisualManager)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManagerXP::CBCGPVisualManagerXP(BOOL bIsTemporary) :
		CBCGPVisualManager (bIsTemporary)
{
	m_bConnectMenuToParent = TRUE;

	m_nVertMargin = 4;
	m_nHorzMargin = 4;
	m_nGroupVertOffset = 4;
	m_nGroupCaptionHeight = 0;
	m_nGroupCaptionHorzOffset = 0;
	m_nGroupCaptionVertOffset = 0;
	m_nTasksHorzOffset = 12;
	m_nTasksIconHorzOffset = 5;
	m_nTasksIconVertOffset = 4;
	m_bActiveCaptions = FALSE;
	
	m_bMenuFlatLook = TRUE;
	m_bShadowHighlightedImage = TRUE;
	m_bEmbossDisabledImage = FALSE;
	m_bFadeInactiveImage = TRUE;
	m_bLook2000 = TRUE;
	m_nMenuShadowDepth = 4;
	m_nMenuBorderSize = 1;
	m_bShdowDroppedDownMenuButton = TRUE;

	m_bOfficeXPStyleMenus = TRUE;
	m_bDrawLastTabLine = TRUE;

	globalData.UpdateSysColors ();
	OnUpdateSystemColors ();
}
//**********************************************************************************
CBCGPVisualManagerXP::~CBCGPVisualManagerXP()
{
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnUpdateSystemColors ()
{
	CBCGPVisualManager::OnUpdateSystemColors ();

	m_brBarBkgnd.DeleteObject ();
	m_brMenuRarelyUsed.DeleteObject ();
	m_brMenuLight.DeleteObject ();
	m_brHighlight.DeleteObject ();
	m_brHighlightDn.DeleteObject ();
	m_brHighlightChecked.DeleteObject ();

	m_brFloatToolBarBorder.DeleteObject ();

	m_penSeparator.DeleteObject ();
	m_brTabBack.DeleteObject ();

	COLORREF clrTabBack;
	COLORREF clrFloatToolBarBorder;

	if (globalData.m_nBitsPerPixel > 8 && !globalData.IsHighContastMode ())
	{
		COLORREF clrWindow = GetWindowColor ();
		COLORREF clrFace = globalData.clrBarFace;

		m_clrMenuLight = RGB (
			(219 * GetRValue (clrWindow) + 36 * GetRValue (clrFace)) / 255,
			(219 * GetGValue (clrWindow) + 36 * GetGValue (clrFace)) / 255,
			(219 * GetBValue (clrWindow) + 36 * GetBValue (clrFace)) / 255);

		double H, S, L;
		CBCGPDrawManager::RGBtoHSL (clrFace, &H, &S, &L);

		double S1;
		double L1;

		if (S < 0.1)
		{
			L1 = min (1., L + (1. - L) * .5);
			S1 = S == 0 ? 0 : min (1., S + .1);
		}
		else
		{
			L1 = min (1., 0.5 * L + 0.5);
			S1 = min (1., S * 2);
		}

		clrTabBack = CBCGPDrawManager::HLStoRGB_ONE (H, L1, S1);

		m_clrBarBkgnd = RGB (
			(40 * GetRValue (clrWindow) + 215 * GetRValue (clrFace)) / 255,
			(40 * GetGValue (clrWindow) + 215 * GetGValue (clrFace)) / 255,
			(40 * GetBValue (clrWindow) + 215 * GetBValue (clrFace)) / 255);

		m_clrMenuRarelyUsed = CBCGPDrawManager::PixelAlpha (
			m_clrBarBkgnd, 94);

		m_clrInactiveTabText = CBCGPDrawManager::PixelAlpha (clrFace, 55);

		COLORREF clrHL = globalData.clrHilite;
		CBCGPDrawManager::RGBtoHSL (clrHL, &H, &S, &L);

		COLORREF clrMix = RGB (
			(77 * GetRValue (clrHL) + 178 * GetRValue (m_clrMenuLight)) / 255,
			(77 * GetGValue (clrHL) + 178 * GetGValue (m_clrMenuLight)) / 255,
			(77 * GetBValue (clrHL) + 178 * GetBValue (m_clrMenuLight)) / 255);

		if (L > .8)	// The highlight color is very light
		{
			m_clrHighlight = CBCGPDrawManager::PixelAlpha (clrMix, 91);
			m_clrHighlightDn = CBCGPDrawManager::PixelAlpha (clrMix, 98);
			m_clrMenuItemBorder = CBCGPDrawManager::PixelAlpha (globalData.clrHilite, 84);
		}
		else
		{
			m_clrHighlight = CBCGPDrawManager::PixelAlpha (clrMix, 102);
			m_clrHighlightDn = CBCGPDrawManager::PixelAlpha (m_clrHighlight, 87);
			m_clrMenuItemBorder = globalData.clrHilite;
		}

		m_clrHighlightChecked = CBCGPDrawManager::PixelAlpha (RGB (
			(GetRValue (clrHL) + 5 * GetRValue (m_clrMenuLight)) / 6,
			(GetGValue (clrHL) + 5 * GetGValue (m_clrMenuLight)) / 6,
			(GetBValue (clrHL) + 5 * GetBValue (m_clrMenuLight)) / 6), 
			100);

		m_clrSeparator = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarFace, .86, .86, .86);

		m_clrPaneBorder = globalData.clrBarShadow;

		m_clrMenuBorder = CBCGPDrawManager::PixelAlpha (
			clrFace, 55);

		clrFloatToolBarBorder = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarShadow, .85, .85, .85);

		m_clrGripper = CBCGPDrawManager::PixelAlpha (
			globalData.clrBarShadow, 110);
	}
	else
	{
		m_clrMenuLight = globalData.clrWindow;

		m_clrBarBkgnd = globalData.clrBtnFace;

		if (globalData.m_bIsBlackHighContrast)
		{
			m_clrHighlightChecked = m_clrHighlightDn = m_clrHighlight = globalData.clrHilite;
			m_clrMenuRarelyUsed = globalData.clrBtnFace;
		}
		else
		{
			m_clrHighlightDn = m_clrHighlight = globalData.clrBtnFace;
			m_clrHighlightChecked = globalData.clrWindow;
			m_clrMenuRarelyUsed = globalData.clrBarLight;
		}

		clrTabBack = globalData.clrBtnFace;
		m_clrInactiveTabText = globalData.clrBtnDkShadow;
		m_clrSeparator = globalData.clrBtnShadow;
		m_clrGripper = globalData.clrBtnShadow;
		m_clrPaneBorder = globalData.clrBtnShadow;
		m_clrMenuBorder = globalData.clrBtnDkShadow;
		clrFloatToolBarBorder = globalData.clrBtnShadow;

		m_clrMenuItemBorder = globalData.IsHighContastMode () ?
			globalData.clrBtnDkShadow : globalData.clrHilite;
	}

	m_brBarBkgnd.CreateSolidBrush (m_clrBarBkgnd);
	m_brMenuRarelyUsed.CreateSolidBrush (m_clrMenuRarelyUsed);
	m_brMenuLight.CreateSolidBrush (m_clrMenuLight);

	m_brHighlight.CreateSolidBrush (m_clrHighlight);
	m_brHighlightDn.CreateSolidBrush (m_clrHighlightDn);
	m_brHighlightChecked.CreateSolidBrush (m_clrHighlightChecked);
	m_brTabBack.CreateSolidBrush (clrTabBack);
	m_penSeparator.CreatePen (PS_SOLID, 1, m_clrSeparator);

	m_brFloatToolBarBorder.CreateSolidBrush (clrFloatToolBarBorder);

	m_clrPressedButtonBorder = (COLORREF)-1;	// Used in derived classes

	m_penMenuItemBorder.DeleteObject ();
	m_penMenuItemBorder.CreatePen (PS_SOLID, 1, m_clrMenuItemBorder);
}
//***************************************************************************************
void CBCGPVisualManagerXP::OnDrawBarGripper (CDC* pDC, CRect rectGripper, BOOL bHorz,
									   CBCGPBaseControlBar* pBar)
{
	ASSERT_VALID (pDC);

	if (pBar != NULL && pBar->IsDialogControl ())
	{
		CBCGPVisualManager::OnDrawBarGripper (pDC, rectGripper, bHorz, pBar);
		return;
	}

	if (m_brGripperHorz.GetSafeHandle () == NULL)
	{
		CreateGripperBrush ();
	}

	BOOL bSideBar = pBar != NULL && pBar->IsKindOf (RUNTIME_CLASS (CBCGPDockingControlBar));
	BOOL bMenuBar = pBar != NULL && pBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar));

	CRect rectFill = rectGripper;

	if (!bSideBar)
	{
		if (bHorz)
		{
			int xCenter = rectFill.CenterPoint ().x;
			rectFill.left = xCenter - 1;
			rectFill.right = xCenter + 2;
			rectFill.DeflateRect (0, 5);
		}
		else
		{
			int yCenter = rectFill.CenterPoint ().y;
			rectFill.top = yCenter - 1;
			rectFill.bottom = yCenter + 2;
			rectFill.DeflateRect (5, 0);
		}
	}
	else
	{
		if (bHorz)
		{
			rectFill.DeflateRect (4, 0);
		}
		else
		{
			rectFill.DeflateRect (4, 0);
		}

		bHorz = !bHorz;
	}

	COLORREF clrTextOld = pDC->SetTextColor (m_clrGripper);
	COLORREF clrBkOld = pDC->SetBkColor (bSideBar || bMenuBar ? 
		globalData.clrBarFace : m_clrBarBkgnd);

	pDC->FillRect (rectFill, bHorz ? &m_brGripperHorz : &m_brGripperVert);

	if (bSideBar)
	{
		//------------------
		// Draw bar caption:
		//------------------
		int nOldBkMode = pDC->SetBkMode (OPAQUE);
		pDC->SetTextColor (globalData.clrBarText);

		const CFont& font = CBCGPMenuBar::GetMenuFont (bHorz);

		CFont* pOldFont = pDC->SelectObject ((CFont*) &font);

		CString strCaption;
		pBar->GetWindowText (strCaption);
		strCaption = _T(" ") + strCaption + _T(" ");

		CRect rectText = rectGripper;
		UINT uiTextFormat = 0;

		TEXTMETRIC tm;
		pDC->GetTextMetrics (&tm);

		CPoint ptTextOffset (0, 0);
		if (bHorz)
		{
			ptTextOffset.y = (rectGripper.Height () - tm.tmHeight - 1) / 2;
		}
		else
		{
			ptTextOffset.x = (rectGripper.Width () - tm.tmHeight + 1) / 2;
		}

		if (bHorz)
		{
			rectText.top += ptTextOffset.y;
			pDC->DrawText (strCaption, &rectText, uiTextFormat);
		}
		else
		{
			rectText.left = rectText.right - ptTextOffset.x;
			rectText.top = rectGripper.top + ptTextOffset.y;
			rectText.bottom = rectGripper.top + 3 * ptTextOffset.y;

			uiTextFormat |= DT_NOCLIP;

			pDC->DrawText (strCaption, &rectText, uiTextFormat);
		}

		pDC->SelectObject(pOldFont);
		pDC->SetBkMode(nOldBkMode);
	}

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuBorder (CDC* pDC, CBCGPPopupMenu* pMenu, CRect rect)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pMenu);

	const BOOL bRTL = pMenu->GetExStyle() & WS_EX_LAYOUTRTL;

	pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, m_clrMenuLight, m_clrMenuLight);

	CRect rectLeft (1, 1, 2, rect.bottom - 1);
	pDC->FillRect (rectLeft, &m_brBarBkgnd);

	//------------------------------------------------
	// Quick Customize Office XP like draw popup menu
	//------------------------------------------------
	CBCGPPopupMenu* pParentPopup = pMenu->GetParentPopupMenu();
	if (pParentPopup != NULL)
	{
		if (pParentPopup->IsQuickCustomize () && !bRTL)
		{
			CBCGPToolbarMenuButton* pParentBtn = pMenu->GetParentButton();
			if ((pParentBtn != NULL) && (pParentBtn->IsQuickMode()))
			{
				CBCGPPopupMenu* pParent = (CBCGPPopupMenu*)pMenu->GetParentPopupMenu();

				CRect rcParent;
				pParent->GetWindowRect(rcParent);

				CRect rcCurrent;
				pMenu->GetWindowRect(rcCurrent);

				CBCGPToolbarMenuButton* pBtn = pMenu->GetMenuItem(0);
				CRect rcButton = pBtn->Rect();

				CRect rectBorder;
				rectBorder.SetRectEmpty();

				if (rcParent.left > rcCurrent.left) 
				{
					if (rcParent.top <= rcCurrent.top)
					{
						rectBorder.SetRect (
							rect.right - 1,
							rect.top,
							rect.right + 1,
							rcButton.bottom);
					}
					else
					{
						// up
						rectBorder.SetRect (
							rect.right - 1,
							rect.bottom - rcButton.Height (), 
							rect.right + 1,
							rect.bottom);
					}
				}
				else
				{
					if (rcParent.top <= rcCurrent.top)
					{
						rectBorder.SetRect (
							rect.left - 1,
							rect.top, 
							rect.left + 1,
							rcButton.bottom);
					}
					else
					{
						// up
						rectBorder.SetRect (
							rect.left - 1, 
							rect.bottom - rcButton.Height (),
							rect.left + 1,
							rect.bottom);
					}
				}
				
				if (!rectBorder.IsRectEmpty ())
				{
					pDC->FillRect (rectBorder, &m_brBarBkgnd);
				}
			}
		}
	}

	if (!CBCGPToolBar::IsCustomizeMode ())
	{
		//-------------------------------------
		// "Connect" menu to the parent button:
		//-------------------------------------
		CBCGPToolbarMenuButton* pParentMenuBtn = pMenu->GetParentButton ();
		if (m_bConnectMenuToParent &&
			pParentMenuBtn != NULL && pMenu->GetParentPopupMenu () == NULL && 
			pParentMenuBtn->IsBorder ())
		{
			CRect rectConnect;
			rectConnect.SetRectEmpty ();

			CRect rectParent = pParentMenuBtn->Rect ();
			CWnd* pWnd = pParentMenuBtn->GetParentWnd();
			pWnd->ClientToScreen(rectParent);
			pMenu->ScreenToClient(&rectParent);

			switch (pMenu->GetDropDirection ())
			{
			case CBCGPPopupMenu::DROP_DIRECTION_BOTTOM:
				rectConnect = CRect (rectParent.left + 1, rect.top - 1, rectParent.right - 1, rect.top);
				
				if (rectConnect.Width () > rect.Width () + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_TOP:
				rectConnect = CRect (rectParent.left + 1, rect.bottom, rectParent.right - 1, rect.bottom + 1);

				if (rectConnect.Width () > rect.Width () + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_RIGHT:
				rectConnect = CRect (rect.left - 1, rectParent.top + 1, rect.left, rectParent.bottom - 1);

				if (rectConnect.Height () > rect.Height () + 2)
				{
					return;
				}

				break;

			case CBCGPPopupMenu::DROP_DIRECTION_LEFT:
				rectConnect = CRect (rect.right, rectParent.top + 1, rect.right + 1, rectParent.bottom - 1);

				if (rectConnect.Height () > rect.Height () + 2)
				{
					return;
				}

				break;
			}

			CRect rectBorder = rect;
			rectBorder.InflateRect (1, 1);
			rectConnect.IntersectRect (&rectConnect, &rectBorder);
			rectParent.InflateRect (1, 1);
			rectConnect.IntersectRect (&rectConnect, &rectParent);

			pDC->FillRect (rectConnect, &m_brBarBkgnd);
		}
	}
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuShadow (CDC* pPaintDC, const CRect& rectClient, const CRect& rectExclude,
								int nDepth,  int iMinBrightness,  int iMaxBrightness,  
								CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight, BOOL bRTL)
{
	ASSERT_VALID (pPaintDC);
	ASSERT_VALID (pBmpSaveBottom);
	ASSERT_VALID (pBmpSaveRight);

	if (rectExclude.IsRectNull ())
	{
		//------------------------
		// Simple draw the shadow:
		//------------------------
		CBCGPDrawManager dm (*pPaintDC);
		dm.DrawShadow (rectClient, nDepth, iMinBrightness, iMaxBrightness,
					pBmpSaveBottom, pBmpSaveRight, m_clrMenuShadowBase,
					!bRTL);
	}
	else
	{
		//--------------------------------------------
		// Copy screen content into the memory bitmap:
		//--------------------------------------------
		CDC dcMem;
		if (!dcMem.CreateCompatibleDC (pPaintDC))
		{
			ASSERT (FALSE);
			return;
		}

		//--------------------------------------------
		// Gets the whole menu and changes the shadow.
		//--------------------------------------------
		CRect rectBmp (0, 0, rectClient.Width(), rectClient.Height());
		int cx = rectBmp.Width() + nDepth;
		int cy = rectBmp.Height() + nDepth;
		CBitmap	bmpMem;
		if (!bmpMem.CreateCompatibleBitmap (pPaintDC, cx, cy))
		{
			ASSERT (FALSE);
			return;
		}

		CBitmap* pOldBmp = dcMem.SelectObject(&bmpMem);
		ASSERT (pOldBmp != NULL);

		dcMem.BitBlt (0, 0, cx, cy, pPaintDC, rectClient.left, rectClient.top, SRCCOPY);

		//-----------------
		// Draw the shadow:
		//-----------------
		CBCGPDrawManager dm (dcMem);
		dm.DrawShadow (rectBmp, nDepth, iMinBrightness, iMaxBrightness,
					pBmpSaveBottom, pBmpSaveRight, m_clrMenuShadowBase,
					!bRTL);

		//------------------------------------------
		// Do not cover rectExclude with the shadow:
		//------------------------------------------
		dcMem.BitBlt (rectExclude.left - rectClient.left, rectExclude.top - rectClient.top,
			rectExclude.Width(), rectExclude.Height(), 
			pPaintDC, rectExclude.left, rectExclude.top, SRCCOPY);

		//-----------------------------------------
		// Copy shadowed bitmap back to the screen:
		//-----------------------------------------
		pPaintDC->BitBlt (rectClient.left, rectClient.top, cx, cy, &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pOldBmp);
	}
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnDrawBarBorder (CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
	ASSERT_VALID(pBar);
	ASSERT_VALID(pDC);

	if (pBar->IsDialogControl ())
	{
		CBCGPVisualManager::OnDrawBarBorder (pDC, pBar, rect);
		return;
	}

	DWORD dwBarStyle = pBar->GetBarStyle ();
	if (!(dwBarStyle & CBRS_BORDER_ANY))
	{
		return;
	}

	COLORREF clrBckOld = pDC->GetBkColor ();	// FillSolidRect changes it

	if (dwBarStyle & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, 0, 1, rect.Height() - 1, globalData.clrBarFace);
	if (dwBarStyle & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.Width()-1 , 1, globalData.clrBarFace);
	if (dwBarStyle & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect.right, 0/*RGL~:1*/, -1,
			rect.Height()/*RGL-: - 1*/, globalData.clrBarFace);	
	if (dwBarStyle & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect.bottom, rect.Width()-1, -1, globalData.clrBarFace);

	if (dwBarStyle & CBRS_BORDER_LEFT)
		++rect.left;
	if (dwBarStyle & CBRS_BORDER_TOP)
		++rect.top;
	if (dwBarStyle & CBRS_BORDER_RIGHT)
		--rect.right;
	if (dwBarStyle & CBRS_BORDER_BOTTOM)
		--rect.bottom;

	// Restore Bk color:
	pDC->SetBkColor (clrBckOld);
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnFillBarBackground (CDC* pDC, CBCGPBaseControlBar* pBar,
									CRect rectClient, CRect rectClip,
									BOOL /*bNCArea*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (DYNAMIC_DOWNCAST (CReBar, pBar) != NULL ||
		DYNAMIC_DOWNCAST (CReBar, pBar->GetParent ()))
	{
		FillRebarPane (pDC, pBar, rectClient);
		return;
	}

	if (rectClip.IsRectEmpty ())
	{
		rectClip = rectClient;
	}

	CRuntimeClass* pBarClass = pBar->GetRuntimeClass ();

	if (pBarClass == NULL || pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPMenuBar)))
	{
		CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPOutlookBarPane)))
	{
		CBCGPOutlookBarPane* pOlBar = DYNAMIC_DOWNCAST (CBCGPOutlookBarPane, pBar);
		ASSERT_VALID (pOlBar);

		if (pOlBar->IsBackgroundTexture ())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
			return;
		}
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPColorBar)))
	{
		if (pBar->IsDialogControl ())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, 
				((CBCGPColorBar*) pBar)->IsTearOff () ?
					&m_brBarBkgnd : &m_brMenuLight);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalculator)))
	{
		if (pBar->IsDialogControl ())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, &m_brMenuLight);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPCalendarBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		CRect rectImages = rectClient;
		rectImages.right = rectImages.left + CBCGPToolBar::GetMenuImageSize ().cx + 
					2 * GetMenuImageMargin () + 1;
		return;
	}

    if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPPopupMenuBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		BOOL bQuickMode = FALSE;

		CBCGPPopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, pBar);
		if (!pMenuBar->m_bDisableSideBarInXPMode)
		{
			CWnd* pWnd = pMenuBar->GetParent();

			if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CBCGPPopupMenu)))
			{
				CBCGPPopupMenu* pMenu = DYNAMIC_DOWNCAST (CBCGPPopupMenu, pWnd);

				if (pMenu->IsCustomizePane())
				{
					bQuickMode = TRUE;
				}
			}

			CRect rectImages = rectClient;

			if (bQuickMode)
			{
				rectImages.right = rectImages.left + 2*CBCGPToolBar::GetMenuImageSize ().cx + 
							4 * GetMenuImageMargin () + 4;

			}
			else
			{
				rectImages.right = rectImages.left + CBCGPToolBar::GetMenuImageSize ().cx + 
							2 * GetMenuImageMargin () + 2;
			}

			rectImages.DeflateRect (0, 1);
			pDC->FillRect (rectImages, &m_brBarBkgnd);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPToolBar)))
	{
		if (pBar->IsDialogControl ())
		{
			CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		}
		else
		{
			pDC->FillRect (rectClip, &m_brBarBkgnd);
		}

		return;
	}

	if (pBarClass->IsDerivedFrom (RUNTIME_CLASS (CBCGPAutoHideDockBar)))
	{
		pDC->FillRect (rectClip, &m_brTabBack);
		return;
	}

	CBCGPVisualManager::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawSeparator (CDC* pDC, CBCGPBaseControlBar* pBar,
										 CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (pBar->IsDialogControl ())
	{
		CBCGPVisualManager::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}

	CRect rectSeparator = rect;

	CPen* pOldPen = pDC->SelectObject (&m_penSeparator);
	ASSERT (pOldPen != NULL);

	int x1, x2;
	int y1, y2;

	if (bHorz)
	{
		x1 = x2 = (rect.left + rect.right) / 2;
		y1 = rect.top;
		y2 = rect.bottom - 1;
	}
	else
	{
		y1 = y2 = (rect.top + rect.bottom) / 2;
		x1 = rect.left;
		x2 = rect.right;

		BOOL bIsRibbon = FALSE;

#ifndef BCGP_EXCLUDE_RIBBON
		bIsRibbon = pBar->IsKindOf (RUNTIME_CLASS (CBCGPRibbonPanelMenuBar));
#endif

		if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)) &&
			!bIsRibbon &&
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPColorBar)) && 
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPCalculator)) && 
			!pBar->IsKindOf (RUNTIME_CLASS (CBCGPCalendarBar)))
		{
			
			x1 = rect.left + CBCGPToolBar::GetMenuImageSize ().cx + 
						GetMenuImageMargin () + 1;

			CRect rectBar;
			pBar->GetClientRect (rectBar);

			if (rectBar.right - x2 < 50) // Last item in row
			{
				x2 = rectBar.right;
			}

			if (((CBCGPPopupMenuBar*) pBar)->m_bDisableSideBarInXPMode)
			{
				x1 = 0;
			}

			//---------------------------------
			//	Maybe Quick Customize separator
			//---------------------------------
			if (pBar->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar)))
			{
				CWnd* pWnd = pBar->GetParent();
				if (pWnd != NULL && pWnd->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenu)))
				{
					CBCGPPopupMenu* pMenu = (CBCGPPopupMenu*)pWnd;
					if (pMenu->IsCustomizePane())
					{
						x1 = rect.left + 2*CBCGPToolBar::GetMenuImageSize ().cx + 
								3*GetMenuImageMargin () + 2;
					}
				}
			}
		}
	}

	pDC->MoveTo (x1, y1);
	pDC->LineTo (x2, y2);

	pDC->SelectObject (pOldPen);
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawButtonBorder (CDC* pDC,
		CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (state != ButtonsIsPressed && state != ButtonsIsHighlighted)
	{
		ASSERT ((pButton->m_nStyle & TBBS_CHECKED) == 0);
		return;
	}

	COLORREF clrBorder = m_clrMenuItemBorder;

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	BOOL bIsPressedBorder = !bIsPopupMenu;

	if (bIsMenuButton && !bIsPopupMenu && 
		pMenuButton->IsDroppedDown ())
	{
		bIsPressedBorder = FALSE;

		CBCGPPopupMenu* pPopupMenu= pMenuButton->GetPopupMenu ();
		if (pPopupMenu != NULL && 
			(pPopupMenu->IsWindowVisible () || pPopupMenu->IsShown()))
		{
			clrBorder = m_clrMenuBorder;
			ExtendMenuButton (pMenuButton, rect);

			BOOL bRTL = pPopupMenu->GetExStyle() & WS_EX_LAYOUTRTL;

			if (m_bShdowDroppedDownMenuButton && !bRTL && 
				CBCGPMenuBar::IsMenuShadows () &&
				!CBCGPToolBar::IsCustomizeMode () &&
				globalData.m_nBitsPerPixel > 8 &&
				!globalData.IsHighContastMode () &&
				!pPopupMenu->IsRightAlign ())
			{
				CBCGPDrawManager dm (*pDC);

				dm.DrawShadow (rect, m_nMenuShadowDepth, 100, 75, NULL, NULL,
					m_clrMenuShadowBase);
			}
		}
	}

	const BOOL bIsChecked = (pButton->m_nStyle & TBBS_CHECKED);

	switch (state)
	{
	case ButtonsIsPressed:
		if (bIsPressedBorder && m_clrPressedButtonBorder != (COLORREF)-1 &&
			!bIsChecked &&
			rect.Width () > 5 && rect.Height () > 5)
		{
			clrBorder = m_clrPressedButtonBorder;
		}

	case ButtonsIsHighlighted:
		if (bIsPopupMenu && bIsChecked)
		{
			if (pButton->m_nStyle & TBBS_MARKED)
			{
				clrBorder = m_clrPressedButtonBorder;
			}

			rect.bottom ++;
		}

		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}
}
//*****************************************************************************************
void CBCGPVisualManagerXP::OnFillButtonInterior (CDC* pDC,
	CBCGPToolbarButton* pButton, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (state != ButtonsIsPressed && state != ButtonsIsHighlighted)
	{
		return;
	}

	if (CBCGPToolBar::IsCustomizeMode () && 
		!CBCGPToolBar::IsAltCustomizeMode () && !pButton->IsLocked ())
	{
		return;
	}

	CBCGPToolbarMenuButton* pMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPToolbarMenuButton, pButton);
	BOOL bIsMenuButton = pMenuButton != NULL;

	BOOL bIsPopupMenu = bIsMenuButton &&
		pMenuButton->GetParentWnd () != NULL &&
		pMenuButton->GetParentWnd ()->IsKindOf (RUNTIME_CLASS (CBCGPPopupMenuBar));

	if (!bIsPopupMenu && !m_bEnableToolbarButtonFill)
	{
		return;
	}

	CBrush* pBrush = ((pButton->m_nStyle & TBBS_PRESSED) && !bIsPopupMenu) ? 
		&m_brHighlightDn : &m_brHighlight;

	if (bIsMenuButton && !bIsPopupMenu && pMenuButton->IsDroppedDown ())
	{
		ExtendMenuButton (pMenuButton, rect);
		pBrush = &m_brBarBkgnd;
	}
	
	if (pButton->m_nStyle & TBBS_CHECKED)
	{
		pBrush = (state == ButtonsIsHighlighted) ? 
				&m_brHighlightDn : &m_brHighlightChecked;
	}

	if (bIsMenuButton && (pButton->m_nStyle & TBBS_DISABLED))
	{
		pBrush = &m_brMenuLight;
	}

	switch (state)
	{
	case ButtonsIsPressed:
	case ButtonsIsHighlighted:
		if ((pButton->m_nStyle & TBBS_CHECKED) == 0)
		{
			rect.DeflateRect (1, 1);
		}

		OnFillHighlightedArea (pDC, rect, pBrush, pButton);
	}
}
//************************************************************************************
void CBCGPVisualManagerXP::OnHighlightMenuItem (CDC* pDC, CBCGPToolbarMenuButton* pButton,
											CRect rect, COLORREF& clrText)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CBrush* pBrush = (pButton->m_nStyle & TBBS_DISABLED) ? 
					&m_brMenuLight : &m_brHighlight;

	rect.DeflateRect (1, 0);

	OnFillHighlightedArea (pDC, rect, pBrush, pButton);
	pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

	clrText = GetHighlightedMenuItemTextColor (pButton);
}
//*****************************************************************************
COLORREF CBCGPVisualManagerXP::GetHighlightedMenuItemTextColor (CBCGPToolbarMenuButton* pButton)
{
	ASSERT_VALID (pButton);

	if (pButton->m_nStyle & TBBS_DISABLED)
	{
		return globalData.clrGrayedText;
	}

	if (GetRValue (m_clrHighlight) > 128 &&
		GetGValue (m_clrHighlight) > 128 &&
		GetBValue (m_clrHighlight) > 128)
	{
		return RGB (0, 0, 0);
	}
	else
	{
		return RGB (255, 255, 255);
	}
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnHighlightQuickCustomizeMenuButton (CDC* pDC, 
	CBCGPToolbarMenuButton* /*pButton*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);
	pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);
}
//************************************************************************************
void CBCGPVisualManagerXP::OnHighlightRarelyUsedMenuItems (CDC* pDC, CRect rectRarelyUsed)
{
	ASSERT_VALID (pDC);

	rectRarelyUsed.left --;
	rectRarelyUsed.right = rectRarelyUsed.left + CBCGPToolBar::GetMenuImageSize ().cx + 
		2 * GetMenuImageMargin () + 2;

	pDC->FillRect (rectRarelyUsed, &m_brMenuRarelyUsed);
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawTab (CDC* pDC, CRect rectTab,
						int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
	#define TEXT_MARGIN				4
	#define IMAGE_MARGIN			4

	ASSERT_VALID (pTabWnd);
	ASSERT_VALID (pDC);

	if (pTabWnd->IsFlatTab () || pTabWnd->IsOneNoteStyle () ||
		pTabWnd->IsColored () || pTabWnd->IsVS2005Style () ||
		pTabWnd->IsLeftRightRounded ())
	{
		CBCGPVisualManager::OnDrawTab (pDC, rectTab, iTab, bIsActive, pTabWnd);
		return;
	}

	COLORREF	clrDark;
	COLORREF	clrBlack;
	COLORREF	clrHighlight;
	COLORREF	clrFace;
	COLORREF	clrDarkShadow;
	COLORREF	clrLight;
	CBrush*		pbrFace = NULL;
	CBrush*		pbrBlack = NULL;
				   
	GetTabFrameColors (
		pTabWnd, clrDark, clrBlack, clrHighlight, clrFace, clrDarkShadow, clrLight,
		pbrFace, pbrBlack);

	CPen penGray (PS_SOLID, 1, clrDark);
	CPen penDkGray (PS_SOLID, 1, clrBlack);
	CPen penHiLight (PS_SOLID, 1, clrHighlight);
	
	CPen* pOldPen = pDC->SelectObject (&penGray);
	ASSERT (pOldPen != NULL);
	
	if (iTab != pTabWnd->GetActiveTab () - 1)
	{
		if (iTab < pTabWnd->GetVisibleTabsNum () - 1 || m_bDrawLastTabLine)
		{
			pDC->MoveTo (rectTab.right, rectTab.top + 3);
			pDC->LineTo (rectTab.right, rectTab.bottom - 3);
		}
	}

	if (bIsActive)
	{
		if (pTabWnd->GetLocation () == CBCGPBaseTabWnd::LOCATION_BOTTOM)
		{
			CRect rectFace = rectTab;
			rectFace.top--;
			
			OnFillTab (pDC, rectFace, pbrFace, iTab, bIsActive, pTabWnd);
			
			pDC->SelectObject (&penDkGray);

			pDC->MoveTo (rectTab.right, rectTab.top);
			pDC->LineTo (rectTab.right, rectTab.bottom);
			pDC->LineTo (rectTab.left, rectTab.bottom);

			pDC->SelectObject (&penHiLight);
			pDC->LineTo (rectTab.left, rectTab.top - 2);
		}
		else
		{
			CPen penLight (PS_SOLID, 1, m_clrMenuLight);

			CRect rectFace = rectTab;
			rectFace.bottom++;
			rectFace.left++;
			
			OnFillTab (pDC, rectFace, pbrFace, iTab, bIsActive, pTabWnd);
			
			pDC->SelectObject (&penDkGray);
			pDC->MoveTo (rectTab.right, rectTab.bottom);
			pDC->LineTo (rectTab.right, rectTab.top);

			pDC->SelectObject (&penHiLight);

			pDC->LineTo (rectTab.right, rectTab.top);
			pDC->LineTo(rectTab.left, rectTab.top);
			pDC->LineTo(rectTab.left, rectTab.bottom);
		}
	}

	pDC->SelectObject (pOldPen);

	COLORREF clrText;

	if (pTabWnd->IsDialogControl ())
	{
		clrText = globalData.clrBtnText;
	}
	else
	{
		clrText = bIsActive ? globalData.clrBarText : m_clrInactiveTabText;
	}

	OnDrawTabContent (pDC, rectTab, iTab, bIsActive, pTabWnd, clrText);
}
//*********************************************************************************
void CBCGPVisualManagerXP::OnFillTab (CDC* pDC, CRect rectFill, CBrush* pbrFill,
									 int iTab, BOOL bIsActive, 
									 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pbrFill);
	ASSERT_VALID (pTabWnd);

	if (pTabWnd->GetTabBkColor (iTab) != (COLORREF)-1 && !bIsActive)
	{
		CBrush br (pTabWnd->GetTabBkColor (iTab));
		pDC->FillRect (rectFill, &br);
		return;
	}

	if (pTabWnd->IsOneNoteStyle () || pTabWnd->IsVS2005Style () ||
		pTabWnd->IsLeftRightRounded ())
	{
		CBCGPVisualManager::OnFillTab (pDC, rectFill, pbrFill,
									 iTab, bIsActive, pTabWnd);
	}
	else if (bIsActive)
	{
		pDC->FillRect (rectFill, pbrFill);
	}
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnEraseTabsArea (CDC* pDC, CRect rect, 
										 const CBCGPBaseTabWnd* pTabWnd)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pTabWnd);

	if (pTabWnd->IsFlatTab ())
	{
		CBCGPVisualManager::OnEraseTabsArea (pDC, rect, pTabWnd);
		return;
	}

	if (pTabWnd->IsDialogControl ())
	{
		pDC->FillRect (rect, &globalData.brBtnFace);
		return;
	}

	pDC->FillRect (rect, &m_brTabBack);
}
//**************************************************************************************
COLORREF CBCGPVisualManagerXP::OnDrawControlBarCaption (CDC* pDC, CBCGPDockingControlBar* /*pBar*/, 
			BOOL bActive, CRect rectCaption, CRect /*rectButtons*/)
{
	ASSERT_VALID (pDC);

	CPen pen (PS_SOLID, 1, 
		bActive ? globalData.clrBarLight : globalData.clrBarShadow);
	CPen* pOldPen = pDC->SelectObject (&pen);

	CBrush* pOldBrush = (CBrush*) pDC->SelectObject (
		bActive ? &globalData.brActiveCaption : &globalData.brBarFace);

	if (bActive)
	{
		rectCaption.InflateRect (1, 1);
	}

	pDC->RoundRect (rectCaption, CPoint (2, 2));

	pDC->SelectObject (pOldBrush);
	pDC->SelectObject (pOldPen);

    // get the text color
    COLORREF clrCptnText = bActive ?
        globalData.clrCaptionText :
        globalData.clrBarText;

	return clrCptnText;
}
//****************************************************************************************
void CBCGPVisualManagerXP::OnDrawCaptionButton (CDC* pDC, CBCGPCaptionButton* pButton, 
											BOOL bActive,
											BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
											int nImageID /*= -1*/)
{
	ASSERT_VALID (pDC);
	ASSERT (pButton != NULL);

    CRect rc = pButton->GetRect ();

	if (pButton->m_bPushed && (pButton->m_bFocused || pButton->m_bDroppedDown) && !bDisabled)
	{
		OnFillHighlightedArea (pDC, rc, &m_brHighlightDn, NULL);
		bActive = TRUE;
	}
	else if (pButton->m_bPushed || pButton->m_bFocused || pButton->m_bDroppedDown)
	{
		if (!bDisabled)
		{
			OnFillHighlightedArea (pDC, rc, &m_brHighlight, NULL);
		}

		bActive = FALSE;
	}

	CMenuImages::IMAGES_IDS id = (CMenuImages::IMAGES_IDS)-1;
	
	if (nImageID != -1)
	{
		id = (CMenuImages::IMAGES_IDS)nImageID;
	}
	else
	{
		id = pButton->GetIconID (bHorz, bMaximized);
	}

	if (id != (CMenuImages::IMAGES_IDS)-1)
	{
		CSize sizeImage = CMenuImages::Size ();
		CPoint ptImage (rc.left + (rc.Width () - sizeImage.cx) / 2,
						rc.top + (rc.Height () - sizeImage.cy) / 2);

		OnDrawCaptionButtonIcon (pDC, pButton, id, bActive, bDisabled, ptImage);
	}

	if ((pButton->m_bPushed || pButton->m_bFocused || pButton->m_bDroppedDown) && !bDisabled)
	{
		COLORREF clrDark = globalData.clrBarDkShadow;
		pDC->Draw3dRect (rc, clrDark, clrDark);
	}
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawCaptionButtonIcon (CDC* pDC, 
													CBCGPCaptionButton* pButton,
													CMenuImages::IMAGES_IDS id,
													BOOL bActive, BOOL bDisabled,
													CPoint ptImage)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CMenuImages::IMAGE_STATE imageState;
	
	if (bDisabled)
	{
		imageState = CMenuImages::ImageGray;
	}
	else if (pButton->m_bFocused || pButton->m_bPushed)
	{
		COLORREF clrBack = pButton->m_bPushed ? m_clrHighlightDn : m_clrHighlight;

		if (GetRValue (clrBack) <= 192 &&
			GetGValue (clrBack) <= 192 &&
			GetBValue (clrBack) <= 192)
		{
			imageState = CMenuImages::ImageWhite;
		}
		else
		{
			imageState = CMenuImages::ImageBlack;
		}
	}
	else if (pButton->m_clrForeground == (COLORREF)-1)
	{
		imageState = bActive ? CMenuImages::ImageWhite : CMenuImages::ImageBlack;
	}
	else
	{
		if (GetRValue (pButton->m_clrForeground) > 192 &&
			GetGValue (pButton->m_clrForeground) > 192 &&
			GetBValue (pButton->m_clrForeground) > 192)
		{
			imageState = CMenuImages::ImageWhite;
		}
		else
		{
			imageState = CMenuImages::ImageBlack;
		}
	}

	CMenuImages::Draw (pDC, id, ptImage, imageState);
}
//********************************************************************************
COLORREF CBCGPVisualManagerXP::OnFillCommandsListBackground (CDC* pDC, CRect rect, BOOL bIsSelected)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDC);

	COLORREF clrText = globalData.clrBarText;

	int iImageWidth = CBCGPToolBar::GetMenuImageSize ().cx + GetMenuImageMargin ();

	if (bIsSelected)
	{
		if (m_bEnableToolbarButtonFill)
		{
			rect.left = 0;
		}

		OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);

		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

		// Now, we should define a menu text color...
		if (GetRValue (m_clrHighlight) > 128 &&
			GetGValue (m_clrHighlight) > 128 &&
			GetBValue (m_clrHighlight) > 128)
		{
			clrText = RGB (0, 0, 0);
		}
		else
		{
			clrText = RGB (255, 255, 255);
		}
	}
	else
	{
		pDC->FillRect (rect, &m_brMenuLight);

		CRect rectImages = rect;
		rectImages.right = rectImages.left + iImageWidth + MENU_IMAGE_MARGIN;

		pDC->FillRect (rectImages, &m_brBarBkgnd);

		clrText = globalData.clrBarText;
	}

	return clrText;
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuArrowOnCustomizeList (CDC* pDC, 
	CRect rectCommand, BOOL /*bSelected*/)
{
	ASSERT_VALID (pDC);

	CRect rectTriangle = rectCommand;
	rectTriangle.left = rectTriangle.right - CMenuImages::Size ().cx;

	CMenuImages::IMAGE_STATE state = CMenuImages::ImageBlack;
	COLORREF clrPen = RGB (0, 0, 0);

	if (GetRValue (m_clrHighlight) < 128 ||
		GetGValue (m_clrHighlight) < 128 ||
		GetBValue (m_clrHighlight) < 128)
	{
		state = CMenuImages::ImageWhite;
		clrPen = RGB (255, 255, 255);
	}

	CMenuImages::Draw (pDC, CMenuImages::IdArowRightLarge, rectTriangle, state);

	CPen penLine (PS_SOLID, 1, clrPen);
	CPen* pOldPen = pDC->SelectObject (&penLine);
	ASSERT (pOldPen != NULL);

	pDC->MoveTo (rectTriangle.left - 1, rectCommand.top + 2);
	pDC->LineTo (rectTriangle.left - 1, rectCommand.bottom - 2);

	pDC->SelectObject (pOldPen);
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawTearOffCaption (CDC* pDC, CRect rect, BOOL bIsActive)
{
	const int iBorderSize = 1;
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuLight);

	rect.DeflateRect (iBorderSize, iBorderSize);
	OnFillHighlightedArea (pDC, rect, bIsActive ? &m_brHighlight : &m_brBarBkgnd,
		NULL);
	
	// Draw gripper:
	int nGripperWidth = max (20, CBCGPToolBar::GetMenuImageSize ().cx * 2);

	CRect rectGripper = rect;
	rectGripper.DeflateRect ((rectGripper.Width () - nGripperWidth) / 2, 1);

	if (m_brGripperHorz.GetSafeHandle () == NULL)
	{
		CreateGripperBrush ();
	}

	COLORREF clrTextOld = pDC->SetTextColor (bIsActive ?
		globalData.clrBarDkShadow : globalData.clrBarShadow);
	COLORREF clrBkOld = pDC->SetBkColor (
		bIsActive ? m_clrHighlight : m_clrBarBkgnd);

	if (bIsActive)
	{
		rectGripper.DeflateRect (0, 1);
	}

	pDC->FillRect (rectGripper, &m_brGripperHorz);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);

	if (bIsActive)
	{
		pDC->Draw3dRect (rect, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
	}
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuResizeBar (CDC* pDC, CRect rect, int nResizeFlags)
{
	ASSERT_VALID (pDC);

	const int iBorderSize = 1;
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brMenuLight);

	rect.DeflateRect (iBorderSize, iBorderSize);

	OnFillHighlightedArea (pDC, rect, &m_brBarBkgnd, NULL);
	
	CRect rectGripper = rect;

	if (nResizeFlags == (int) CBCGPPopupMenu::MENU_RESIZE_BOTTOM_RIGHT)
	{
		rectGripper.left = rectGripper.right - rectGripper.Height ();
	}
	else
	{
		rectGripper.left = rectGripper.CenterPoint ().x - rectGripper.Height () / 2;
		rectGripper.right = rectGripper.left + rectGripper.Height ();
	}

	rectGripper.DeflateRect (2, 2);

	if (m_brGripperHorz.GetSafeHandle () == NULL)
	{
		CreateGripperBrush ();
	}

	COLORREF clrTextOld = pDC->SetTextColor (globalData.clrBarShadow);
	COLORREF clrBkOld = pDC->SetBkColor (m_clrBarBkgnd);

	pDC->FillRect (rectGripper, &m_brGripperHorz);

	pDC->SetTextColor (clrTextOld);
	pDC->SetBkColor (clrBkOld);
}
//*************************************************************************************
void CBCGPVisualManagerXP::CreateGripperBrush ()
{
	ASSERT (m_brGripperHorz.GetSafeHandle () == NULL);
	ASSERT (m_brGripperVert.GetSafeHandle () == NULL);

	WORD horzHatchBits [8] = { 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00 };

	CBitmap bmpGripperHorz;
	bmpGripperHorz.CreateBitmap (8, 8, 1, 1, horzHatchBits);

	m_brGripperHorz.CreatePatternBrush (&bmpGripperHorz);

	WORD vertHatchBits[8] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

	CBitmap bmpGripperVert;
	bmpGripperVert.CreateBitmap (8, 8, 1, 1, vertHatchBits);

	m_brGripperVert.CreatePatternBrush (&bmpGripperVert);
}
//***********************************************************************************
void CBCGPVisualManagerXP::ExtendMenuButton (CBCGPToolbarMenuButton* pMenuButton,
											CRect& rect)
{
	ASSERT_VALID (pMenuButton);

	CBCGPPopupMenu* pPopupMenu= pMenuButton->GetPopupMenu ();
	if (pPopupMenu == NULL || pPopupMenu->GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectMenu;
	pPopupMenu->GetWindowRect (rectMenu);

	if (DYNAMIC_DOWNCAST (CCustomizeButton, pMenuButton) != NULL)
	{
		CBCGPBaseControlBar* pParentBar = DYNAMIC_DOWNCAST (
			CBCGPBaseControlBar, pMenuButton->GetParentWnd ());

		if (pParentBar != NULL)
		{
			CRect rectScreen = rect;
			pParentBar->ClientToScreen (&rectScreen);

			if (pParentBar->IsHorizontal ())
			{
				rectScreen.top = rectMenu.top;
				rectScreen.bottom = rectMenu.bottom;
			}
			else
			{
				rectScreen.left = rectMenu.left;
				rectScreen.right = rectMenu.right;
				rectScreen.bottom++;
			}

			CRect rectInter;
			if (!rectInter.IntersectRect (rectScreen, rectMenu))
			{
				return;
			}
		}
	}

	int nGrow = 4;

	switch (pPopupMenu->GetDropDirection ())
	{
	case CBCGPPopupMenu::DROP_DIRECTION_BOTTOM:
		if (rectMenu.Width () < rect.Width ())
		{
			nGrow = 1;
		}

		rect.bottom += nGrow;
		break;

	case CBCGPPopupMenu::DROP_DIRECTION_TOP:
		if (rectMenu.Width () < rect.Width ())
		{
			nGrow = 1;
		}

		rect.top -= nGrow;
		break;

	case CBCGPPopupMenu::DROP_DIRECTION_RIGHT:
		if (rectMenu.Height () < rect.Height ())
		{
			nGrow = 1;
		}

		rect.right += nGrow;
		break;

	case CBCGPPopupMenu::DROP_DIRECTION_LEFT:
		if (rectMenu.Height () < rect.Height ())
		{
			nGrow = 1;
		}

		rect.left -= nGrow;
		break;
	}
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawMenuSystemButton (CDC* pDC, CRect rect, UINT uiSystemCommand, 
										UINT nStyle, BOOL bHighlight)
{
	ASSERT_VALID (pDC);

	BOOL bIsDisabled = (nStyle & TBBS_DISABLED);
	BOOL bIsPressed = (nStyle & TBBS_PRESSED);

	CMenuImages::IMAGES_IDS imageID;

	switch (uiSystemCommand)
	{
	case SC_CLOSE:
		imageID = CMenuImages::IdClose;
		break;

	case SC_MINIMIZE:
		imageID = CMenuImages::IdMinimize;
		break;

	case SC_RESTORE:
		imageID = CMenuImages::IdRestore;
		break;

	default:
		return;
	}

	if (bHighlight && !bIsDisabled)
	{
		OnFillHighlightedArea (pDC, rect, 
			bIsPressed ? &m_brHighlightDn : &m_brHighlight, NULL);

		COLORREF clrBorder = m_clrMenuItemBorder;
		pDC->Draw3dRect (rect, clrBorder, clrBorder);
	}

	CMenuImages::Draw (pDC, imageID, rect,
		bIsDisabled ? CMenuImages::ImageGray : bHighlight ? 
			CMenuImages::ImageWhite : CMenuImages::ImageBlack);
}
//********************************************************************************
void CBCGPVisualManagerXP::OnDrawStatusBarPaneBorder (CDC* pDC, CBCGPStatusBar* /*pBar*/,
					CRect rectPane, UINT /*uiID*/, UINT nStyle)
{
	if (!(nStyle & SBPS_NOBORDERS))
	{
		if (nStyle & SBPS_POPOUT)
		{
			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rectPane);
		}

		// Draw pane border:
		pDC->Draw3dRect (rectPane, m_clrPaneBorder, m_clrPaneBorder);
	}
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawComboDropButton (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	COLORREF clrText = pDC->GetTextColor ();

	if (bIsDropped || bIsHighlighted)
	{
		OnFillHighlightedArea (pDC, rect, 
			bIsDropped ? &m_brHighlightDn : &m_brHighlight,
			NULL);

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawLine (rect.left, rect.top, rect.left, rect.bottom, m_clrMenuItemBorder);
		}
		else
		{
			CPen* pOldPen = pDC->SelectObject (&m_penMenuItemBorder);
			ASSERT (pOldPen != NULL);

			pDC->MoveTo (rect.left, rect.top);
			pDC->LineTo (rect.left, rect.bottom);

			pDC->SelectObject (pOldPen);
		}
	}
	else
	{
		pDC->FillRect (rect, &globalData.brBarFace);

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, globalData.clrWindow);
		}
		else
		{
			pDC->Draw3dRect (rect, globalData.clrBarWindow, globalData.clrBarWindow);
		}
	}

	CMenuImages::Draw (pDC, CMenuImages::IdArowDown, rect,
		bDisabled ? CMenuImages::ImageGray : (bIsDropped && bIsHighlighted) ? 
			CMenuImages::ImageWhite : CMenuImages::ImageBlack);

	pDC->SetTextColor (clrText);
}
//*************************************************************************************
void CBCGPVisualManagerXP::OnDrawComboBorder (CDC* pDC, CRect rect,
												BOOL /*bDisabled*/,
												BOOL bIsDropped,
												BOOL bIsHighlighted,
												CBCGPToolbarComboBoxButton* /*pButton*/)
{
	if (bIsHighlighted || bIsDropped)
	{
		rect.DeflateRect (1, 1);
		pDC->Draw3dRect (&rect,  m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}
//*********************************************************************************
void CBCGPVisualManagerXP::OnEraseTabsButton (CDC* pDC, CRect rect,
											  CBCGPButton* pButton,
											  CBCGPBaseTabWnd* pWndTab)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pWndTab);

	if (pWndTab->IsFlatTab ())
	{
		CBrush* pBrush = pButton->IsPressed () ? 
			&m_brHighlightDn : pButton->IsHighlighted () ? &m_brHighlight : &globalData.brBarFace;

		pDC->FillRect (rect, pBrush);
		OnFillHighlightedArea (pDC, rect, pBrush, NULL);
	}
	else if (pWndTab->IsDialogControl ())
	{
		pDC->FillRect (rect, &globalData.brBtnFace);
	}
	else
	{
		pDC->FillRect (rect, &m_brTabBack);
	}
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawTabsButtonBorder (CDC* pDC, CRect& rect, 
												 CBCGPButton* pButton, UINT /*uiState*/,
												 CBCGPBaseTabWnd* pWndTab)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);
	ASSERT_VALID (pWndTab);

	if (pWndTab->IsFlatTab ())
	{
		if (pButton->IsPushed () || pButton->IsHighlighted ())
		{
			COLORREF clrDark = globalData.clrBarDkShadow;
			pDC->Draw3dRect (rect, clrDark, clrDark);
		}
	}
	else
	{
		if (pButton->IsPushed () || pButton->IsHighlighted ())
		{
			if (pButton->IsPressed ())
			{
				pDC->Draw3dRect (rect, globalData.clrBarDkShadow, m_clrGripper);
			}
			else
			{
				pDC->Draw3dRect (rect, m_clrGripper, globalData.clrBarDkShadow);
			}
		}
	}
}
//*********************************************************************************
COLORREF CBCGPVisualManagerXP::OnFillMiniFrameCaption (CDC* pDC, 
								CRect rectCaption, 
								CBCGPMiniFrameWnd* pFrameWnd,
								BOOL bActive)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

	BOOL bIsToolBar = FALSE;
#ifndef BCGP_EXCLUDE_TASK_PANE
	BOOL bIsTasksPane = pFrameWnd->IsKindOf( RUNTIME_CLASS( CBCGPTaskPaneMiniFrameWnd ) );
#else
	BOOL bIsTasksPane = FALSE;
#endif

	if (DYNAMIC_DOWNCAST (CBCGPBaseToolBar, pFrameWnd->GetControlBar ()) != NULL)
	{
		bActive = FALSE;
		bIsToolBar = TRUE;
	}

	if (bIsToolBar)
	{
		pDC->FillRect (rectCaption, &m_brFloatToolBarBorder);
		return globalData.clrCaptionText;
	}
	else if (bIsTasksPane)
	{
		pDC->FillRect (rectCaption, &globalData.brBarFace);
		return globalData.clrBarText;
	}

	pDC->FillRect (rectCaption, 
		bActive ? &globalData.brActiveCaption : &globalData.brInactiveCaption);

    // get the text color
	return globalData.clrCaptionText;
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawMiniFrameBorder (
										CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
										CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pFrameWnd);

#ifndef BCGP_EXCLUDE_TASK_PANE
	BOOL bIsTasksPane = pFrameWnd->IsKindOf( RUNTIME_CLASS( CBCGPTaskPaneMiniFrameWnd ) );
#else
	BOOL bIsTasksPane = FALSE;
#endif

	if (bIsTasksPane)
	{
		CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
		ASSERT (pOldBrush != NULL);

		pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorderSize.left, rectBorder.Height (), PATCOPY);
		pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorder.Width (), rectBorderSize.top, PATCOPY);
		pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, rectBorder.Height (), PATCOPY);
		pDC->PatBlt (rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width (), rectBorderSize.bottom, PATCOPY);

		rectBorderSize.DeflateRect (2, 2);
		rectBorder.DeflateRect (2, 2);

		pDC->SelectObject (bIsTasksPane ? &globalData.brLight : &globalData.brBarFace);

		pDC->PatBlt (rectBorder.left, rectBorder.top + 1, rectBorderSize.left, rectBorder.Height () - 2, PATCOPY);
		pDC->PatBlt (rectBorder.left + 1, rectBorder.top, rectBorder.Width () - 2, rectBorderSize.top, PATCOPY);
		pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, rectBorder.Height () - 2, PATCOPY);
		pDC->PatBlt (rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width () - 2, rectBorderSize.bottom, PATCOPY);

		pDC->SelectObject (pOldBrush);
	}
	else
	{
		CBCGPVisualManager::OnDrawMiniFrameBorder (pDC, pFrameWnd, rectBorder, rectBorderSize);
	}
}
//**************************************************************************************
void CBCGPVisualManagerXP::OnDrawFloatingToolbarBorder (
												CDC* pDC, CBCGPBaseToolBar* /*pToolBar*/, 
												CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);

	CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
	ASSERT (pOldBrush != NULL);

	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorderSize.left, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorder.Width (), rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width (), rectBorderSize.bottom, PATCOPY);

	rectBorderSize.DeflateRect (2, 2);
	rectBorder.DeflateRect (2, 2);

	pDC->SelectObject (&globalData.brBarFace);

	pDC->PatBlt (rectBorder.left, rectBorder.top + 1, rectBorderSize.left, rectBorder.Height () - 2, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.top, rectBorder.Width () - 2, rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, rectBorder.Height () - 2, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width () - 2, rectBorderSize.bottom, PATCOPY);

	pDC->SelectObject (pOldBrush);
}
//**************************************************************************************
COLORREF CBCGPVisualManagerXP::GetToolbarButtonTextColor (CBCGPToolbarButton* pButton, 
														CBCGPVisualManager::BCGBUTTON_STATE state)
{
	ASSERT_VALID (pButton);

	if (!globalData.IsHighContastMode ())
	{
		BOOL bDisabled = (CBCGPToolBar::IsCustomizeMode () && !pButton->IsEditable ()) ||
			(!CBCGPToolBar::IsCustomizeMode () && (pButton->m_nStyle & TBBS_DISABLED));

		if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPOutlookButton)))
		{
			if (bDisabled)
			{
				return globalData.clrGrayedText;
			}

			return globalData.IsHighContastMode () ? 
				globalData.clrWindowText : globalData.clrBarText;
		}

		if (state == ButtonsIsHighlighted && 
			(pButton->m_nStyle & (TBBS_PRESSED | TBBS_CHECKED)))
		{
			return globalData.clrTextHilite;
		}
	}

	return	CBCGPVisualManager::GetToolbarButtonTextColor (pButton, state);
}
//*************************************************************************************
void CBCGPVisualManagerXP::OnDrawEditBorder (CDC* pDC, CRect rect,
												BOOL bDisabled,
												BOOL bIsHighlighted,
												CBCGPToolbarEditBoxButton* pButton)
{
	if (!CBCGPToolbarEditBoxButton::IsFlatMode ())
	{
		CBCGPVisualManager::OnDrawEditBorder (pDC, rect, bDisabled, bIsHighlighted, pButton);
		return;
	}

	if (bIsHighlighted)
	{
		pDC->Draw3dRect (&rect,  m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}

#ifndef BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManagerXP::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup, 
						BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse)
{
	ASSERT_VALID(pDC);
	ASSERT(pGroup != NULL);
	ASSERT_VALID (pGroup->m_pPage);

#ifndef BCGP_EXCLUDE_TOOLBOX
	BOOL bIsToolBox = pGroup->m_pPage->m_pTaskPane != NULL &&
		(pGroup->m_pPage->m_pTaskPane->IsKindOf (RUNTIME_CLASS (CBCGPToolBoxEx)));
#else
	BOOL bIsToolBox = FALSE;
#endif

	CRect rectGroup = pGroup->m_rect;

	if (bIsToolBox)
	{
		CRect rectFill = rectGroup;
		rectFill.DeflateRect (1, 0, 1, 1);

		CBrush brFill (globalData.IsHighContastMode () ?
			globalData.clrWindow : globalData.clrBarShadow);

		pDC->FillRect (rectFill, &brFill);

		if (bCanCollapse)
		{
			//--------------------
			// Draw expanding box:
			//--------------------
			const int nBoxSize = 9;
			const int nBoxOffset = 6;

			CRect rectButton = rectFill;
			
			rectButton.left += nBoxOffset;
			rectButton.right = rectButton.left + nBoxSize;
			rectButton.top = rectButton.CenterPoint ().y - nBoxSize / 2;
			rectButton.bottom = rectButton.top + nBoxSize;

			pDC->FillRect (rectButton, &globalData.brBarFace);

			OnDrawExpandingBox (pDC, rectButton, !pGroup->m_bIsCollapsed, 
				globalData.clrBarText);

			rectGroup.left = rectButton.right + nBoxOffset;
			bCanCollapse = FALSE;
		}
	}

	// ------------------------------
	// Draw group caption (Office XP)
	// ------------------------------
	
	// ---------------------------
	// Draw an icon if it presents
	// ---------------------------
	BOOL bShowIcon = (pGroup->m_hIcon != NULL 
		&& pGroup->m_sizeIcon.cx < rectGroup.Width () - rectGroup.Height());
	if (bShowIcon)
	{
		OnDrawTasksGroupIcon(pDC, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
	}
	
	// -----------------------
	// Draw group caption text
	// -----------------------
	CFont* pFontOld = pDC->SelectObject (&globalData.fontBold);
	COLORREF clrTextOld = pDC->GetTextColor();

	if (bIsToolBox && !globalData.IsHighContastMode ())
	{
		pDC->SetTextColor (globalData.clrWindow);
	}
	else
	{
		if (bCanCollapse && bIsHighlighted)
		{
			clrTextOld = pDC->SetTextColor (pGroup->m_clrTextHot == (COLORREF)-1 ?
				globalData.clrWindowText : pGroup->m_clrTextHot);
		}
		else
		{
			clrTextOld = pDC->SetTextColor (pGroup->m_clrText == (COLORREF)-1 ?
				globalData.clrWindowText : pGroup->m_clrText);
		}
	}

	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);
	
	int nTaskPaneHOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionHorzOffset();
	int nTaskPaneVOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionVertOffset();
	int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : m_nGroupCaptionHorzOffset);
	
	CRect rectText = rectGroup;
	rectText.left += (bShowIcon ? pGroup->m_sizeIcon.cx	+ 5: nCaptionHOffset);
	rectText.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
	rectText.right = max(rectText.left, 
						rectText.right - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));

	pDC->DrawText (pGroup->m_strName, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);

	// -------------------------
	// Draw group caption button
	// -------------------------
	if (bCanCollapse && !pGroup->m_strName.IsEmpty())
	{
		CSize sizeButton = CMenuImages::Size();
		CRect rectButton = rectGroup;
		rectButton.left = max(rectButton.left, rectButton.right - sizeButton.cx);
		rectButton.top = max(rectButton.top, rectButton.bottom - sizeButton.cy);
		
		if (rectButton.Width () >= sizeButton.cx && rectButton.Height () >= sizeButton.cy)
		{
			if (bIsHighlighted)
			{
				// Draw button frame
				CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penHilite);
				CBrush* pBrushOld = (CBrush*) pDC->SelectObject (&m_brHighlight);
				COLORREF clrBckOld = pDC->GetBkColor ();

				pDC->Rectangle(&rectButton);

				pDC->SetBkColor (clrBckOld);
				pDC->SelectObject (pPenOld);
				pDC->SelectObject (pBrushOld);
			}
			
			if (!pGroup->m_bIsCollapsed)
			{
				CMenuImages::Draw(pDC, CMenuImages::IdArowUp, rectButton.TopLeft());
			}
			else
			{
				CMenuImages::Draw(pDC, CMenuImages::IdArowDown, rectButton.TopLeft());
			}
		}
	}
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnFillTasksGroupInterior(CDC* pDC, CRect rect, BOOL /*bSpecial*/)
{
	ASSERT_VALID(pDC);

	// Draw underline
	CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penBarShadow);
	pDC->MoveTo (rect.left, rect.top);
	pDC->LineTo (rect.right, rect.top);
	pDC->SelectObject (pPenOld);

}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawTasksGroupAreaBorder(CDC* /*pDC*/, CRect /*rect*/, 
													 BOOL /*bSpecial*/, BOOL /*bNoTitle*/)
{
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons, BOOL bIsHighlighted, BOOL /*bIsSelected*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pIcons);
	ASSERT(pTask != NULL);

	CRect rectText = pTask->m_rect;

	if (pTask->m_bIsSeparator)
	{
		CPen* pPenOld = (CPen*) pDC->SelectObject (&globalData.penBarShadow);

		pDC->MoveTo (rectText.left, rectText.CenterPoint ().y);
		pDC->LineTo (rectText.right, rectText.CenterPoint ().y);

		pDC->SelectObject (pPenOld);
		return;
	}

	// ---------
	// Draw icon
	// ---------
	CSize sizeIcon(0, 0);
	::ImageList_GetIconSize (pIcons->m_hImageList, (int*) &sizeIcon.cx, (int*) &sizeIcon.cy);
	if (pTask->m_nIcon >= 0 && sizeIcon.cx > 0)
	{
		pIcons->Draw (pDC, pTask->m_nIcon, rectText.TopLeft (), ILD_TRANSPARENT);
	}
	int nTaskPaneOffset = pTask->m_pGroup->m_pPage->m_pTaskPane->GetTasksIconHorzOffset();
	rectText.left += sizeIcon.cx + (nTaskPaneOffset != -1 ? nTaskPaneOffset : m_nTasksIconHorzOffset);

	// ---------
	// Draw text
	// ---------
	BOOL bIsLabel = (pTask->m_uiCommandID == 0);

	CFont* pFontOld = NULL;
	COLORREF clrTextOld = pDC->GetTextColor();
	if (bIsLabel)
	{
		pFontOld = pDC->SelectObject (
			pTask->m_bIsBold ? &globalData.fontBold : &globalData.fontRegular);
		pDC->SetTextColor (pTask->m_clrText == (COLORREF)-1 ?
			globalData.clrWindowText : pTask->m_clrText);
	}
	else if (!pTask->m_bEnabled)
	{
		pDC->SetTextColor (globalData.clrGrayedText);
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
	}
	else if (bIsHighlighted)
	{
		pDC->SetTextColor (pTask->m_clrTextHot == (COLORREF)-1 ?
			globalData.clrHotText : pTask->m_clrTextHot);
		pFontOld = pDC->SelectObject (&globalData.fontUnderline);
	}
	else
	{
		pDC->SetTextColor (pTask->m_clrText == (COLORREF)-1 ?
			globalData.clrWindowText : pTask->m_clrText);
		pFontOld = pDC->SelectObject (&globalData.fontRegular);
	}
	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);

	CBCGPTasksPane* pTaskPane = pTask->m_pGroup->m_pPage->m_pTaskPane;
	ASSERT_VALID (pTaskPane);

	BOOL bMultiline = bIsLabel ? 
		pTaskPane->IsWrapLabelsEnabled () : pTaskPane->IsWrapTasksEnabled ();

	if (bMultiline)
	{
		pDC->DrawText (pTask->m_strName, rectText, DT_WORDBREAK);
	}
	else
	{
		pDC->DrawText (pTask->m_strName, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	}

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject (pFontOld);
	pDC->SetTextColor (clrTextOld);
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
									int iImage, BOOL bHilited)
{
	ASSERT_VALID (pDC);

	CRect rectFill = rect;
	rectFill.top -= nBorderSize;

	pDC->FillRect (rectFill, &globalData.brWindow);

	if (bHilited)
	{
		pDC->FillRect (rect, &m_brHighlight);
		pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}

	CMenuImages::Draw (pDC, (CMenuImages::IMAGES_IDS) iImage, rect);
}

#endif	// BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManagerXP::OnDrawSpinButtons (CDC* pDC, CRect rectSpin, 
	int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* /*pSpinCtrl*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID (this);

	CRect rect [2];
	rect[0] = rect[1] = rectSpin;

	if (!bOrientation) 
	{
		rect[0].DeflateRect(0, 0, 0, rect[0].Height() / 2);
		rect[1].top = rect[0].bottom ;
	}
	else
	{
		rect[0].DeflateRect(0, 0, rect[0].Width() / 2, 0);
		rect[1].left = rect[0].right ;
	}

	CMenuImages::IMAGES_IDS id[2][2] = {{CMenuImages::IdArowUp, CMenuImages::IdArowDown}, {CMenuImages::IdArowLeft, CMenuImages::IdArowRight}};

	int idxPressed = (nState & (SPIN_PRESSEDUP | SPIN_PRESSEDDOWN)) - 1;
	
	int idxHighlighted = -1;
	if (nState & SPIN_HIGHLIGHTEDUP)
	{
		idxHighlighted = 0;
	}
	else if (nState & SPIN_HIGHLIGHTEDDOWN)
	{
		idxHighlighted = 1;
	}

	BOOL bDisabled = nState & SPIN_DISABLED;

	for (int i = 0; i < 2; i ++)
	{
		if (idxPressed == i || idxHighlighted == i)
		{
			OnFillHighlightedArea (pDC, rect [i], 
				(idxPressed == i) ? &m_brHighlightDn : &m_brHighlight, NULL);
		}
		else
		{
			if (CBCGPToolBarImages::m_bIsDrawOnGlass)
			{
				CBCGPDrawManager dm (*pDC);
				dm.DrawRect (rect[i], globalData.clrBarFace, globalData.clrBarHilite);
			}
			else
			{
				pDC->FillRect (rect[i], &globalData.brBarFace);
				pDC->Draw3dRect (rect[i], globalData.clrBarHilite, globalData.clrBarHilite);
			}
		}

		CMenuImages::Draw (pDC, id[bOrientation][i], rect[i],
			bDisabled ? CMenuImages::ImageGray : CMenuImages::ImageBlack);
	}

	if (idxHighlighted >= 0)
	{
		CRect rectHot = rect [idxHighlighted];

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rectHot, (COLORREF)-1, m_clrMenuItemBorder);
		}
		else
		{
			pDC->Draw3dRect (rectHot, m_clrMenuItemBorder, m_clrMenuItemBorder);
		}
	}
}
//*************************************************************************************
void CBCGPVisualManagerXP::OnFillHighlightedArea (CDC* pDC, CRect rect, 
		CBrush* pBrush, CBCGPToolbarButton* /*pButton*/)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBrush);

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		LOGBRUSH br; 
        pBrush->GetLogBrush(&br);

		CBCGPDrawManager dm (*pDC);
		dm.DrawRect (rect, br.lbColor, (COLORREF)-1);
	}
	else
	{
		pDC->FillRect (rect, pBrush);
	}
}
//********************************************************************************
void CBCGPVisualManagerXP::OnDrawSplitterBorder (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
	ASSERT_VALID(pDC);

	pDC->Draw3dRect (rect, globalData.clrBarShadow, globalData.clrBarShadow);
	rect.InflateRect(-CX_BORDER, -CY_BORDER);
	pDC->Draw3dRect (rect, globalData.clrBarFace, globalData.clrBarFace);
}
//********************************************************************************
void CBCGPVisualManagerXP::OnDrawSplitterBox (CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect& rect)
{
	ASSERT_VALID(pDC);
	pDC->Draw3dRect(rect, globalData.clrBarFace, globalData.clrBarFace);
}
//********************************************************************************
BOOL CBCGPVisualManagerXP::OnDrawCalculatorButton (CDC* pDC, 
	CRect rect, CBCGPToolbarButton* pButton, 
	CBCGPVisualManager::BCGBUTTON_STATE state, 
	int /*cmd*/ /* CBCGPCalculator::CalculatorCommands */,
	CBCGPCalculator* /*pCalculator*/)
{
	ASSERT_VALID (pButton);
	ASSERT_VALID (pDC);

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->FillRect (rect, &m_brHighlightDn);
		pDC->SetTextColor (globalData.clrWindow);
		break;

	case ButtonsIsHighlighted:
		pDC->FillRect (rect, &m_brHighlight);
		break;
	}

	pDC->Draw3dRect (&rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
	return TRUE;
}
//********************************************************************************
BOOL CBCGPVisualManagerXP::OnDrawCalculatorDisplay (CDC* pDC, CRect rect, 
												  const CString& /*strText*/, BOOL /*bMem*/,
												  CBCGPCalculator* /*pCalculator*/)
{
	pDC->FillRect (rect, &globalData.brWindow);
	pDC->Draw3dRect (&rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

	return TRUE;
}
//********************************************************************************
BOOL CBCGPVisualManagerXP::OnDrawBrowseButton (CDC* pDC, CRect rect, 
	CBCGPEdit* /*pEdit*/, CBCGPVisualManager::BCGBUTTON_STATE state, COLORREF& /*clrText*/)
{
	ASSERT_VALID (pDC);

	CRect rectFrame = rect;
	rectFrame.InflateRect (0, 1, 1, 1);

	switch (state)
	{
	case ButtonsIsPressed:
		pDC->FillRect (rect, &m_brHighlightDn);
		pDC->Draw3dRect (&rectFrame, m_clrMenuItemBorder, m_clrMenuItemBorder);
		pDC->SetTextColor (globalData.clrWindow);
		break;

	case ButtonsIsHighlighted:
		pDC->FillRect (rect, &m_brHighlight);
		pDC->Draw3dRect (&rectFrame, m_clrMenuItemBorder, m_clrMenuItemBorder);
		break;

	default:
		pDC->FillRect (rect, &globalData.brBtnFace);
		pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarHilite);
		break;
	}

	return TRUE;
}
//*********************************************************************************
COLORREF CBCGPVisualManagerXP::GetWindowColor () const
{
	return globalData.clrWindow;
}
//*******************************************************************************
COLORREF CBCGPVisualManagerXP::GetAutoHideButtonTextColor (CBCGPAutoHideButton* /*pButton*/)
{
	return globalData.clrBtnDkShadow;
}
//*******************************************************************************
void CBCGPVisualManagerXP::OnDrawAppBarBorder (
								CDC* pDC, CBCGPAppBarWnd* /*pAppBarWnd*/,
								CRect rectBorder, CRect rectBorderSize)
{
	ASSERT_VALID (pDC);

	CBrush* pOldBrush = pDC->SelectObject (&m_brFloatToolBarBorder);
	ASSERT (pOldBrush != NULL);

	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorderSize.left, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.top, rectBorder.Width (), rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, rectBorder.Height (), PATCOPY);
	pDC->PatBlt (rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width (), rectBorderSize.bottom, PATCOPY);

	rectBorderSize.DeflateRect (2, 2);
	rectBorder.DeflateRect (2, 2);

	pDC->SelectObject (&globalData.brLight);

	pDC->PatBlt (rectBorder.left, rectBorder.top + 1, rectBorderSize.left, rectBorder.Height () - 2, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.top, rectBorder.Width () - 2, rectBorderSize.top, PATCOPY);
	pDC->PatBlt (rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, rectBorder.Height () - 2, PATCOPY);
	pDC->PatBlt (rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, rectBorder.Width () - 2, rectBorderSize.bottom, PATCOPY);

	pDC->SelectObject (pOldBrush);
}
//******************************************************************************
void CBCGPVisualManagerXP::GetSmartDockingBaseMarkerColors (
		COLORREF& clrBaseGroupBackground,
		COLORREF& clrBaseGroupBorder)
{
	clrBaseGroupBackground = m_clrBarBkgnd;
	clrBaseGroupBorder = m_clrMenuBorder;
}
//*************************************************************************************
void CBCGPVisualManagerXP::OnDrawButtonSeparator (CDC* pDC,
		CBCGPToolbarButton* /*pButton*/, CRect rect, CBCGPVisualManager::BCGBUTTON_STATE /*state*/,
		BOOL bHorz)
{
	CPen* pOldPen = pDC->SelectObject (&m_penMenuItemBorder);
	ASSERT (pOldPen != NULL);

	if (bHorz)
	{
		pDC->MoveTo (rect.left, rect.top);
		pDC->LineTo (rect.left, rect.bottom);
	}
	else
	{
		pDC->MoveTo (rect.left, rect.top);
		pDC->LineTo (rect.right, rect.top);
	}

	pDC->SelectObject (pOldPen);
}

#ifndef BCGP_EXCLUDE_POPUP_WINDOW

void CBCGPVisualManagerXP::OnDrawPopupWindowBorder (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->Draw3dRect (rect, m_clrMenuBorder, m_clrMenuBorder);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, m_clrMenuLight, m_clrMenuLight);
}
//**********************************************************************************
COLORREF  CBCGPVisualManagerXP::OnDrawPopupWindowCaption (CDC* pDC, CRect rectCaption, CBCGPPopupWindow* /*pPopupWnd*/)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rectCaption, &m_brHighlight);

    // get the text color
	return globalData.clrBarText;
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnErasePopupWindowButton (CDC* pDC, CRect rc, CBCGPPopupWndButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsPressed ())
	{
		CBrush br (m_clrHighlightDn);
		pDC->FillRect (&rc, &br);
		return;
	}
	else if (pButton->IsHighlighted () || pButton->IsPushed ())
	{
		CBrush br (m_clrHighlight);
		pDC->FillRect (&rc, &br);
		return;
	}

	CRect rectParent;
	pButton->GetParent ()->GetClientRect (rectParent);

	pButton->GetParent ()->MapWindowPoints (pButton, rectParent);
	OnFillPopupWindowBackground (pDC, rectParent);
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnDrawPopupWindowButtonBorder (CDC* pDC, CRect rc, CBCGPPopupWndButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsHighlighted () || pButton->IsPushed () ||
		pButton->IsCaptionButton ())
	{
		pDC->Draw3dRect (rc, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}
}
//**********************************************************************************
void CBCGPVisualManagerXP::OnFillPopupWindowBackground (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &m_brMenuLight);
}

#endif // BCGP_EXCLUDE_POPUP_WINDOW

#ifndef BCGP_EXCLUDE_PLANNER

COLORREF CBCGPVisualManagerXP::OnFillPlannerCaption (CDC* pDC, CBCGPPlannerView* pView,
		CRect rect, BOOL bIsToday, BOOL bIsSelected, BOOL bNoBorder/* = FALSE*/)
{
	ASSERT_VALID (pDC);

	if (bIsToday)
	{
		rect.DeflateRect (1, 1);

		pDC->FillRect (rect, &m_brHighlight);

		CPen pen (PS_SOLID, 1, m_clrMenuItemBorder);
		CPen* pOldPen = pDC->SelectObject (&pen);

		pDC->MoveTo (rect.left, rect.bottom);
		pDC->LineTo (rect.right, rect.bottom);

		pDC->SelectObject (pOldPen);

		return globalData.clrBtnText;
	}

	return CBCGPVisualManager::OnFillPlannerCaption (pDC, pView, rect, bIsToday, 
		bIsSelected, bNoBorder);
}
//*******************************************************************************
void CBCGPVisualManagerXP::OnDrawPlannerTimeLine (CDC* pDC, CBCGPPlannerView* /*pView*/, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brHighlight);

	CPen* pOldPen = pDC->SelectObject (&m_penSeparator);

	pDC->MoveTo (rect.left, rect.bottom);
	pDC->LineTo (rect.right, rect.bottom);

	pDC->SelectObject (pOldPen);
}

#endif // BCGP_EXCLUDE_PLANNER

//*************************************************************************************
void CBCGPVisualManagerXP::GetCalendarColors (const CBCGPCalendar* pCalendar,
				   CBCGPCalendarColors& colors)
{
	CBCGPVisualManager::GetCalendarColors (pCalendar, colors);
	colors.clrSelected = m_clrHighlight;
}

#ifndef BCGP_EXCLUDE_PROP_LIST

COLORREF CBCGPVisualManagerXP::GetPropListGroupColor (CBCGPPropList* pPropList)
{
	ASSERT_VALID (pPropList);

	if (globalData.m_nBitsPerPixel <= 8)
	{
		return CBCGPVisualManager::GetPropListGroupColor (pPropList);
	}

	return CBCGPDrawManager::PixelAlpha (
			pPropList->DrawControlBarColors () ? 
			globalData.clrBarFace : globalData.clrBtnFace, 94);
}

COLORREF CBCGPVisualManagerXP::GetPropListGroupTextColor (CBCGPPropList* pPropList)
{
	ASSERT_VALID (pPropList);

	return pPropList->DrawControlBarColors () ?
		globalData.clrBarShadow : globalData.clrBtnShadow;
}

#endif // BCGP_EXCLUDE_PROP_LIST

#ifndef BCGP_EXCLUDE_RIBBON

COLORREF CBCGPVisualManagerXP::OnFillRibbonButton (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsDefaultPanelButton () && !pButton->IsQATMode ())
	{
		return (COLORREF)-1;
	}

	const BOOL bIsMenuMode = pButton->IsMenuMode ();

	const BOOL bIsHighlighted = 
		(pButton->IsHighlighted () || pButton->IsDroppedDown ()) && 
		!pButton->IsDisabled ();

	if (pButton->IsKindOf (RUNTIME_CLASS (CBCGPRibbonEdit)))
	{
		COLORREF clrBorder = globalData.clrBarShadow;
		CRect rectCommand = pButton->GetCommandRect ();

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (pButton->GetRect (), globalData.clrWindow, clrBorder);
		}
		else
		{
			if (bIsHighlighted)
			{
				pDC->FillRect (rectCommand, &globalData.brWindow);
			}
			else 
			{
				pDC->FillRect (rectCommand, &globalData.brBarFace);
				
				CBCGPDrawManager dm (*pDC);
				dm.HighlightRect (rectCommand);
			}

			pDC->Draw3dRect (pButton->GetRect (), clrBorder, clrBorder);
		}

		return (COLORREF)-1;
	}

	if (!pButton->IsChecked () && !bIsHighlighted)
	{
		return (COLORREF)-1;
	}

	if (pButton->IsChecked () && bIsMenuMode && !bIsHighlighted)
	{
		return (COLORREF)-1;
	}

	CRect rectMenu = pButton->GetMenuRect ();
	
	if (!rectMenu.IsRectEmpty () && bIsHighlighted)
	{
		CRect rectCommand = pButton->GetCommandRect ();

		if (pButton->IsCommandAreaHighlighted ())
		{
			OnFillHighlightedArea (pDC, rectCommand, 
				(pButton->IsPressed () || pButton->IsDroppedDown ()) && !bIsMenuMode ? 
				&m_brHighlightDn : &m_brHighlight, NULL);
		}
		else
		{
			OnFillHighlightedArea (pDC, rectCommand, 
				&m_brHighlight, NULL);

			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rectCommand);
		}

		if (pButton->IsMenuAreaHighlighted ())
		{
			OnFillHighlightedArea (pDC, rectMenu, 
				(pButton->IsPressed () || pButton->IsDroppedDown ()) && !bIsMenuMode ? 
				&m_brHighlightDn : &m_brHighlight, NULL);
		}
		else
		{
			OnFillHighlightedArea (pDC, rectMenu, 
				&m_brHighlight, NULL);

			CBCGPDrawManager dm (*pDC);
			dm.HighlightRect (rectMenu);
		}
	}
	else
	{
		CBrush* pBrush = 
			(pButton->IsPressed () || pButton->IsDroppedDown ()) && !bIsMenuMode ? 
				&m_brHighlightDn : &m_brHighlight;

		if (pButton->IsChecked () && !bIsMenuMode)
		{
			pBrush = bIsHighlighted ? 
					&m_brHighlightDn : &m_brHighlightChecked;
		}

		OnFillHighlightedArea (pDC, pButton->GetRect (), pBrush, NULL);
	}

	return (COLORREF)-1;
}
//****************************************************************************
void CBCGPVisualManagerXP::OnDrawRibbonButtonBorder (
					CDC* pDC, 
					CBCGPRibbonButton* pButton)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if (pButton->IsDefaultPanelButton () && !pButton->IsQATMode ())
	{
		return;
	}

	const BOOL bIsMenuMode = pButton->IsMenuMode ();

	const BOOL bIsHighlighted = 
		(pButton->IsHighlighted () || pButton->IsDroppedDown ()) && 
		!pButton->IsDisabled ();

	if (pButton->IsChecked () && bIsMenuMode && !bIsHighlighted)
	{
		return;
	}

	CRect rect = pButton->GetRect ();
	CRect rectMenu = pButton->GetMenuRect ();

	if ((bIsHighlighted || pButton->IsChecked ()) && 
		(!pButton->IsDisabled () || pButton->IsFocused () || pButton->IsChecked ()))
	{
		COLORREF clrLine = 
			((pButton->IsPressed () || pButton->IsDroppedDown ()) && !bIsMenuMode) ?
			m_clrPressedButtonBorder : m_clrMenuItemBorder;

		if (CBCGPToolBarImages::m_bIsDrawOnGlass)
		{
			CBCGPDrawManager dm (*pDC);
			dm.DrawRect (rect, (COLORREF)-1, m_clrMenuItemBorder);
		}
		else
		{
			pDC->Draw3dRect (rect, clrLine, clrLine);
		}

		if (!rectMenu.IsRectEmpty ())
		{
			if (CBCGPToolBarImages::m_bIsDrawOnGlass)
			{
				CBCGPDrawManager dm (*pDC);
				
				if (pButton->IsMenuOnBottom ())
				{
					dm.DrawLine (rectMenu.left, rectMenu.top, rectMenu.right, rectMenu.top, m_clrMenuItemBorder);
				}
				else
				{
					dm.DrawLine (rectMenu.left, rectMenu.top, rectMenu.left, rectMenu.bottom, m_clrMenuItemBorder);
				}
			}
			else
			{
				CPen* pOldPen = pDC->SelectObject (&m_penMenuItemBorder);
				ASSERT (pOldPen != NULL);

				if (pButton->IsMenuOnBottom ())
				{
					pDC->MoveTo (rectMenu.left, rectMenu.top);
					pDC->LineTo (rectMenu.right, rectMenu.top);
				}
				else
				{
					pDC->MoveTo (rectMenu.left, rectMenu.top);
					pDC->LineTo (rectMenu.left, rectMenu.bottom);
				}

				pDC->SelectObject (pOldPen);
			}
		}
	}
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawRibbonMenuCheckFrame (
					CDC* pDC,
					CBCGPRibbonButton* /*pButton*/, 
					CRect rect)
{
	ASSERT_VALID (pDC);

	OnFillHighlightedArea (pDC, rect, &m_brHighlight, NULL);
	pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnFillRibbonMenuFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* /*pPanel*/,
					CRect rect)
{
	ASSERT_VALID (pDC);
	pDC->FillRect (rect, &m_brMenuLight);
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawRibbonRecentFilesFrame (
					CDC* pDC, 
					CBCGPRibbonMainPanel* /*pPanel*/,
					CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);

	CRect rectSeparator = rect;
	rectSeparator.right = rectSeparator.left + 2;

	pDC->Draw3dRect (rectSeparator, globalData.clrBarShadow,
									globalData.clrBarHilite);
}
//***********************************************************************************
void CBCGPVisualManagerXP::OnDrawRibbonLabel (
					CDC* pDC, 
					CBCGPRibbonLabel* pLabel,
					CRect rect)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pLabel);

	if (pLabel->IsMenuMode ())
	{
		OnDrawMenuLabel (pDC, rect);
	}
}
//********************************************************************************
COLORREF CBCGPVisualManagerXP::OnDrawRibbonStatusBarPane (CDC* pDC, CBCGPRibbonStatusBar* /*pBar*/,
					CBCGPRibbonStatusBarPane* pPane)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pPane);

	CRect rect = pPane->GetRect ();

	if (pPane->IsHighlighted ())
	{
		CRect rectButton = rect;
		rectButton.DeflateRect (1, 1);

		OnFillHighlightedArea (pDC, rectButton, 
			pPane->IsPressed () ? 
			&m_brHighlightDn : &m_brHighlight, NULL);

		pDC->Draw3dRect (rectButton, m_clrMenuItemBorder, m_clrMenuItemBorder);
	}

	CRect rectSeparator = rect;
	rectSeparator.DeflateRect (0, 2);

	rectSeparator.left = rectSeparator.right - 1;

	pDC->Draw3dRect (rectSeparator, globalData.clrBarShadow,
									globalData.clrBarShadow);

	return (COLORREF)-1;
}
//********************************************************************************
void CBCGPVisualManagerXP::GetRibbonSliderColors (CBCGPRibbonSlider* /*pSlider*/,
							BOOL bIsHighlighted, 
							BOOL bIsPressed,
							BOOL bIsDisabled,
							COLORREF& clrLine,
							COLORREF& clrFill)
{
	clrLine = bIsDisabled ? globalData.clrBarShadow : 
		(bIsPressed || bIsHighlighted) ? m_clrMenuItemBorder : globalData.clrBarDkShadow;

	clrFill = bIsPressed && bIsHighlighted ?
		m_clrHighlightDn :
		bIsHighlighted ? m_clrHighlight : globalData.clrBarFace;
}
//********************************************************************************
void CBCGPVisualManagerXP::OnDrawRibbonQATSeparator (CDC* pDC, 
												   CBCGPRibbonSeparator* /*pSeparator*/, CRect rect)
{
	ASSERT_VALID (pDC);

	int x = rect.CenterPoint ().x;
	int y1 = rect.top;
	int y2 = rect.bottom - 1;

	if (CBCGPToolBarImages::m_bIsDrawOnGlass)
	{
		CBCGPDrawManager dm (*pDC);
		dm.DrawLine (x, y1, x, y2, m_clrSeparator);
	}
	else
	{
		CPen* pOldPen = pDC->SelectObject (&m_penSeparator);
		ASSERT (pOldPen != NULL);

		pDC->MoveTo (x, y1);
		pDC->LineTo (x, y2);

		pDC->SelectObject (pOldPen);
	}
}

#endif // BCGP_EXCLUDE_RIBBON

COLORREF CBCGPVisualManagerXP::OnDrawPropSheetListItem (CDC* pDC, CBCGPPropertySheet* /*pParent*/, 
		CRect rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
	ASSERT_VALID (pDC);

	CBrush* pBrush = NULL; 

	if (bIsSelected)
	{
		pBrush = !bIsHighlihted ? &m_brHighlightChecked : &m_brHighlightDn;
	}
	else if (bIsHighlihted)
	{
		pBrush = &m_brHighlight;
	}

	OnFillHighlightedArea (pDC, rect, pBrush, NULL);
	pDC->Draw3dRect (rect, m_clrMenuItemBorder, m_clrMenuItemBorder);

	return globalData.clrBtnText;
}
//***************************************************************************************
COLORREF CBCGPVisualManagerXP::OnDrawMenuLabel (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	pDC->FillRect (rect, &m_brBarBkgnd);

	CRect rectSeparator = rect;
	rectSeparator.top = rectSeparator.bottom - 2;

	pDC->Draw3dRect (rectSeparator, globalData.clrBarShadow,
									globalData.clrBarHilite);

	return globalData.clrBarText;
}
