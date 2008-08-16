// ToolbarLabelView.cpp : implementation of the CToolbarLabelView class
//

#include "stdafx.h"
#include "ToolbarLabel.h"

#include "ToolbarLabelDoc.h"
#include "ToolbarLabelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelView

IMPLEMENT_DYNCREATE(CToolbarLabelView, CView)

BEGIN_MESSAGE_MAP(CToolbarLabelView, CView)
	//{{AFX_MSG_MAP(CToolbarLabelView)
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
// CToolbarLabelView construction/destruction

CToolbarLabelView::CToolbarLabelView()
{
	// TODO: add construction code here

}

CToolbarLabelView::~CToolbarLabelView()
{
}

BOOL CToolbarLabelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelView drawing

void CToolbarLabelView::OnDraw(CDC* pDC)
{
	CToolbarLabelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelView printing

void CToolbarLabelView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CToolbarLabelView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CToolbarLabelView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CToolbarLabelView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelView diagnostics

#ifdef _DEBUG
void CToolbarLabelView::AssertValid() const
{
	CView::AssertValid();
}

void CToolbarLabelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolbarLabelDoc* CToolbarLabelView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolbarLabelDoc)));
	return (CToolbarLabelDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolbarLabelView message handlers

void CToolbarLabelView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
