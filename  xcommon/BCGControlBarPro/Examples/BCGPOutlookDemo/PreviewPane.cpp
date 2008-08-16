// PreviewPane.cpp : implementation file
//

#include "stdafx.h"
#include "BCGPOutlookDemo.h"
#include "PreviewPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nHorzMargin = 2;

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane

IMPLEMENT_DYNCREATE(CPreviewPane, CView)

CPreviewPane::CPreviewPane()
{
}

CPreviewPane::~CPreviewPane()
{
}


BEGIN_MESSAGE_MAP(CPreviewPane, CView)
	//{{AFX_MSG_MAP(CPreviewPane)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane drawing

void CPreviewPane::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	CRect rectCaption;
	GetClientRect (rectCaption);

	rectCaption.bottom = rectCaption.top + 20;

	pDC->FillSolidRect (rectCaption, globalData.clrBarFace);

	int nOldBkMode = pDC->SetBkMode (TRANSPARENT);
	COLORREF clrOldText = pDC->SetTextColor (globalData.clrBarText);
	CFont* pOldFont = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);

	CRect rectText = rectCaption;
	rectText.left += 2 * nHorzMargin;
	rectText.right -= 2 * nHorzMargin + rectText.Height ();

	CString strCaption = _T("Preview area...");
	pDC->DrawText (strCaption, rectText, 
				DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
	pDC->SetTextColor (clrOldText);
	pDC->SetBkMode (nOldBkMode);
}

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane diagnostics

#ifdef _DEBUG
void CPreviewPane::AssertValid() const
{
	CView::AssertValid();
}

void CPreviewPane::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPreviewPane message handlers
