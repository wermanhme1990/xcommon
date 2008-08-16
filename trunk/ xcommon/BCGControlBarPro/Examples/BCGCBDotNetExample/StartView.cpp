// StartView.cpp : implementation file
//

#include "stdafx.h"
#include "bcgcbdotnetexample.h"
#include "StartView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartView

#define IDC_START_IMAGE	1

IMPLEMENT_DYNCREATE(CStartView, CView)

CStartView::CStartView()
{
}

CStartView::~CStartView()
{
}

BEGIN_MESSAGE_MAP(CStartView, CView)
	//{{AFX_MSG_MAP(CStartView)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnDisableUpdate)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnDisableUpdate)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, OnDisableUpdate)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnDisableUpdate)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnDisableUpdate)
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDS_CARET_POS, OnCaretPos)
	ON_BN_CLICKED(IDC_START_IMAGE, OnClickHotSpot)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartView diagnostics

#ifdef _DEBUG
void CStartView::AssertValid() const
{
	CView::AssertValid();
}

void CStartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStartView message handlers

void CStartView::OnInitialUpdate() 
{
	CDocument* pDoc = GetDocument ();
	ASSERT_VALID (pDoc);

	CRect rectClient;
	GetClientRect (rectClient);

	m_Image.Create (rectClient, this, IDC_START_IMAGE);
	SetBitmap ();

	pDoc->SetTitle (_T("Start Page"));
}

void CStartView::OnDraw(CDC* pDC)
{
}

void CStartView::OnCaretPos(CCmdUI* pCmdUI)
{
	pCmdUI->SetText (_T(""));
	pCmdUI->Enable(FALSE);
}

BOOL CStartView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CStartView::OnDisableUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void CStartView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (m_Image.GetSafeHwnd () != NULL)
	{
		m_Image.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CStartView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
	if (m_Image.GetSafeHwnd () != NULL)
	{
		m_Image.SetFocus ();
	}	
}

void CStartView::OnClickHotSpot()
{
	UINT uiCmd = m_Image.GetHotSpot ();
	if (uiCmd == 1)
	{
		uiCmd = ID_HELP_WEB;
	}

	AfxGetMainWnd ()->SendMessage (WM_COMMAND, uiCmd);
}

void CStartView::OnSysColorChange() 
{
	CView::OnSysColorChange();
	SetBitmap ();	
}

void CStartView::SetBitmap ()
{
	if (m_Image.GetSafeHwnd () == NULL)
	{
		return;
	}

	CWindowDC dc (NULL);

	if (dc.GetDeviceCaps (BITSPIXEL) >= 16)
	{
		m_Image.SetImage (IDB_START);
		m_Image.SetHighlightHotArea (82, 45);
	}
	else
	{
		m_Image.SetImage (IDB_START16);
		m_Image.SetHighlightHotArea (0);
	}

	m_Image.AddHotSpot (1, CRect (CPoint (5, 3), CSize (132, 42)));
	m_Image.AddHotSpot (ID_VIEW_CUSTOMIZE, CRect (CPoint (220, 13), CSize (155, 36)));
	m_Image.AddHotSpot (ID_VIEW_APP_LOOK, CRect (CPoint (220, 48), CSize (145, 18)));
	m_Image.AddHotSpot (ID_FILE_NEW, CRect (CPoint (220, 68), CSize (157, 36)));
	m_Image.AddHotSpot (ID_HELP_WEB, CRect (CPoint (220, 102), CSize (136, 36)));
}
