// UndoBoxView.cpp : implementation of the CUndoBoxView class
//

#include "stdafx.h"
#include "UndoBox.h"
#include "UndoBar.h"

#include "UndoBoxDoc.h"
#include "UndoBoxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strInfo = 
	"This sample illustrates how to create MS Office-style \"Undo\" button.";

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxView

IMPLEMENT_DYNCREATE(CUndoBoxView, CView)

BEGIN_MESSAGE_MAP(CUndoBoxView, CView)
	//{{AFX_MSG_MAP(CUndoBoxView)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxView construction/destruction

CUndoBoxView::CUndoBoxView()
{
	// TODO: add construction code here

}

CUndoBoxView::~CUndoBoxView()
{
}

BOOL CUndoBoxView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CView::PreCreateWindow(cs);

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxView drawing

void CUndoBoxView::OnDraw(CDC* pDC)
{
	CUndoBoxDoc* pDoc = GetDocument();
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
// CUndoBoxView printing

void CUndoBoxView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CUndoBoxView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CUndoBoxView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CView begin printing.
	CView::OnBeginPrinting(pDC, pInfo);
}

void CUndoBoxView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CView end printing
	CView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxView diagnostics

#ifdef _DEBUG
void CUndoBoxView::AssertValid() const
{
	CView::AssertValid();
}

void CUndoBoxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUndoBoxDoc* CUndoBoxView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUndoBoxDoc)));
	return (CUndoBoxDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUndoBoxView message handlers

void CUndoBoxView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CUndoBoxView::OnEditUndo() 
{
	int nCount = CUndoButton::GetSelNum ();
	if (nCount > 0)
	{
		CString str;
		str.Format ("Undo %d Actions....", nCount);

		MessageBox (str);
	}
	else
	{
		MessageBox ("Undo Last Action...");
	}

	CUndoButton::ResetSelNum ();
}
