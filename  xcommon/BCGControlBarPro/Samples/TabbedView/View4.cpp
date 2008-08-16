// View4.cpp : implementation file
//

#include "stdafx.h"
#include "tabbedview.h"
#include "View4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView4

IMPLEMENT_DYNCREATE(CView4, CScrollView)

CView4::CView4()
{
}

CView4::~CView4()
{
}


BEGIN_MESSAGE_MAP(CView4, CScrollView)
	//{{AFX_MSG_MAP(CView4)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView4 drawing

void CView4::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 1000;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CView4::OnDraw(CDC* pDC)
{
	CRect rectClient;
	GetClientRect (&rectClient);

	pDC->FillSolidRect (rectClient, ::GetSysColor (COLOR_WINDOW));

	CString str (_T("This is a scroll view"));
	pDC->TextOut (130, 30, str);
}

/////////////////////////////////////////////////////////////////////////////
// CView4 diagnostics

#ifdef _DEBUG
void CView4::AssertValid() const
{
	CScrollView::AssertValid();
}

void CView4::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView4 message handlers

void CView4::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CScrollView::OnWindowPosChanging(lpwndpos);
	
	// Hide horizontal scrollbar:
	ShowScrollBar (SB_HORZ, FALSE);
	ModifyStyle (WS_HSCROLL, 0, SWP_DRAWFRAME);
}

CScrollBar* CView4::GetScrollBarCtrl(int nBar) const
{
	if (nBar == SB_HORZ)
	{
		return GetParentTab ()->GetScrollBar ();
	}
	
	return CScrollView::GetScrollBarCtrl(nBar);
}

void CView4::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CScrollView::OnHScroll(nSBCode, nPos, GetParentTab ()->GetScrollBar ());
}

BOOL CView4::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
