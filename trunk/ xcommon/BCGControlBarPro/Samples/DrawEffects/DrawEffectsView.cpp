// DrawEffectsView.cpp : implementation of the CDrawEffectsView class
//

#include "stdafx.h"
#include "DrawEffects.h"

#include "DrawEffectsDoc.h"
#include "DrawEffectsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nHeaderSize = 30;
const int nBlockSize = 150;
const int nMarginSize = 10;

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsView

IMPLEMENT_DYNCREATE(CDrawEffectsView, CScrollView)

BEGIN_MESSAGE_MAP(CDrawEffectsView, CScrollView)
	//{{AFX_MSG_MAP(CDrawEffectsView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsView construction/destruction

CDrawEffectsView::CDrawEffectsView()
{
	// TODO: add construction code here

}

CDrawEffectsView::~CDrawEffectsView()
{
}

BOOL CDrawEffectsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsView drawing

static const CString strInfo = 
	"This sample illustrates usage of CBCGPDrawManager class";

void CDrawEffectsView::OnDraw(CDC* pDC)
{
	CDrawEffectsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rectClient;
	GetClientRect (rectClient);

	CFont* pOldFont = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT_VALID (pOldFont);

	pDC->SetTextColor (::GetSysColor (COLOR_INFOTEXT));
	pDC->SetBkMode (TRANSPARENT);

	CRect rectFrame (CPoint (nMarginSize, nMarginSize), CSize (nBlockSize * 2, nHeaderSize));
	rectFrame.DeflateRect (5, 5);

	pDC->DrawText (strInfo, rectFrame, DT_WORDBREAK);

	pDC->SetTextColor (RGB (0, 0, 0));

	CRect rect = rectClient;
	rect.top += nHeaderSize;
	rect.DeflateRect (nMarginSize, nMarginSize);

	//---------------
	// Shadow effect:
	//---------------
	Shadow (pDC, CRect (CPoint (rect.left, rect.top), CSize (nBlockSize, nBlockSize)));

	//-----------------
	// Gradient effect:
	//-----------------
	Gradient (pDC, CRect (CPoint (rect.left + nBlockSize + 2 * nMarginSize, rect.top), CSize (nBlockSize, nBlockSize)));

	//------------------
	// Highlight effect:
	//------------------
	Highlight (pDC, CRect (CPoint (rect.left, rect.top + nBlockSize + 2 * nMarginSize), CSize (nBlockSize, nBlockSize)));

	//-----------------------
	// Gradient rings effect:
	//-----------------------
	GradientRings (pDC, CRect (CPoint (rect.left + nBlockSize + 2 * nMarginSize, rect.top + nBlockSize + 2 * nMarginSize), CSize (nBlockSize, nBlockSize)));

	pDC->SelectObject (pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsView printing

void CDrawEffectsView::OnFilePrintPreview() 
{
	BCGPPrintPreview (this);
}

BOOL CDrawEffectsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawEffectsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawEffectsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsView diagnostics

#ifdef _DEBUG
void CDrawEffectsView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDrawEffectsView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDrawEffectsDoc* CDrawEffectsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawEffectsDoc)));
	return (CDrawEffectsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawEffectsView message handlers

void CDrawEffectsView::Shadow (CDC* pDC, CRect rect)
{
	pDC->FillSolidRect (rect, RGB (255, 255, 255));
	pDC->Draw3dRect (rect, ::GetSysColor (COLOR_3DSHADOW),
					::GetSysColor (COLOR_3DLIGHT));
	rect.DeflateRect (10, 10);

	CBCGPDrawManager dm (*pDC);

	// Draw caption:
	int nTextHeight = pDC->DrawText (CString (_T("Shadow Effect:")), rect, 0);
	rect.top += nTextHeight + 10;

	// Fill area:
	CBrush br1 (HS_FDIAGONAL, RGB (120, 204, 202));
	pDC->SetBkColor (RGB (255, 255, 255));
	pDC->FillRect (rect, &br1);

	rect.DeflateRect (15, 15);
	CRect rect2 = rect;

	rect.bottom -= rect.Height () / 3;

	// Draw solid rectangle
	CBrush br2 (RGB (238, 238, 238));
	pDC->FillRect (rect, &br2);

	// Drop shadow:
	dm.DrawShadow (rect, 10);

	rect2.left += 30;
	rect2.right -= 10;
	rect2.top += 15;

	// Draw solid rectangle
	CBrush br3 (RGB (141, 207, 244));
	pDC->FillRect (rect2, &br3);

	// Drop shadow:
	dm.DrawShadow (rect2, 5);
}

void CDrawEffectsView::Gradient (CDC* pDC, CRect rect)
{
	pDC->FillSolidRect (rect, RGB (255, 255, 255));
	pDC->Draw3dRect (rect, ::GetSysColor (COLOR_3DSHADOW),
					::GetSysColor (COLOR_3DLIGHT));

	rect.DeflateRect (10, 10);

	CBCGPDrawManager dm (*pDC);

	// Draw caption:
	int nTextHeight = pDC->DrawText (CString (_T("Gradient Effect:")), rect, 0);
	rect.top += nTextHeight + 10;

	CRect rect1 = rect;
	rect1.bottom = rect1.top + 20;

	pDC->Rectangle (rect1);
	rect1.DeflateRect (1, 1);
	dm.FillGradient (rect1, RGB (0, 159, 98), RGB (255, 255, 255), FALSE);

	CRect rect2 = rect;
	rect2.top = rect1.bottom + 10;
	rect2.right = rect2.left + 30;

	pDC->Rectangle (rect2);
	rect2.DeflateRect (1, 1);
	dm.FillGradient (rect2, RGB (245, 183, 87), RGB (255, 255, 255), TRUE);

	CRect rect3 = rect;
	rect3.left = rect2.right + 10;
	rect3.top = rect2.top;
	rect3.bottom = rect3.top + 20;

	pDC->Rectangle (rect3);
	rect3.DeflateRect (1, 1);

	rect3.right = rect3.left + rect3.Width () / 3;
	dm.FillGradient (rect3, RGB (153, 153, 153), RGB (255, 255, 255), FALSE);

	rect3.left = rect3.right;
	rect3.right = rect.right - 1;
	dm.FillGradient (rect3, RGB (255, 255, 255), RGB (153, 153, 153), FALSE);

	CRect rect4 = rect;
	rect4.left = rect2.right + 30;
	rect4.top = rect3.bottom + 10;

	pDC->Rectangle (rect4);
	rect4.DeflateRect (1, 1);

	dm.FillGradient2 (rect4, RGB (0, 255, 255), RGB (0, 0, 255), 45);
}

void CDrawEffectsView::Highlight (CDC* pDC, CRect rect)
{
	pDC->FillSolidRect (rect, RGB (255, 255, 255));
	pDC->Draw3dRect (rect, ::GetSysColor (COLOR_3DSHADOW),
					::GetSysColor (COLOR_3DLIGHT));
	rect.DeflateRect (10, 10);

	CBCGPDrawManager dm (*pDC);

	// Draw caption:
	int nTextHeight = pDC->DrawText (CString (_T("Highlight Effect:")), rect, 0);
	rect.top += nTextHeight + 10;

	CBCGPDrawState ds;
	m_Bitmap.PrepareDrawImage (ds);
	
	// Draw original image:
	int x = rect.left + 80;
	int y = rect.top;

	CRect rectText = rect;
	rectText.left += 10;
	rectText.right = x;
	rectText.top += 10;
	rectText.bottom = rectText.top + rect.Height () / 3;

	pDC->DrawText (CString (_T("Original:")), rectText, 0);
	m_Bitmap.Draw (pDC, x, y, 0);

	// Draw 20% lighter:
	y += rect.Height () / 3;
	rectText.OffsetRect (0, rect.Height () / 3); 

	pDC->DrawText (CString (_T("Light:")), rectText, 0);
	m_Bitmap.Draw (pDC, x, y, 0);

	dm.HighlightRect (CRect (CPoint (x, y), CSize (32, 32)), 150);

	// Draw 50% darker:
	y += rect.Height () / 3;
	rectText.OffsetRect (0, rect.Height () / 3);

	pDC->DrawText (CString (_T("Dark:")), rectText, 0);
	m_Bitmap.Draw (pDC, x, y, 0);

	dm.HighlightRect (CRect (CPoint (x, y), CSize (32, 32)), 80);

	m_Bitmap.EndDrawImage (ds);
}

void CDrawEffectsView::GradientRings (CDC* pDC, CRect rect)
{
	pDC->FillSolidRect (rect, RGB (255, 255, 255));
	pDC->Draw3dRect (rect, ::GetSysColor (COLOR_3DSHADOW),
					::GetSysColor (COLOR_3DLIGHT));
	rect.DeflateRect (10, 10);

	CBCGPDrawManager dm (*pDC);

	// Draw caption:
	int nTextHeight = pDC->DrawText (CString (_T("Gradient Ring Effect:")), rect, 0);
	rect.top += nTextHeight + 10;

	// Draw "ring button" (combined 3 gradient rings):
	CRect rect1 = rect;
	rect1.right = rect.left + 50;
	rect1.bottom = rect1.top + 50;

	dm.DrawGradientRing (rect1,
		RGB (176, 186, 201),
		RGB (255, 255, 255),
		RGB (255, 255, 255),
		145, 10, RGB (250, 250, 250));

	rect1.DeflateRect (10, 10);
	dm.DrawGradientRing (rect1,
		RGB (255, 255, 255),
		RGB (176, 186, 201),
		RGB (255, 255, 255),
		145, 7, RGB (250, 250, 250));
	rect1.DeflateRect (6, 6);

	dm.DrawGradientRing (rect1,
		RGB (0, 153, 0),
		RGB (255, 255, 255),
		RGB (255, 255, 255),
		145, 10, RGB (250, 250, 250));

	// Draw push button:
	CRect rect2 = rect;
	rect2.top = rect2.bottom - 26;
	rect2.right = rect2.left + 60;

	dm.DrawGradientRing (rect2,
		RGB (176, 186, 201),
		RGB (255, 255, 255),
		RGB (176, 186, 201),
		45, 4, RGB (230, 240, 255));

	const CString str = _T("Button");
	pDC->DrawText (str, rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// Anothet example:
	CRect rect3 = rect;
	rect3.left += 80;
	rect3.top = rect3.CenterPoint ().y - rect3.Width () / 2;
	rect3.bottom = rect3.top + rect3.Width ();

	dm.DrawGradientRing (rect3,
		RGB (192, 192, 192),
		RGB (255, 255, 255),
		-1,
		225, 4, RGB (230, 230, 230));
}

BOOL CDrawEffectsView::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (rectClient);

	pDC->FillSolidRect (rectClient, RGB (255, 255, 234));

	return TRUE;
}

int CDrawEffectsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Bitmap.Load (IDB_BITMAP1);
	m_Bitmap.SetImageSize (CSize (32, 32));
	
	return 0;
}

void CDrawEffectsView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	const int nBlockSize = 150;
	const int nMarginSize = 10;

	SetScrollSizes (MM_TEXT, CSize (
		nBlockSize * 2 + nMarginSize * 4, 
		nHeaderSize + nBlockSize * 2 + nMarginSize * 3));
}
