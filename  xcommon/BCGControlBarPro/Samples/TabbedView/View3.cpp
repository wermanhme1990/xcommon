// View3.cpp : implementation file
//

#include "stdafx.h"
#include "TabbedView.h"
#include "View3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView3

IMPLEMENT_DYNCREATE(CView3, CFormView)

CView3::CView3()
	: CFormView(CView3::IDD)
{
	//{{AFX_DATA_INIT(CView3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CView3::~CView3()
{
}

void CView3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CView3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CView3, CFormView)
	//{{AFX_MSG_MAP(CView3)
	ON_WM_CONTEXTMENU()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView3 diagnostics

#ifdef _DEBUG
void CView3::AssertValid() const
{
	CFormView::AssertValid();
}

void CView3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView3 message handlers

void CView3::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CView3::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanging(lpwndpos);
	
	// Hide horizontal scrollbar:
	ShowScrollBar (SB_HORZ, FALSE);
	ModifyStyle (WS_HSCROLL, 0, SWP_DRAWFRAME);
}


BOOL CView3::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CView3::OnDraw(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (rectClient);
	
	pDC->FillSolidRect (rectClient, ::GetSysColor (COLOR_3DFACE));
}
