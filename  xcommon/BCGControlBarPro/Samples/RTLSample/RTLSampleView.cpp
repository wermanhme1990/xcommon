// RTLSampleView.cpp : implementation of the CRTLSampleView class
//

#include "stdafx.h"
#include "RTLSample.h"

#include "RTLSampleDoc.h"
#include "RTLSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleView

IMPLEMENT_DYNCREATE(CRTLSampleView, CView)

BEGIN_MESSAGE_MAP(CRTLSampleView, CView)
	//{{AFX_MSG_MAP(CRTLSampleView)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleView construction/destruction

CRTLSampleView::CRTLSampleView()
{
}

CRTLSampleView::~CRTLSampleView()
{
}

BOOL CRTLSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CRTLSampleView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

}

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleView printing

void CRTLSampleView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CRTLSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRTLSampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRTLSampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRTLSampleView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleView diagnostics

#ifdef _DEBUG
void CRTLSampleView::AssertValid() const
{
	CView::AssertValid();
}

void CRTLSampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRTLSampleDoc* CRTLSampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRTLSampleDoc)));
	return (CRTLSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRTLSampleView message handlers

void CRTLSampleView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CRTLSampleView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	
}
