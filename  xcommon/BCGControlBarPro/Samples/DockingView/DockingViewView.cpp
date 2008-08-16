// DockingViewView.cpp : implementation of the CDockingViewView class
//

#include "stdafx.h"
#include "DockingView.h"

#include "DockingViewDoc.h"
#include "DockingViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strInfo = 
	"This sample illustares how to add a CView-derived class into a docking pane.\r\n\r\n"
	"Please take a look at CMyFormView::CreateView method and\r\n"
	"CWorkspaceBar class for details.\r\n\r\n"
	"Click 'Update All Views' toolbar button to see how the view\r\n"
	"reflects the document's notification.\r\n\r\n"
	"This example was created using BCGApplicationWizard";

/////////////////////////////////////////////////////////////////////////////
// CDockingViewView

IMPLEMENT_DYNCREATE(CDockingViewView, CView)

BEGIN_MESSAGE_MAP(CDockingViewView, CView)
	//{{AFX_MSG_MAP(CDockingViewView)
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
// CDockingViewView construction/destruction

CDockingViewView::CDockingViewView()
{
	// TODO: add construction code here

}

CDockingViewView::~CDockingViewView()
{
}

BOOL CDockingViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDockingViewView drawing

void CDockingViewView::OnDraw(CDC* pDC)
{
	CDockingViewDoc* pDoc = GetDocument();
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
// CDockingViewView printing

void CDockingViewView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CDockingViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDockingViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDockingViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDockingViewView diagnostics

#ifdef _DEBUG
void CDockingViewView::AssertValid() const
{
	CView::AssertValid();
}

void CDockingViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDockingViewDoc* CDockingViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDockingViewDoc)));
	return (CDockingViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDockingViewView message handlers

void CDockingViewView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
