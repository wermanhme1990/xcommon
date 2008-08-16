// CustomPagesView.cpp : implementation of the CCustomPagesView class
//

#include "stdafx.h"
#include "CustomPages.h"

#include "CustomPagesDoc.h"
#include "CustomPagesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strInfo = 
	"This sample illustares how to add a custom property page to customization dialog. \r\n\r\n"
	"The following features are available:\r\n"
	"* insert custom property page 'My Customization Page' into customization dialog.\r\n\r\n"
	"This example was created using BCGPApplicationWizard\r\n"
	"Using resource editor create a new dialog form\r\n"
	"Using application wizard create a new (CMyCustomizationPage) class for this form and derive it from CPropertyPage\r\n"
	"CMyCustomizationPage must be enabled to be created dynamically (DECLARE_DYNCREATE)\r\n"
	"See CMainFrame::OnViewCustomize() for details.";

/////////////////////////////////////////////////////////////////////////////
// CCustomPagesView

IMPLEMENT_DYNCREATE(CCustomPagesView, CView)

BEGIN_MESSAGE_MAP(CCustomPagesView, CView)
	//{{AFX_MSG_MAP(CCustomPagesView)
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
// CCustomPagesView construction/destruction

CCustomPagesView::CCustomPagesView()
{
	// TODO: add construction code here

}

CCustomPagesView::~CCustomPagesView()
{
}

BOOL CCustomPagesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomPagesView drawing

void CCustomPagesView::OnDraw(CDC* pDC)
{
	CCustomPagesDoc* pDoc = GetDocument();
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
// CCustomPagesView printing

void CCustomPagesView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CCustomPagesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCustomPagesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCustomPagesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCustomPagesView diagnostics

#ifdef _DEBUG
void CCustomPagesView::AssertValid() const
{
	CView::AssertValid();
}

void CCustomPagesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCustomPagesDoc* CCustomPagesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCustomPagesDoc)));
	return (CCustomPagesDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCustomPagesView message handlers

void CCustomPagesView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
