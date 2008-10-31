/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XFlatEdit.cpp					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/19
*********************************************************************/
#include "stdafx.h"
#include "XDC.h"
#include "XFlatEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define EVENT_TIMER   1000
/////////////////////////////////////////////////////////////////////////////
// CXFlatEdit
/////////////////////////////////////////////////////////////////////////////
CXFlatEdit::CXFlatEdit()
: m_bFlatLook(TRUE)
, m_bHasFocus(FALSE)
, m_bPainted(FALSE)
{
	m_nStyle = m_nStyleEx = 0;		
}

CXFlatEdit::~CXFlatEdit()
{
}

IMPLEMENT_DYNAMIC(CXFlatEdit, CEdit)

BEGIN_MESSAGE_MAP(CXFlatEdit, CEdit)
	//{{AFX_MSG_MAP(CXFlatEdit)
	ON_WM_NCPAINT()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_CREATETOOLTIP, OnCreateTip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTFlatEdit message handlers

BOOL CXFlatEdit::PointInRect()
{
	ASSERT(::IsWindow(m_hWnd));

	CRect rc;
	GetWindowRect(rc);

	CPoint pt;
	GetCursorPos(&pt);

	return rc.PtInRect(pt);
}

BOOL CXFlatEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (IsFlat() && (message == WM_MOUSEMOVE) && PointInRect())
	{
		SetTimer(EVENT_TIMER, 10, NULL);
		OnTimer(EVENT_TIMER);
	}

	return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CXFlatEdit::DisableFlatLook(BOOL bDisable)
{
	if (m_bFlatLook == bDisable)
	{
		m_bFlatLook = !bDisable;
		SendMessage(WM_NCPAINT);
	}
}

void CXFlatEdit::OnTimer(UINT_PTR nIDEvent)
{
	if (EVENT_TIMER == nIDEvent)
	{
		if (!PointInRect())
		{
			KillTimer(EVENT_TIMER);

			if (m_bPainted == TRUE)
			{
				SendMessage(WM_NCPAINT);
			}

			m_bPainted = FALSE;
		}

		// on mouse over, show raised.
		else if (!m_bPainted)
		{
			SendMessage(WM_NCPAINT);
			m_bPainted = TRUE;
		}
	}
	else
	{
		__super::OnTimer(nIDEvent);
	}
}

void CXFlatEdit::OnNcPaint()
{
	if (IsFlat())
	{
		CWindowDC dc(this);
		
		CRect rClient, rWindow;
		GetClientRect(rClient);
		GetWindowRect(rWindow);
		//CXTPClientRect rClient(this);
		//CXTPWindowRect rWindow(this);
		ScreenToClient(rWindow);
		rClient.OffsetRect(-rWindow.left, -rWindow.top);
		dc.ExcludeClipRect(rClient);
		rWindow.OffsetRect(-rWindow.left, -rWindow.top);

		CXBufferDC memDC(dc, rWindow);
		dc.FillSolidRect(rWindow, IsWindowEnabled()? 	GetSysColor(COLOR_WINDOW) : GetSysColor(COLOR_3DFACE));

		DrawBorders(&memDC, rWindow);
	}
	else
	{
		Default();
	}
}

void CXFlatEdit::DrawBorders(CDC* pDC, const CRect& rWindow)
{
	if (m_bHasFocus || PointInRect())
	{
		DrawBorders(pDC, this, rWindow, TRUE);
	}
	else
	{
		DrawBorders(pDC, this, rWindow, FALSE);
	}
}

void CXFlatEdit::DrawBorders(CDC* pDC, CXFlatEdit* pEdit, const CRect& rWindow, BOOL blMouseOn)
{
	if (pEdit->IsWindowEnabled())
	{		
		pDC->FillSolidRect(&rWindow, GetSysColor(COLOR_WINDOW));
		if (blMouseOn)
		{
			pDC->Draw3dRect(&rWindow, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));
		}
	}
	else
	{
		pDC->FillSolidRect(&rWindow, GetSysColor(COLOR_3DFACE));

		pDC->Draw3dRect(&rWindow, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));
	}
}
void CXFlatEdit::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	if (IsFlat())
	{
		m_bHasFocus = TRUE;
		Invalidate();
		SendMessage(WM_NCPAINT);
	}
}

void CXFlatEdit::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	if (IsFlat())
	{
		m_bHasFocus = FALSE;
		Invalidate();
		SendMessage(WM_NCPAINT);
	}
}


BOOL CXFlatEdit::IsFlat() 
{
	return m_bFlatLook;
}


void CXFlatEdit::PreSubclassWindow()
{		
	PostMessage(WM_CREATETOOLTIP);
	__super::PreSubclassWindow();
}
BOOL CXFlatEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (IsWindow(m_ToolTip.GetSafeHwnd()))
	{
		m_ToolTip.RelayEvent(pMsg);
	}	
	return __super::PreTranslateMessage(pMsg);
}
void CXFlatEdit::SetToolTip(LPCTSTR lpszToolTip)
{
	m_strToolTip = lpszToolTip;	
	PostMessage(WM_UPDATETIP);
}
CString CXFlatEdit::GetToolTip() const
{
	return m_strToolTip;
}
void CXFlatEdit::EnableToolTip( BOOL blEnable/*= TRUE*/ )
{
	PostMessage(WM_ENABLETIP, WPARAM(blEnable), 0);	
}

LRESULT CXFlatEdit::OnCreateTip( WPARAM, LPARAM l )
{
	if (!::IsWindow(m_ToolTip.GetSafeHwnd()))
	{
		m_ToolTip.Create(this);
		m_ToolTip.AddTool(this, m_strToolTip/*, rtClient, GetDlgCtrlID()*/);
	}
	return 1L;
}

LRESULT CXFlatEdit::OnUpdateTip(WPARAM w, LPARAM l)
{	
	if (::IsWindow(m_ToolTip.GetSafeHwnd())) 
	{		
		m_ToolTip.UpdateTipText(m_strToolTip, this/*, GetDlgCtrlID()*/);	
	}
	return 1L;
}

LRESULT CXFlatEdit::OnEnableToolTip(WPARAM w, LPARAM l)
{
	BOOL blEnable = (BOOL) w;
	BOOL blReturn = FALSE;
	if (::IsWindow(m_ToolTip.GetSafeHwnd()))
	{		
		blReturn = m_ToolTip.EnableWindow(blEnable);
	}
	return blReturn;
}