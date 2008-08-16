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

// BCGPInplaceToolTipCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "multimon.h"
#include "bcgcbpro.h"
#include "BCGGlobals.h"
#include "BCGPInplaceToolTipCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPInplaceToolTipCtrl

CString CBCGPInplaceToolTipCtrl::m_strClassName;

IMPLEMENT_DYNAMIC(CBCGPInplaceToolTipCtrl, CWnd)

CBCGPInplaceToolTipCtrl::CBCGPInplaceToolTipCtrl()
{
	m_rectLast.SetRectEmpty ();
	m_nTextMargin = 10;
	m_hFont	= NULL;
	m_pWndParent = NULL;
}

CBCGPInplaceToolTipCtrl::~CBCGPInplaceToolTipCtrl()
{
}


BEGIN_MESSAGE_MAP(CBCGPInplaceToolTipCtrl, CWnd)
	//{{AFX_MSG_MAP(CBCGPInplaceToolTipCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPInplaceToolTipCtrl message handlers

BOOL CBCGPInplaceToolTipCtrl::Create (CWnd* pWndParent) 
{
	ASSERT_VALID (pWndParent);
	m_pWndParent = pWndParent;

	if (m_strClassName.IsEmpty ())
	{
		m_strClassName = ::AfxRegisterWndClass (
			CS_SAVEBITS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_BTNFACE + 1));
	}  

	return CreateEx (0,
					m_strClassName, _T (""), WS_POPUP,
					0, 0, 0, 0,
					pWndParent->GetSafeHwnd (), (HMENU) NULL);
}

BOOL CBCGPInplaceToolTipCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CBCGPInplaceToolTipCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect (rect);

	dc.FillSolidRect (&rect, ::GetSysColor (COLOR_INFOBK));
	dc.Draw3dRect (rect, GetSysColor (COLOR_INFOTEXT), GetSysColor (COLOR_INFOTEXT));

	CFont* pPrevFont = m_hFont == NULL ?
		(CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT) :
		dc.SelectObject (CFont::FromHandle (m_hFont));
	ASSERT (pPrevFont != NULL);

	dc.SetBkMode (TRANSPARENT);
	dc.SetTextColor(::GetSysColor (COLOR_INFOTEXT));

	if (m_strText.FindOneOf (_T("\n")) != -1)	// multi-line tooltip
	{
		rect.DeflateRect (m_nTextMargin, m_nTextMargin);
		if (rect.Height () < m_rectLast.Height ())
		{
			// center tooltip vertically
			rect.top += (m_rectLast.Height () - rect.Height ()) / 2;
		}

		dc.DrawText (m_strText, rect, DT_LEFT | DT_WORDBREAK);
	}
	else // single line tooltip
	{
		rect.DeflateRect (m_nTextMargin, 0);
		dc.DrawText (m_strText, rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	}

	dc.SelectObject (pPrevFont);
}
//*******************************************************************************************
void CBCGPInplaceToolTipCtrl::Track (CRect rect, const CString& strText)
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	if (m_rectLast == rect && m_strText == strText)
	{
		return;
	}

	ASSERT_VALID (m_pWndParent);

	m_rectLast = rect;
	m_strText = strText;

	CClientDC dc (this);

	CFont* pPrevFont = m_hFont == NULL ?
		(CFont*) dc.SelectStockObject (DEFAULT_GUI_FONT) :
		dc.SelectObject (CFont::FromHandle (m_hFont));
	ASSERT (pPrevFont != NULL);

	int nTextHeight = rect.Height ();
	int nTextWidth = rect.Width ();
	if (m_strText.FindOneOf (_T("\n")) != -1)	// multi-line tooltip
	{
		const int nDefaultHeight = globalData.GetTextHeight ();
		const int nDefaultWidth = 200;
		CRect rectText (0, 0, nDefaultWidth, nDefaultHeight);
 
		nTextHeight = dc.DrawText (m_strText, rectText, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
		nTextWidth = rectText.Width ();
		nTextHeight += 2 * m_nTextMargin;
		nTextWidth += 2 * m_nTextMargin;
	}
	else
	{
		nTextWidth = dc.GetTextExtent (m_strText).cx + 2 * m_nTextMargin;
	}

	dc.SelectObject (pPrevFont);

	if (m_pWndParent->GetExStyle () & WS_EX_LAYOUTRTL)
	{
		rect.left = rect.right - nTextWidth;
	}
	else
	{
		rect.right = rect.left + nTextWidth;
	}
	rect.bottom = rect.top + nTextHeight;
	if (rect.Height () < m_rectLast.Height ())
	{
		rect.top = m_rectLast.top;
		rect.bottom = m_rectLast.bottom;
	}

	MONITORINFO mi;
	mi.cbSize = sizeof (MONITORINFO);

	CRect rectScreen;

	if (GetMonitorInfo (MonitorFromPoint (rect.TopLeft (), MONITOR_DEFAULTTONEAREST), &mi))
	{
		rectScreen = mi.rcWork;
	}
	else
	{
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectScreen, 0);
	}

	if (rect.Width () > rectScreen.Width ())
	{
		rect.left = rectScreen.left;
		rect.right = rectScreen.right;
	}
	else if (rect.right > rectScreen.right)
	{
		rect.right = rectScreen.right;
		rect.left = rect.right - nTextWidth;
	}
	else if (rect.left < rectScreen.left)
	{
		rect.left = rectScreen.left;
		rect.right = rect.left + nTextWidth;
	}

	if (rect.Height () > rectScreen.Height ())
	{
		rect.top = rectScreen.top;
		rect.bottom = rectScreen.bottom;
	}
	else if (rect.bottom > rectScreen.bottom)
	{
		rect.bottom = rectScreen.bottom;
		rect.top = rect.bottom - nTextHeight;
	}
	else if (rect.top < rectScreen.top)
	{
		rect.top = rectScreen.top;
		rect.bottom = rect.bottom + nTextHeight;
	}

	SetWindowPos (&wndTop, rect.left, rect.top, 
		rect.Width (), rect.Height (), SWP_NOACTIVATE);
  
    ShowWindow (SW_SHOWNOACTIVATE);
	Invalidate ();
	UpdateWindow ();

	SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_ARROW));
}
//*******************************************************************************************
void CBCGPInplaceToolTipCtrl::Hide ()
{
	if (GetSafeHwnd () != NULL)
	{
		ShowWindow (SW_HIDE);
	}
}
//*******************************************************************************************
void CBCGPInplaceToolTipCtrl::Deactivate ()
{
	m_strText.Empty ();
	m_rectLast.SetRectEmpty ();

	Hide ();
}
//*****************************************************************************
LRESULT CBCGPInplaceToolTipCtrl::OnSetFont (WPARAM wParam, LPARAM lParam)
{
	BOOL bRedraw = (BOOL) LOWORD (lParam);

	m_hFont = (HFONT) wParam;

	if (bRedraw)
	{
		Invalidate ();
		UpdateWindow ();
	}

	return 0;
}
//***************************************************************************
BOOL CBCGPInplaceToolTipCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_MOUSEFIRST &&
		pMsg->message <= WM_MOUSELAST)
	{
		if (pMsg->message != WM_MOUSEMOVE)
		{
			Hide ();
		}

		ASSERT_VALID (m_pWndParent);

		// the parent should receive the mouse message in its client coordinates
		CPoint pt (LOWORD (pMsg->lParam), HIWORD (pMsg->lParam));
		MapWindowPoints (m_pWndParent, &pt, 1);
		LPARAM lParam = MAKELPARAM (pt.x, pt.y);

		m_pWndParent->SendMessage (pMsg->message, pMsg->wParam, lParam);
		return TRUE;
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}
