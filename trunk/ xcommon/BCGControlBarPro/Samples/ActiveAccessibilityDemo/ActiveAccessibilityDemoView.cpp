// ActiveAccessibilityDemoView.cpp : implementation of the CActiveAccessibilityDemoView class
//

#include "stdafx.h"
#include "ActiveAccessibilityDemo.h"

#include "ActiveAccessibilityDemoDoc.h"
#include "ActiveAccessibilityDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strInfo = 
	"This sample illustrates how to use Microsoft® Active Accessibility along \r\n"
	"with library toolbars and menus. To see this feature in action, you need\r\n"
	"to run Narrator Accessibility Tool and highlight any menu item in this demo.\r\n"
	"Selected menu item name will be speaked by your sound system";

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoView

IMPLEMENT_DYNCREATE(CActiveAccessibilityDemoView, CView)

BEGIN_MESSAGE_MAP(CActiveAccessibilityDemoView, CView)
	//{{AFX_MSG_MAP(CActiveAccessibilityDemoView)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoView construction/destruction

CActiveAccessibilityDemoView::CActiveAccessibilityDemoView()
{
	// TODO: add construction code here

}

CActiveAccessibilityDemoView::~CActiveAccessibilityDemoView()
{
}

BOOL CActiveAccessibilityDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoView drawing

void CActiveAccessibilityDemoView::OnDraw(CDC* pDC)
{
	CActiveAccessibilityDemoDoc* pDoc = GetDocument();
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
// CActiveAccessibilityDemoView printing

void CActiveAccessibilityDemoView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CActiveAccessibilityDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CActiveAccessibilityDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CActiveAccessibilityDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoView diagnostics

#ifdef _DEBUG
void CActiveAccessibilityDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CActiveAccessibilityDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CActiveAccessibilityDemoDoc* CActiveAccessibilityDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CActiveAccessibilityDemoDoc)));
	return (CActiveAccessibilityDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CActiveAccessibilityDemoView message handlers

void CActiveAccessibilityDemoView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CActiveAccessibilityDemoView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	
}

void CActiveAccessibilityDemoView::OnEditCut() 
{
	// TODO: Add your command handler code here
	
}

void CActiveAccessibilityDemoView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	
}
