// MDICustomBackgroundView.cpp : implementation of the CMDICustomBackgroundView class
//

#include "stdafx.h"
#include "MDICustomBackground.h"

#include "MDICustomBackgroundDoc.h"
#include "MDICustomBackgroundView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strInfo = 
	"This sample illustrates how to draw on MDI Client area. \r\n\r\n"
	"See CMainFrame::OnEraseMDIClientBackground and CMainFrame::OnSizeMDIClient for details.";

/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundView

IMPLEMENT_DYNCREATE(CMDICustomBackgroundView, CView)

BEGIN_MESSAGE_MAP(CMDICustomBackgroundView, CView)
	//{{AFX_MSG_MAP(CMDICustomBackgroundView)
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
// CMDICustomBackgroundView construction/destruction

CMDICustomBackgroundView::CMDICustomBackgroundView()
{
	// TODO: add construction code here

}

CMDICustomBackgroundView::~CMDICustomBackgroundView()
{
}

BOOL CMDICustomBackgroundView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundView drawing

void CMDICustomBackgroundView::OnDraw(CDC* pDC)
{
	CMDICustomBackgroundDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	const int iOffset = 20;

	CFont* pFontOld = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT (pFontOld != NULL);

	CRect rectClient;
	GetClientRect (&rectClient);

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
// CMDICustomBackgroundView printing

void CMDICustomBackgroundView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CMDICustomBackgroundView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMDICustomBackgroundView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMDICustomBackgroundView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundView diagnostics

#ifdef _DEBUG
void CMDICustomBackgroundView::AssertValid() const
{
	CView::AssertValid();
}

void CMDICustomBackgroundView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDICustomBackgroundDoc* CMDICustomBackgroundView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDICustomBackgroundDoc)));
	return (CMDICustomBackgroundDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDICustomBackgroundView message handlers

void CMDICustomBackgroundView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}