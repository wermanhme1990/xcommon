// StateToFileView.cpp : implementation of the CStateToFileView class
//

#include "stdafx.h"
#include "StateToFile.h"

#include "StateToFileDoc.h"
#include "StateToFileView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString strInfo = 
	"This sample illustrates how to save/load the current profile (application state) to a file. \r\n"
	"It uses a fact that the application state is saved/loaded to/from the registry\r\n"
	"The implementation just executes 'regedit' in silent mode to import/export the state from/to the registry.\r\n\r\n"
	"See CMainFrame::LoadFrame and CMainFrame::OnClose for details.";


/////////////////////////////////////////////////////////////////////////////
// CStateToFileView

IMPLEMENT_DYNCREATE(CStateToFileView, CView)

BEGIN_MESSAGE_MAP(CStateToFileView, CView)
	//{{AFX_MSG_MAP(CStateToFileView)
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
// CStateToFileView construction/destruction

CStateToFileView::CStateToFileView()
{
	// TODO: add construction code here

}

CStateToFileView::~CStateToFileView()
{
}

BOOL CStateToFileView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CStateToFileView drawing

void CStateToFileView::OnDraw(CDC* pDC)
{
	CStateToFileDoc* pDoc = GetDocument();
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
// CStateToFileView printing

void CStateToFileView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CStateToFileView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStateToFileView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CStateToFileView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CStateToFileView diagnostics

#ifdef _DEBUG
void CStateToFileView::AssertValid() const
{
	CView::AssertValid();
}

void CStateToFileView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStateToFileDoc* CStateToFileView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStateToFileDoc)));
	return (CStateToFileDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStateToFileView message handlers

void CStateToFileView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	theApp.OnViewDoubleClick (this, IDR_MAINFRAME);
}

void CStateToFileView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}
