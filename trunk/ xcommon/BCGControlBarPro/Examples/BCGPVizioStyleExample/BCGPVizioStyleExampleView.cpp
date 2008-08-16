// BCGPVizioStyleExampleView.cpp : implementation of the CBCGPVizioStyleExampleView class
//

#include "stdafx.h"
#include "BCGPVizioStyleExample.h"

#include "BCGPVizioStyleExampleDoc.h"
#include "BCGPVizioStyleExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strInfo = 
	_T("This program illustrates a Visio(r)-style application. It contains several ")
	_T("dynamic outlook-style control bars with \"immediate\" docking. Please, pay ")
	_T("attention, that all control bars are created using CBRS_BCGP_OUTLOOK_TABS ")
	_T("style to achieve outlook-style tabbing . ");

/////////////////////////////////////////////////////////////////////////////
// CBCGPVizioStyleExampleView

IMPLEMENT_DYNCREATE(CBCGPVizioStyleExampleView, CView)

BEGIN_MESSAGE_MAP(CBCGPVizioStyleExampleView, CView)
	//{{AFX_MSG_MAP(CBCGPVizioStyleExampleView)
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
// CBCGPVizioStyleExampleView construction/destruction

CBCGPVizioStyleExampleView::CBCGPVizioStyleExampleView()
{
	// TODO: add construction code here

}

CBCGPVizioStyleExampleView::~CBCGPVizioStyleExampleView()
{
}

BOOL CBCGPVizioStyleExampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPVizioStyleExampleView drawing

void CBCGPVizioStyleExampleView::OnDraw(CDC* pDC)
{
	CBCGPVizioStyleExampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	const int iOffset = 20;

	CFont* pFontOld = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT (pFontOld != NULL);

	CRect rectClient;
	GetClientRect (&rectClient);

	rectClient.DeflateRect (30, 0);

	CRect rectText = rectClient;
	rectText.DeflateRect (iOffset, iOffset);
	pDC->DrawText (strInfo, rectText, DT_CALCRECT | DT_WORDBREAK);

	rectText.OffsetRect (	(rectClient.Width () - rectText.Width () - 2 * iOffset) / 2,
							(rectClient.Height () - rectText.Height () - 2 * iOffset) / 2);

	CRect rectFrame = rectText;
	rectFrame.InflateRect (iOffset, iOffset);

	pDC->FillSolidRect (rectFrame, ::GetSysColor (COLOR_INFOBK));
	
	rectFrame.DeflateRect (1, 1);
	pDC->Draw3dRect (rectFrame, ::GetSysColor (COLOR_3DSHADOW), 
					::GetSysColor (COLOR_3DLIGHT));

	rectFrame.DeflateRect (2, 2);
	pDC->Draw3dRect (rectFrame, ::GetSysColor (COLOR_3DSHADOW), 
					::GetSysColor (COLOR_3DLIGHT));

	pDC->SetTextColor (::GetSysColor (COLOR_INFOTEXT));
	pDC->SetBkMode (TRANSPARENT);

	pDC->DrawText (strInfo, rectText, DT_WORDBREAK);

	pDC->SelectObject (pFontOld);

}

/////////////////////////////////////////////////////////////////////////////
// CBCGPVizioStyleExampleView printing

void CBCGPVizioStyleExampleView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CBCGPVizioStyleExampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBCGPVizioStyleExampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBCGPVizioStyleExampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPVizioStyleExampleView diagnostics

#ifdef _DEBUG
void CBCGPVizioStyleExampleView::AssertValid() const
{
	CView::AssertValid();
}

void CBCGPVizioStyleExampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBCGPVizioStyleExampleDoc* CBCGPVizioStyleExampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBCGPVizioStyleExampleDoc)));
	return (CBCGPVizioStyleExampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBCGPVizioStyleExampleView message handlers

void CBCGPVizioStyleExampleView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
