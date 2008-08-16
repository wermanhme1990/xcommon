//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGPro Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonPanelMenu.cpp : implementation file
//

#include "stdafx.h"
#include "trackmouse.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonBar.h"
#include "BCGPVisualManager.h"
#include "BCGPTooltipManager.h"
#include "BCGPToolTipCtrl.h"
#include "BCGPRibbonPaletteButton.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int uiPopupTimerEvent = 1;
static const int uiRemovePopupTimerEvent = 2;
static const int nScrollBarID = 1;

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonPanelMenuBar window

IMPLEMENT_DYNAMIC(CBCGPRibbonPanelMenuBar, CBCGPPopupMenuBar)

#pragma warning (disable : 4355)

CBCGPRibbonPanelMenuBar::CBCGPRibbonPanelMenuBar(CBCGPRibbonPanel* pPanel)
{
	ASSERT_VALID (pPanel);

	m_pPanel = DYNAMIC_DOWNCAST (CBCGPRibbonPanel,
		pPanel->GetRuntimeClass ()->CreateObject ());
	ASSERT_VALID (m_pPanel);

	m_pPanel->CopyFrom (*pPanel);

	CommonInit ();

	m_pPanelOrigin = pPanel;

	m_pPanel->m_pParentMenuBar = this;
	m_pPanel->m_btnLaunch.SetParentMenu (this);

	for (int i = 0; i < m_pPanel->m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_pPanel->m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->SetParentMenu (this);
	}

	m_pRibbonBar = m_pPanel->m_pParent->GetParentRibbonBar ();
}
//*******************************************************************************
CBCGPRibbonPanelMenuBar::CBCGPRibbonPanelMenuBar(
	CBCGPRibbonBar* pRibbonBar,
	const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arButtons)
{
	m_pPanel = new CBCGPRibbonPanel;

	CommonInit ();
	AddButtons (pRibbonBar, arButtons, TRUE);
}
//*******************************************************************************
CBCGPRibbonPanelMenuBar::CBCGPRibbonPanelMenuBar(CBCGPRibbonPaletteButton* pPaletteButton)
{
	ASSERT_VALID (pPaletteButton);

	m_pPanel = new CBCGPRibbonPanel (pPaletteButton);

	CommonInit ();

	// Create array without scroll buttons:
	CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*> arButtons;
	pPaletteButton->GetMenuItems (arButtons);

	AddButtons (pPaletteButton->GetTopLevelRibbonBar (), arButtons, FALSE);
}
//*******************************************************************************
CBCGPRibbonPanelMenuBar::CBCGPRibbonPanelMenuBar (CBCGPRibbonCategory* pCategory, CSize size)
{
	ASSERT_VALID (pCategory);

	m_pPanel = NULL;

	CommonInit ();

	m_pCategory = (CBCGPRibbonCategory*) pCategory->GetRuntimeClass ()->CreateObject ();
	ASSERT_VALID (m_pCategory);

	m_pCategory->CopyFrom (*pCategory);
	m_pCategory->m_pParentMenuBar = this;

	for (int iPanel = 0; iPanel < m_pCategory->GetPanelCount (); iPanel++)
	{
		CBCGPRibbonPanel* pPanel = m_pCategory->GetPanel (iPanel);
		ASSERT_VALID (pPanel);

		pPanel->m_pParentMenuBar = this;
		pPanel->m_btnLaunch.SetParentMenu (this);
		pPanel->m_btnDefault.SetParentMenu (this);

		for (int i = 0; i < pPanel->m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = pPanel->m_arElements [i];
			ASSERT_VALID (pElem);

			pElem->SetParentMenu (this);
		}
	}

	m_sizeCategory = size;
	m_pRibbonBar = m_pCategory->GetParentRibbonBar ();
}
//*******************************************************************************
void CBCGPRibbonPanelMenuBar::AddButtons (CBCGPRibbonBar* pRibbonBar,
	const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arButtons,
	BOOL bFloatyMode)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pRibbonBar);

	m_bSimpleMode = TRUE;
	m_pRibbonBar = pRibbonBar;

	m_pPanel->m_pParentMenuBar = this;
	m_pPanel->m_bFloatyMode = bFloatyMode;
	m_pPanel->m_nXMargin = 2;
	m_pPanel->m_nYMargin = 2;
	m_pPanel->RemoveAll ();

	for (int i = 0; i < arButtons.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pSrcButton = arButtons [i];
		ASSERT_VALID (pSrcButton);

		CBCGPBaseRibbonElement* pButton =
			(CBCGPBaseRibbonElement*) pSrcButton->GetRuntimeClass ()->CreateObject ();
		ASSERT_VALID (pButton);

		pButton->CopyFrom (*pSrcButton);
		pButton->SetOriginal (pSrcButton);
		pButton->m_bCompactMode = TRUE;

		pButton->SetParentMenu (this);

		m_pPanel->Add (pButton);
	}
}
//*******************************************************************************
CBCGPRibbonPanelMenuBar::CBCGPRibbonPanelMenuBar()
{
	m_pPanel = new CBCGPRibbonPanel;
	CommonInit ();
}
//*******************************************************************************
void CBCGPRibbonPanelMenuBar::CommonInit ()
{
	if (m_pPanel != NULL)
	{
		ASSERT_VALID (m_pPanel);
		m_pPanel->m_pParentMenuBar = this;
	}

	m_pCategory = NULL;
	m_sizeCategory = CSize (0, 0);

	m_pDelayedCloseButton = NULL;
	m_pDelayedButton = NULL;

	m_bSimpleMode = FALSE;
	m_bIsMenuMode = FALSE;
	m_pPanelOrigin = NULL;
	m_pRibbonBar = NULL;

	m_bTracked = FALSE;
	m_pToolTip = NULL;
	m_bDisableSideBarInXPMode = TRUE;

	m_sizePrefered = CSize (0, 0);
	m_bIsQATPopup = FALSE;
	m_bCustomizeMenu = TRUE;
}

#pragma warning (default : 4355)

CBCGPRibbonPanelMenuBar::~CBCGPRibbonPanelMenuBar()
{
	if (m_pPanel != NULL)
	{
		ASSERT_VALID (m_pPanel);
		delete m_pPanel;
	}

	if (m_pCategory != NULL)
	{
		ASSERT_VALID (m_pCategory);

		delete m_pCategory;

		if (m_pRibbonBar != NULL && m_pRibbonBar->GetActiveCategory () != NULL)
		{
			// Redraw ribbon tab:
			ASSERT_VALID (m_pRibbonBar);
			ASSERT_VALID (m_pRibbonBar->GetActiveCategory ());

			if (!m_pRibbonBar->IsQuickAccessToolbarOnTop ())
			{
				CBCGPRibbonTab& tab = m_pRibbonBar->GetActiveCategory ()->m_Tab;
				
				tab.m_bIsDroppedDown = FALSE;
				tab.m_bIsHighlighted = FALSE;

				CRect rectRedraw = tab.GetRect ();
				rectRedraw.bottom = m_pRibbonBar->GetQuickAccessToolbarLocation ().bottom;
				rectRedraw.InflateRect (1, 1);

				m_pRibbonBar->RedrawWindow (rectRedraw);
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CBCGPRibbonPanelMenuBar, CBCGPPopupMenuBar)
	//{{AFX_MSG_MAP(CBCGPRibbonPanelMenuBar)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_REGISTERED_MESSAGE(BCGM_UPDATETOOLTIPS, OnBCGUpdateToolTips)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnNeedTipText)
END_MESSAGE_MAP()

void CBCGPRibbonPanelMenuBar::AdjustLocations ()
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pRibbonBar);

	CClientDC dc (this);

	CFont* pOldFont = dc.SelectObject (m_pRibbonBar->GetFont ());
	ASSERT (pOldFont != NULL);

	CRect rectClient;
	GetClientRect (rectClient);

	if (m_pCategory != NULL)
	{
		ASSERT_VALID (m_pCategory);

		m_pCategory->m_rect = rectClient;
		m_pCategory->RecalcLayout (&dc);
	}
	else
	{
		m_pPanel->m_bSizeIsLocked = m_bResizeTracking;

		m_pPanel->Repos (&dc, rectClient);
		m_pPanel->m_rect.InflateRect (2, 2);
		m_pPanel->m_rect.bottom++;

		m_pPanel->OnAfterChangeRect (&dc);
		
		m_pPanel->m_bSizeIsLocked = FALSE;
	}

	dc.SelectObject (pOldFont);
}
//*****************************************************************************************
CSize CBCGPRibbonPanelMenuBar::CalcSize (BOOL /*bVertDock*/)
{
	ASSERT_VALID (this);

	if (m_pCategory != NULL)
	{
		ASSERT_VALID (m_pCategory);
		ASSERT (m_sizeCategory != CSize (0, 0));

		return m_sizeCategory;
	}

	ASSERT_VALID (m_pRibbonBar);
	ASSERT_VALID (m_pPanel);

	m_pPanel->m_bIsQATPopup = m_bIsQATPopup;

	CClientDC dc (m_pRibbonBar);

	CFont* pOldFont = dc.SelectObject (m_pRibbonBar->GetFont ());
	ASSERT (pOldFont != NULL);

	if (m_bIsMenuMode)
	{
		m_pPanel->m_bMenuMode = TRUE;
		
		m_pPanel->Repos (&dc, CRect (0, 0, m_sizePrefered.cx, m_sizePrefered.cy));

		dc.SelectObject (pOldFont);

		CSize size = m_pPanel->m_rect.Size ();

		if (m_sizePrefered != CSize (0, 0))
		{
			size.cx = max (m_sizePrefered.cx, size.cx);

			if (m_sizePrefered.cy <= 0)
			{
				size.cy = m_pPanel->m_rect.Size ().cy;
			}
			else
			{
				size.cy = max (m_sizePrefered.cy, size.cy);
			}
		}

		return size;
	}

	if (m_bSimpleMode && m_pPanel->m_arWidths.GetSize () == 0)
	{
		CWaitCursor wait;
		m_pPanel->RecalcWidths (&dc, 32767);
	}

	const int nWidthSize = (int) m_pPanel->m_arWidths.GetSize ();
	if (nWidthSize == 0)
	{
		dc.SelectObject (pOldFont);
		return CSize (10, 10);
	}

	if (m_pPanel->m_bAlignByColumn && !m_pPanel->m_bFloatyMode && !m_pPanel->IsFixedSize ())
	{
		int nHeight = m_pRibbonBar->GetCategoryHeight ();
		int nCaptionHeight = m_pPanel->GetCaptionSize (&dc).cy;
		int nStartIndex = nWidthSize > 1 ? nWidthSize - 2 : 0;

		// Make all elemnts visible, regular size
		for (int nWidthIndex = nStartIndex; nWidthIndex >= 0; nWidthIndex--)
		{
			int nWidth = m_pPanel->m_arWidths [nWidthIndex];
			m_pPanel->Repos (&dc, CRect (0, 0, nWidth, nHeight - nCaptionHeight + 4 * m_pPanel->m_nYMargin));

			BOOL bHasCompactSize = FALSE;

			for (int i = 0; i < m_pPanel->GetCount (); i++)
			{
				if (m_pPanel->GetElement (i)->IsCompactMode ())
				{
					bHasCompactSize = TRUE;
					break;
				}
			}

			if (!bHasCompactSize)
			{
				break;
			}
		}
	}
	else if (m_bIsQATPopup)
	{
		int nWidth = m_pPanel->m_arWidths [0] + 2 * m_pPanel->m_nXMargin;
		m_pPanel->Repos (&dc, CRect (0, 0, nWidth, 32767));
	}
	else
	{
		int nWidth = m_pPanel->m_arWidths [nWidthSize > 2 ? 1 : 0] + 4 * m_pPanel->m_nXMargin;
		m_pPanel->Repos (&dc, CRect (0, 0, nWidth, 32767));
	}

	CSize size = m_pPanel->m_rect.Size ();
	dc.SelectObject (pOldFont);

	if (m_bSimpleMode && m_pPanel->GetCount () > 0 && !m_bIsQATPopup)
	{
		int xMin = 32767;
		int xMax = 0;

		int yMin = 32767;
		int yMax = 0;

		for (int i = 0; i < m_pPanel->GetCount (); i++)
		{
			CBCGPBaseRibbonElement* pButton = m_pPanel->GetElement (i);
			ASSERT_VALID (pButton);

			CRect rectButton = pButton->GetRect ();

			xMin = min (xMin, rectButton.left);
			yMin = min (yMin, rectButton.top);

			xMax = max (xMax, rectButton.right);
			yMax = max (yMax, rectButton.bottom);
		}

		return CSize (
			xMax - xMin + 2 * m_pPanel->m_nXMargin, 
			yMax - yMin + 2 * m_pPanel->m_nYMargin);
	}

	return size;
}
//*****************************************************************************************
void CBCGPRibbonPanelMenuBar::DoPaint (CDC* pDCPaint)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pDCPaint);
	ASSERT_VALID (m_pRibbonBar);

	CBCGPMemDC memDC (*pDCPaint, this);
	CDC* pDC = &memDC.GetDC ();

	CRect rectClip;
	pDCPaint->GetClipBox (rectClip);

	CRgn rgnClip;

	if (!rectClip.IsRectEmpty ())
	{
		rgnClip.CreateRectRgnIndirect (rectClip);
		pDC->SelectClipRgn (&rgnClip);
	}
	
	CFont* pOldFont = pDC->SelectObject (m_pRibbonBar->GetFont ());
	ASSERT (pOldFont != NULL);

	pDC->SetBkMode (TRANSPARENT);

	CRect rectClient;
	GetClientRect (rectClient);

	CRect rectFill = rectClient;
	rectFill.InflateRect (3, 3);

	if (m_pCategory != NULL)
	{
		ASSERT_VALID (m_pCategory);

		CBCGPVisualManager::GetInstance ()->OnDrawRibbonCategory (
			pDC, m_pCategory, rectFill);

		m_pCategory->OnDraw (pDC);
	}
	else
	{
		ASSERT_VALID (m_pPanel);

		if (m_pPanel->m_pParent != NULL)
		{
			CBCGPRibbonCategory* pCategory = m_pPanel->m_pParent;
			ASSERT_VALID (pCategory);

			CBCGPRibbonPanelMenuBar* pMenuBarSaved = pCategory->m_pParentMenuBar;
			pCategory->m_pParentMenuBar = this;

			CBCGPVisualManager::GetInstance ()->OnDrawRibbonCategory (
				pDC, pCategory, rectFill);

			pCategory->m_pParentMenuBar = pMenuBarSaved;
		}
		else if (m_bIsQATPopup)
		{
			CBCGPVisualManager::GetInstance ()->OnFillRibbonQATPopup (
				pDC, this, rectClient);
		}
		else
		{
			CBCGPVisualManager::GetInstance ()->OnFillBarBackground (
				pDC, this, rectClient, rectClient);
		}

		m_pPanel->DoPaint (pDC);
	}

	pDC->SelectObject (pOldFont);
	pDC->SelectClipRgn (NULL);
}
//*****************************************************************************************
void CBCGPRibbonPanelMenuBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	CBCGPPopupMenuBar::OnMouseMove(nFlags, point);

	if (m_pPanel != NULL &&
		!m_pPanel->m_bMenuMode && m_pPanel->GetDroppedDown () != NULL)
	{
		return;
	}

	if (m_pCategory != NULL && m_pCategory->GetDroppedDown () != NULL)
	{
		return;
	}

	if (point == CPoint (-1, -1))
	{
		m_bTracked = FALSE;
	}
	else if (!m_bTracked)
	{
		m_bTracked = TRUE;
		
		TRACKMOUSEEVENT trackmouseevent;
		trackmouseevent.cbSize = sizeof(trackmouseevent);
		trackmouseevent.dwFlags = TME_LEAVE;
		trackmouseevent.hwndTrack = GetSafeHwnd();
		trackmouseevent.dwHoverTime = HOVER_DEFAULT;
		::BCGPTrackMouse (&trackmouseevent);

		CBCGPBaseRibbonElement* pPressed = NULL;

		if (m_pCategory != NULL)
		{
			// pPressed = m_pCategory->GetPr TODO
		}
		else
		{
			pPressed = m_pPanel->GetPressed ();
		}

		if (pPressed != NULL && ((nFlags & MK_LBUTTON) == 0))
		{
			ASSERT_VALID (pPressed);
			pPressed->m_bIsPressed = FALSE;
		}
	}

	if (m_pCategory != NULL)
	{
		m_pCategory->OnMouseMove (point);
	}
	else if (m_pPanel != NULL)
	{
		BOOL bWasHighlighted = m_pPanel->IsHighlighted ();
		m_pPanel->Highlight (TRUE, point);

		if (!bWasHighlighted)
		{
			RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
		}
	}
}
//*****************************************************************************************
LRESULT CBCGPRibbonPanelMenuBar::OnMouseLeave(WPARAM,LPARAM)
{
	CPoint point;
	::GetCursorPos (&point);
	ScreenToClient (&point);

	CRect rectClient;
	GetClientRect (rectClient);

	if (!rectClient.PtInRect (point))
	{
		OnMouseMove (0, CPoint (-1, -1));
		m_bTracked = FALSE;

		if (m_pPanel != NULL)
		{
			m_pPanel->Highlight (FALSE, CPoint (-1, -1));
		}

		RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}

	m_bTracked = FALSE;
	return 0;
}
//*****************************************************************************************
void CBCGPRibbonPanelMenuBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	HWND hwndThis = GetSafeHwnd ();

	CBCGPPopupMenuBar::OnLButtonUp(nFlags, point);

	if (m_pCategory != NULL)
	{
		m_pCategory->OnLButtonUp (point);
	}
	else
	{
		m_pPanel->MouseButtonUp (point);
	}

	if (::IsWindow (hwndThis))
	{
		::GetCursorPos (&point);
		ScreenToClient (&point);

		OnMouseMove (nFlags, point);
	}
}
//*****************************************************************************************
void CBCGPRibbonPanelMenuBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBCGPPopupMenuBar::OnLButtonDown(nFlags, point);

	CBCGPBaseRibbonElement* pDroppedDown = GetDroppedDown ();
	if (pDroppedDown != NULL)
	{
		ASSERT_VALID (pDroppedDown);
		pDroppedDown->ClosePopupMenu ();
	}

	OnMouseMove (nFlags, point);

	if (m_pCategory != NULL)
	{
		m_pCategory->OnLButtonDown (point);
	}
	else
	{
		m_pPanel->MouseButtonDown (point);
	}
}
//*******************************************************************************
void CBCGPRibbonPanelMenuBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CBCGPPopupMenuBar::OnLButtonDblClk(nFlags, point);

	if (IsRibbonPanelInRegularMode ())
	{
		CBCGPRibbonButton* pDroppedDown = GetDroppedDown ();
		if (pDroppedDown != NULL)
		{
			pDroppedDown->ClosePopupMenu ();
		}
	}

	CBCGPBaseRibbonElement* pHit = HitTest (point);
	if (pHit != NULL)
	{
		ASSERT_VALID (pHit);

		pHit->OnLButtonDblClk (point);
	}
}
//*******************************************************************************
void CBCGPRibbonPanelMenuBar::OnClickButton (CBCGPRibbonButton* pButton, CPoint /*point*/)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pButton);

	pButton->m_bIsHighlighted = pButton->m_bIsPressed = FALSE;
	RedrawWindow (pButton->GetRect ());

	pButton->NotifyCommand (TRUE);

	CFrameWnd* pParentFrame = BCGPGetParentFrame (this);
	ASSERT_VALID (pParentFrame);

	pParentFrame->DestroyWindow ();
}
//*************************************************************************************
void CBCGPRibbonPanelMenuBar::OnChangeHighlighted (CBCGPBaseRibbonElement* pHot)
{
	ASSERT_VALID (this);

	if (m_pPanel == NULL || !m_pPanel->m_bMenuMode)
	{
		return;
	}

	CBCGPRibbonButton* pDroppedDown = DYNAMIC_DOWNCAST (
		CBCGPRibbonButton, m_pPanel->GetDroppedDown ());

	CBCGPRibbonButton* pHotButton = DYNAMIC_DOWNCAST (
		CBCGPRibbonButton, pHot);

	if (pDroppedDown != NULL && pHot == NULL)
	{
		return;
	}

	BOOL bHotWasChanged = pDroppedDown != pHot;

	if (pHotButton != NULL && pDroppedDown == pHotButton &&
		!pHotButton->GetMenuRect ().IsRectEmpty () &&
		!pHotButton->IsMenuAreaHighlighted ())
	{
		//------------------------------------------------
		// Mouse moved away from the menu area, hide menu:
		//------------------------------------------------
		bHotWasChanged = TRUE;
	}

	if (bHotWasChanged)
	{
		CBCGPRibbonPanelMenu* pParentMenu = 
			DYNAMIC_DOWNCAST (CBCGPRibbonPanelMenu, GetParent ());

		if (pDroppedDown != NULL)
		{
			const MSG* pMsg = GetCurrentMessage ();

			if (CBCGPToolBar::IsCustomizeMode () ||
				(pMsg != NULL && pMsg->message == WM_KEYDOWN))
			{
				KillTimer (uiRemovePopupTimerEvent);
				m_pDelayedCloseButton = NULL;

				pDroppedDown->ClosePopupMenu ();

				if (pParentMenu != NULL)
				{
					CBCGPPopupMenu::ActivatePopupMenu (
						BCGCBProGetTopLevelFrame (this), pParentMenu);
				}
			}
			else
			{
				m_pDelayedCloseButton = pDroppedDown;
				m_pDelayedCloseButton->m_bToBeClosed = TRUE;

				SetTimer (uiRemovePopupTimerEvent, 
						max (0, m_uiPopupTimerDelay - 1), NULL);

				pDroppedDown->Redraw ();
			}
		}

		if (pHotButton != NULL && pHotButton->HasMenu ())
		{
			if (m_pDelayedButton != NULL)
			{
				KillTimer (uiPopupTimerEvent);
			}

			if ((m_pDelayedButton = pHotButton) != NULL)
			{
				if (m_pDelayedButton == m_pDelayedCloseButton)
				{
					BOOL bRestoreSubMenu = TRUE;
					
					CRect rectMenu = m_pDelayedButton->GetMenuRect ();

					if (!rectMenu.IsRectEmpty ())
					{
						CPoint point;
						
						::GetCursorPos (&point);
						ScreenToClient (&point);

						if (!rectMenu.PtInRect (point))
						{
							bRestoreSubMenu = FALSE;
						}
					}

					if (bRestoreSubMenu)
					{
						RestoreDelayedSubMenu ();
						m_pDelayedButton = NULL;
					}
				}
				else if (pHotButton->GetMenuRect ().IsRectEmpty () ||
					pHotButton->IsMenuAreaHighlighted ())
				{
					SetTimer (uiPopupTimerEvent, m_uiPopupTimerDelay, NULL);
				}
			}
		}

		// Maybe, this menu will be closed by the parent menu bar timer proc.?
		CBCGPRibbonPanelMenuBar* pParentBar = NULL;

		if (pParentMenu != NULL &&
			(pParentBar = pParentMenu->GetParentRibbonMenuBar ()) != NULL &&
			pParentBar->m_pDelayedCloseButton == pParentMenu->GetParentRibbonElement ())
		{
			pParentBar->RestoreDelayedSubMenu ();
		}
	}
	else if (pHotButton != NULL && pHotButton == m_pDelayedCloseButton)
	{
		m_pDelayedCloseButton->m_bToBeClosed = FALSE;
		m_pDelayedCloseButton = NULL;

		KillTimer (uiRemovePopupTimerEvent);
	}
}
//*************************************************************************************
void CBCGPRibbonPanelMenuBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	ASSERT_VALID (this);

	CBCGPRibbonCmdUI state;
	state.m_pOther = this;

	if (m_pCategory != NULL)
	{
		m_pCategory->OnUpdateCmdUI (&state, pTarget, bDisableIfNoHndler);
	}
	else
	{
		m_pPanel->OnUpdateCmdUI (&state, pTarget, bDisableIfNoHndler);
	}

	// update the dialog controls added to the ribbon
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}
//*************************************************************************************
void CBCGPRibbonPanelMenuBar::OnDrawMenuBorder (CDC* pDC)
{
	ASSERT_VALID (this);

	if (m_pCategory != NULL)
	{
		m_pCategory->OnDrawMenuBorder (pDC, this);
	}
	else
	{
		m_pPanel->OnDrawMenuBorder (pDC, this);
	}
}
//*************************************************************************************
int CBCGPRibbonPanelMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPPopupMenuBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CBCGPTooltipManager::CreateToolTip (m_pToolTip, this,
		BCGP_TOOLTIP_TYPE_RIBBON);

	if (m_pToolTip->GetSafeHwnd () != NULL)
	{
		CRect rectClient;
		GetClientRect (&rectClient);

		m_pToolTip->SetMaxTipWidth (640);
		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectClient, GetDlgCtrlID ());
	}

	if (m_pPanel != NULL && m_pPanel->m_pPaletteButton != NULL)
	{
		m_wndScrollBarVert.Create (WS_CHILD | WS_VISIBLE | SBS_VERT,
			CRect (0, 0, 0, 0), this, nScrollBarID);
		m_pPanel->m_pScrollBar = &m_wndScrollBarVert;
	}

	return 0;
}
//*************************************************************************************
void CBCGPRibbonPanelMenuBar::OnDestroy() 
{
	CBCGPTooltipManager::DeleteToolTip (m_pToolTip);
	CBCGPPopupMenuBar::OnDestroy();
}
//*************************************************************************************
void CBCGPRibbonPanelMenuBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPPopupMenuBar::OnSize(nType, cx, cy);
	
	if (m_pToolTip->GetSafeHwnd () != NULL)
	{
		m_pToolTip->SetToolRect (this, GetDlgCtrlID (), CRect (0, 0, cx, cy));
	}
}
//**********************************************************************************
BOOL CBCGPRibbonPanelMenuBar::OnNeedTipText(UINT /*id*/, NMHDR* pNMH, LRESULT* /*pResult*/)
{
	static CString strTipText;

	if (m_pToolTip->GetSafeHwnd () == NULL || 
		pNMH->hwndFrom != m_pToolTip->GetSafeHwnd ())
	{
		return FALSE;
	}

	if (CBCGPPopupMenu::GetActiveMenu () != NULL &&
		CBCGPPopupMenu::GetActiveMenu () != GetParent ())
	{
		return FALSE;
	}

	CBCGPRibbonBar*	pTopRibbon = GetTopLevelRibbonBar ();
	if (pTopRibbon != NULL && !pTopRibbon->IsToolTipEnabled ())
	{
		return TRUE;
	}

	LPNMTTDISPINFO	pTTDispInfo	= (LPNMTTDISPINFO) pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	CPoint point;
	
	::GetCursorPos (&point);
	ScreenToClient (&point);

	CBCGPBaseRibbonElement* pHit = HitTest (point);
	if (pHit == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID (pHit);

	strTipText = pHit->GetToolTipText ();
	if (strTipText.IsEmpty ())
	{
		return FALSE;
	}

	if (pTopRibbon == NULL || pTopRibbon->IsToolTipDescrEnabled ())
	{
		CBCGPToolTipCtrl* pToolTip = DYNAMIC_DOWNCAST (
			CBCGPToolTipCtrl, m_pToolTip);

		if (pToolTip != NULL)
		{
			ASSERT_VALID (pToolTip);
			pToolTip->SetDescription (pHit->GetDescription ());
		}
	}

	pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR) strTipText);
	return TRUE;
}
//**************************************************************************
LRESULT CBCGPRibbonPanelMenuBar::OnBCGUpdateToolTips (WPARAM wp, LPARAM)
{
	UINT nTypes = (UINT) wp;

	if (nTypes & BCGP_TOOLTIP_TYPE_RIBBON)
	{
		CBCGPTooltipManager::CreateToolTip (m_pToolTip, this,
			BCGP_TOOLTIP_TYPE_RIBBON);

		CRect rectClient;
		GetClientRect (&rectClient);

		m_pToolTip->SetMaxTipWidth (640);
		
		m_pToolTip->AddTool (this, LPSTR_TEXTCALLBACK, &rectClient, GetDlgCtrlID ());
	}

	return 0;
}
//**************************************************************************
void CBCGPRibbonPanelMenuBar::PopTooltip ()
{
	ASSERT_VALID (this);

	if (m_pToolTip->GetSafeHwnd () != NULL)
	{
		m_pToolTip->Pop ();
	}
}
//**************************************************************************
void CBCGPRibbonPanelMenuBar::SetActive (BOOL bIsActive)
{
	ASSERT_VALID (this);

	CBCGPRibbonPanelMenu* pParentMenu = DYNAMIC_DOWNCAST (
		CBCGPRibbonPanelMenu, GetParent ());
	if (pParentMenu != NULL)
	{
		ASSERT_VALID (pParentMenu);
		pParentMenu->SetActive (bIsActive);
	}
}
//**************************************************************************
CBCGPRibbonBar*	CBCGPRibbonPanelMenuBar::GetTopLevelRibbonBar () const
{
	ASSERT_VALID (this);

	if (m_pRibbonBar != NULL)
	{
		ASSERT_VALID (m_pRibbonBar);
		return m_pRibbonBar;
	}
	else
	{
		ASSERT_VALID (m_pPanelOrigin);
		ASSERT_VALID (m_pPanelOrigin->m_pParent);

		return m_pPanelOrigin->m_pParent->GetParentRibbonBar ();
	}
}
//**************************************************************************
void CBCGPRibbonPanelMenuBar::OnTimer(UINT_PTR nIDEvent)
{
	CPoint ptCursor;
	::GetCursorPos (&ptCursor);
	ScreenToClient (&ptCursor);

	if (nIDEvent == uiPopupTimerEvent)
	{
		KillTimer (uiPopupTimerEvent);

		//---------------------------------
		// Remove current tooltip (if any):
		//---------------------------------
		if (m_pToolTip->GetSafeHwnd () != NULL)
		{
			m_pToolTip->ShowWindow (SW_HIDE);
		}

		if (m_pDelayedCloseButton != NULL &&
			m_pDelayedCloseButton->GetRect ().PtInRect (ptCursor))
		{
			return;
		}

		CloseDelayedSubMenu ();

		CBCGPRibbonButton* pDelayedPopupMenuButton = m_pDelayedButton;
		m_pDelayedButton = NULL;

		if (pDelayedPopupMenuButton != NULL &&
			pDelayedPopupMenuButton->IsHighlighted ())
		{
			pDelayedPopupMenuButton->OnShowPopupMenu ();
		}
	}
	else if (nIDEvent == uiRemovePopupTimerEvent)
	{
		KillTimer (uiRemovePopupTimerEvent);

		if (m_pDelayedCloseButton != NULL)
		{
			ASSERT_VALID (m_pDelayedCloseButton);
			CBCGPPopupMenu* pParentMenu = DYNAMIC_DOWNCAST (CBCGPPopupMenu, GetParent ());

			CRect rectMenu = m_pDelayedCloseButton->GetRect ();

			if (!m_pDelayedCloseButton->GetMenuRect ().IsRectEmpty ())
			{
				rectMenu = m_pDelayedCloseButton->GetMenuRect ();
			}

			if (rectMenu.PtInRect (ptCursor))
			{
				return;
			}

			m_pDelayedCloseButton->ClosePopupMenu ();
			m_pDelayedCloseButton = NULL;

			if (pParentMenu != NULL)
			{
				CBCGPPopupMenu::ActivatePopupMenu (BCGCBProGetTopLevelFrame (this), pParentMenu);
			}
		}
	}
	else if (nIDEvent == uiAccNotifyEvent)
	{
		KillTimer (uiAccNotifyEvent);

		CRect rc;
		GetClientRect (&rc);
		if (!rc.PtInRect (ptCursor))
		{
			return;
		}

		int nIndex = CBCGPPopupMenuBar::HitTest (ptCursor);
		if (m_iAccHotItem == nIndex && m_iAccHotItem != -1)
		{
			globalData.NotifyWinEvent (EVENT_OBJECT_FOCUS, GetParent ()->GetSafeHwnd (), 
					OBJID_CLIENT , nIndex);
		}
	}
}
//*******************************************************************************
void CBCGPRibbonPanelMenuBar::CloseDelayedSubMenu ()
{
	ASSERT_VALID (this);

	if (m_pDelayedCloseButton != NULL)
	{
		ASSERT_VALID (m_pDelayedCloseButton);

		KillTimer (uiRemovePopupTimerEvent);

		m_pDelayedCloseButton->ClosePopupMenu ();
		m_pDelayedCloseButton = NULL;
	}
}
//*******************************************************************************
void CBCGPRibbonPanelMenuBar::RestoreDelayedSubMenu ()
{
	ASSERT_VALID (this);

	if (m_pDelayedCloseButton == NULL || m_pPanel == NULL)
	{
		return;
	}

	ASSERT_VALID (m_pDelayedCloseButton);
	m_pDelayedCloseButton->m_bToBeClosed = FALSE;

	CBCGPBaseRibbonElement* pPrev = m_pPanel->GetHighlighted ();

	m_pPanel->Highlight (TRUE, m_pDelayedCloseButton->GetRect ().TopLeft ());

	BOOL bUpdate = FALSE;

	if (m_pDelayedCloseButton != pPrev)
	{
		if (m_pDelayedCloseButton != NULL)
		{
			ASSERT_VALID (m_pDelayedCloseButton);
			InvalidateRect (m_pDelayedCloseButton->GetRect ());
		}

		if (pPrev != NULL)
		{
			ASSERT_VALID (pPrev);
			InvalidateRect (pPrev->GetRect ());
		}

		bUpdate = TRUE;
	}

	m_pDelayedCloseButton = NULL;

	KillTimer (uiRemovePopupTimerEvent);

	if (bUpdate)
	{
		UpdateWindow ();
	}
}
//****************************************************************************************
BOOL CBCGPRibbonPanelMenuBar::OnKey (UINT nChar)
{
	ASSERT_VALID (this);

	if (m_pPanel != NULL)
	{
		ASSERT_VALID (m_pPanel);
		return m_pPanel->OnKey (nChar);
	}

	return FALSE;
}
//*****************************************************************************
void CBCGPRibbonPanelMenuBar::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pRibbonBar);

	if (IsRibbonPanel () && m_bCustomizeMenu)
	{
		if (IsFloaty () && !IsQATPopup ())
		{
			return;
		}

		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)	// Left mouse button is pressed
		{
			return;
		}

		CPoint ptClient = point;
		ScreenToClient (&ptClient);

		CBCGPRibbonButton* pDroppedDown = GetDroppedDown ();
		if (pDroppedDown != NULL)
		{
			pDroppedDown->ClosePopupMenu ();
		}

		if (m_pDelayedButton != NULL)
		{
			KillTimer (uiPopupTimerEvent);
		}

		m_pRibbonBar->OnShowRibbonContextMenu (this, point.x, point.y, HitTest (ptClient));
	}
}
//**************************************************************************
void CBCGPRibbonPanelMenuBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pPanel);

	if (pScrollBar->GetSafeHwnd () != m_wndScrollBarVert.GetSafeHwnd () ||
		m_pPanel->m_pPaletteButton == NULL)
	{
		CBCGPPopupMenuBar::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	ASSERT_VALID (m_pPanel->m_pPaletteButton);

	SCROLLINFO scrollInfo;
	ZeroMemory(&scrollInfo, sizeof(SCROLLINFO));

    scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;

	m_wndScrollBarVert.GetScrollInfo (&scrollInfo);

	int iOffset = m_pPanel->m_nScrollOffset;
	int nMaxOffset = scrollInfo.nMax;
	int nPage = scrollInfo.nPage;

	int nRowHeight = m_pPanel->m_pPaletteButton->GetMenuRowHeight ();

	switch (nSBCode)
	{
	case SB_LINEUP:
		iOffset -= nRowHeight;
		break;

	case SB_LINEDOWN:
		iOffset += nRowHeight;
		break;

	case SB_TOP:
		iOffset = 0;
		break;

	case SB_BOTTOM:
		iOffset = nMaxOffset;
		break;

	case SB_PAGEUP:
		iOffset -= nPage;
		break;

	case SB_PAGEDOWN:
		iOffset += nPage;
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		iOffset = nPos;
		break;

	default:
		return;
	}

	iOffset = min (max (0, iOffset), nMaxOffset - nPage);

	if (iOffset == m_pPanel->m_nScrollOffset)
	{
		return;
	}

	m_pPanel->ScrollPalette (iOffset);
	m_wndScrollBarVert.SetScrollPos (iOffset);
	RedrawWindow ();
}
//**********************************************************************************
CBCGPRibbonButton* CBCGPRibbonPanelMenuBar::GetDroppedDown () const
{
	if (m_pCategory != NULL)
	{
		ASSERT_VALID (m_pCategory);

		return DYNAMIC_DOWNCAST (
			CBCGPRibbonButton, m_pCategory->GetDroppedDown ());
	}
	else
	{
		ASSERT_VALID (m_pPanel);

		return DYNAMIC_DOWNCAST (
			CBCGPRibbonButton, m_pPanel->GetDroppedDown ());
	}
}
//**********************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonPanelMenuBar::HitTest (CPoint point) const
{
	if (m_pCategory != NULL)
	{
		ASSERT_VALID (m_pCategory);
		return m_pCategory->HitTest (point, TRUE);
	}
	else
	{
		ASSERT_VALID (m_pPanel);
		return m_pPanel->HitTest (point);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonPanelMenu

IMPLEMENT_DYNAMIC(CBCGPRibbonPanelMenu, CBCGPPopupMenu)

CBCGPRibbonPanelMenu::CBCGPRibbonPanelMenu (CBCGPRibbonPanel* pPanel) :
	m_wndRibbonBar (pPanel)
{
}

CBCGPRibbonPanelMenu::CBCGPRibbonPanelMenu (
		CBCGPRibbonBar* pRibbonBar,
		const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>&	arButtons) :
	m_wndRibbonBar (pRibbonBar, arButtons)
{
}

CBCGPRibbonPanelMenu::CBCGPRibbonPanelMenu (
		CBCGPRibbonPaletteButton* pPaletteButton) :
	m_wndRibbonBar (pPaletteButton)
{
	ASSERT_VALID (pPaletteButton);

	m_bScrollable = TRUE;

	if (pPaletteButton->IsMenuResizeEnabled ())
	{
		ASSERT_VALID (m_wndRibbonBar.m_pPanel);

		CSize sizeMin = m_wndRibbonBar.m_pPanel->GetPaltteMinSize ();

		if (sizeMin.cx > 0 && sizeMin.cy > 0)
		{
			CSize sizeBorder = GetBorderSize ();

			sizeMin.cx += sizeBorder.cx * 2;
			sizeMin.cy += sizeBorder.cy * 2;
			
			EnableResize (sizeMin);
		}
	}
}

CBCGPRibbonPanelMenu::CBCGPRibbonPanelMenu (
		CBCGPRibbonCategory* pCategory, CSize size) :
	m_wndRibbonBar (pCategory, size)
{
}

CBCGPRibbonPanelMenu::CBCGPRibbonPanelMenu ()
{
}

CBCGPRibbonPanelMenu::~CBCGPRibbonPanelMenu()
{
}

BEGIN_MESSAGE_MAP(CBCGPRibbonPanelMenu, CBCGPPopupMenu)
	//{{AFX_MSG_MAP(CBCGPRibbonPanelMenu)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBCGPRibbonPanelMenuBar* CBCGPRibbonPanelMenu::GetParentRibbonMenuBar () const
{
	ASSERT_VALID (this);

	CBCGPPopupMenu* pParentMenu = GetParentPopupMenu ();
	if (pParentMenu == NULL)
	{
		return NULL;
	}

	ASSERT_VALID (pParentMenu);

	return DYNAMIC_DOWNCAST (CBCGPRibbonPanelMenuBar,
		pParentMenu->GetMenuBar ());
}
//****************************************************************************************
void CBCGPRibbonPanelMenu::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	ASSERT_VALID (this);

	if (!m_wndRibbonBar.OnKey (nChar))
	{
		CBCGPPopupMenu::OnKeyDown (nChar, nRepCnt, nFlags);
	}
}
//**************************************************************************
BOOL CBCGPRibbonPanelMenu::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/) 
{
	ASSERT_VALID (this);

	const int nSteps = abs(zDelta) / WHEEL_DELTA;

	for (int i = 0; i < nSteps; i++)
	{
		m_wndRibbonBar.OnVScroll (zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0, 
			&m_wndRibbonBar.m_wndScrollBarVert);
	}

	return TRUE;
}
//**************************************************************************
BOOL CBCGPRibbonPanelMenu::IsAlwaysClose () const
{
	return ((CBCGPRibbonPanelMenu*) this)->m_wndRibbonBar.IsMainPanel ();
}
//**************************************************************************
void CBCGPRibbonPanelMenu::DoPaint (CDC* pDC)
{
	CBCGPPopupMenu::DoPaint (pDC);
	m_wndRibbonBar.OnDrawMenuBorder (pDC);
}
//*****************************************************************************************
void CBCGPRibbonPanelMenu::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CBCGPPopupMenu::OnLButtonDown(nFlags, point);

	if (m_wndRibbonBar.IsMainPanel ())
	{
		ClientToScreen (&point);
		ScreenToClient (&point);

		m_wndRibbonBar.GetPanel ()->MouseButtonDown (point);
	}
}
//**************************************************************************
int CBCGPRibbonPanelMenu::GetBorderSize () const
{
	return IsMenuMode () ? 
		CBCGPPopupMenu::GetBorderSize () : 
		CBCGPVisualManager::GetInstance ()->GetRibbonPopupBorderSize (this);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonPanelMenu message handlers

#endif // BCGP_EXCLUDE_RIBBON

