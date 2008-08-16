// XMLSettingsView.cpp : implementation of the CXMLSettingsView class
//

#include "stdafx.h"
#include "XMLSettings.h"

#include "XMLSettingsDoc.h"
#include "XMLSettingsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strInfo = 
	"This sample illustrates how to use CBCGPRegistrySP class."
	"CBCGPXMLSettings that is derived from CBCGPRegistry provides a simple way"
	"to save/load a user-defined data in the external XML file (user.xml).";

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsView

IMPLEMENT_DYNCREATE(CXMLSettingsView, CView)

BEGIN_MESSAGE_MAP(CXMLSettingsView, CView)
	//{{AFX_MSG_MAP(CXMLSettingsView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsView construction/destruction

CXMLSettingsView::CXMLSettingsView()
{
	// TODO: add construction code here

}

CXMLSettingsView::~CXMLSettingsView()
{
}

BOOL CXMLSettingsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsView drawing

void CXMLSettingsView::OnDraw(CDC* pDC)
{
	CXMLSettingsDoc* pDoc = GetDocument();
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
// CXMLSettingsView printing

void CXMLSettingsView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CXMLSettingsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CXMLSettingsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CXMLSettingsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsView diagnostics

#ifdef _DEBUG
void CXMLSettingsView::AssertValid() const
{
	CView::AssertValid();
}

void CXMLSettingsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CXMLSettingsDoc* CXMLSettingsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXMLSettingsDoc)));
	return (CXMLSettingsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXMLSettingsView message handlers

void CXMLSettingsView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	theApp.OnViewDoubleClick (this, IDR_XMLSETTYPE);
}

void CXMLSettingsView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
