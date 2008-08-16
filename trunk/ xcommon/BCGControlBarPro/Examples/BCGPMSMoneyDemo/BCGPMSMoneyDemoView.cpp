// BCGPMSMoneyDemoView.cpp : implementation of the CBCGPMSMoneyDemoView class
//

#include "stdafx.h"
#include "BCGPMSMoneyDemo.h"

#include "BCGPMSMoneyDemoDoc.h"
#include "BCGPMSMoneyDemoView.h"

#include "MainFrm.h"

#include "BCGPMSMVisualManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMoneyDemoView

IMPLEMENT_DYNCREATE(CBCGPMSMoneyDemoView, CView)

BEGIN_MESSAGE_MAP(CBCGPMSMoneyDemoView, CView)
	//{{AFX_MSG_MAP(CBCGPMSMoneyDemoView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMoneyDemoView construction/destruction

CBCGPMSMoneyDemoView::CBCGPMSMoneyDemoView()
{
	// TODO: add construction code here

}

CBCGPMSMoneyDemoView::~CBCGPMSMoneyDemoView()
{
	m_sizeImage = CSize (0, 0);
}

BOOL CBCGPMSMoneyDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMoneyDemoView drawing

void CBCGPMSMoneyDemoView::OnDraw(CDC* pDCView)
{
	CBCGPMemDC memDC (*pDCView, this);
	CDC* pDC = &memDC.GetDC ();

	CBCGPMSMoneyDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rectClient;
	GetClientRect (rectClient);

	pDC->FillSolidRect (rectClient, RGB (255, 255, 255));

	pDC->DrawState (CPoint (20, 20), m_sizeImage, &m_Image, DSS_NORMAL);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMoneyDemoView printing

BOOL CBCGPMSMoneyDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBCGPMSMoneyDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBCGPMSMoneyDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMoneyDemoView diagnostics

#ifdef _DEBUG
void CBCGPMSMoneyDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CBCGPMSMoneyDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBCGPMSMoneyDemoDoc* CBCGPMSMoneyDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBCGPMSMoneyDemoDoc)));
	return (CBCGPMSMoneyDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPMSMoneyDemoView message handlers
// these are all simple one-liners to do simple controlling of the browser

BOOL CBCGPMSMoneyDemoView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

int CBCGPMSMoneyDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Image.LoadBitmap (IDB_BCGPMSM_VIEW_BKGND);

	BITMAP bitmap;
	m_Image.GetBitmap (&bitmap);

	m_sizeImage = CSize (bitmap.bmWidth, bitmap.bmHeight);
	return 0;
}
