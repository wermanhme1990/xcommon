// ControlsContainer.cpp : implementation file
//

#include "stdafx.h"
#include "DockingControls.h"
#include "ControlsContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT idList = 1;
const UINT idEdit = 2;
const UINT idButton1 = 3;
const UINT idButton2 = 4;

/////////////////////////////////////////////////////////////////////////////
// CControlsContainer

CControlsContainer::CControlsContainer() :
	m_nTextHeight (0)
{
}

CControlsContainer::~CControlsContainer()
{
}


BEGIN_MESSAGE_MAP(CControlsContainer, CWnd)
	//{{AFX_MSG_MAP(CControlsContainer)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlsContainer message handlers

void CControlsContainer::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	Layout ();
}

BOOL CControlsContainer::Create(const RECT& rect, CWnd* pParentWnd) 
{
	return CWnd::Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
						rect, pParentWnd, 10);
}

BOOL CControlsContainer::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID (pDC);

	CRect rectClient;
	GetClientRect (rectClient);

	::FillRect (pDC->GetSafeHdc (), &rectClient, ::GetSysColorBrush (COLOR_3DFACE));
	return TRUE;
}

int CControlsContainer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create controls:
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (!m_wndStatic.Create (_T("Static control"), SS_LEFT, rectDummy, this) ||
		!m_wndList.Create (WS_BORDER | WS_CHILD | WS_VISIBLE, rectDummy, this, idList) ||
		!m_wndEdit.Create (WS_CHILD | WS_VISIBLE, rectDummy, this, idEdit) ||
		!m_wndButton1.Create (_T("&Add"), BS_PUSHBUTTON, rectDummy, this, idButton1) ||
		!m_wndButton2.Create (_T("&Delete"), BS_PUSHBUTTON, rectDummy, this, idButton2))
	{
		TRACE0("CControlsContainer: Unable to create controls\n");
		return -1;
	}

	m_wndList.ModifyStyleEx (0, WS_EX_CLIENTEDGE);

	// Set fonts:
	m_Font.CreateStockObject (DEFAULT_GUI_FONT);

	for (CWnd* pWndChild = GetWindow (GW_CHILD); pWndChild != NULL;
		pWndChild = pWndChild->GetNextWindow ())
	{
		ASSERT_VALID (pWndChild);
		pWndChild->SetFont (&m_Font, FALSE);
	}

	// Calculate text height:
	CClientDC dc (this);
	CFont* pOldFont = dc.SelectObject (&m_Font);
	ASSERT (pOldFont != NULL);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);

	m_nTextHeight = tm.tmHeight;

	dc.SelectObject (pOldFont);

	Layout ();
	return 0;
}

void CControlsContainer::Layout ()
{
	const int nMargin = 5;

	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);

	rectClient.DeflateRect (nMargin, nMargin);

	BOOL bShow = TRUE;

	if (rectClient.Width () < 2 * nMargin ||
		rectClient.Height () < 2 * nMargin)
	{
		// Hide all controls to prevent a bad look:
		bShow = FALSE;
	}
	else
	{
		// Set static control:
		m_wndStatic.SetWindowPos (NULL,
			rectClient.left, rectClient.top, rectClient.Width (), m_nTextHeight,
			SWP_NOZORDER | SWP_NOACTIVATE);
		rectClient.top += m_nTextHeight + nMargin;

		// Set list control:
		CRect rectList = rectClient;
		rectList.bottom -= 2 * m_nTextHeight + nMargin;

		m_wndList.SetWindowPos (NULL,
			rectList.left, rectList.top, rectList.Width (), rectList.Height (),
			SWP_NOZORDER | SWP_NOACTIVATE);

		// Set buttons:
		CRect rectButtons = rectClient;
		rectButtons.top = rectButtons.bottom - 2 * m_nTextHeight;

		m_wndButton1.SetWindowPos (NULL,
			rectButtons.left, rectButtons.top, 
			rectButtons.Width () / 2 - nMargin, rectButtons.Height (),
			SWP_NOZORDER | SWP_NOACTIVATE);

		m_wndButton2.SetWindowPos (NULL,
			rectButtons.left + rectButtons.Width () / 2 + nMargin, rectButtons.top, 
			rectButtons.Width () / 2 - nMargin, rectButtons.Height (),
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	for (CWnd* pWndChild = GetWindow (GW_CHILD); pWndChild != NULL;
		pWndChild = pWndChild->GetNextWindow ())
	{
		ASSERT_VALID (pWndChild);
		pWndChild->ShowWindow (bShow ? SW_SHOWNOACTIVATE : SW_HIDE);
	}
}
