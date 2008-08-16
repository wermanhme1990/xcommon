//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2006 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// BCGPRibbonStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbpro.h"
#include "BCGPContextMenuManager.h"
#include "BCGPRibbonStatusBar.h"
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPRibbonStatusBarPane.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonLabel.h"
#include "BCGPToolbarMenuButton.h"
#include "RegPath.h"
#include "BCGPRegistry.h"
#include "BCGPLocalResource.h"
#include "bcgprores.h"

#ifndef BCGP_EXCLUDE_RIBBON

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strRibbonProfile	= _T("BCGPRibbons");

#define REG_SECTION_FMT					_T("%sBCGRibbonBar-%d")
#define REG_SECTION_FMT_EX				_T("%sBCGRibbonBar-%d%x")
#define REG_ENTRY_STATUSBAR_PANES		_T("StatusBarPanes")

static const int nMaxValueLen = 50;

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonStatusBarCustomizeButton

class CBCGPRibbonStatusBarCustomizeButton : public CBCGPRibbonButton
{
	DECLARE_DYNCREATE(CBCGPRibbonStatusBarCustomizeButton)

public:
	CBCGPRibbonStatusBarCustomizeButton()	{}

	CBCGPRibbonStatusBarCustomizeButton (LPCTSTR lpszLabel) :
		CBCGPRibbonButton (0, lpszLabel)
	{
	}

	virtual CSize GetRegularSize (CDC* pDC)
	{
		ASSERT_VALID (pDC);

		CBCGPBaseRibbonElement* pElement = (CBCGPBaseRibbonElement*) m_dwData;
		ASSERT_VALID (pElement);

		CSize size = CBCGPRibbonButton::GetRegularSize (pDC);

		size.cx += size.cy * 2;	// Reserve space for checkbox

		CString strValue = pElement->GetText ();

		if (strValue.GetLength () > nMaxValueLen)
		{
			strValue = strValue.Left (nMaxValueLen - 1);
		}

		if (!strValue.IsEmpty ())
		{
			size.cx += pDC->GetTextExtent (strValue).cx + 4 * m_szMargin.cx;
		}

		return size;
	}

	virtual void OnDraw (CDC* pDC)
	{
		ASSERT_VALID (pDC);

		CBCGPBaseRibbonElement* pElement = (CBCGPBaseRibbonElement*) m_dwData;
		ASSERT_VALID (pElement);

		CBCGPToolbarMenuButton dummy;

		dummy.m_strText = m_strText;

		CString strValue = pElement->GetText ();

		if (strValue.GetLength () > nMaxValueLen)
		{
			strValue = strValue.Left (nMaxValueLen - 1);
		}

		if (!strValue.IsEmpty ())
		{
			dummy.m_strText += _T('\t');
			dummy.m_strText += strValue;
		}

		dummy.m_bMenuMode = TRUE;
		dummy.m_pWndParent = GetParentWnd ();

		if (pElement->IsVisible ())
		{
			dummy.m_nStyle |= TBBS_CHECKED;
		}

		dummy.OnDraw (pDC, m_rect, NULL, TRUE, FALSE, m_bIsHighlighted);
	}

	virtual void OnClick (CPoint /*point*/)
	{
		CBCGPBaseRibbonElement* pElement = (CBCGPBaseRibbonElement*) m_dwData;
		ASSERT_VALID (pElement);

		pElement->SetVisible (!pElement->IsVisible ());
		Redraw ();

		CBCGPRibbonBar* pRibbonStatusBar = pElement->GetParentRibbonBar ();
		ASSERT_VALID (pRibbonStatusBar);

		pRibbonStatusBar->RecalcLayout ();
		pRibbonStatusBar->RedrawWindow ();

		CFrameWnd* pParentFrame = pRibbonStatusBar->GetParentFrame ();
		ASSERT_VALID (pParentFrame);

		pParentFrame->RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);

		CRect rectScreen;
		pRibbonStatusBar->GetWindowRect (&rectScreen);

		CBCGPPopupMenu::UpdateAllShadows (rectScreen);
	}
};

IMPLEMENT_DYNCREATE(CBCGPRibbonStatusBarCustomizeButton, CBCGPRibbonButton)

const int xExtAreaMargin = 5;

/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonStatusBar

IMPLEMENT_DYNAMIC(CBCGPRibbonStatusBar, CBCGPRibbonBar)

CBCGPRibbonStatusBar::CBCGPRibbonStatusBar()
{
	m_cxSizeBox = 0;
	m_cxFree = -1;
	m_rectSizeBox.SetRectEmpty ();
}

CBCGPRibbonStatusBar::~CBCGPRibbonStatusBar()
{
	RemoveAll ();
}


BEGIN_MESSAGE_MAP(CBCGPRibbonStatusBar, CBCGPRibbonBar)
	//{{AFX_MSG_MAP(CBCGPRibbonStatusBar)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonStatusBar message handlers

BOOL CBCGPRibbonStatusBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ((m_dwStyle & (CBRS_ALIGN_ANY|CBRS_BORDER_ANY)) == CBRS_BOTTOM)
	{
		m_dwStyle &= ~(CBRS_BORDER_ANY|CBRS_BORDER_3D);
	}

	return CBCGPRibbonBar::PreCreateWindow(cs);
}
//********************************************************************************
BOOL CBCGPRibbonStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	return CreateEx(pParentWnd, 0, dwStyle, nID);
}
//********************************************************************************
BOOL CBCGPRibbonStatusBar::CreateEx (CWnd* pParentWnd, DWORD /*dwCtrlStyle*/, DWORD dwStyle, 
							 UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	// save the style
	SetBarAlignment (dwStyle & CBRS_ALL);

	// create the HWND
	CRect rect;
	rect.SetRectEmpty();

	m_dwBCGStyle = 0; // can't float, resize, close, slide

	if (pParentWnd->GetStyle() & WS_THICKFRAME)
	{
		dwStyle |= SBARS_SIZEGRIP;
	}

	if (!CWnd::Create (globalData.RegisterWindowClass (_T("BCGPRibbonStatusBar")),
		NULL, dwStyle | WS_CLIPSIBLINGS, rect, pParentWnd, nID))
	{
		return FALSE;
	}

	if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPFrameWnd)))
	{
		((CBCGPFrameWnd*) pParentWnd)->AddControlBar (this);
	}
	else if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPMDIFrameWnd)))
	{
		((CBCGPMDIFrameWnd*) pParentWnd)->AddControlBar (this);
	}
	else
	{
		ASSERT (FALSE);
		return FALSE;
	}
	return TRUE;
}
//********************************************************************************
CSize CBCGPRibbonStatusBar::CalcFixedLayout(BOOL, BOOL /*bHorz*/)
{
	ASSERT_VALID(this);

	CClientDC dc (this);

	CFont* pOldFont = dc.SelectObject (GetFont ());
	ASSERT (pOldFont != NULL);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);

	int i = 0;
	int cyMax = tm.tmHeight;

	for (i = 0; i < m_arExElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arExElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (&dc);

		CSize sizeElem = pElem->GetCompactSize (&dc);
		cyMax = max (cyMax, sizeElem.cy + 1);
	}

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (&dc);

		CSize sizeElem = pElem->GetCompactSize (&dc);
		cyMax = max (cyMax, sizeElem.cy + 1);
	}

	dc.SelectObject (pOldFont);

	return CSize (32767, cyMax + CY_BORDER * 4);
}
//********************************************************************************
void CBCGPRibbonStatusBar::AddElement (CBCGPBaseRibbonElement* pElement, 
									   LPCTSTR lpszLabel, BOOL bIsVisible)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElement);
	ASSERT (lpszLabel != NULL);

	pElement->SetParentRibbonBar (this);
	pElement->m_bIsVisible = bIsVisible;

	m_arElements.Add (pElement);
	m_arElementLabels.Add (lpszLabel);

	CleanUpCustomizeItems ();
}
//********************************************************************************
void CBCGPRibbonStatusBar::AddExtendedElement (CBCGPBaseRibbonElement* pElement,
											   LPCTSTR lpszLabel, BOOL bIsVisible)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElement);
	ASSERT (lpszLabel != NULL);

	pElement->SetParentRibbonBar (this);
	pElement->m_bIsVisible = bIsVisible;

	CBCGPRibbonStatusBarPane* pPane = DYNAMIC_DOWNCAST (
		CBCGPRibbonStatusBarPane, pElement);

	if (pPane != NULL)
	{
		ASSERT_VALID (pPane);
		pPane->m_bIsExtended = TRUE;
	}

	m_arExElements.Add (pElement);
	m_arExElementLabels.Add (lpszLabel);

	CleanUpCustomizeItems ();
}
//********************************************************************************
void CBCGPRibbonStatusBar::AddSeparator ()
{
	ASSERT_VALID (this);

	CBCGPRibbonSeparator* pSeparator = new CBCGPRibbonSeparator;
	pSeparator->SetParentRibbonBar (this);

	m_arElements.Add (pSeparator);
	m_arElementLabels.Add (_T(""));

	CleanUpCustomizeItems ();
}
//********************************************************************************
void CBCGPRibbonStatusBar::AddDynamicElement (CBCGPBaseRibbonElement* pElement)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pElement);

	pElement->SetParentRibbonBar (this);
	pElement->m_bIsVisible = TRUE;

	m_arElements.Add (pElement);
	m_arElementLabels.Add (_T(""));

	m_lstDynElements.AddTail (pElement);
}
//********************************************************************************
BOOL CBCGPRibbonStatusBar::RemoveElement (UINT uiID)
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (pElem->GetID () == uiID)
		{
			POSITION pos = m_lstDynElements.Find (pElem);
			if (pos != NULL)
			{
				// Element is dynamic: remove it from dynamic elements list
				m_lstDynElements.RemoveAt (pos);
			}

			if (pElem == m_pHighlighted)
			{
				m_pHighlighted = NULL;
			}

			if (pElem == m_pPressed)
			{
				m_pPressed = NULL;
			}

			delete pElem;
			m_arElements.RemoveAt (i);

			return TRUE;
		}
	}

	for (i = 0; i < m_arExElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arExElements [i];
		ASSERT_VALID (pElem);

		if (pElem->GetID () == uiID)
		{
			if (pElem == m_pHighlighted)
			{
				m_pHighlighted = NULL;
			}

			if (pElem == m_pPressed)
			{
				m_pPressed = NULL;
			}

			delete pElem;
			m_arExElements.RemoveAt (i);

			return TRUE;
		}
	}

	return FALSE;
}
//********************************************************************************
void CBCGPRibbonStatusBar::RemoveAll ()
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		delete m_arElements [i];
	}

	m_arElements.RemoveAll ();

	for (i = 0; i < m_arExElements.GetSize (); i++)
	{
		delete m_arExElements [i];
	}

	m_arExElements.RemoveAll ();
	
	m_arElementLabels.RemoveAll ();
	m_arExElementLabels.RemoveAll ();

	CleanUpCustomizeItems ();
}
//********************************************************************************
int CBCGPRibbonStatusBar::GetCount () const
{
	ASSERT_VALID (this);
	return (int) m_arElements.GetSize ();
}
//********************************************************************************
int CBCGPRibbonStatusBar::GetExCount () const
{
	ASSERT_VALID (this);
	return (int) m_arExElements.GetSize ();
}
//********************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonStatusBar::GetElement (int nIndex)
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= (int) m_arElements.GetSize ())
	{
		ASSERT (FALSE);
		return NULL;
	}

	return m_arElements [nIndex];
}
//********************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonStatusBar::GetExElement (int nIndex)
{
	ASSERT_VALID (this);

	if (nIndex < 0 || nIndex >= (int) m_arExElements.GetSize ())
	{
		ASSERT (FALSE);
		return NULL;
	}

	return m_arExElements [nIndex];
}
//********************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonStatusBar::FindElement (UINT uiID)
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < m_arElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arElements [i]);

		if (m_arElements [i]->GetID () == uiID)
		{
			return m_arElements [i];
		}
	}

	for (i = 0; i < m_arExElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arExElements [i]);

		if (m_arExElements [i]->GetID () == uiID)
		{
			return m_arExElements [i];
		}
	}

	return NULL;
}
//********************************************************************************
BOOL CBCGPRibbonStatusBar::GetExtendedArea (CRect& rect) const
{
	ASSERT_VALID (this);

	for (int i = 0; i < m_arExElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arExElements [i];
		ASSERT_VALID (pElem);

		CRect rectElem = pElem->GetRect ();
		
		if (!rectElem.IsRectEmpty ())
		{
			CRect rectClient;
			GetClientRect (rectClient);

			rect = rectClient;
			rect.left = rectElem.left - xExtAreaMargin;

			return TRUE;
		}
	}

	return FALSE;
}
//********************************************************************************
void CBCGPRibbonStatusBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPRibbonBar::OnSize(nType, cx, cy);

	RecalcLayout ();
	RedrawWindow ();
}
//********************************************************************************
BCGNcHitTestType CBCGPRibbonStatusBar::OnNcHitTest(CPoint point)
{
	BOOL bRTL = GetExStyle() & WS_EX_LAYOUTRTL;

	// hit test the size box - convert to HTCAPTION if so
	if (!m_rectSizeBox.IsRectEmpty ())
	{
		CRect rect = m_rectSizeBox;
		ClientToScreen(&rect);

		if (rect.PtInRect (point))
		{
			OnCancelMode ();
			return bRTL ? HTBOTTOMLEFT : HTBOTTOMRIGHT;
		}
	}

	return CBCGPRibbonBar::OnNcHitTest(point);
}
//********************************************************************************
void CBCGPRibbonStatusBar::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (!m_cxSizeBox != 0 && (nID & 0xFFF0) == SC_SIZE)
	{
		CFrameWnd* pFrameWnd = BCGPGetParentFrame(this);
		if (pFrameWnd != NULL)
		{
			pFrameWnd->SendMessage(WM_SYSCOMMAND, (WPARAM)nID, lParam);
			return;
		}
	}

	CBCGPRibbonBar::OnSysCommand(nID, lParam);
}
//**********************************************************************************
void CBCGPRibbonStatusBar::RecalcLayout ()
{
	ASSERT_VALID (this);
	ASSERT (GetSafeHwnd () != NULL);

	// get the drawing area for the status bar
	CRect rect;
	GetClientRect(rect);

	// the size box is based off the size of a scrollbar
	m_cxSizeBox = min(GetSystemMetrics(SM_CXVSCROLL)+1, rect.Height());
	
	CFrameWnd* pFrameWnd = BCGPGetParentFrame(this);
	if (pFrameWnd != NULL && pFrameWnd->IsZoomed())
	{
		m_cxSizeBox = 0;
	}

	if ((GetStyle() & SBARS_SIZEGRIP) == 0)
	{
		m_cxSizeBox = 0;
	}

	CClientDC dc (this);

	CFont* pOldFont = dc.SelectObject (GetFont ());
	ASSERT (pOldFont != NULL);

	int xMax = (rect.right -= m_cxSizeBox);

	if (m_cxSizeBox != 0)
	{
		int cxMax = min(m_cxSizeBox, rect.Height()+m_cyTopBorder);

		m_rectSizeBox = rect;
		m_rectSizeBox.left = rect.right;
		m_rectSizeBox.right = m_rectSizeBox.left + cxMax;
	}
	else
	{
		m_rectSizeBox.SetRectEmpty ();
	}

	int i = 0;

	rect.top += 2;

	//---------------------------------
	// Repos extended (right) elements:
	//---------------------------------
	for (i = (int) m_arExElements.GetSize () - 1; i >= 0; i--)
	{
		CBCGPBaseRibbonElement* pElem = m_arExElements [i];
		ASSERT_VALID (pElem);

		pElem->OnCalcTextSize (&dc);

		CSize sizeElem = pElem->GetCompactSize (&dc);

		if (xMax - sizeElem.cx < rect.left || !pElem->m_bIsVisible)
		{
			pElem->SetRect (CRect (0, 0, 0, 0));
		}
		else
		{
			if (pElem->CanBeStretched ())
			{
				pElem->SetRect (CRect (xMax - sizeElem.cx, rect.top, xMax, rect.bottom));
			}
			else
			{
				int yOffset = max (0, (rect.Height () - sizeElem.cy) / 2);

				pElem->SetRect (CRect (CPoint (xMax - sizeElem.cx, rect.top + yOffset), sizeElem));
			}

			xMax = pElem->GetRect ().left;
		}

		pElem->OnAfterChangeRect (&dc);
	}

	xMax -= 2 * xExtAreaMargin;

	//----------------------------
	// Repos main (left) elements:
	//----------------------------
	int x = rect.left;

	m_cxFree = xMax - rect.left;

	BOOL bIsPrevSeparator = TRUE;
	CBCGPBaseRibbonElement* pLastVisibleElem = NULL;

	for (i = 0; i < (int) m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		BOOL bIsSeparator = pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator));

		if (bIsSeparator && bIsPrevSeparator)
		{
			pElem->SetRect (CRect (0, 0, 0, 0));
			continue;
		}

		pElem->OnCalcTextSize (&dc);

		CSize sizeElem = pElem->GetCompactSize (&dc);

		if (x + sizeElem.cx > xMax || !pElem->m_bIsVisible)
		{
			pElem->SetRect (CRect (0, 0, 0, 0));
		}
		else
		{
			if (pElem->CanBeStretched ())
			{
				pElem->SetRect (CRect (x, rect.top, x + sizeElem.cx, rect.bottom));
			}
			else
			{
				int yOffset = max (0, (rect.Height () - sizeElem.cy) / 2);

				pElem->SetRect (CRect (CPoint (x, rect.top + yOffset), sizeElem));
			}

			x += sizeElem.cx;

			m_cxFree = xMax - x;
			bIsPrevSeparator = bIsSeparator;

			pLastVisibleElem = pElem;
		}

		pElem->OnAfterChangeRect (&dc);
	}

	if (pLastVisibleElem != NULL)
	{
		ASSERT_VALID (pLastVisibleElem);

		if (pLastVisibleElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
		{
			// Last visible element is separator - hide it:
			pLastVisibleElem->SetRect (CRect (0, 0, 0, 0));
			pLastVisibleElem->OnAfterChangeRect (&dc);
		}
	}

	dc.SelectObject (pOldFont);
}
//*****************************************************************************************************
void CBCGPRibbonStatusBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBCGPMemDC memDC (dc, this);
	CDC* pDC = &memDC.GetDC ();

	CRect rectClip;
	dc.GetClipBox (rectClip);

	CRgn rgnClip;

	if (!rectClip.IsRectEmpty ())
	{
		rgnClip.CreateRectRgnIndirect (rectClip);
		pDC->SelectClipRgn (&rgnClip);
	}
	
	pDC->SetBkMode (TRANSPARENT);
	
	CRect rectClient;
	GetClientRect (rectClient);

	OnFillBackground (pDC, rectClient);

	// draw the size box in the bottom right corner
	if (!m_rectSizeBox.IsRectEmpty ())
	{
		CBCGPVisualManager::GetInstance ()->OnDrawStatusBarSizeBox (pDC, NULL,	// TODO
			m_rectSizeBox);
	}

	CFont* pOldFont = pDC->SelectObject (GetFont ());
	ASSERT (pOldFont != NULL);

	int i = 0;

	for (i = 0; i < (int) m_arElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arElements [i]);
		m_arElements [i]->OnDraw (pDC);
	}

	for (i = 0; i < (int) m_arExElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arExElements [i]);
		m_arExElements [i]->OnDraw (pDC);
	}

	pDC->SelectObject (pOldFont);
	pDC->SelectClipRgn (NULL);
}
//******************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonStatusBar::HitTest (CPoint point, 
												 BOOL /*bCheckActiveCategory*/,
												 BOOL /*bCheckPanelCaption*/)
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < (int) m_arElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arElements [i]);

		if (m_arElements [i]->GetRect ().PtInRect (point))
		{
			return m_arElements [i]->HitTest (point);
		}
	}

	for (i = 0; i < (int) m_arExElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arExElements [i]);

		if (m_arExElements [i]->GetRect ().PtInRect (point))
		{
			return m_arExElements [i]->HitTest (point);
		}
	}

	return NULL;
}
//*******************************************************************************
void CBCGPRibbonStatusBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	ASSERT_VALID (this);

	CBCGPRibbonCmdUI state;
	state.m_pOther = this;

	int i = 0;

	for (i = 0; i < (int) m_arElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arElements [i]);
		m_arElements [i]->OnUpdateCmdUI (&state, pTarget, bDisableIfNoHndler);
	}

	for (i = 0; i < (int) m_arExElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arExElements [i]);
		m_arExElements [i]->OnUpdateCmdUI (&state, pTarget, bDisableIfNoHndler);
	}

	// update the dialog controls added to the ribbon
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}
//*******************************************************************************
CBCGPBaseRibbonElement* CBCGPRibbonStatusBar::GetDroppedDown ()
{
	ASSERT_VALID (this);

	int i = 0;

	for (i = 0; i < (int) m_arElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arElements [i]);

		if (m_arElements [i]->GetDroppedDown () != NULL)
		{
			return m_arElements [i];
		}
	}

	for (i = 0; i < (int) m_arExElements.GetSize (); i++)
	{
		ASSERT_VALID (m_arExElements [i]);

		if (m_arExElements [i]->GetDroppedDown () != NULL)
		{
			return m_arExElements [i];
		}
	}

	return NULL;
}
//*****************************************************************************
void CBCGPRibbonStatusBar::OnControlBarContextMenu (CWnd* /*pParentFrame*/, CPoint point)
{
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)	// Left mouse button is pressed
	{
		return;
	}

	if (m_arCustomizeItems.GetSize () == 0)
	{
		CString strCaption;

		{
			CBCGPLocalResource locaRes;
			strCaption.LoadString (IDS_BCGBARRES_STATBAR_CUSTOMIZE);
		}

		m_arCustomizeItems.Add (new CBCGPRibbonLabel (strCaption));

		int i = 0;

		for (i = 0; i < (int) m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (m_lstDynElements.Find (pElem) != NULL)
			{
				// Dynamic element, don't add it to customization menu
				continue;
			}

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
			{
				m_arCustomizeItems.Add (new CBCGPRibbonSeparator (TRUE));
			}
			else
			{
				CBCGPRibbonStatusBarCustomizeButton* pItem = new 
					CBCGPRibbonStatusBarCustomizeButton (
						m_arElementLabels [i]);

				pItem->SetData ((DWORD_PTR) pElem);
				m_arCustomizeItems.Add (pItem);
			}
		}

		if ((int) m_arCustomizeItems.GetSize () > 1 && m_arExElements.GetSize () > 0)
		{
			m_arCustomizeItems.Add (new CBCGPRibbonSeparator (TRUE));
		}

		for (i = 0; i < (int) m_arExElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arExElements [i];
			ASSERT_VALID (pElem);

			if (pElem->IsKindOf (RUNTIME_CLASS (CBCGPRibbonSeparator)))
			{
				m_arCustomizeItems.Add (new CBCGPRibbonSeparator);
			}
			else
			{
				CBCGPRibbonStatusBarCustomizeButton* pItem = new 
					CBCGPRibbonStatusBarCustomizeButton (
						m_arExElementLabels [i]);

				pItem->SetData ((DWORD_PTR) pElem);
				m_arCustomizeItems.Add (pItem);
			}
		}
	}
	
	CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu (this, m_arCustomizeItems);
	pMenu->SetMenuMode ();
	pMenu->EnableCustomizeMenu (FALSE);
	
	pMenu->Create (this, point.x, point.y, (HMENU) NULL);
}
//*****************************************************************************
void CBCGPRibbonStatusBar::CleanUpCustomizeItems ()
{
	for (int i = 0; i < (int) m_arCustomizeItems.GetSize (); i++)
	{
		ASSERT_VALID (m_arCustomizeItems [i]);
		delete m_arCustomizeItems [i];
	}

	m_arCustomizeItems.RemoveAll ();
}
//*******************************************************************************************
BOOL CBCGPRibbonStatusBar::SaveState (LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	CString strProfileName = ::BCGPGetRegPath (strRibbonProfile, lpszProfileName);

	BOOL bResult = FALSE;

	if (nIndex == -1)
	{
		nIndex = GetDlgCtrlID ();
	}

	CString strSection;
	if (uiID == (UINT) -1)
	{
		strSection.Format (REG_SECTION_FMT, strProfileName, nIndex);
	}
	else
	{
		strSection.Format (REG_SECTION_FMT_EX, strProfileName, nIndex, uiID);
	}

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, FALSE);

	if (reg.CreateKey (strSection))
	{
		CList<UINT,UINT> lstInvisiblePanes;

		int i = 0;

		for (i = 0; i < (int) m_arElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements [i];
			ASSERT_VALID (pElem);

			if (!pElem->m_bIsVisible && pElem->GetID () != 0)
			{
				lstInvisiblePanes.AddTail (pElem->GetID ());
			}
		}

		for (i = 0; i < (int) m_arExElements.GetSize (); i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arExElements [i];
			ASSERT_VALID (pElem);

			if (!pElem->m_bIsVisible && pElem->GetID () != 0)
			{
				lstInvisiblePanes.AddTail (pElem->GetID ());
			}
		}

		reg.Write (REG_ENTRY_STATUSBAR_PANES, lstInvisiblePanes);
	}

	bResult = CBCGPControlBar::SaveState (lpszProfileName, nIndex, uiID);

	return bResult;
}
//*********************************************************************
BOOL CBCGPRibbonStatusBar::LoadState (LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	CString strProfileName = ::BCGPGetRegPath (strRibbonProfile, lpszProfileName);

	if (nIndex == -1)
	{
		nIndex = GetDlgCtrlID ();
	}

	CString strSection;
	if (uiID == (UINT) -1)
	{
		strSection.Format (REG_SECTION_FMT, strProfileName, nIndex);
	}
	else
	{
		strSection.Format (REG_SECTION_FMT_EX, strProfileName, nIndex, uiID);
	}

	CBCGPRegistrySP regSP;
	CBCGPRegistry& reg = regSP.Create (FALSE, TRUE);

	if (!reg.Open (strSection))
	{
		return FALSE;
	}

	CList<UINT,UINT> lstInvisiblePanes;
	reg.Read (REG_ENTRY_STATUSBAR_PANES, lstInvisiblePanes);

	int i = 0;

	for (i = 0; i < (int) m_arElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements [i];
		ASSERT_VALID (pElem);

		if (lstInvisiblePanes.Find (pElem->GetID ()) != NULL)
		{
			pElem->SetVisible (FALSE);
		}
	}

	for (i = 0; i < (int) m_arExElements.GetSize (); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arExElements [i];
		ASSERT_VALID (pElem);

		if (lstInvisiblePanes.Find (pElem->GetID ()) != NULL)
		{
			pElem->SetVisible (FALSE);
		}
	}

	RecalcLayout ();

	return CBCGPControlBar::LoadState (lpszProfileName, nIndex, uiID);
}

#endif // BCGP_EXCLUDE_RIBBON
