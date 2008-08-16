// WorkSpaceToolBarView.cpp : implementation of the CWorkSpaceToolBarView class
//

#include "stdafx.h"
#include "WorkSpaceToolBar.h"

#include "WorkSpaceToolBarDoc.h"
#include "WorkSpaceToolBarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarView

IMPLEMENT_DYNCREATE(CWorkSpaceToolBarView, CView)

BEGIN_MESSAGE_MAP(CWorkSpaceToolBarView, CView)
	//{{AFX_MSG_MAP(CWorkSpaceToolBarView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarView construction/destruction

CWorkSpaceToolBarView::CWorkSpaceToolBarView()
{
	// TODO: add construction code here

}

CWorkSpaceToolBarView::~CWorkSpaceToolBarView()
{
}

BOOL CWorkSpaceToolBarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarView drawing

void CWorkSpaceToolBarView::OnDraw(CDC* pDC)
{
	CWorkSpaceToolBarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarView printing

void CWorkSpaceToolBarView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CWorkSpaceToolBarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWorkSpaceToolBarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWorkSpaceToolBarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarView diagnostics

#ifdef _DEBUG
void CWorkSpaceToolBarView::AssertValid() const
{
	CView::AssertValid();
}

void CWorkSpaceToolBarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWorkSpaceToolBarDoc* CWorkSpaceToolBarView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWorkSpaceToolBarDoc)));
	return (CWorkSpaceToolBarDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarView message handlers

void CWorkSpaceToolBarView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
