// MDITabsDemoView.cpp : implementation of the CMDITabsDemoView class
//

#include "stdafx.h"
#include "MDITabsDemo.h"

#include "MDITabsDemoDoc.h"
#include "MDITabsDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoView

IMPLEMENT_DYNCREATE(CMDITabsDemoView, CView)

BEGIN_MESSAGE_MAP(CMDITabsDemoView, CView)
	//{{AFX_MSG_MAP(CMDITabsDemoView)
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
// CMDITabsDemoView construction/destruction

CMDITabsDemoView::CMDITabsDemoView()
{
	// TODO: add construction code here

}

CMDITabsDemoView::~CMDITabsDemoView()
{
}

BOOL CMDITabsDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoView drawing

void CMDITabsDemoView::OnDraw(CDC* pDC)
{
	CMDITabsDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CString strTitle = pDoc->GetTitle ();

	CFont* pOldFont = pDC->SelectObject (&globalData.fontRegular);
	pDC->SetBkMode (TRANSPARENT);

	pDC->TextOut (10, 10, strTitle);

	pDC->SelectObject (pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoView printing

void CMDITabsDemoView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CMDITabsDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMDITabsDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMDITabsDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoView diagnostics

#ifdef _DEBUG
void CMDITabsDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CMDITabsDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDITabsDemoDoc* CMDITabsDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDITabsDemoDoc)));
	return (CMDITabsDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDITabsDemoView message handlers

void CMDITabsDemoView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
